
#pragma once
#include "Rooter.h"
#include "Objects.h"

#include <iostream>

class Values : public Rooter{
    public:
    virtual Values* Clone() = 0;
    virtual int setData(void*) =0 ;
    virtual int removeData(unsigned int) = 0;
    virtual void* getData(unsigned int index = 0) = 0;
    virtual ~Values() = 0;
};

enum ValueType{
    eString = 0,
    eNumber = 1,
    eObject = 2,
    eArray = 3,
    eBoolean = 4,
    notFound = -1
};


class vObject : public Values{
    protected:
    jsonObject *value;
    public:

    Values* Clone() override;
    int setData(void*) override ;
    int removeData(unsigned int) override;
    void* getData(unsigned int) override;

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

    vArray();
    vArray(vArray&);
    vArray(jsonArray*);
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

  
    vBoolean();
    vBoolean(vBoolean&);
    vBoolean(bool*);
    vBoolean(bool);
    ~vBoolean() override;
};
