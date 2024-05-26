#include <iostream>
#include "mainFunctions.cpp"
#include <ctype.h>
#include <iostream>
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
    if(str == "data") return edata;
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
    arr.push_back(s);
    return arr;
}


int main(){

    // std::ifstream file; 
    //myfile.open("shopping_list.txt");
    
    std::string cmd;
    while(true){

    std::cout << "\x1B[96m"<<ds.getFileName()<<"\033[0m" <<"> "; 
    std::getline(std::cin, cmd);
    std::vector<std::string> cmdSplit = splitString(cmd);
    if(cmdSplit.size() <=0) {continue;}
        switch(conversionCmd(cmdSplit[0])){

            case ehelp:
                help(); 
                break;
            case edata:
                if(_displayData()== true){
                    Messenger::LogInfo("Current data is", "\033[1;32mvalid\033[m");
                }else{
                    Messenger::LogInfo("Current data is", "\033[1;31minvalid\033[m");
                }
            break;
            case eopen:
                if(cmdSplit.size() != 2 ){
                    Messenger::LogError("This function takes one argument");
                    break;
                }
                open(cmdSplit[1]);
                break;
            case esave:
                save();
                break;
            case esaveas:
                if(cmdSplit.size() != 2 ){
                    Messenger::LogError("This function takes only one argument");
                    break;
                }
                saveAs(cmdSplit[1]);
                break;
            case eclose:
                close();
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