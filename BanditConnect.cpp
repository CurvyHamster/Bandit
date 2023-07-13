#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

string createFile(string level, string password)
{
    string filename = level + "password.txt";
    std::ofstream outfile(filename, std::ofstream::out);
    outfile << password;
    return filename;
}
string readfile(string level)
{
    string filename = level + "password.txt";
    return filename;
}

int main()
{
    // int input;
    string level;
    string password;
    string beenHere;
    string filename;

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
                std::cerr << "Give it another go. I don't know where you're heading to smuggler." << '\n';
            }
        }
    }

    cout << "\n\nReckon you've been here before? y/n: ";
    cin >> beenHere;
    if (beenHere == "n")
    {
        cout << "\nDon't you Forget it partner. \n\n"
             << "Paste your jargon here!: ";
        cin >> password;

        cout << "\nConnecting...\n";
        filename = createFile(level, password);
    }
    else if (beenHere == "y")
    {
        cout << "\nConnecting...\n";
        filename = readfile(level);
    }
    else
    {
        cout << "Good Day Sir!";
        return 1;
    }

    // stringstream stream;
    // stream << input;
    // stream >> level;

    string composite = std::string("sudo sshpass -f ") + filename + " ssh bandit" + level + std::string("@bandit.labs.overthewire.org -p 2220");

    char command[100];
    strcpy(command, composite.c_str());

    system(command);
    system("pause");

    return 0;
}
