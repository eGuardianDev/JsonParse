#include <iostream>
#include "JsonParse.cpp"
#include "mainFunctions.cpp"
#include <ctype.h>

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

int main(){
    std::string cmd;
    while(true){
    std::cout << "> "; std::getline(std::cin, cmd);
        switch(conversionCmd(cmd)){

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