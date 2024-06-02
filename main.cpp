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
    if(str == "print") return eprint;
    if(str == "validate") return evalidate;
    if(str == "search") return efind;
    if(str == "set") return emodify;
    if(str == "create") return ecreate;
    if(str == "delete") return edelete;
    if(str == "move") return emove;
    return -1;
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
            case evalidate:
                if(_displayData()== true){
                    Messenger::LogInfo("Current data is", "\033[1;32mvalid\033[m");
                }else{
                    Messenger::LogInfo("Current data is", "\033[1;31minvalid\033[m");
                }
            break;
            case eprint:
                print();
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
            case efind:
                if(cmdSplit.size() != 2 ){
                    Messenger::LogError("This function takes one argument");
                    break;
                }
                search(cmdSplit[1]);
            break;
            case emodify:
                 if(cmdSplit.size() != 3 ){
                    Messenger::LogError("This function takes two argument");
                    break;
                }
                set(cmdSplit[1],cmdSplit[2]);
            break;
            case ecreate:
              if(cmdSplit.size() != 3 ){
                    Messenger::LogError("This function takes two argument");
                    break;
                }
                create(cmdSplit[1],cmdSplit[2]);
            break; 
            case emove:
              if(cmdSplit.size() ==2){
                    move(cmdSplit[1],"");
                }else if(cmdSplit.size() == 3){
                    move(cmdSplit[1],cmdSplit[2]);

                }else{
                       Messenger::LogError("This function takes one or two argument");
                 break;
                }
            break;
            case edelete:
                if(cmdSplit.size() != 2 ){
                    Messenger::LogError("This function takes one argument");
                    break;
                }
                _delete(cmdSplit[1]);
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