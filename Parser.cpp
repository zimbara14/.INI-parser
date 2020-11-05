//
//  Parser.cpp
//  Parser3
//
//  Created by Sara Nikolić on 16.09.2020.
//  Copyright © 2020 Sara Nikolić. All rights reserved.
//
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <regex>
#include <algorithm>
#include "Parser.h"

// trim leading white-spaces
static std::string& ltrim(std::string& s) {
    size_t startpos = s.find_first_not_of(" \t\r\n\v\f");
    if (std::string::npos != startpos) {
        s = s.substr(startpos);
    }
    return s;
}

// trim trailing white-spaces
static std::string& rtrim(std::string& s) {
    size_t endpos = s.find_last_not_of(" \t\r\n\v\f");
    if (std::string::npos != endpos) {
        s = s.substr(0, endpos + 1);
    }
    return s;
}

// random trimming
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    std::string strrr = str.substr(first, (last - first + 1));
    std::remove(strrr.begin(), strrr.end(), ' ');
    return strrr;
}

// to get rid of unnecessary comments
static std::string& findComment(std::string& s){
    trim(s);
    size_t index = s.find_first_of(";");
    if(std::string::npos != index){
        s = s.substr(0, index);
    }
    return s;
}

Parser::Parser(){}

Parser::Parser (const std::string& filename, std::map<std::string, std::map<std::string, std::string>> sections){
    this->FileName = filename;
    this->sections = sections;
}

void Parser::makeNewSection(std::string name, std::string key, std::string value){
    sections[name][key] = value;
}

std::pair<std::string, std::string> Parser::PutParametersAndKeys(std::map<std::string, std::map<std::string, std::string>> new_section, std::string line){
    size_t index = line.find_first_of("=");
    if(index != std::string::npos){
        std::string Key = line.substr(0, index);
        std::string Value = line.substr(index + 1);
        ltrim(rtrim(Key));
        ltrim(rtrim(Value));
        return {Key, Value};
    }
    return {" ", " "};
}

bool Parser::is_valid(std::string string){
    auto const reg = std::regex("[^a-zA-Z0-9_]");
    bool const flag = std::regex_search(string, reg);
    return flag;
}

bool Parser::is_value(std::string string){
    auto const reg = std::regex("[^a-zA-Z0-9_.]");
    bool const flag = std::regex_search(string, reg);
    return flag;
}

std::string Parser::determineType(std::string variable){
    auto const reg = std::regex("[^0-9]");
    bool const flag1 = std::regex_search(variable, reg);
    if(flag1) return "double";
    else return "int";
}

template<typename T>
T Parser::changetype(std::string value_){
    T value;
    std::stringstream ss(value_);
//    ss.exceptions(std::ios::failbit);
    ss >> value;
    if(ss.fail() || !ss.eof()) throw std::runtime_error("Error! Invalid conversion.");
    return value;
}

void Parser::findValue(std::string section_name, std::string key_name, std::string key_type){
    if(!(key_type == "string" || key_type == "int" || key_type == "double")){
        throw std::runtime_error("Error! Unknown data type.");
    }
    if(!findSection(section_name)) throw std::runtime_error("Error! This section does not exist.");
    std::string value_ = sections[section_name][key_name];
    if(value_ == "") throw std::runtime_error("Error! Coundln't find the value you were looking for.");
    std::string final_value = value_;
    if(key_type == "string"){
        std::cout << "The found value is... " << final_value << std::endl;
    }
    if(key_type == "int"){
        if(determineType(final_value) == "int"){
            int result = changetype<int>(final_value);
            std::cout << "The found value is... " << result << std::endl;
        } else throw std::runtime_error("Error! Could not convert.");
    }
    if(key_type == "double"){
        if(determineType(final_value) == "double"){
            double result = changetype<double>(final_value);
            std::cout << "The found value is... " << result << std::endl;
        } else throw std::runtime_error("Error! Could not convert.");
    }
}

void Parser::undefinedParameter(std::string string){
    ltrim(rtrim(string));
    std::remove(string.begin(), string.end(), ' ');
    size_t indexofb = string.find_first_of('[');
//    size_t indexofe = string.find('=');
    size_t indexofspace = string.find(' ');
    if(indexofb == std::string::npos && indexofspace == std::string::npos) throw std::runtime_error ("Error! Unknown parameter.");
    trim(string);
}

bool Parser::findSection(std::string section_name){
    auto it = sections.begin();
    for( ; it != sections.end(); ++it){
        if(it->first == section_name) return true;
    }
    return false;
}

void Parser::parse(const std::string& filename){
    size_t indexofini = filename.find(".ini");
    if(indexofini == std::string::npos) throw std::runtime_error("Error! Invalid file format.");
    std::ifstream in;
    in.open(filename);
    if(!in.is_open()) throw std::runtime_error("Error! Couldn't open the file.");
    for(std::string line; std::getline(in, line); ){
       
        findComment(line);
        ltrim(rtrim(line));
        trim(line);
        if(line.empty()) continue;
        if(line[0] == ';') continue; // we are ignoring line comment - all the time!!

        if(line[0] == '['){
            
            size_t endi = line.find_first_of(']');
            if(endi != std::string::npos){
                std::string name = line.substr(1, endi - 1);
                if(is_valid(name)) throw std::runtime_error("Error! Invalid name of a section.");
                if(findSection(name)) throw std::runtime_error("Error! A section with the same name already exists."); // checking for section duplicates and error-ing
                
                for(std::string line2; std::getline(in, line2); ){
                    findComment(line2);
                    ltrim(rtrim(line2));
                    if(line2[0] == ';' || line2.empty()) break;
                    if(line2[0] == '[' || line2.find_first_of('[') != std::string::npos) throw std::runtime_error ("Error! Sections must be separated with an empty line.");
                    std::pair<std::string, std::string> new_pair;
                    if(!line2.empty() && !(line2.find('[') != std::string::npos || line2.find(']') != std::string::npos) && line2.find('=') != std::string::npos) new_pair = PutParametersAndKeys(sections, line2);
                    else throw std::runtime_error("Error! Something went wrong with parameters and keys.");
                    
                    if(is_valid(new_pair.first)) throw std::runtime_error("Error! Invalid name of a key.");
                    if(is_value(new_pair.second)) throw std::runtime_error("Error! Invalid name of a value.");
                    
                    if(new_pair.first != " " && new_pair.second != " ") makeNewSection(name, new_pair.first, new_pair.second);                    
                }
            } else {
                throw std::runtime_error ("Error! Invalid name of a section.");
            }
        }
        undefinedParameter(line); // neither section nor comment
    }
}

void Parser::printSections(){
    std::cout << "Table of content...\n";
    for (auto const &[k1, v1] : sections) {
        std::cout << " * " << k1 << " -> ";
        for (auto const &[k2, v2] : v1) {
            if (&k2 != &v1.begin()->first)
                std::cout << "; ";
            std::cout << k2 << " = " << v2;
        }
        std::cout << std::endl;
    }
}
