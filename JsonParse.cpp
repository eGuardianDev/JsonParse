
#include "JsonObjects.h"
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

    bool ValidateJson(Objects *_currentObj = nullptr , int _line = 1, int _index = 0, bool _toReturn = false,int pass = 0,bool expectedObject = false){
        //object current
        
        Objects *currentOBJ = _currentObj;
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
        bool soloData = false;

        int ignoreIncoming = 0;
        bool ignoreCommingArray = false;
        int ignoreInCommingArr = 0;

        bool expectingNewElement = false;
        bool expectingNewValue = false;
        bool expectingNewKey = false;
        bool justReturned = false;

        if(data[0] != '{' && data[0] !='['){
            inValue= true;
            _index++;
            value+=data[0];
            soloData = true;
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
                            std::cout <<std::string(pass,'\t');
                            std::cout << pass<<" "<<key << ":\n";
                        }
                        key = ""; value = "";
                        bool temp = ValidateJson(currentOBJ,line, i,true,pass+1);
                        if(!temp) return temp;
                        justReturned = true;
                        break;
                    }else{
                        inKey = true;
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
                            if(!validateString(key) || !validateString(value))
                            {
                                Messenger::LogError("Parsing multiple strings as key or value");
                                Messenger::LogInfo("line", std::to_string(line));
                                return false;
                            } 

                        std::cout <<std::string(pass,'\t');
                        std::cout <<pass << " "<< key << ":" << value << std::endl;
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
            std::cout << value;
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
        // if(dynamic_cast<jsonArray*>(obj) != nullptr) return eArray;
        return notFound;
    }
    bool _displayTree(){
        std::cout << "displaying tree" << std::endl;
        jsonObject js = jsonObject();
        return 0;
    }

    public:
    std::string getFileName(){
        return fileName;
    }
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
        Messenger::LogInfo("Opened:", file);
        fileName = file;
        LoadFile();
        return 0;
    }
    int DisplayTree(){
        return _displayTree();
        return 0;
    }
    int displayData(){
        // std::cout << data <<std::endl;
        //GenerateDataTree();
       // validateJson();
        return ValidateJson(root);
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
            src.close();
            Messenger::LogInfo("Closed file", fileName);
            fileName = "";
        }
        return 0;
    }
};



