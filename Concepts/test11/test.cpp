#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "t.h"

TEST_CASE("save data in json ojbect - Multiple data, string and double "){
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
    

    // delete pointer;
    // delete o;
}


TEST_CASE("in object deleting, with key and with index"){


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
    

    // delete pointer;
    // delete o;
}

TEST_CASE("Array testing"){

    jsonArray *ja = new jsonArray();
    ja->AddValue(new vNumber(12));
    CHECK(ja->Size() == 1);
    double a =*(double*)ja->ReturnValue(0)->getData();

    CHECK(a==12);
}

TEST_CASE("array sizing, and value removing"){
    
    int currentSize =0;
    int currentVal = 0;
    
    jsonArray *ja = new jsonArray();
    ja->AddValue(new vNumber(12));
    currentVal = *(double*)ja->ReturnValue(0)->getData();
    CHECK(currentVal == 12);
    ja->AddValue(new vNumber(55));
    currentVal = *(double*)ja->ReturnValue(1)->getData();
    CHECK(currentVal == 55);
    currentSize = ja->Size();
    CHECK(currentSize == 2);
    ja->RemoveValue(0);
    currentSize = ja->Size();
    CHECK(currentSize == 1);

    currentVal = *(double*)ja->ReturnValue(0)->getData();
    CHECK(currentVal == 55);
}


TEST_CASE("Boolean test"){

    vBoolean tester = new vBoolean();
    tester.setData((void*)true);
    CHECK(*(bool*)tester.getData(0));
    tester.removeData(0);
    CHECK_FALSE(*(bool*)tester.getData(0));
    //std::cout << (bool*) tester.getData(0);

}

TEST_CASE("Array as object"){
    
    jsonObject *jo = new jsonObject();
    jo->AddPair("arr",new vArray(new jsonArray()));
    jsonArray *ar = (jsonArray*)jo->ReturnPair("arr")->value->getData();
    ar->AddValue(new vNumber(5));
    ar->AddValue(new vNumber(2));
    ar->AddValue(new vNumber(3));
    std::cout << *(double*)ar->ReturnValue(0)->getData();
    CHECK(ar->Size() == 3);
    ar->RemoveValue(2);
    CHECK(*(double*)ar->ReturnValue(1)->getData()==2);
    CHECK(ar->Size() == 2);

}

