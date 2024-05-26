#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "t.h"

TEST_CASE("Multiple data, string and double "){
    std::string dataString;
    double dataDouble;
    int size = 0;
    Objects* o = new jsonObject();
    Values *pointer = new vNumber(new double(123));
    ((jsonObject*)o)->AddPair(new Pair("asd",pointer));
    Pair p3 = *new Pair("dog", new vNumber(12));
    ((jsonObject*) o)->AddPair(&p3);
    ((jsonObject*)o)->AddPair(new Pair("stringer", new vString("dog")));
    

    
    ((jsonObject*)o)->RemovePair(0);
    size = o->Size();
    CHECK(size == 2);
    dataDouble= *(double*)(((jsonObject*)o)->ReturnPair("dog")
    ->value->getData());
    CHECK(dataDouble == 12);

    
    ((jsonObject*)o)->ReturnPair("dog")->value->setData((void*)new double (5555)); 
        CHECK(dataDouble == 12);

    dataDouble = *(double*)(((jsonObject*)o)->ReturnPair("dog")
    ->value->getData());
    CHECK(dataDouble == 5555);
    dataString= *(std::string*)(((jsonObject*)o)->ReturnPair("stringer")->value->getData());
    CHECK(dataString == "dog");


    ((jsonObject*)o)->ReturnPair("stringer")->value->setData((void*)new std::string("paca")); 
    
    dataString = *(std::string*)(((jsonObject*)o)->ReturnPair("stringer")->value->getData());
    CHECK(dataString == "paca");
    

    delete pointer;
    delete o;
}


TEST_CASE("Test deleting, with key and with index"){


    int size = 0;

    Objects* o = new jsonObject();
    Values *pointer = new vNumber(new double(123));
    ((jsonObject*)o)->AddPair(new Pair("asd",pointer));
    size = o->Size();
    CHECK(size == 1);

    Pair p3 = *new Pair("dog", new vNumber(12));
    ((jsonObject*) o)->AddPair(&p3);
    size = o->Size();
    CHECK(size == 2);
    
    ((jsonObject*)o)->AddPair(new Pair("stringer", new vString("dog")));
    size = o->Size();
    CHECK(size == 3);
    
    ((jsonObject*)o)->RemovePair(0);
    size = o->Size();
    CHECK(size == 2);
    
    ((jsonObject*)o)->RemovePair("stringer");
    size = o->Size();
    CHECK(size == 1);
    
    double data =*(double*)(((jsonObject*)o)->ReturnPair("dog")->value->getData());
    CHECK(data == 12);
    

    delete pointer;
    delete o;
}