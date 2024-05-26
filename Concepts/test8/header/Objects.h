#pragma once
#include "Rooter.h"


#include <cstddef>
#include <iostream>
#include <vector>


struct Pair{
    public:
    std::string key;
    Values *value;
    Pair* Clone(){
        return new Pair(this->key, value->Clone());
    }
    Pair(std::string key, Values* value){
        this->key = key;
        this->value = value;
    }
    ~Pair(){
        //delete this->value;
    }
};

class Objects : public Rooter{
    
    protected:
    size_t size;
    virtual void expand() = 0;
    virtual void shrink() = 0;
    public:
    size_t Size();
    virtual Objects* Clone() = 0;
    virtual ~Objects() {}
};

class jsonObject : public Objects{
    protected:
    Pair** pairs;

    void expand() override;
    void shrink() override;
 
    public:   
    jsonObject();
    jsonObject(jsonObject&);
    Objects* Clone() override ;
 
    void AddPair(Pair * pair);
    void AddPair(std::string key, Values* value);
    
    Pair* ReturnPair(std::string key);
    Pair* ReturnPair(unsigned int index);

    int RemovePair(std::string key);
    int RemovePair(unsigned int index);
    ~jsonObject() override;
};

class jsonArray : public Objects{
    protected:
    Values** values;
    void expand() override;
    void shrink() override;

    public:
    jsonArray();
    Objects* Clone() override;

    int AddValue(Values* value);
    Values* ReturnValue(unsigned int index);
    int RemoveValue(unsigned int index);
    
};