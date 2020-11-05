//
//  Parser.h
//  Parser3
//
//  Created by Sara Nikolić on 16.09.2020.
//  Copyright © 2020 Sara Nikolić. All rights reserved.
//

#ifndef Parser_h
#define Parser_h
#include <unordered_map>
#include <list>
#include <vector>
#include <utility>
#include <map>

class Parser{
public:
    std::string FileName;
    std::map<std::string, std::map<std::string, std::string>> sections;
    
    Parser();
    Parser(const std::string& filename, std::map<std::string, std::map<std::string, std::string>> sections);
    void makeNewSection(std::string name, std::string key, std::string value);
    std::pair<std::string, std::string> PutParametersAndKeys(std::map<std::string, std::map<std::string, std::string>> new_section, std::string line);
    void findValue(std::string section_name, std::string key_name, std::string key_type);
    bool findSection(std::string section_name);
    void parse(const std::string& filename);
    void undefinedParameter(std::string string);
    bool is_valid(std::string string);
    bool is_value(std::string string);
    template<typename T> T changetype(std::string value_);
    std::string determineType(std::string variable);
    void printSections();
};

static std::string& ltrim(std::string& s);
static std::string& rtrim(std::string& s);
static std::string& findComment(std::string& s);

#endif /* Parser_h */
