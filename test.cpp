#include <iostream>

// class dataInfo{
//     public:
//     virtual std::string getValue(){
//         return 0;
//     }
// };


// class dataInt : public dataInfo{
//     public:
//     int value;
//     dataInt(int data){
//         this->value = data;
//     }
//     std::string getValue(){
//         return std::to_string(this->value);
//     }
// };

// class dataString : public dataInfo{
//     public:
//     std::string value;
//     dataString(std::string data){
//         this->value = data;
//     }
//     std::string getValue(){
//         return this->value;
//     }
// };

// class dataNode : public dataInfo{
//     public:
//     Node value;
//     dataString(Node data){
//         this->value = data;
//     }
//     std::string getValue(){
//         return this->value;
//     }
// };


// class Node{
//     public:
//     std::string key;
//     dataInfo* value;
//     Node child;
// };

#include <sstream> //for std::stringstream 
#include <string>  //for std::string

class parenterr{
    public:
    virtual void* getData() = 0;
    // ~parenterr(){
    //     delete pointer;
    // }
};
class child1: public parenterr{

    int data = 123;
    public:
    void* getData() override{
        return static_cast<void*>(&data);
    }
};
class child2 : public parenterr{

    std::string data = "string";
    public:
    void* getData() override{
        return static_cast<void*>(&data);
    }
};


int main(){ 
    child1 c1;
    child2 c2;
    parenterr *point = &c1;
    int* data =  static_cast<int*>(point->getData());
    std::cout << *data;
    parenterr *point1 = &c2;
    std::string* data2 =  static_cast<std::string*>(point1->getData());
    std::cout << *data2;
    return 0;
}