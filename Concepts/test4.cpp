#include <iostream>
//data parse



std::string 
JsonObject *jo = new JsonObjectInt();

int main(){

    int* data =  static_cast<int*>(jo->getData());

    std::cout << data[0];
}