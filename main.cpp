#include <iostream>
#include "JsonParse.cpp"
#include "mainFunctions.cpp"
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//inspired https://stackoverflow.com/questions/650162/why-cant-the-switch-statement-be-applied-to-strings
int conversionCmd(std::string str){
    for(int i =0 ;i<str.length();i++){
        str[i] = tolower(str[i]);
    }
    if(str == "help") return ehelp;
    if(str == "open") return eopen;
    if(str == "save") return esave;
    if(str == "saveas") return esaveas;
    if(str == "close") return eclose;
    if(str == "exit") return eexit;
    return -1;
}
// implementation https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> splitString(std::string s, std::string splitter = " "){
    if(s.size() <= 0){ return std::vector<std::string>();}
    std::string delimiter = splitter;
    std::vector<std::string> arr;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        arr.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    if(arr.size() ==0) {
        arr.push_back(s);
    }
    return arr;
}


int main(){
    
    std::ifstream file; 
    //myfile.open("shopping_list.txt");
    std::string cmd;
    while(true){
    std::cout << "> "; std::getline(std::cin, cmd);
    std::vector<std::string> cmdSplit = splitString(cmd);
    if(cmdSplit.size() <=0) {continue;}
        switch(conversionCmd(cmdSplit[0])){

            case ehelp:
                help();
                break;
            case eexit:
                exit();
                return 0;
            case -1:
                std::cout << "Invalid command." << std::endl;
            break;
        }
    }
    return 0;
}