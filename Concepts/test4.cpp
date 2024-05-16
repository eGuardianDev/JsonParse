#include <cstddef>
#include <iostream>
#include <stdexcept>
//data parse


class DefaultJsonObject{
    bool beingArray = false;
    public:
    // virtual bool isArray() = 0; 
    virtual void* getData(unsigned int index =0) const = 0;
    virtual void setData(void *pointer) = 0;
    virtual void deleteData(unsigned int index= 0) = 0;
    // virtual void modifyData(std::string key, void *value) = 0;
};
class JsonObjectNumber: public DefaultJsonObject{

    int *value = 0;
    public:
    JsonObjectNumber(int value){
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
};
class JsonObjectString: public DefaultJsonObject{

    std::string *value = new std::string("");
    public:
    JsonObjectString(std::string value){
        this->value = new std::string(value);
    }
    void* getData(unsigned int index = 0) const override{
        return static_cast<void*>(value);
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
    JsonObjectArray(){
        this->value = new DefaultJsonObject*[0];
    }
    JsonObjectArray(DefaultJsonObject *value){
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
        //shifter requred
    } 
};

int main(){

    DefaultJsonObject *a = new JsonObjectNumber(3);
    DefaultJsonObject *b = new JsonObjectNumber(6);
    DefaultJsonObject *test =new JsonObjectArray(a);
    test->setData(b);
    a->setData(new int(10));
    // delete data;
    std::cout << *(int*)(a->getData());
    DefaultJsonObject * obj = (DefaultJsonObject*)test->getData(1);
    std::cout << *(int*)obj->getData();

    // void *vec = test->getData(); 
    // DefaultJsonObject **pointers = static_cast<DefaultJsonObject**>(vec);
    // std::cout << *static_cast<int*>(pointers[0]->getData());
}