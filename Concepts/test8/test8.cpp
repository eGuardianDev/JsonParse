
// #include <cstddef>
// #include <iostream>
// #include <stdexcept>
// #include <string>
// #include <vector>


// class valueBase{
//     private:
//     public:
//     virtual valueBase* copy()  {
//         return this;
//     }
//     virtual ~valueBase(){
//     }
//     virtual void* Data()= 0;
// };






// class jsonObjectBase{
//     protected:
//     bool _isDeleted = false;
    
//     public:
//     jsonObjectBase(){
//         _isDeleted = false;    
//     }
//     bool isDeleted(){
//         return _isDeleted;
//     }
//     virtual jsonObjectBase* copy(){
//         return this;
//     }
//     virtual ~jsonObjectBase() { _isDeleted = true;};
//     virtual void* getData(unsigned int index =0) const = 0;
//     virtual void addData(valueBase*) = 0;
//     virtual void deleteData(unsigned int index= 0) = 0;
//     virtual void modifyData(std::string key, void *value) = 0;
// };




// class StringValue :public valueBase{
//     private:
//     std::string* value;
//     public:
//     StringValue(std::string v){
//         this->value = new std::string(v);
//     }
//     StringValue(std::string* v){
//         this->value = new std::string(*v);
//     }
//     valueBase* copy() override{
//         return new StringValue(new std::string(*value));
//     }
//     void* Data() override {return (void*)value;};

// };
// class NumberValue :public valueBase{
//     private:
//     double* value;
//     public:
//     NumberValue(double i =0){
//        this->value = new double(i);
//     }
//     NumberValue(double *i =0){
//        this->value = new double(*i);
//     }
//     valueBase* copy() override{
//         return new NumberValue(new double(*value));
//     }
//     void* Data() override {return (void*)value;};

// };
// class ObjectValue :public valueBase{
//     private:
//     jsonObjectBase* value;
//     public:
//     ObjectValue(jsonObjectBase* b){
//         this->value = b;
//     }

//     valueBase* copy() override{
//         return new ObjectValue(value->copy());
//     }
//     void* Data() override {return (void*)value;};

// };
// class ArrayValue :public valueBase{
//     private:
//     jsonObjectBase* value;
//     public:
//     ArrayValue(jsonObjectBase* v){
//         this->value = v;
//     }
//     valueBase* copy() override{
//         return new ArrayValue(value->copy());
//     }
//     void* Data() override {return (void*)value;};
    
// };
// class BoolValue :public valueBase{
//     private:
//     bool value;
//     public:
//     valueBase* copy() override{
//         return new BoolValue(value);
//     }
//     BoolValue(bool v){
//         this->value = v;
//     }
//     void* Data() override {return (void*)value;};

// };















// struct Pair{
//     std::string key;
//     valueBase* value;
//     Pair(std::string key, valueBase* value){
//         this->key = key;
//         this->value = value;
//     }
//     ~Pair(){
//         delete value;
//     }
//     Pair *copy(){
//     {
//         return new Pair(key,value);
//     }
// };


// class Object : public jsonObjectBase{

//     size_t count = 0;
//     std::vector<Pair> pairs;


//     public:
//     //---------------init
//     Object(std::string key){
//         _isDeleted = false;
//         this->setKey(key);
//     }
    
//     Object(std::string key, valueBase* vb ){
//         _isDeleted = false;
//         pairs.push_back(new Pair(key, vb));
        
//     }
//     Object()
//     jsonObjectBase* copy() override
//     {      
        
//         return new Object(this->getKey(), pairs);
//     }
//     ~Object(){
//         for(int i =0;i<count;i++){
//             delete value[i];
//         }
//         delete [] value;
//     }

//     //-------------keys
//     void setKey(std::string key,int index =0) {
//         if(count)
//         this->key = key;
//         }
//     std::string getKey() {return this->key;}
  
//     //-------------values
//     size_t getCount(){ return this->count;}


//     void* getData(unsigned int index =0) const override {
//         if(index > count)throw new std::overflow_error("Out of bound exception");
//         return (void*)value[index];
//     }
//     void addData(valueBase* pointer)  override {
//         expand();
//         value[count-1] = pointer->copy(); 
//     }
//     void deleteData(unsigned int index= 0) override {
//         if(index > count-1){
//             throw new std::runtime_error("Out of bound for deletion");
//         }
//         delete value[index];
//         for(int i =index;index <count-1;i++){
//             value[i] = value[i+1];
//         }
//         valueBase** temp = new valueBase*[count-1];
//         for(int i =0;i<count-1;i++){
//             temp[i] = value[i];
//         }
//         value = temp;
//     }
//     void modifyData(std::string key, void *value) override {
//     }
// };


// class Array : public jsonObjectBase{
 
//     size_t count = 0;
//     valueBase** value;

//     void expand(){
//         valueBase **temp = new valueBase*[++count];
//         for(int i =0;i<count-1;i++){
//             temp[i] = value[i];
//         }
//         value = temp;
//     }

//     public:
//     //---------------init
//     Array(valueBase* values ){
//         _isDeleted = false;
//     }
//     jsonObjectBase* copy() override
//     {      
//         valueBase **tempValues;
//             for(int i =0 ;i<count;i++){
//                 tempValues[i] = value[i]->copy();
//             }
//         return new Array(*tempValues);
//     }
//     ~Array(){
//         for(int i =0;i<count;i++){
//             delete value[i];
//         }
//         delete [] value;
//     }
  
//     //-------------values
//     size_t getCount(){ return this->count;}


//     void* getData(unsigned int index =0) const override {
//         if(index > count)throw new std::overflow_error("Out of bound exception");
//         return (void*)value[index];
//     }
//     void addData(valueBase* pointer)  override {
//         expand();
//         value[count-1] = pointer; 
//     }
//     void deleteData(unsigned int index= 0) override {
//         if(index > count-1){
//             throw new std::runtime_error("Out of bound for deletion");
//         }
//         delete value[index];
//         for(int i =index;index <count-1;i++){
//             value[i] = value[i+1];
//         }
//         valueBase** temp = new valueBase*[count-1];
//         for(int i =0;i<count-1;i++){
//             temp[i] = value[i];
//         }
//         value = temp;
//     }
//     void modifyData(std::string key, void *value) override {
//     }
// };






// using json = jsonObjectBase;
// using jvalue = valueBase;

// int main(){
//     json* j1 = new Object("key1");
//     jvalue* v1 = new NumberValue(5);
//     jvalue* v2 = new NumberValue(10);
//     j1->addData(v1);
//     j1->addData(v2);
//     std::cout << (*(double*)((jvalue*)((j1)->getData(1)))->Data());
// }