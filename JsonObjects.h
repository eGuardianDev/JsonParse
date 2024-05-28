#pragma once

#include "Rooter.h"

#include <iostream>

#include <cstddef>
#include <ostream>
#include <vector>



class Values : public Rooter{
    public:
    Values() {};
    virtual void print(std::ostream&) = 0;

    virtual Values* Clone() = 0;
    virtual int setData(void*) =0 ;
    virtual int removeData(unsigned int) = 0;
    virtual void* getData(unsigned int index = 0) = 0;
    virtual ~Values() {};
};
struct Pair{
    public:
    std::string key;
    Values *value;
    Pair* Clone(){
        return new Pair(this->key, value->Clone());
    }
    Pair(){
     
    }Pair(Pair & p){
        this->key  =p.key;
        this->value = p.value;
    }
    Pair(std::string key, Values* value){
        this->key = key;
        this->value = value;
    }
    ~Pair(){
        // delete this->value;
    }
};

class Objects : public Rooter{
    
    protected:
    size_t size;
    virtual void expand() = 0;
    virtual void shrink() = 0;
    public:
    size_t Size();
    virtual void print(std::ostream&) = 0;
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
    void print(std::ostream&) override;


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
    jsonArray(jsonArray&);
    Objects* Clone() override;
    void print(std::ostream&) override;

    int AddValue(Values* value);
    Values* ReturnValue(unsigned int index);
    int RemoveValue(unsigned int index);
    
};



enum ValueType{
    eString = 0,
    eNumber = 1,
    eObject = 2,
    eArray = 3,
    eBoolean = 4,
    notFound = -1
};

// ValueType TypeOfValue(Values* val);
// ValueType TypeOfObject(Objects* obj);

class vObject : public Values{
    protected:
    jsonObject *value;
    public:

    Values* Clone() override;
    int setData(void*) override ;
    int removeData(unsigned int) override;
    void* getData(unsigned int) override;
    void print(std::ostream&) override;

    vObject();
    vObject(jsonObject);
    vObject(jsonObject*);
    vObject(vObject&);
    vObject(double*);
    ~vObject() override;
};

class vArray : public Values{
    protected:
    jsonArray *value;
    public:

    Values* Clone() override;
    int setData(void*) override ;
    int removeData(unsigned int) override;
    void* getData(unsigned int) override;
    void print(std::ostream&) override;

    vArray();
    vArray(vArray&);
    vArray(jsonArray*);
    vArray(jsonArray);
    ~vArray() override;
};

class vString : public Values{
    protected:
    std::string *value;
    public:

    Values* Clone() override;
    int setData(void*) override ;
    int removeData(unsigned int) override;
    void* getData(unsigned int) override;
    void print(std::ostream&) override;

    vString();
    vString(vString&);
    vString(std::string);
    vString(std::string*);
    ~vString() override;
};

class vNumber : public Values{
    protected:
    double *value;
    public:

    Values* Clone() override;
    int setData(void*) override ;
    int removeData(unsigned int) override;
    void* getData(unsigned int) override;
    void print(std::ostream&) override;

    vNumber();
    vNumber(vNumber&);
    vNumber(double*);
    vNumber(double);
    ~vNumber() override;
};

class vBoolean : public Values{
    protected:
    bool *value;
    public:

    Values* Clone() override;
    int setData(void*) override ;
    int removeData(unsigned int) override;
    void* getData(unsigned int) override;
    void print(std::ostream&) override;

  
    vBoolean();
    vBoolean(vBoolean&);
    vBoolean(bool*);
    vBoolean(bool);
    ~vBoolean() override;
};
