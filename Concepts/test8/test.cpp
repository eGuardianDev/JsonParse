
#include "t.h"

int main(){

    Objects* o = new jsonObject();
   ((jsonObject*) o)->AddPair(new Pair("123",new vNumber(new double(12))));
    // Values *pointer = new vNumber(new double(1));
    // Pair p3 = *new Pair("dog", pointer);

    
    // ((jsonObject*) o)->AddPair(&p3);
    // Pair *p2 = ((jsonObject*)o)->ReturnPair("123");
    
    // std::cout << *(double*)p2->value->getData();
    // p2 = ((jsonObject*)o)->ReturnPair("doge");

    // std::cout << *(double*)p2->value->getData();
    // delete o;
    return 0;
}