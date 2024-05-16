#include <iostream>
#include <vector>
//data parse


class DefaultJsonObject{
    public:
    virtual void* getData() = 0;
};
class JsonObjectNumber: public DefaultJsonObject{

    int value = 0;
    public:
    JsonObjectNumber(int value){
        this->value = value;
    }
    void* getData() override{
        return static_cast<void*>(&value);
    }
};
class JsonObjectString: public DefaultJsonObject{

    std::string value = "";
    public:
    JsonObjectString(std::string value){
        this->value = value;
    }
    void* getData() override{
        return static_cast<void*>(&value);
    }
};
class JsonObjectArray: public DefaultJsonObject{

    std::vector<DefaultJsonObject*> value;
    public:
    JsonObjectArray(){
    }
    JsonObjectArray(DefaultJsonObject *value){
        this->value.push_back(value);
    }
    JsonObjectArray(DefaultJsonObject *value, DefaultJsonObject* v2){
        this->value.push_back(value);
        this->value.push_back(v2);
    }
    void* getData() override{
        return static_cast<void*>(&value);
    }
};

int main(){

    DefaultJsonObject *a = new JsonObjectNumber(3);
    JsonObjectArray test =new JsonObjectArray(a,a );
    std::cout << *static_cast<int*>(a->getData());
    std::cout << "\n"; 
     std::vector<DefaultJsonObject*> *values = static_cast<std::vector<DefaultJsonObject*>*>(test.getData());

    std::cout << values->size()<< "\n"; 

    // std::vector<DefaultJsonObject*> *value = static_cast<std::vector<DefaultJsonObject*>*>(test.getData());
    // std::vector<DefaultJsonObject*> values = *value;
    // std::cout << values.size() << std::endl;
    // int *v = static_cast<int*>(values[0]->getData());
    // std::cout << &v;
}