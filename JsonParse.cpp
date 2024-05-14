#pragma one

#include <fstream>
#include <ios>
#include <string>
#include <iostream>

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
class DataSave{
    private:  
    std::fstream src;
    std::string data;
    std::string fileName;

    public:
    int openFile(std::string file){   
        src.open(file, std::ios::in | std::ios::out);
        if(src.fail()){
            Messanger::LogError("File failed to open");
            return 1;
        }
        Messanger::LogInfo("Opened:", file);
        fileName = file;
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
        }
        return 0;
    }
};




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