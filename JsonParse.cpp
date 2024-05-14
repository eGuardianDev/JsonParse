#pragma one
#include <sstream>
#include <fstream>
#include <ios>
#include <string>
#include <iostream>
#include <algorithm>

#include <vector>

// implementation https://chatgpt.com/c/46e9dfad-e87f-4be2-9349-159e96d2c068

class JsonObject{
    public:
    bool is_array = false;
    virtual void* getData() = 0;
    int to_int(JsonObject *jsonObject){
        int* data =  static_cast<int*>(jsonObject->getData());
        return *data;
    }
    std::string to_string(JsonObject *jsonObject){

        std::string* data =  static_cast<std::string*>(jsonObject->getData());
        return *data;
    }
    JsonObject* to_object(JsonObject *jsonObject){

        JsonObject* data =  static_cast<JsonObject*>(jsonObject->getData());
        return data;
    }
};
class JsonObjectInt: public JsonObject{

    int value = 0;
    public:
    JsonObjectInt(int value){
        this->value = value;
    }
    void* getData() override{
        return static_cast<void*>(&value);
    }
};
class JsonObjectString: public JsonObject{

    std::string value = "";
    public:
    JsonObjectString(std::string value){
        this->value = value;
    }
    void* getData() override{
        return static_cast<void*>(&value);
    }
};
class JsonObjectNode: public JsonObject{

    JsonObject *value;
    public:
    JsonObjectNode(JsonObject *value){
        this->value = value;
    }
    void* getData() override{
        return static_cast<void*>(&value);
    }
};




//https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
class Messanger{
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

    JsonObject *mainNode;
    JsonObject **currentNode;

    int LoadFile(){
        currentNode = &mainNode;
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

    int validateJson(){
        for(int i =0;i<data.size();i++){
            
        }
        
        
        return 0;
    }

    int GenerateDataTree(){
        
        std::string key;
        std::string value;
        bool inValue = false;
        bool inKey = false;
        std::vector<char> brackets;
        auto display = [] (std::string &k, std::string &v, int place = -1){
            if(place>=0) std::cout <<place << " ";
            std::cout << k<< ":" << v << std::endl;
            k = "";
            v = "";
        };
        int line = 1;
        for(int i =0;i<data.size();i++){
            
            if(data[i] == '\n'){
                line++;
            }
            if(data[i]=='['){
                if(inValue){
                    display(key,value,line);
                    inValue = false;
                    continue;
                }
            }
            if(data[i]==']'){
                continue;
            }
         
            if(data[i] == '{'){
                if(inValue){
                    display(key,value,line);
                    std::cout << "new object\n";
                    inValue = false;
                }
                std::cout <<"{";
                continue;
            }
           
            if(data[i]=='"'){
                if(!inValue && !inKey){
                    inKey = true;
                    continue;
                }
                if(inKey){
                    inKey = false;
                    continue;
                }
            }
            if(data[i] == ':'){
                inKey = false;
                inValue = true;
                continue;
            }
             if(data[i]==','){
                if(inValue){
                    inValue = false;
                    display(key,value,line);
                    continue;
                }
            }

           if(data[i]=='}'){
            if(inKey || inValue){
                display(key,value);
            }
                inValue = false;
                inKey = false;
            std::cout <<"}";
            continue;
           }

           if(inKey) key+=data[i];
           else if(inValue) value+=data[i];
        
        }



        return 0;
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
            Messanger::LogWarning("You need to close the file, before opening new one");
            return 2;
        } 
        src.open(file, std::ios::in | std::ios::out);
        if(src.fail()){
            Messanger::LogError("File failed to open");
            return 1;
        }
        Messanger::LogInfo("Opened:", file);
        fileName = file;
        LoadFile();
        return 0;
    }
    int displayData(){
        // std::cout << data <<std::endl;
        GenerateDataTree();
       // validateJson();
        
        return 0;
    }

    //https://stackoverflow.com/questions/53359727/how-to-save-and-also-read-c-fstream-file-without-closing-it  
    int saveFile(){  
        if(!src.is_open()){
            Messanger::LogError("No file currently opened");
            return -1;

        } 
        src.flush();
        Messanger::LogInfo("Successful saved.");
        return 0;
    }
    int saveAsFile(std::string file){  
        std::ofstream output;
        Messanger::LogInfo("Saving data to:", file);
        output.open(file, std::ios::out);
        if(output.fail()){
            Messanger::LogError("File failed to open");
            return 1;
        }
        output << src.rdbuf();
        output.close();
        Messanger::LogInfo("Successful saved.");
        return 0;
    }
    int closeFile(bool displayError = true){   
        if(!src.is_open() && displayError){
            Messanger::LogError("No file currently opened");
            return -1;
        } 
        if(src.is_open()){
            src.close();
            Messanger::LogInfo("Closed file", fileName);
            fileName = "";
        }
        return 0;
    }
};



