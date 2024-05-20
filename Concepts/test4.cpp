

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>


class DefaultJsonObject{

    bool beingArray = false;
    std::string key = "";
    public:
    void setKey(std::string key) {this->key = key;}
    std::string getKey() {return this->key;}
    
    virtual ~DefaultJsonObject() {};
    // virtual bool isArray() = 0; 
    virtual void* getData(unsigned int index =0) const = 0;
    virtual void setData(void *pointer) = 0;
    virtual void deleteData(unsigned int index= 0) = 0;
   
    // virtual void modifyData(std::string key, void *value) = 0;
};
class JsonObjectNumber: public DefaultJsonObject{

    int *value = 0;
    public:
    JsonObjectNumber(std::string key,int value){
        this->setKey(key);
        this->value = new int(value);
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
        this->value = new std::string(value);
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
        delete value;
    }

};
class JsonObjectBool: public DefaultJsonObject{

    bool value = false;
    public:
    JsonObjectBool(std::string key, bool value){
        this->setKey(key);
        this->value = value;
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
    JsonObjectArray(std::string key){
        this->setKey(key);
        this->value = new DefaultJsonObject*[0];
    }
    JsonObjectArray(std::string key, DefaultJsonObject *value){
        this->setKey(key);
        this->value = new DefaultJsonObject*[1];
        this->value[0] = value;
        count++;
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
    return vNoObject;
}
valueType JsonObjectType(void * obj){
    if ((JsonObjectString*)(obj) != nullptr) return vString;
    if ((JsonObjectArray*)(obj) != nullptr) return vArray;
    if ((JsonObjectNumber*)(obj) != nullptr) return vNumber;
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

TEST_CASE("Array with 2 numbers and 1 string in another array "){
    json *a = new JsonObjectNumber("asd",3);
    json *b = new JsonObjectNumber("asd2",6);
    json *c = new JsonObjectString("asd3","data");
    json *test =new JsonObjectArray("arr",a);
    json *d =new JsonObjectBool("bollean",true);
    test->setData(b);
    test->setData(c);
    a->setData(new int(10));
    DefaultJsonObject *test2 =new JsonObjectArray("arr2");
    test2->setData(test);


    json *obj1 = jto_arr(test2);
    json *obj2 = jto_arr((obj1->getData()));
    json *obj3 = jto_str(obj2->getData(2));
    // std::cout << *(std::string*)obj3->getData();
    
    
    bool boolean = d->getData();

    CHECK(boolean == true);
}