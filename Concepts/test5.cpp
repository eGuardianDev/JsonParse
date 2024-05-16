#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>
//data parse


template<class T>
class DefaultJsonObject{
    bool beingArray = false;
    public:
    // virtual bool isArray() = 0; 
    virtual T getData() = 0;
    virtual void setData(T pointer) = 0;
    // virtual void modifyData(std::string key, void *value) = 0;
};

class JsonObjectNumber: public DefaultJsonObject<int>{

    int value = 0;
    public:
    JsonObjectNumber(int value){
        this->value = value;
    }
    int getData() override{
        return value;
    }
    void setData(int pointer) override{
        this->value = pointer;
    }
};
class JsonObjectString: public DefaultJsonObject<std::string>{

    std::string value = "";
    public:
    JsonObjectString(std::string value){
        this->value = value;
    }
    std::string getData() override{
        return this->value;
    }
};
class JsonObjectArray: public DefaultJsonObject<DefaultJsonObject*>{
    std::vector<DefaultJsonObject<class T>*> values;
    public:
    JsonObjectArray(DefaultJsonObject<class T>* a)
    {
        values.push_back(a);
    }
    DefaultJsonObject<class T>* getData() override{
        return values[0];
    }
   
};

int main(){
    JsonObjectNumber a = JsonObjectNumber(1);

}