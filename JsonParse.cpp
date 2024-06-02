
#include "JsonObjects.h"
#include "Rooter.h"
#include <cerrno>
#include <iterator>
#include <ostream>
#include <sstream>
#include <fstream>
#include <ios>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

//https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
class Messenger{
    public:
    static void LogError(std::string data){

    std::cout << "\x1B[31m" << data <<"\033[0m"<< std::endl;

    }

    static void LogWarning(std::string data){
    std::cout << "\x1B[33m" << data <<"\033[0m"<< std::endl;

    }

    static void LogInfo(std::string data,std::string info = ""){
    std::cout << "\x1B[36m" << data <<"\033[0m";
    if(info.size()>0){
        std::cout << " " << info;
    }
    std::cout <<std::endl;

    }
};

//https://stackoverflow.com/questions/46535422/c-copying-another-file
class JsonParser{ //privessly called datasave
    private:  
    std::fstream src;
    std::string data;
    std::string fileName;

    // *mainNode;
    // **currentNode;

    int LoadFile(){
        // currentNode = &mainNode;
        std::stringstream buf;
        buf << src.rdbuf();
        data = buf.str();

        //https://stackoverflow.com/questions/556277/trim-remove-a-tab-t-from-a-string
        data.erase(std::remove(data.begin(), data.end(), '\t'), data.end());
    
        return 0;
    }

    //can start as object, 1 value, array


    

    //https://www.json.org/json-en.html
    //objects from docs
    //object
    //array
    //value
    //string
    //number
    //whitespace

    //validates if string is capable of being key or value
    bool validateString(std::string str){
        int quotesNumber = 0;
        // std::cout << "current check:"<<str << " with l: " <<str.length() <<'\n';
        if(str.length() == 0 ) return true;
        for(int i = 0;i<str.length();i++){
            if(str[i] == '\"'){
                quotesNumber++;
            }
        }
        if(quotesNumber ==2 ||quotesNumber == 0){
            return true;
        }

        return false; 
    }
    //validates if string is capable of being key (cantains "")
    bool isItString(std::string str){
        int quotesNumber = 0;
        for(int i = 0;i<str.length();i++){
            if(str[i] == '\"'){
                quotesNumber++;
            }
        }
        if(quotesNumber ==2){
            return true;
        }

        return false; 
    }
    ValueType defineValueString(std::string val, int line){
        // std::cout << "Validating : " << val ;
        if(!validateString(val)){
            Messenger::LogError("Parsing multiple strings as key or value");
                                Messenger::LogInfo("line", std::to_string(line));
            return notFound;
        }
        if(isItString(val)){
            return eString;
        }
    
        if(val == "false" || val == "true") return eBoolean;
        bool isNumber = std::find_if(val.begin(), val.end(), ::isdigit) != val.end();
        if(isNumber) return eNumber;


        return notFound;
    }

    bool soloData = false;
    Objects *root;
    Values *globalTemp;
    bool ValidateJson(Objects *_currentObj = nullptr , int _line = 1, int _index = 0, bool _toReturn = false,int pass = 0,bool expectedObject = false){
        if(!src.is_open())
        {
            Messenger::LogError("No file open");
            return false;
        }
        std::vector<std::string> Foundkeys;
        if(data.size() ==0) {
            Messenger::LogError("No file opened");
            return 0;
        }
        Objects *currentOBJ = nullptr;
        if(_line ==1){
            delete root;
            root = NULL;
        }
        // free(root);
        soloData = false;
        currentOBJ = _currentObj;
        bool inValue;
        bool inKey;
        std::string key = "";
        std::string value = "";
        std::string currentLine= "";
        int line = _line;

        bool inObject = false;
        bool inArray = false;

        bool expectKey =false;
        bool expectVal =false;
        bool ignore = false;
        soloData = false;

        int ignoreIncoming = 0;
        bool ignoreCommingArray = false;
        int ignoreInCommingArr = 0;

        bool expectingNewElement = false;
        bool expectingNewValue = false;
        bool expectingNewKey = false;
        bool justReturned = false;
        if(line >1){

        }else {
            
            if(data[0] != '{' && data[0] !='['){
                inValue= true;
                _index++;
                value+=data[0];
                soloData = true;
                
            }else{
                if(data[0] == '{'){
                    delete root;
                    root = new jsonObject();
                }else{
                    delete root;
                    root = new jsonArray();
                }
            currentOBJ = root;
            }
        }
        
        for(int i =_index;i<data.size();i++){
            switch(data[i]){
                case '\n':
                    line++;
                break;

                case '{':
                    if(ignore == true){
                        ignoreIncoming++;
                        break;
                    }
                    if(expectingNewKey){
                        Messenger::LogError("expected key, but founded object");
                        Messenger::LogInfo("line" , std::to_string(line));
                        return false;
                    }
                    if(inValue){
                        bool temp;
                        if(value.size() >0){
                             Messenger::LogError("Cannot create object after alreading assigning values");
                            Messenger::LogInfo("line" , std::to_string(line));
                            return false;
                        }
                        ignore = true;
                        inValue = false;
                        ignoreIncoming = 2;
                        if(key.size()>0){
                             if(!validateString(key) || !validateString(value))

                            {
                                Messenger::LogError("Parsing multiple strings as key or value");
                                Messenger::LogInfo("line", std::to_string(line));
                                return false;
                            } 
                            ValueType vt = typeOfObject(currentOBJ);
                            
                            switch(vt){
                                case eObject:
                                    if(true){
                                        jsonObject *toPass = new jsonObject(); 
                                        bool temp = ValidateJson(toPass,line, i,true,pass+1);
                                        ((jsonObject*)currentOBJ)->AddPair(key,new vObject(toPass));
                                        if(!temp) return temp;

                                    }
                                break;
                                case eArray:
                                break;
                                default:
                                Messenger::LogError("Something is wrong with checking type");
                                return false;
                                break;
                            }
                        }
                        key = ""; value = "";
                        justReturned = true;
                        break;
                    }else{
                        inKey = true;
                        expectKey = true;   
                        inValue = false;
                    }
                    
                   
                
                break;
                case '}':
                    ignoreIncoming--;
                    if(ignoreIncoming == 1) {
                        ignore = false;   
                    }
                    if(ignore) break;
                     
                    if(key.size()>0){
                            if(!validateString(key) || !validateString(value))
                            {
                                Messenger::LogError("Parsing multiple strings as key or value");
                                Messenger::LogInfo("line", std::to_string(line));
                                return false;
                            } 
                            ValueType vt = typeOfObject(currentOBJ);

                            switch(vt){
                                case eObject:
                                    if(true){
                                        Values *v;
                                        switch(defineValueString(value,line)){
                                            case eString:
                                            v= new vString(value);
                                            break;
                                            case eNumber:
                                            v = new vNumber(std::stod(value));
                                            break;
                                            case eBoolean:
                                            if(value == "false") v = new vBoolean(false);
                                            else v = new vBoolean(true);
                                            break;
                                            default:
                                            Messenger::LogError("Passing data cannot be defined");
                                            return false;
                                            break;
                                        }
                                        ((jsonObject*)currentOBJ)->AddPair(key,v);
                                        
                                    }
                                break;
                                case eArray:
                                break;
                                default:
                                Messenger::LogError("Something is wrong with checking type");
                                return false;
                                break;
                            }
                        key = ""; value = "";
                    }  
                    if(justReturned == false){

                        // *used when displaying data while testing validator
                        // ::cout <<std::string(pass,'\t');
                        // std::cout <<pass<<"}\n"; 
                    }
                    justReturned = false;
                    if(_toReturn) return true;


                    inKey = false;
                    inValue = false;


                break;


                case ':':
                    if(ignoreCommingArray) break;
                    if(ignore) break;
                    if(key.size()==0){
                        Messenger::LogError("no key found. Check if comma is missing");
                        Messenger::LogInfo("line", std::to_string(line));
                        return false;
                    }
                    Foundkeys.push_back(key);

                    inKey = false;
                    inValue = true;
                    expectingNewValue = true;
                    expectingNewKey = false;
                    
                break;
                case ',':
                    if(ignoreCommingArray) break;
                    if(ignore) break;
                    if(inKey){
                        Messenger::LogError("expected key, but none found");
                        Messenger::LogInfo("line", std::to_string(line));
                        return false;
                    }
                    if(key.size()>0 && value.size() ==0){
                         Messenger::LogError("key with no value found");
                        Messenger::LogInfo("line", std::to_string(line));
                        return false;
                    }

                    if(inValue){
                            if(!validateString(key) || !validateString(value))
                            {
                                Messenger::LogError("Parsing multiple strings as key or value");
                                Messenger::LogInfo("line", std::to_string(line));
                                return false;
                            } 
                        
                        
                        switch(typeOfObject(currentOBJ)){
                            case eObject:
                            if(true){
                                Values *v;
                                 switch(defineValueString(value,line)){
                                            case eString:
                                            v= new vString(value);
                                            break;
                                            case eNumber:
                                            v = new vNumber(std::stod(value));
                                            break;
                                            case eBoolean:
                                            if(value == "false") v = new vBoolean(false);
                                            else v = new vBoolean(true);
                                            break;
                                            default:
                                            Messenger::LogError("Passing data cannot be defined");
                                            return false;
                                            break;
                                    }
                                ((jsonObject*)currentOBJ)->AddPair(key,v);
                            }
                            break;
                            case eArray:
                            break;
                        }
                        key = ""; value = "";
                    }
                    if(inArray){expectingNewElement = true;}
                    else {expectingNewKey = true;}

                    // std::cout << pass<<",\n";
                    inKey = true;
                    inValue = false;
                  
                break;
                case '[':
                    // TODO: IMPLEMENT...

                    // if(ignore) break;
                    // if(key.size() > 0){
                    //     std::cout <<pass<< key << ":";
                    //     key = "";
                    //     value = "";
                    // }
                    // std::cout << "[\n";
                    // ignoreCommingArray = true;
                    // ignoreInCommingArr=2;

                break;
                case ']':
                    // std::cout <<pass<< "]\n";
                    // if(ignore) break;
                    // ignoreInCommingArr--;
                    // if(ignoreCommingArray == 1){
                    //     ignoreCommingArray = false;
                    // }

                break;

            
                default:
                    if(inKey){
                        key += data[i];
                    }
                    else if(inValue){
                        value += data[i];
                    }    
                break;
                
            }
            
        }
        if(soloData){
            // value = data;
            switch(defineValueString(value,line)){
                case eString:
                globalTemp= new vString(value);
                break;
                case eNumber:
                globalTemp= new vNumber(std::stod(value));
                break;
                case eBoolean:
                if(value == "false") globalTemp = new vBoolean(false);
                else globalTemp = new vBoolean(true);
                break;
                default:
                Messenger::LogError("Passing data cannot be defined");
                return false;
                break;
            }
        }
        for(int i =0 ;i <Foundkeys.size()-1;i++){
            for(int j=i+1; j<Foundkeys.size();j++){
                if(Foundkeys[i] == Foundkeys[j]){
                    Messenger::LogError("Multiple keys with the same name");
                    delete root;
                    root = nullptr;
                    return false;
                } 
            }
        }



        return true;
    }


    ValueType typeOfValue(Values* val){
        if(dynamic_cast<vString*>(val) != nullptr) return eString;
        if(dynamic_cast<vNumber*>(val) != nullptr) return eNumber;
        if(dynamic_cast<vObject*>(val) != nullptr) return eObject;
        if(dynamic_cast<vArray*>(val) != nullptr) return eArray;
        if(dynamic_cast<vBoolean*>(val) != nullptr) return eBoolean;
        return notFound;
    }   

    ValueType typeOfObject(Objects* obj){
        if(dynamic_cast<jsonObject*>(obj) != nullptr) return eObject;
        if(dynamic_cast<jsonArray*>(obj) != nullptr) return eArray;
        return notFound;
    }
    bool _displayTree(){
        std::cout << "Displaying data:" << std::endl;
        ((jsonObject*)root)->print(std::cout, 0);

        return 0;
    }

    public:
    std::string getFileName(){
        return fileName;
    }
    int open = 0;
    int openFile(std::string file){  
        if(src.is_open()){
            Messenger::LogWarning("You need to close the file, before opening new one");
            return 2;
        } 
        src.open(file, std::ios::in | std::ios::out);
        if(src.fail()){
            Messenger::LogError("File failed to open");
            return 1;
        }
        open++;
        Messenger::LogInfo("Opened:", file);
        fileName = file;
        LoadFile();
        return 0;
    }
    int count;
    int DisplayTree(){
        

        if(soloData){
            globalTemp->print(std::cout,0);    
            return 0;
        }
        if(root == nullptr){
            Messenger::LogError("First use \"validate\" before printing");
            return 0;
        }
        return _displayTree();

        return 0;
    }
    int validateData(){

        return ValidateJson(root);
   
        return 0;
    }
    bool isValidated(bool display = true){
        if(root == nullptr){
            if(display == true)Messenger::LogError("Need to validate data before modifying");
            return false;
        }
        return true;
    }
    
    Pair* SearchForKey(std::string key, Objects* _currentObj= nullptr){
        if(!isValidated())return new Pair("",nullptr);
        if(_currentObj == nullptr) _currentObj = root;
        for(int i= 0;i<_currentObj->Size();i++){
            Pair* p = ((jsonObject*)_currentObj)->ReturnPair(i);
            if(p->key == key){
                return p;
            }
            if(typeOfValue(p->value)==eObject){
                jsonObject* cur = (jsonObject*)((vObject*)(p->value))->getData(0); 
                Pair* temp = SearchForKey(key,cur);
                if(temp->key != "") return temp;
            }
        }
        return new Pair("", nullptr);
    }
      Pair* SearchForKey(std::vector<std::string> key,int index= 0, Objects* _currentObj= nullptr){
        if(!isValidated())return new Pair("",nullptr);
      
        if(_currentObj == nullptr) _currentObj = root;
        for(int i= 0;i<_currentObj->Size();i++){
            Pair* p = ((jsonObject*)_currentObj)->ReturnPair(i);
            
            if(p->key == key[index] ){
                if (index == key.size()-1) return p;
                if(typeOfValue(p->value)==eObject){
                    jsonObject* cur = (jsonObject*)((vObject*)(p->value))->getData(0); 
                    Pair* temp = SearchForKey(key,index+1,cur);
                    if(temp->key != "") return temp;
                }
            }
        }
        return new Pair("", nullptr);
    }
    int removePair(std::string key, Objects* _currentObj= nullptr){
        if(!isValidated())return 1;
        if(_currentObj == nullptr) _currentObj = root;
        for(int i= 0;i<_currentObj->Size();i++){
            Pair* p = ((jsonObject*)_currentObj)->ReturnPair(i);
            if(p->key == key){
                 ((jsonObject*)_currentObj)->RemovePair(key);
                
                return 0;
            }
            if(typeOfValue(p->value)==eObject){
                jsonObject* cur = (jsonObject*)((vObject*)(p->value))->getData(0); 
                Pair* temp = SearchForKey(key,cur);
                if(temp->key != "") return 0;
            }
        }
        return 1;
    }
    int removePair(std::vector<std::string> key,int index= 0, Objects* _currentObj= nullptr){
     
        if(!isValidated())return 1;
          if(_currentObj == nullptr) _currentObj = root;
        for(int i= 0;i<_currentObj->Size();i++){
            Pair* p = ((jsonObject*)_currentObj)->ReturnPair(i);
            
            if(p->key == key[index] ){
                if (index == key.size()-1) {
                    ((jsonObject*)_currentObj)->RemovePair(key[index]);
                    return 0;
                }
                if(typeOfValue(p->value)==eObject){
                    jsonObject* cur = (jsonObject*)((vObject*)(p->value))->getData(0);
                    int temp = removePair(key,index+1,cur);
                    if(temp == 0) return 0;
                }
            }
        }
        return 1;
    }

    int addPair(std::string key,std::string value, Objects* _currentObj= nullptr){
        if(!isValidated())return 1;
        if(_currentObj == nullptr) _currentObj = root;
        for(int i= 0;i<_currentObj->Size();i++){
            Pair* p = ((jsonObject*)_currentObj)->ReturnPair(i);
            if(p->key == key){
                return 1;
            }
        }
        ((jsonObject*)_currentObj)->AddPair(key, new vString(value));
        return 0;
    }
     int addPair(std::vector<std::string> key,std::string value,int index =0, Objects* _currentObj= nullptr){
        if(!isValidated())return 1;
          if(_currentObj == nullptr) _currentObj = root;
          Pair *p;
            for(int i= 0;i<_currentObj->Size();i++){
            p = ((jsonObject*)_currentObj)->ReturnPair(i);
            
            if(p->key == key[index] ){
                if (index == key.size()-1) {
                    Messenger::LogError("Key already exist");
                    return -1;
                }
                if(typeOfValue(p->value)==eObject){
                    jsonObject* cur = (jsonObject*)((vObject*)(p->value))->getData(0);
                    int temp = addPair(key,value,index+1,cur);
                    if(temp == -1) return 1;
                    else{
                        if(index == key.size()-2){
                            ((jsonObject*)cur)->AddPair(key[index+1],new vString(value));
                           return 0;

                        }
                    }
                }
            }
            }   
        
        return 1;
    }
       int movePair(std::vector<std::string> key,std::vector<std::string> to,Pair* p= nullptr,int index =0, Objects* _currentObj= nullptr){
        if(!isValidated())return 1;
        if(_currentObj ==nullptr) _currentObj = root;
        

        p = SearchForKey(key)->Clone();
        if(p->key == "") return -1;
        if(to.capacity() == 0){
            removePair(key);
            ((jsonObject*)_currentObj)->AddPair(p);
            delete p;
            return 0;
        }else{  
                Pair* pair = SearchForKey("\"" +to[to.size()-1]+ "\"");
                if(typeOfValue(pair->value) == eObject){
                    ((jsonObject*)pair->value->getData())->AddPair(p);
                    removePair(key);                
                }else{
                    delete p;
                    return 1;

                }
               

                // delete pair; // dont delete because is reference
            return 0;
        }
        delete p;
        
        return 1;
    }
    //https://stackoverflow.com/questions/53359727/how-to-save-and-also-read-c-fstream-file-without-closing-it  
    int saveFile(){  
        if(!isValidated()){
            return 0;
        };
        if(!src.is_open()){
            Messenger::LogError("No file currently opened");
            return -1;

        }
        src.close();
        src.open(fileName, std::ofstream::out | std::ofstream::trunc);
        root->print(src, 0);
        src.close();
        openFile(fileName);
        // src.flush();
        Messenger::LogInfo("Successful saved.");
        return 0;
    }
    int saveAsFile(std::string file){  
        if(!isValidated())return false;

        std::ofstream output;
        Messenger::LogInfo("Saving data to:", file);
        output.open(file, std::ios::out);
        if(output.fail()){
            Messenger::LogError("File failed to open");
            return 1;
        }
        root->print(output, 0);
        // output << src.rdbuf();
        output.close();
        Messenger::LogInfo("Successful saved.");
        return 0;
    }
    
    int closeFile(bool displayError = true){

        if(!src.is_open() && displayError){
            Messenger::LogError("No file currently opened");
            return -1;
        } 
        if(src.is_open()){
            // root = nullptr;
            delete root;
            root = NULL;
            src.close();
            Messenger::LogInfo("Closed file", fileName);
            fileName = "";
            // std::cout << (root==NULL);
        }
        return 0;
    }
};



