//
//  main.cpp
//  Parser3
//
//  Created by Sara Nikolić on 16.09.2020.
//  Copyright © 2020 Sara Nikolić. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <utility>
#include <fstream>

#include "Parser.h"

int main() {
    
    try{
        Parser hello;
        std::string filename;
        std::cout << "Input the name of the .ini file:\n";
        std::cin >> filename;
        hello.parse(filename);
        std::cout << "What do you want to get from .INI file?\n(Also, remember that the data is case sensitive!)\n";
        std::cout << "Enter key name, and its data type (in one line, with one space, and data type must be typed in lowercase): \n";
        std::string key_name, section_name, key_type;
        std::cin >> key_name >> key_type;
        std::cout << "Enter section name:\n";
        std::cin >> section_name;
        hello.findValue(section_name, key_name, key_type);
    } catch(std::runtime_error error){
        std::cout << "Runtime error occured...\n" <<  error.what() << std::endl;
    }
    
    return 0;
}
