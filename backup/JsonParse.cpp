#include "JsonObjects.h"
#include "Rooter.h"
#include <ostream>
#include <stdexcept>
#pragma one
#include <sstream>
#include <fstream>
#include <ios>
#include <string>
#include <iostream>
#include <algorithm>

#include <vector>
#include "JsonObjects.h"

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



    //object
    //array
    //value
    //string
    //number
    //whitespace
    Objects *root;
    bool ValidateJson(Objects *_currentObj = nullptr , int _line = 1, int _index = 1, bool _toReturn = false,int pass = 0,bool expectedObject = false){
        //object current
        
        Objects *currentOBJ = _currentObj;
        bool inValue;
        bool inKey;
        std::string key;
        std::string value;
        std::string currentLine;
        int line = _line;
        if(_index == 0){
            switch(data[0]){
                case '{':
                root = new jsonObject();
                currentOBJ = root;
                line++;
                break;
                case '[':
                root = new jsonArray();
                line++; 
                break;
                default:
                inValue=true;
                break;
            }
        }
        if(inValue && line == 1){
            _index++;
            for(int i =0;i<data.size();i++){
                currentLine+=data[i];
                switch(data[i]){
                    case '\n':
                    currentLine ="";
                    break;
                    case ':':
                    Messenger::LogError("Trying to use value as key");
                    Messenger::LogWarning(currentLine);
                    return false;
                    break;
                    case ',':
                    Messenger::LogError("Trying to read more than one variable");
                    Messenger::LogWarning(currentLine);
                    return false;
                    break;
                    case '[':
                    case ']':
                    Messenger::LogError("This cannot be array");
                    Messenger::LogWarning(currentLine);
                    return false;
                    break;
                    default:
                    value+= data[i];
                    break;
                }
            }
            int numofquotes =0;
            for(int i =0;i<value.size();i++){
                if(value[i] == '\"' )
                {
                    if(i != 0 && value[i-1] !='\''){
                        numofquotes++;
                    }else{
                        if(i == 0){
                            numofquotes++;
                        }
                    }
                }
            }
            if(numofquotes != 2)
            {    Messenger::LogError("Trying to read to many strings");
            std::cout << numofquotes << std::endl;
                return false;
            }
            std::cout << value;
            return true;
        }
        bool toReturn = _toReturn;
        inValue = false;
        bool insideOfObject = false;
        bool expectedNewElement = false;
        int quotes = 0;
        bool expectedNewValue =false;
        for(int i =_index;i<data.size();i++){

            switch(data[i]){
                case '\n':
                    line++;
                break;
                case '{':
                    if(insideOfObject)break;
                    if(inValue){
                        if(value.size() > 0){
                            
                        Messenger::LogError("Trying to create object after some data");
                        Messenger::LogWarning("line " + std::to_string(line));
                            return false;
                        }

                        insideOfObject = true;
                        key = "";
                        value = "";
                        std::cout << "{";
                        bool temp =ValidateJson(currentOBJ,line,i+1,true,pass+1);
                        inValue = false;
                        if(!temp) return false;
                    }else{
                        if(expectedNewElement){
                            if(expectedObject) return true;
                            Messenger::LogError("Trying to create object insted of key");
                            std::cout <<pass+1;
                            Messenger::LogWarning("line " + std::to_string(line));
                            return false;
                        }
                    }

                break;
                case '}':
                    
                    if(insideOfObject) {
                        insideOfObject = false;
                        break;
                    }
                    if(expectedNewValue) {
                        Messenger::LogError("Expected value for key but none forund");
           
                // quotes = 0;
                // for(int i =0 ;i<value.size();i++){
                //     if(value[i] == '"') quotes++;
                // }
                // if(quotes == 2 || quotes ==0){}
                // else return false;
                // quotes =0;

                // for(int i =0 ;i<key.size();i++){
                //     if(key[i] == '"') quotes++;
                // }
                // if(quotes == 2) {}
                // else return false;
                // quotes =0;         Messenger::LogWarning("line: " + std::to_string(line));
                        return false;
                    }
                    if(expectedNewElement){
                        Messenger::LogError("Expecting new element but none found");
                        Messenger::LogWarning("line: " + std::to_string(line));
                        return false;
                    }
                    if(inValue){
                        std::cout << value << std::endl;
                    }

                if(toReturn){
                    std::cout << "}\n";
                    return true;
                }   
                    key= "";
                    value = "";
 
                //afterObject = true;
                insideOfObject = false;
                value = "";
                key = "";
                inValue = false;
                inKey = false;

                break;
                case '"':
                if(insideOfObject) break;
                expectedNewElement = false;

                quotes++;
                if(!inValue) inKey = true;
                if(quotes >2){
                    Messenger::LogError("reading to many strings" + std::to_string(line));
                    return false;
                }if(inValue == true){
                    value+="\"";
                }   
                if(inKey) expectedNewValue= true;


                
                break;
                case ':':
                if(insideOfObject)break;
                expectedNewValue = false;
                    inValue = true; 
                    inKey = false;
                    if(quotes != 2){
                        Messenger::LogError("Missing quotes?" + std::to_string(line));
                        return false;
                    }
                    quotes = 0;
                    std::cout << key<<":"; key = "";
                
                                    
                break;
                case ',':
                    if(expectedNewElement){
                        Messenger::LogError("Expecting new element but none found");
                        Messenger::LogWarning("line: " + std::to_string(line));
                        return false;
                    }
                
                    if(insideOfObject){
                        key = ""; value = "";
                        break;
                    }
                    if(value != ""){
                        std::cout <<value <<std::endl;
                    }
                    inValue = false;
                    inKey = false;
                    expectedNewElement = true;
                    quotes = 0;
                    key = "";
                    value = "";


                break;
                case '[':
                     

                break;
                case ']':
              
                break;

            
                default:
                if(inKey){
                    key += data[i];
                }
                if(inValue){
                    value += data[i];
                }
                
                break;
                
            }

            
        }
        return true;
    }
    // int GenerateDataTree(){
        
    //     std::string key;
    //     std::string value;
    //     bool inValue = false;
    //     bool inKey = false;
    //     std::vector<char> brackets;
    //     auto display = [] (std::string &k, std::string &v, int place = -1){
    //         if(place>=0) std::cout <<place << " ";
    //         std::cout << k<< ":" << v << std::endl;
    //         k = "";
    //         v = "";
    //     };
    //     int line = 1;
    //     for(int i =0;i<data.size();i++){
            
    //         if(data[i] == '\n'){
    //             line++;
    //         }
    //         if(data[i]=='['){
    //             if(inValue){
    //                 display(key,value,line);
    //                 inValue = false;
    //                 continue;
    //             }
    //         }
    //         if(data[i]==']'){
    //             continue;
    //         }
         
    //         if(data[i] == '{'){
    //             if(inValue){
    //                 display(key,value,line);
    //                 std::cout << "new object\n";
    //                 inValue = false;
    //             }
    //             std::cout <<"{";
    //             continue;
    //         }
           
    //         if(data[i]=='"'){
    //             if(!inValue && !inKey){
    //                 inKey = true;
    //                 continue;
    //             }
    //             if(inKey){
    //                 inKey = false;
    //                 continue;
    //             }
    //         }
    //         if(data[i] == ':'){
    //             inKey = false;
    //             inValue = true;
    //             continue;
    //         }
    //          if(data[i]==','){
    //             if(inValue){
    //                 inValue = false;
    //                 display(key,value,line);
    //                 continue;
    //             }
    //         }

    //        if(data[i]=='}'){
    //         if(inKey || inValue){
    //             display(key,value);
    //         }
    //             inValue = false;
    //             inKey = false;
    //         std::cout <<"}";
    //         continue;
    //        }

    //        if(inKey) key+=data[i];
    //        else if(inValue) value+=data[i];
        
    //     }



    //     return 0;
    // }


    void DisplayTree(){

    }



    // JsonObject* SeekObject(){

    //     return JsonObject();
    // }


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
    int displayData(){
        // std::cout << data <<std::endl;
        //GenerateDataTree();
       // validateJson();
        return ValidateJson();
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



