#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <fstream>

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
    string knowsPassword;
    string filename;

    cout << "Giddy Up Bandit! What level are you progressing to?" << endl;
    cout << "----------------------" << endl;

    cin >> level;

    cout << "\n\nReckon you've been here before? y/n: ";
    cin >> knowsPassword;
    if (knowsPassword == "n")
    {
        cout << "\nDon't you Forget it partner. \n\n"
             << "Paste your jargon here!: ";
        cin >> password;

        cout << "\nConnecting...\n";
        filename = createFile(level, password);
    }
    else if (knowsPassword == "y")
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
