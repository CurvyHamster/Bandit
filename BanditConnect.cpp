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

int main()
{
    // int input;
    string level;
    string password;
    string knowsPassword;
    cout << "Giddy Up Bandit! What level are you progressing too?" << endl;
    cout << "----------------------" << endl;

    cin >> level;

    cout << "Been here before? y/n ";
    cin >> knowsPassword;
    if (knowsPassword == "n")
    {
        cout << "Don't you Forget it partner. \n" << "Type your jargon here!: ";
        cin >> password;

        cout << "\nConnecting...\n"
    }
    else (knowsPassword == "y")
    {
        cout << "\nConnecting...\n";
    }
    
         << endl;
    
    string filename = createFile(level, password);
    cout << filename;
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

