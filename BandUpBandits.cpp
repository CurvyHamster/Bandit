#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>
#include <gumbo.h>
#include <algorithm>
#include <vector>
#include "curl/curl.h"
#include "chrono"
#include "thread"
#include <limits>
#include <regex>
#include <stdio.h>

using namespace std;
class FileManager
{
private:
    string level;
    string filename;

public:
    FileManager(string level)
    {
        level = level;
        filename = level + "password.txt";
    };
    void createFile(string password)
    {
        std::ofstream outfile(filename, std::ofstream::out);
        outfile << password;
    }
    string getFilename()
    {
        return filename;
    }
};
class RuleReader
{
private:
    typedef size_t (*curl_write)(char *, size_t, size_t, std::string *);
    static std::string requestHtml(string level)
    {
        CURLcode res_code = CURLE_FAILED_INIT;
        CURL *curl = curl_easy_init();
        std::string result;
        std::string url = "https://overthewire.org/wargames/bandit/bandit" + level;

        curl_global_init(CURL_GLOBAL_ALL);

        if (curl)
        {
            curl_easy_setopt(curl,
                             CURLOPT_WRITEFUNCTION,
                             static_cast<curl_write>([](char *contents, size_t size,
                                                        size_t nmemb, std::string *data) -> size_t
                                                     { 
                                               size_t new_size = size * nmemb;
                                               if (data == NULL) {
                                                 return 0;
                                               } 
                                               data->append(contents, new_size);
                                               return new_size; }));
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Bandit Command and Rule Fetcher");

            res_code = curl_easy_perform(curl);
            // std::cout << result;
            if (res_code != CURLE_OK)
            {
                return curl_easy_strerror(res_code);
            }

            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
        return result;
    }
    static string str_replace(std::string search, std::string replace, std::string &subject)
    {
        size_t count;
        for (std::string::size_type pos{};
             subject.npos != (pos = subject.find(search.data(), pos, search.length()));
             pos += replace.length(), ++count)
        {
            subject.replace(pos, search.length(), replace.data(), replace.length());
        }

        return subject;
    }

    static string extract_text(GumboNode *node)
    {
        if (node->type == GUMBO_NODE_TEXT)
        {
            return std::string(node->v.text.text);
        }
        else if (node->type == GUMBO_NODE_ELEMENT &&
                 node->v.element.tag != GUMBO_TAG_SCRIPT &&
                 node->v.element.tag != GUMBO_TAG_STYLE)
        {
            std::string contents = "";
            GumboVector *children = &node->v.element.children;
            for (unsigned int i = 0; i < children->length; ++i)
            {
                std::string text = extract_text((GumboNode *)children->data[i]);
                if (i != 0 && !text.empty())
                {
                    contents.append("");
                }

                contents.append(str_replace(":", ">", text));
            }

            return contents;
        }
        else
        {
            return "";
        }
    }

    static string Parse(GumboNode *node)
    {
        std::string html = "";
        GumboAttribute *attr;
        GumboAttribute *href_attr;
        if (node->type != GUMBO_NODE_ELEMENT)
        {
            return html;
        }
        if ((href_attr = gumbo_get_attribute(&node->v.element.attributes, "id")) &&
            strstr(href_attr->value, "content"))
        {
            html += extract_text(node);
            html += "\n";
        }

        GumboVector *children = &node->v.element.children;
        for (int i = 0; i < children->length; ++i)
        {
            html += Parse(static_cast<GumboNode *>(children->data[i]));
        }

        return html;
    }

    static string parseText(std::string parseText)
    {
        std::string html;
        GumboOutput *output = gumbo_parse_with_options(&kGumboDefaultOptions, parseText.data(), parseText.length());

        html += Parse(output->root);

        gumbo_destroy_output(&kGumboDefaultOptions, output);

        return html;
    }

    static string createOutputText(std::string parsedText)
    {
        std::string levelgoalRM = "Level Goal";
        std::string commandlist = "Commands you may need to solve this level";
        std::string omit = "Helpful Reading Material";
        // modify the string to take out Level Goal
        size_t pos = 0;
        while ((pos = parsedText.find(levelgoalRM)) != std::string::npos)
        {
            parsedText.erase(pos, levelgoalRM.length());
        }
        // Replace all new line characters with spaces
        pos = 0;
        while ((pos = parsedText.find('\n', pos)) != std::string::npos)
        {
            parsedText.replace(pos, 1, " ");
            pos += 1;
        }
        // add a new line after every period. except for periods that declare a filename

        std::regex re("\\.\\s");
        std::string tracenewline = std::regex_replace(parsedText, re, ".\n");
        parsedText = tracenewline;
        // find position of levells. Change it to "level:  "
        pos = parsedText.find(commandlist);
        if (pos != std::string::npos)
        {
            // Insert a colon and space after "level"
            parsedText.insert(pos + 41, ":\n");
        }
        // add a new line before commandlist
        pos = 0;
        pos = parsedText.find(commandlist);
        if (pos != std::string::npos)
        {
            parsedText.insert(pos + 0, ".\n\n");
        }
        // I know this is a bit tedious but i'm removing any empty spaces.
        pos = 0;
        while ((pos = parsedText.find(".\n.\nC", pos)) != std::string::npos)
        {
            parsedText.erase(pos + 1, 2);
        }
        pos = 0;
        while ((pos = parsedText.find("..", pos)) != std::string::npos)
        {
            parsedText.erase(pos + 1, 1);
        }
        // change > symbol to a colon followed by a new line
        pos = 0;
        while ((pos = parsedText.find(">", pos)) != std::string::npos)
        {
            parsedText.replace(pos, 1, ":\n");
            pos += 1;
        }
        // removes spaces after every new line
        pos = 0;
        while ((pos = parsedText.find("\n ", pos)) != std::string::npos)
        {
            parsedText.erase(pos + 1, 1);
        }
        // Erase everything from the "helpful links thingy" text to the end of the string
        pos = parsedText.find(omit);
        if (pos != std::string::npos)
        {
            parsedText.erase(pos);
        }

        std::string editedstring = parsedText + "\n";
        return editedstring;
    }

public:
    static void outputTheRules(string rules)
    {
        cout << "\n\n\n\n\n\n\n\n\n\nThe Golden Rule is as follows: \n";
        cout << rules << "\n\n\n\n";
        cout << "Caution!, Strangers are Unwelcome.                                Proceed?\n";
        cout << "Press enter to continue...";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin.get();
        cout << "\nConnecting...\n\n";
    }
    static string compileRules(string level)
    {
        int forcurl = stoi(level);
        forcurl += 1;
        level = to_string(forcurl);
        string html = requestHtml(level);
        string parsedText = parseText(html);
        string output = createOutputText(parsedText);
        return output;
    }
};
int main(int argc, char **argv)
{
    string editedstring;
    string level;
    string password;
    string beenHere;

    cout << "Giddy Up Bandit! What level are you progressing to?" << endl;
    cout << "----------------------" << endl;

    for (size_t i = 0; i < 5; i++)
    {
        try
        {
            cin >> level;
            int levelInt = stoi(level);
            if (levelInt <= 34 && levelInt >= 0)
            {
                break;
            }
            else
            {
                throw 1;
            }
        }
        catch (...)
        {

            if (i == 4)
            {
                cout << "Good Day Sir! \n";
                return 1;
            }
            else if (i < 4)
            {
                std::cerr << "Give it another go! I don't know where you're heading to smuggler." << '\n';
            }
        }
    }

    string rules = RuleReader::compileRules(level);
    FileManager fileManager = FileManager(level);

    cout << "\n\nReckon you've been here before? y/n: ";
    cin >> beenHere;
    if (beenHere == "n")
    {
        cout << "\nDon't you Forget it partner. \n\n"
             << "Paste your jargon here!: ";
        cin >> password;
        fileManager.createFile(password);
        RuleReader::outputTheRules(rules);
    }
    else if (beenHere == "y")
    {
        RuleReader::outputTheRules(rules);
    }
    else
    {
        cout << "Good Day Sir!";
        return 1;
    }

    string composite = std::string("sudo sshpass -f ") + fileManager.getFilename() + " ssh -o StrictHostKeyChecking=no bandit" + level + std::string("@bandit.labs.overthewire.org -p 2220");
    char command[100];
    strcpy(command, composite.c_str());
    system(command);
    return 0;
}
