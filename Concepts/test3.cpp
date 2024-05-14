#include <iostream>
//data parse



class JsonObject{
    protected:
    int numOfElements =0;
    virtual int Extend() = 0;
    std::string key;
    public:
    void setKey(std::string key){
        this->key = key;
    }
    std::string getKey(){
        return this->key;
    }
    int size(){return this->numOfElements;}
    bool is_array = false;
    virtual void* getData() = 0;


};
class JsonObjectInt: public JsonObject{

    int *value = new int[0];
    protected:
        int Extend() override{
            int * tempdata = new int[numOfElements++];
            for(int i =0;i<numOfElements-1;i++){
                tempdata[i] = value[i];
            }
            value = tempdata;
            return 0;
        }


    public:
        JsonObjectInt(){
        }
        JsonObjectInt(int value){
            Extend();
            this-> value[0] = value;
        }
        void* getData() override{
            return static_cast<void*>(value);
        }
};

JsonObject *jo = new JsonObjectInt();

int main(){

    int* data =  static_cast<int*>(jo->getData());

    std::cout << data[0];
}