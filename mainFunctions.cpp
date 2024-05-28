
#include <iostream>
#include "JsonParse.cpp"
enum cmdData{
    ehelp = 0,
    eopen = 1,
    esave =2,
    esaveas = 3,
    eclose = 4,
    eexit = 5,
    eprint = 6,
    evalidate= 7
};

JsonParser ds;


//https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
int help(){
    std::cout << "\x1B[32m\t" << "help" << "\033[0m" << " - shows you functions" << std::endl;
    std::cout << "\x1B[32m\t" << "open \x1B[96m{files-path}" << "\033[0m" << " - opens json file" << std::endl;
    std::cout << "\x1B[32m\t" << "data" << "\033[0m" << " - display currently load data" << std::endl;
    std::cout << "\x1B[32m\t" << "save" << "\033[0m" << " - saves currently opened  file" << std::endl;
    std::cout << "\x1B[32m\t" << "saveas \x1B[96m{files-path}" << "\033[0m" << " - saves currently opened file as new file" << std::endl;
    std::cout << "\x1B[32m\t" << "close" << "\033[0m" << " - closes currently opened file" << std::endl;
    std::cout << "\x1B[32m\t" << "exit" << "\033[0m" << " - exits program" << std::endl;
    return 0;
}

int open(std::string filePath){
    return ds.openFile(filePath);
}

int save(){
    return ds.saveFile();
}

int saveAs(std::string filePath){
    return ds.saveAsFile(filePath);
}
int close(){
    return ds.closeFile();
}
int _displayData(){
    return ds.displayData();
}
int print(){
    return ds.DisplayTree();
}



int exit(){
    ds.closeFile(false);
    return 0;
}