#pragma one
#include <string>

class DataSave{
    private:
    std::string fileData;
    std::string filePath;

    public:

    int openFile(){   
        return -1;
    }
    int saveFile(){   
        return -1;
    }
    int saveAsFile(){   
        return -1;
    }
    int closeFile(){   
        return -1;
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