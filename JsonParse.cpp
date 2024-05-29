
#include "JsonObjects.h"
#include "Rooter.h"
#include <cerrno>
#include <ostream>
#include <sstream>
#include <fstream>
#include <ios>
#include <string>
#include <iostream>
#include <algorithm>

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
       // std::cout <<data;
       // data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
       // data = CleanSpaces(data);
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

    Objects *root;
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

    bool generateTree(Objects* _currentObj, std::string key, std::string value ){
        return true;
    }
    bool soloData = false;
    Values *globalTemp;
    bool ValidateJson(Objects *_currentObj = nullptr , int _line = 1, int _index = 0, bool _toReturn = false,int pass = 0,bool expectedObject = false){
        // std::cout << "trie";
        //object current
        
        Objects *currentOBJ;
        currentOBJ = _currentObj;
        bool inValue;
        bool inKey;
        std::string key;
        std::string value;
        std::string currentLine;
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
            
            delete root;
            root = new jsonObject();
            if(data[0] != '{' && data[0] !='['){
                inValue= true;
                _index++;
                value+=data[0];
                soloData = true;
                
            }else{
                if(data[0] == '{'){
                    root = new jsonObject();
                }else{
                    root = new jsonArray();
                }
            }
            currentOBJ = root;
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
                            std::cout << "this thing" << key;   
                            // std::cout << pass<<" "<<key << ":\n";
                             if(!validateString(key) || !validateString(value))

                            {
                                Messenger::LogError("Parsing multiple strings as key or value");
                                Messenger::LogInfo("line", std::to_string(line));
                                return false;
                            } 
                            std::cout <<std::string(pass,'\t');
                            ValueType vt = typeOfObject(currentOBJ);
                            switch(vt){
                                case eObject:
                                    if(true){
                                        // std::cout << "creating object";
                                        jsonObject *toPass = new jsonObject(); 
                                        ((jsonObject*)currentOBJ)->AddPair(key,new vObject(toPass));
                                        bool temp = ValidateJson(toPass,line, i,true,pass+1);
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
                    std::cout <<std::string(pass,'\t');

                    std::cout << pass<<"{\n";
                    
                   
                
                break;
                case '}':
                    ignoreIncoming--;
                    if(ignoreIncoming == 1) {
                        ignore = false;   
                    }
                    if(ignore) break;
                     
                    if(key.size()>0){
                        std::cout << "this things:: " <<key;
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
                        std::cout <<std::string(pass,'\t');
                        std::cout <<pass<<"}\n";
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

                    inKey = false;
                    inValue = true;
                    expectingNewValue = true;
                    expectingNewKey = false;
                    
                break;
                case ',':
                    std::cout << "founded this";
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
                        
                        
                        std::cout <<std::string(pass,'\t');
                        switch(typeOfObject(currentOBJ)){
                            case eObject:
                            if(true){
                                Values *v;
                                 switch(defineValueString(value,line)){
                                            case eString:
                                            v= new vString(value);
                                            std::cout << *(std::string*)v->getData();
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
                                 std::cout << key <<" : " << value;
                            }
                            break;
                            case eArray:
                            break;
                        }
                        std::cout <<pass << " "<< key << ":" << value << "";
                        key = ""; value = "";
                    }
                    if(inArray){expectingNewElement = true;}
                    else {expectingNewKey = true;}

                    std::cout << pass<<",\n";
                    inKey = true;
                    inValue = false;
                  
                break;
                case '[':
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
            // std::cout << value;
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
        std::cout << "displaying tree" << std::endl;
        ((jsonObject*)root)->print(std::cout, 0);
        // jsonObject *jo = new jsonObject();
        // jo->AddPair("asd", new vNumber(12));
        // jo->AddPair("asd", new vNumber(12));
        // jo->print(std::cout);
        // root = new jsonObject();
        // ((jsonObject*)root)->AddPair("key", new vString("\"cat\""));
        // root->print(std::cout);
        // std::cout << typeOfObject(root);
        // root = new jsonObject();
        // jsonObject *jo = new jsonObject();
        // jo->AddPair("nutss",new vString("lol"));
        // ((jsonObject*)root)->AddPair(new Pair("cat",new vObject(jo)));
        // ((jsonObject*)root)->AddPair(new Pair("val",new vString("nut")));
        // ((jsonObject*)root)->AddPair(new Pair("key",new vNumber(12)));
        // // std::cout << *(double*)((jsonObject*)root)->ReturnPair("key")->value->getData();
        // root->print(std::cout);
        // jo->print(std::cout);
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
        std::cout << data;
        return 0;
    }
    int DisplayTree(){
        if(soloData){
            globalTemp->print(std::cout,0);    
            return 0;
        }
        return _displayTree();
        return 0;
    }
    int displayData(){
        // std::cout << data <<std::endl;
        //GenerateDataTree();
       // validateJson();
        if(root !=nullptr){
            // delete root;
            root = nullptr;
        }
      
        // if(root == nullptr && open >1){
            // std::cout << "null";
            // return 0;
        // }else{
            return ValidateJson(root);
        // }
        
    }

    //https://stackoverflow.com/questions/53359727/how-to-save-and-also-read-c-fstream-file-without-closing-it  
    int saveFile(){  
        if(!src.is_open()){
            Messenger::LogError("No file currently opened");
            return -1;

        } 
        src.flush();
        Messenger::LogInfo("Successful saved.");
        return 0;
    }
    int saveAsFile(std::string file){  
        std::ofstream output;
        Messenger::LogInfo("Saving data to:", file);
        output.open(file, std::ios::out);
        if(output.fail()){
            Messenger::LogError("File failed to open");
            return 1;
        }
        output << src.rdbuf();
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
            
            // delete root;
            src.close();
            Messenger::LogInfo("Closed file", fileName);
            fileName = "";
        }
        return 0;
    }
};



