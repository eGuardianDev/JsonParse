
#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>


class DefaultJsonObject{

    std::string key = "";
    protected:
        bool _isDeleted = false;
    public:
    DefaultJsonObject(){
        _isDeleted = false;    
    }
    void setKey(std::string key) {this->key = key;}
    std::string getKey() {return this->key;}
    bool isDeleted(){
        return _isDeleted;
    }
    // virtual bool isArray() = 0; 
    virtual void* getData(unsigned int index =0) const = 0;
    virtual void setData(void *pointer) = 0;
    virtual void deleteData(unsigned int index= 0) = 0;

    virtual DefaultJsonObject* copy(){
        return this;
    }
 
    virtual ~DefaultJsonObject() { _isDeleted = true;};

   
    // virtual void modifyData(std::string key, void *value) = 0;
};
class JsonObjectNumber: public DefaultJsonObject{

    int *value = 0;
    public:
    JsonObjectNumber(std::string key,int value){
        this->setKey(key);
        this->value = new int(value);
        _isDeleted = false;

    }
    DefaultJsonObject* copy() override{       
        return new JsonObjectNumber(this->getKey(), *value);
    }

    void* getData(unsigned int index = 0) const override{
        return static_cast<void*>(value);
    }
    void setData(void *pointer) override{
        this->value = new int(*static_cast<int*>(pointer));
    }
    void deleteData(unsigned int index =0) override{
        this->value = new int(index);
    }
    ~JsonObjectNumber() override{
        delete value;
    }
};
class JsonObjectString: public DefaultJsonObject{

    std::string *value = new std::string("");
    public:
    JsonObjectString(std::string key, std::string value){
        this->setKey(key);
        _isDeleted = false;

        this->value = new std::string(value);
    }

    DefaultJsonObject* copy() override{       
        return new JsonObjectString(this->getKey(), *value);
    }
    void* getData(unsigned int index = 0) const override{
        return static_cast<void*>(value);
    }

    void setData(void *pointer) override{
        delete value;
        this->value = new std::string(*(std::string*)pointer);
    }
    void deleteData(unsigned int index= 0) override {
        delete value;
        this->value = new std::string("");
    };
    ~JsonObjectString(){
        this->_isDeleted = true;
        delete value;
    }

};
class JsonObjectBool: public DefaultJsonObject{

    bool value = false;
    public:
    JsonObjectBool(std::string key, bool value){
        this->setKey(key);
        this->value = value;
        _isDeleted = false;

    }

    DefaultJsonObject* copy() override{       
        return new JsonObjectBool(this->getKey(), value);
    }
    void* getData(unsigned int index = 0) const override{
        return (void*)value;
    }

    void setData(void *pointer) override{
        this->value = *(bool*)pointer;
    }
    void deleteData(unsigned int index= 0) override {
        this->value = false;
    };
    ~JsonObjectBool(){ 
        this->_isDeleted = true;
        
    }

};
class JsonObject: public DefaultJsonObject{


    DefaultJsonObject **value;
    size_t count = 0;
    void expand(){
        
        DefaultJsonObject **temp = new DefaultJsonObject*[++count];
        for(int i =0;i<count-1;i++){
            temp[i] = value[i];
        }
        value = temp;
    }

    public:
    DefaultJsonObject* copy() override{      
        DefaultJsonObject **tempValues;
        for(int i =0 ;i<count;i++){
            tempValues[i] = value[i]->copy();
        }
        return new JsonObject(this->getKey(),*tempValues);
    }
    size_t getCount(){ return this->count;}
    JsonObject(std::string key){
        this->setKey(key);
        this->value = new DefaultJsonObject*[0];
        _isDeleted = false;
    }
    JsonObject(std::string key, DefaultJsonObject *value){
        this->setKey(key);
        this->value = new DefaultJsonObject*[1];
        this->value[0] = value;
        count++;
        _isDeleted = false;

    }
    
    void* getData(unsigned int index =0) const override{
        if(index >= count) throw std::runtime_error("out of scope argument");
        return static_cast<DefaultJsonObject*>(value[index]);
    }
    
    void setData(void *pointer) override{
        expand();
        this->value[count-1] = (DefaultJsonObject*) pointer;
    }
    void deleteData(unsigned int p =0) override{
        delete value[p];
        for(int i = p;i<count;i++){
            value[i] = value[i+1];
        }
        count--;
    } 

    ~JsonObject() override{
        for(int i = 0;i<count;i++){
            delete value[i];
        }
        delete [] value;
        this->_isDeleted = true;
    }

};


class JsonObjectArray: public DefaultJsonObject{
    DefaultJsonObject **value;
    size_t count = 0;
    void expand(){
        
        DefaultJsonObject **temp = new DefaultJsonObject*[++count];
        for(int i =0;i<count-1;i++){
            temp[i] = value[i];
        }
        value = temp;
    }
    public:
     DefaultJsonObject* copy() override{      
        DefaultJsonObject **tempValues;
        for(int i =0 ;i<count;i++){
            tempValues[i] = value[i]->copy();
        }
        return new JsonObjectArray(this->getKey(),*tempValues);
    }
    size_t getCount(){ return count;}
    JsonObjectArray(std::string key){
        this->setKey(key);
        this->value = new DefaultJsonObject*[0];
        _isDeleted = false;
    }
    JsonObjectArray(std::string key, DefaultJsonObject *value){
        this->setKey(key);
        this->value = new DefaultJsonObject*[1];
        this->value[0] = value;
        count++;
        _isDeleted = false;

    }
    void* getData(unsigned int index =0) const override{
        if(index >= count) throw std::runtime_error("out of scope argument");
        return static_cast<DefaultJsonObject*>(value[index]);
    }
    
    void setData(void *pointer) override{
        expand();
        this->value[count-1] = (DefaultJsonObject*) pointer;
    }
    void deleteData(unsigned int p =0) override{
        delete value[p];
        for(int i = p;i<count;i++){
            value[i] = value[i+1];
        }
        count--;
    } 

    ~JsonObjectArray() override{
        for(int i = 0;i<count;i++){
            delete value[i];
        }
        delete [] value;
        this->_isDeleted = true;
    }
};
enum valueType{
    vString = 0,
    vNumber = 1,
    vArray =  3,
    vObject = 4,
    vBool = 5,
    vNull = 6,
    vNoObject =-1
};
using json = DefaultJsonObject;

//https://stackoverflow.com/questions/307765/how-do-i-check-if-an-objects-type-is-a-particular-subclass-in-c
valueType JsonObjectType(DefaultJsonObject * obj){
    if (dynamic_cast<JsonObjectString*>(obj) != nullptr) return vString;
    if (dynamic_cast<JsonObjectArray*>(obj) != nullptr) return vArray;
    if (dynamic_cast<JsonObjectNumber*>(obj) != nullptr) return vNumber;
    if (dynamic_cast<JsonObject*>(obj) != nullptr) return vObject;
    return vNoObject;
}
valueType JsonObjectType(void * obj){
    if ((JsonObjectString*)(obj) != nullptr) return vString;
    if ((JsonObjectArray*)(obj) != nullptr) return vArray;
    if ((JsonObjectNumber*)(obj) != nullptr) return vNumber;
    if ((JsonObject*)(obj) != nullptr) return vObject;
    return vNoObject;
}


void printData(DefaultJsonObject *obj){
        switch(JsonObjectType(obj)){
            case vString:
            std::cout << obj->getKey() << ":" << *(std::string*)obj->getData();
            break;
            case vNumber:
            std::cout << obj->getKey() << ":" << *(int*)obj->getData();
            break;
            case vArray:
            std::cout << obj->getKey() << ":" << "array";
            break;
            default:
            std::cout << " to be defined type";
            break;
        }
    };

JsonObjectArray* jto_arr(DefaultJsonObject* obj){
    if(JsonObjectType(obj) == vArray) return (JsonObjectArray*) obj;
    return nullptr;
}
JsonObjectArray* jto_arr(void* obj){
    // if(JsonObjectType(obj) == vArray)
    // return nullptr;
     return (JsonObjectArray*) obj;
}

JsonObjectString* jto_str(DefaultJsonObject* obj){
    if(JsonObjectType(obj) == vString) return (JsonObjectString*) obj;
    return nullptr;
}
JsonObjectString* jto_str(void* obj){
    // if(JsonObjectType(obj) == vString)
    // return nullptr;
     return (JsonObjectString*) obj;
}

JsonObjectNumber* jto_num(DefaultJsonObject* obj){
    if(JsonObjectType(obj) == vNumber) return (JsonObjectNumber*) obj;
    return nullptr;
}
JsonObjectNumber* jto_num(void* obj){
    // if(JsonObjectType(obj) == vNumber) 
    // return nullptr;
    return (JsonObjectNumber*) obj;
}
JsonObjectBool* jto_bool(DefaultJsonObject* obj){
    if(JsonObjectType(obj) == vBool) return (JsonObjectBool*) obj;
    return nullptr;
}
JsonObjectBool* jto_bool(void* obj){
    // if(JsonObjectType(obj) == vNumber) 
    // return nullptr;
    return (JsonObjectBool*) obj;
}
// int main(){
 
   

// }

void simpleDisplayInfo(DefaultJsonObject * obj,  int tabs = 0, int side = 0)
{
    // auto isNull =[] (DefaultJsonObject * obj){
    //     if(dynamic_cast<JsonObjectArray*>(obj) != nullptr) return false;
    //     if(dynamic_cast<JsonObjectBool*>(obj) != nullptr) return false;
    //     if(dynamic_cast<JsonObjectNumber*>(obj) != nullptr) return false;
    //     if(dynamic_cast<JsonObjectString*>(obj) != nullptr) return false;
    //     return true;
    // };
    if(obj->isDeleted())
    {
        return;
    }
    std::string key = obj->getKey();
    // json *temp = (json*)obj->getData();
    valueType vt = JsonObjectType(obj);
    if(side == 0){
        if(vt == vArray){
            std::cout << "[";
        }else std::cout << "{";
    }
    switch(vt){
        case vArray:
        

      
        
        break;
        case vNumber:
        std::cout << "\""<<key << "\":" << *(int*) obj->getData();
        break;
        case vString:
        std::cout << "\""<< key << "\":\"" <<  *(std::string*) obj->getData() << "\"";
        break;
        case vBool:
        if(obj->getData() !=0){
        std::cout << "\""<< key << "\":TRUE";
        break;
        }
        std::cout << "\""<<key << "\":FALSE";
        break;
        case vNull:
        std::cout <<"\""<< key << "\":NULL";
        break;
        case vNoObject:

        break;
        case vObject:
            std::cout << "\"" << key <<"\": ";
            if(((JsonObject*)obj)->getCount()>1){
                std::cout << "\n";
            }
            std::cout << "{";
            for(int i = 0;i<((JsonObject*)obj)->getCount();i++){
                simpleDisplayInfo((DefaultJsonObject*)(obj->getData(i)),(tabs+1),(side+1));
                if(i !=((JsonObject*)obj)->getCount()-1 ){
                    std::cout << ",\n";
                }else std::cout << "\n";
            }
            std::cout << "}\n";
        break;
        default :
        break;

    }

    if(side == 0){
        if(vt == vArray){
            std::cout << "]";
        }else std::cout << "}";
    }

}

int main(){
    json *a = new JsonObjectNumber("das",4);
    json *b = new JsonObjectNumber("asd",3);
    json*a1 = new JsonObject("default",a);
    a1->setData(b);
    simpleDisplayInfo(a1);
    // json *obj1 = jto_arr(test2);
    // json *obj2 = jto_arr((obj1->getData()));
    // json *obj3 = jto_str(obj2->getData(2));
    // // std::cout << *(std::string*)obj3->getData();
}