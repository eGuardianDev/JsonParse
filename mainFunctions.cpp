
#include <iostream>
#include <vector>
#include "JsonObjects.h"
#include "JsonParse.cpp"
#include "Rooter.h"
enum cmdData{
    ehelp = 0,
    eopen = 1,
    esave =2,
    esaveas = 3,
    eclose = 4,
    eexit = 5,
    eprint = 6,
    evalidate= 7,
    efind = 8,
    emodify = 9,
    ecreate = 10,
    edelete = 11,
    emove = 12
};

JsonParser ds;

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

//https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
int help(){
    std::cout << "\x1B[32m\t" << "help" << "\033[0m" << " - shows you functions" << std::endl;
    std::cout << "\x1B[32m\t" << "open \x1B[96m{files-path}" << "\033[0m" << " - opens json file" << std::endl;
    std::cout << "\x1B[32m\t" << "data" << "\033[0m" << " - display currently load data" << std::endl;
    std::cout << "\x1B[32m\t" << "save" << "\033[0m" << " - saves currently opened  file" << std::endl;
    std::cout << "\x1B[32m\t" << "saveas \x1B[96m{files-path}" << "\033[0m" << " - saves currently opened file as new file" << std::endl;
    std::cout << "\x1B[32m\t" << "close" << "\033[0m" << " - closes currently opened file" << std::endl;
    std::cout << "\x1B[32m\t" << "exit" << "\033[0m" << " - exits program" << std::endl;
    return 0;
}

int open(std::string filePath){
    return ds.openFile(filePath);
}

int save(){
    return ds.saveFile();
}

int saveAs(std::string filePath){
    return ds.saveAsFile(filePath);
}
int close(){
    return ds.closeFile();
}
int _displayData(){
    return ds.validateData();
}
int print(){
    int a = ds.DisplayTree();
    std::cout << '\n';
    return a;
}

   
int search(std::string key){
   
    std::vector<std::string> keysplit = splitString(key, "/"); 
    if(keysplit.size() ==0) {
        Messenger::LogError("Passed data to be search is invalid");
        return 0;
    }
    if(keysplit.size() == 1){

        std::string newKey = "\"" + keysplit[0] + "\"";
        Pair* p = ds.SearchForKey(newKey);
        if(p->key != ""){
            std::cout << p->key <<":";
            p->value->print(std::cout, 0);
        }else{
            Messenger::LogInfo("Element not found: ", key);
            return 1;
        }
    }else{
        for(int i =0 ;i <keysplit.size();i++){
            keysplit[i] = '\"' + keysplit[i] +'\"';
        }
        

        Pair* p = ds.SearchForKey(keysplit);
        if(p->key != ""){
            std::cout << p->key <<":";
            p->value->print(std::cout, 0);
        }else{
            Messenger::LogInfo("Element not found: ", key);
            return 1;
        }
    }
    return 0;
}

int set(std::string key, std::string value){

    std::vector<std::string> keysplit = splitString(key, "/"); 
    if(keysplit.size() ==0) {
        Messenger::LogError("Passed data to be search is invalid");
        return 0;
    }
    if(keysplit.size() == 1){
        std::string newKey = "\"" + keysplit[0] + "\"";
        
        Pair* p = ds.SearchForKey(newKey);
        if(p->key != ""){
            
            // std::cout << p->key <<":";
            // p->value->print(std::cout, 0);
            delete p->value;
            p->value = new vString(value);
            Messenger::LogInfo("Set successful!");

        }else{
            Messenger::LogInfo("Element not found: ", key);
            return 1;
        }
    }else{
        for(int i =0 ;i <keysplit.size();i++){
            keysplit[i] = '\"' + keysplit[i] +'\"';
        }
        

        Pair* p = ds.SearchForKey(keysplit);
        if(p->key != ""){
            // std::cout << p->key <<":";

            delete p->value;
            p->value = new vString(value);
            Messenger::LogInfo("Set successful!");
            // p->value->print(std::cout, 0);
        }else{
            Messenger::LogInfo("Element not found: ", key);
            return 1;
        }
    }
    return 0;
}

int _delete(std::string key){
    
    std::vector<std::string> keysplit = splitString(key, "/"); 
    if(keysplit.size() ==0) {
        Messenger::LogError("Passed data to be search is invalid");
        return 0;
    }
    if(keysplit.size() == 1){
        std::string newKey = "\"" + keysplit[0] + "\"";
       
        int a =ds.removePair(newKey);
        if(a == 0)  {          
            Messenger::LogInfo("delete successful!");

        }else{
            Messenger::LogInfo("Element not found: ", key);
            return 1;
        }
    }else{
        for(int i =0 ;i <keysplit.size();i++){
            keysplit[i] = '\"' + keysplit[i] +'\"';
        }
        
        int a =ds.removePair(keysplit);
        if(a == 0)  {          
            Messenger::LogInfo("delete successful!");

        }else{
            Messenger::LogInfo("Element not found: ", key);
            return 1;
        }
        
    }
    return 0;
}

int create(std::string key, std::string value){
    std::vector<std::string> keysplit = splitString(key, "/"); 
    if(keysplit.size() ==0) {
        Messenger::LogError("Passed data to be search is invalid");
        return 0;
    }
    if(keysplit.size() == 1){
       
        std::string newKey = "\"" + keysplit[0] + "\"";
        int a =ds.addPair(newKey,value);
        if(a == 0)  {          
            Messenger::LogInfo("Created successful!");

        }else{
            Messenger::LogInfo("Cannot create element, because already exist: ", key);
            return 1;
        }
    }else{
        for(int i =0 ;i <keysplit.size();i++){
            keysplit[i] = '\"' + keysplit[i] +'\"';
        }
             
        int a =ds.addPair(keysplit,value);
        if(a == 0)  {          
            Messenger::LogInfo("created successful!");

        }else{
            Messenger::LogInfo("Element not found: ", key);
            return 1;
        }
        
    }
    return 0;
}

int move(std::string from ,std::string to){
    
    std::vector<std::string> keysplit1 = splitString(from, "/"); 
    std::vector<std::string> keysplit2;
 
    if(to.length() !=0){
         keysplit2 = splitString(to, "/"); 
    }
    
    if(keysplit1.size() ==0) {
        Messenger::LogError("Passed data to be search is invalid");
        return 0;
    }
    // for(int i =0 ;i <keysplit2.size();i++){
    //     keysplit2[i] = '\"' + keysplit2[i] +'\"';
    // }
    for(int i = 0;i<keysplit2.size();i++){
        if(keysplit1[keysplit1.size()-1] == keysplit2[i]){
            
            Messenger::LogError("Cannot pass object to its child");
            return 1;

        }
    }
       for(int i =0 ;i <keysplit1.size();i++){
        keysplit1[i] = '\"' + keysplit1[i] +'\"';
    }
    
    int a = ds.movePair(keysplit1, keysplit2);
    if( a ==0){
        Messenger::LogInfo("Moved successful");
    }else if (a==-1){
        Messenger::LogError("Moved failed, key missing");
    }else{
        Messenger::LogError("Moved failed, check key values");

    }
    return 0;
    // return a;
}


int exit(){
    ds.closeFile(false);
    return 0;
}