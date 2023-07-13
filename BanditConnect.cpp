#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>

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
int main()
{
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
    FileManager fileManager = FileManager(level);
    cout << "\n\nReckon you've been here before? y/n: ";
    cin >> beenHere;
    if (beenHere == "n")
    {
        cout << "\nDon't you Forget it partner. \n\n"
             << "Paste your jargon here!: ";
        cin >> password;

        cout << "\nConnecting...\n";

        fileManager.createFile(password);
    }
    else if (beenHere == "y")
    {
        cout << "\nConnecting...\n";
    }
    else
    {
        cout << "Good Day Sir!";
        return 1;
    }

    string composite = std::string("sudo sshpass -f ") + fileManager.getFilename() + " ssh bandit" + level + std::string("@bandit.labs.overthewire.org -p 2220");

    char command[100];
    strcpy(command, composite.c_str());

    system(command);
    system("pause");

    return 0;
}
