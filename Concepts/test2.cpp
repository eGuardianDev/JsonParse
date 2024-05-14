

#include <iostream>
//read 
void* returnVoidPointer() {
    int* ptr = new int(10);
    return static_cast<void*>(ptr);
}

int main() {
    void* voidPtr = returnVoidPointer();
    
    // Statically cast the void pointer to int pointer
    int* intPtr = static_cast<int*>(voidPtr);

    // Accessing the value
    std::cout << "Value stored at the memory location: " << *intPtr << std::endl;

    // Remember to free the allocated memory
    delete intPtr;

    return 0;
}