# BanditRepo
"Bandit Over The Wire." supplemental program for password allocation and ease of use.

Requires SSHPASS, libCurl, Gumbo

PreInstallation Instructions:
sudo apt-get install sshpass
sudo apt install libcurl4-openssl-dev libgumbo-dev

Self Compilation Instructions:
g++ "Source File" -lcurl -lgumbo -std=c++11 -o "Output Executable"
