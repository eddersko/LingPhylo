//
//  UserSettings.cpp
//  LingPhylo
//
//  Created by Edwin Ko on 10/1/21.
//

#include "UserSettings.hpp"
#include <iostream>
#include <vector>
#define DEBUGMODE

UserSettings::UserSettings(void) {
    filename = "";
    chainLength = 0;
    isInitialized = false;
}

void UserSettings::initialize(int argc, char* argv[]) {
    
    if (isInitialized == true)
        return;
    
    std::vector<std::string> commands;

#if defined(DEBUGMODE)
    commands.push_back(argv[0]);
    commands.push_back("-f");
    commands.push_back("/Users/edwinko/Desktop/wals-sherzer.nex");
    commands.push_back("-l");
    commands.push_back("100000");
#else
    for (int i = 0; i < argc; i++)
        commands.push_back(argv[i]);
    
#endif
    if (commands.size() % 2 == 0) {
        std::cout << "Incorrectly formatted command string" << std::endl;
        exit(1);
    }
    
    std::string cmd = "";
    
    for (int i = 1; i < commands.size(); i++) {
        if (cmd == "") {
            cmd = commands[i];
        } else {
            if (cmd == "-f") {
                filename = commands[i];
            } else if (cmd == "-l") {
                chainLength = stoi(commands[i]);
        } else {
                std::cout << "Did not understand command " << cmd << std::endl;
                exit(1);
            }
            cmd = "";
        }
    }
    
    isInitialized = true;
}


std::string UserSettings::getFileName(void){
    return filename;
}

void UserSettings::print(void) {
    std::cout << "The filename is " << filename << " and the chain length is " << chainLength << std::endl;
}
