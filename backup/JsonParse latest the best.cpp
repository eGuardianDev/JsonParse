#include "JsonObjects.h"
#include "Rooter.h"
#include <ostream>
#include <stdexcept>
#include <valarray>
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

        int ignoreIncoming = 0;
        
        for(int i =_index;i<data.size();i++){

            switch(data[i]){
                case '\n':
                    line++;
                break;

                case '{':
                    if(inValue){
                        ignore = true;
                        inValue = false;
                        ignoreIncoming++;
                        // std::cout << key << ":";
                        key = ""; value = "";
                        bool temp = ValidateJson(currentOBJ,line, i,true,pass+1);
                        if(!temp) return temp;
                        break;
                    }else{
                        inKey = true;
                        inValue = false;
                    }
                    if(ignore){
                        std::cout << "found one more";
                        ignoreIncoming++; break;
                    } 
                    std::cout << "{\n";
                    
                   
                
                break;
                case '}':
                    if(ignore) {ignoreIncoming--; }
                    if(ignoreIncoming == 0) { ignore = false;}
                    if(ignore) break;

                    if(key.size()>0){
                        // std::cout << key << ":" << value;
                        key = ""; value = "";
                    }
                    
                    if(_toReturn) return true;

                    std::cout << "}\n";
                    inKey = false;
                    inValue = false;


                break;


                case ':':
                    if(ignore) break;
                    inKey = false;
                    inValue = true;
                    
                break;
                case ',':
                    if(ignore) break;
                    if(inValue){
                        // std::cout << key << ":" << value << "";
                        key = ""; value = "";
                    }
                    std::cout << ",\n";
                    inKey = true;
                    inValue = false;
                  
                break;
                case '[':
                    
                break;
                case ']':
                   
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
    //         v = "";//
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



