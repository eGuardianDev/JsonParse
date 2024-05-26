#include "t.h"

ValueType TypeOfValue(Values* val){
    if(dynamic_cast<vString*>(val) != nullptr) return eString;
    if(dynamic_cast<vNumber*>(val) != nullptr) return eNumber;
    if(dynamic_cast<vObject*>(val) != nullptr) return eObject;
    // if(dynamic_cast<vArray*>(val) != nullptr) return eArray;
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
    this->value = new jsonObject(*(jsonObject*)data);
    return 0;
}
int vObject::removeData(unsigned int index =0){
    this->value = new jsonObject();
    return 0;
}
void* vObject::getData(unsigned int index =0 ){
    return (void*)this->value;
}
vObject::~vObject(){
    delete this->value;
}




// =========== DEFUALT OBJECT
size_t Objects::Size(){
    return this->size;
}

// =========== JSON OBJECT
jsonObject::jsonObject(){
}
jsonObject::jsonObject(jsonObject& obj){
    this->pairs = new Pair*[obj.Size()];
    for(int i = 0;i<obj.Size();i++){
        this->pairs[i] = obj.pairs[i]->Clone();
    }
}
void jsonObject::expand(){
    Pair** temp = new Pair*[this->size+1];
    for(int i =0; i<Size();i++){
        temp[i] = this->pairs[i];
    }
    size++;
    delete [] this->pairs;
    this->pairs = temp;
}
void jsonObject::shrink(){
    Pair** temp = new Pair*[this->size-1];
    for(int i =0; i<Size()-1;i++){
        temp[i] = this->pairs[i];
    }
    size--;
    delete [] this->pairs;
    this->pairs = temp;
    return;
}
Objects* jsonObject::Clone() {
    return new jsonObject(*this);
}
void jsonObject::AddPair(Pair* pair){
    
    this->pairs[this->Size()-1] = pair;
}
void jsonObject::AddPair(std::string key, Values* value){
    this->AddPair(new Pair(key, value));    
}

Pair* jsonObject::ReturnPair(std::string key){

    std::cout << "Size >" << this->Size() << " ";
    for(int i = 0;i<this->Size();i++){
        if(this->pairs[i]->key == key){
            return this->pairs[i];
        }
    }

    return new Pair("", new vNumber(new double(1)));
}
Pair* jsonObject::ReturnPair(unsigned int index = 0){
    if(index >= this->Size()) return nullptr;
    return this->pairs[index];    
}

int jsonObject::RemovePair(std::string key){
    Pair* p = nullptr; 
    int index =-1;
    for(int i =0 ;i<this->Size();i++){
        if(this->pairs[i]->key == key){
            p = this->pairs[i];
            index = i;
            break;
        }
    }
    if(index == -1 || p == nullptr) return -1;
    for(int i = index; i<this->Size()-1;i++){
        this->pairs[i] = this->pairs[i+1];
        break;
    }
    std::cout << p;
    delete p;
    return 0;


}
int jsonObject::RemovePair(unsigned int index){
    if(index >=this->Size()) return -1;
    for(int i =index;i<this->Size()-1;i++){
        this->pairs[i] = this->pairs[i+1];
    }
    return 0;

}
jsonObject::~jsonObject(){
    for(int i =0;i<this->Size();i++){
        delete this->pairs[i]->value;
        // std::cout <<(*(double*)(this->pairs[i]->value->getData())) << 
    }
    // std::cout << "nuts";
    delete [] this->pairs;
}



// ============== VALUE ARRAY


void jsonArray::expand(){
    Values** temp = new Values*[this->size+1];
    for(int i =0; i<Size();i++){
        temp[i] = this->values[i];
    }
    size++;
    delete [] this->values;
    this->values = temp;
    
}
void jsonArray::shrink(){
    Values** temp = new Values*[this->size-1];
    for(int i =0; i<Size()-1;i++){
        temp[i] = this->values[i];
    }
    size--;
    delete [] this->values;
    this->values= temp;
    return;
}
jsonArray::jsonArray(){

}
Objects* jsonArray::Clone(){
    jsonArray* ja = new jsonArray();
    for(int i = 0;i<this->Size();i++){
        ja->AddValue(this->ReturnValue(i));
    }
    return ja; 
}
int jsonArray::AddValue(Values* value){
    this->expand();
    this->values[this->Size()-1] = value->Clone();
    return 0;
}
Values* jsonArray::ReturnValue(unsigned int index){
  
    return values[index]->Clone();
}
int jsonArray::RemoveValue(unsigned int index){
    for(int i =index;i<this->Size()-1;i++){
        this->values[i] = this->values[i+1];
    }
    this->shrink();
    return 0;
}



// class jsonArray : public Objects{
//     protected:
//     Values* values;
//     void expand() override;
//     void shrink() override;
// };