#include "Values.h"

ValueType TypeOfValue(Values* val){
    if(dynamic_cast<vString*>(val) != nullptr) return eString;
    if(dynamic_cast<vNumber*>(val) != nullptr) return eNumber;
    if(dynamic_cast<vObject*>(val) != nullptr) return eObject;
    if(dynamic_cast<vArray*>(val) != nullptr) return eArray;
    if(dynamic_cast<vBoolean*>(val) != nullptr) return eBoolean;
    return notFound;
}

    // virtual int setData(void*) =0 ;
    // virtual int removeData(unsigned int);
    // virtual void* getData(unsigned int);
    // virtual ~Values();
//================ DEFAULTVALUES
Values::~Values(){

}

//================ Number Values
Values* vNumber::Clone(){
    return new vNumber(this->value);
}
vNumber::vNumber(){
    this->value = new double(0);
}
vNumber::vNumber(double * data){
    this->setData(data);
}
vNumber::vNumber(double data){
    this->setData(new double(data));
}
int vNumber::setData(void* data){
    if(this->value != nullptr){
        delete this->value;
    }
    this->value = new double(*(double*)data);
    return 0;
}
int vNumber::removeData(unsigned int index =0){
    if(this->value !=nullptr){
        delete this->value;
    }
    this->value = new double(0);
    return 0;
}
void* vNumber::getData(unsigned int index =0 ){
    return (void*)this->value;
}
vNumber::~vNumber(){
    delete this->value;
}
//=========== String Value

Values* vString::Clone(){
    return new vString(this->value);
}
vString::vString(){
    this->value = new std::string("");
}
vString::vString(std::string * data){
    this->setData(data);
}
vString::vString(std::string data){
    this->setData(new std::string(data));
}
int vString::setData(void* data){
    if(this->value != nullptr){
        delete this->value;
    }
    this->value = new std::string(*(std::string*)data);
    return 0;
}
int vString::removeData(unsigned int index =0){
    if(this->value !=nullptr){
        delete this->value;
    }
    this->value = new std::string("");
    return 0;
}
void* vString::getData(unsigned int index =0 ){
    return (void*)this->value;
}
vString::~vString(){
    delete this->value;
}


//=========== Bool Value

Values* vBoolean::Clone(){
    return new vBoolean(this->value);
}
vBoolean::vBoolean(){
    this->value = new bool(false);
}
vBoolean::vBoolean(bool * data){
    this->setData(data);
}
vBoolean::vBoolean(bool data){
    this->setData(new bool(data));
}
int vBoolean::setData(void* data){
    if(this->value != nullptr){
        delete this->value;
    }
    this->value = new bool(*(bool*)data);
    return 0;
}
int vBoolean::removeData(unsigned int index =0){
  
    this->value = new bool(false);
    return 0;
}
void* vBoolean::getData(unsigned int index =0 ){
    return (void*)this->value;
}
vBoolean::~vBoolean(){
    delete this->value;
}

//=========== Object Value

Values* vObject::Clone(){
    return new vObject(this->value);
}
vObject::vObject(){
    this->value = new jsonObject();
}
vObject::vObject(jsonObject * data){
    this->setData(data);
}
vObject::vObject(jsonObject data){
    this->setData(new jsonObject(data));
}
int vObject::setData(void* data){
    if(this->value != nullptr){
        delete this->value;
    }
    this->value = new vObject(*(jsonObject*)data);
    return 0;
}
int vObject::removeData(unsigned int index =0){
    this->value = new vObject();
    return 0;
}
void* vObject::getData(unsigned int index =0 ){
    return (void*)this->value;
}
vObject::~vObject(){
    delete this->value;
}
