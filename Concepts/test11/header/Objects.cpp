#include "Objects.h"



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