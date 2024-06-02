

//https://stackoverflow.com/questions/46535422/c-copying-another-file
class JsonParser{ //privessly called datasave
    private:  
    std::fstream src;
    std::string data;
    std::string fileName;

    int LoadFile(){
        // currentNode = &mainNode;
        std::stringstream buf;
        buf << src.rdbuf();
        data = buf.str();

        //https://stackoverflow.com/questions/556277/trim-remove-a-tab-t-from-a-string
        data.erase(std::remove(data.begin(), data.end(), '\t'), data.end());
       // std::cout <<data;
       // data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
       // data = CleanSpaces(data);
        return 0;
}

    public:
    std::string getFileName(){
        return fileName;
    }
    int open = 0;
    int openFile(std::string file){  
        if(src.is_open()){
            Messenger::LogWarning("You need to close the file, before opening new one");
            return 2;
        } 
        src.open(file, std::ios::in | std::ios::out);
        if(src.fail()){
            Messenger::LogError("File failed to open");
            return 1;
        }
        open++;
        Messenger::LogInfo("Opened:", file);
        fileName = file;
        LoadFile();
        // std::cout << data;
        return 0;
    }
   
 }
    //https://stackoverflow.com/questions/53359727/how-to-save-and-also-read-c-fstream-file-without-closing-it  
    int saveFile(){  
        if(!src.is_open()){
            Messenger::LogError("No file currently opened");
            return -1;

        }
        root->print(src, 0);
        // src.flush();
        Messenger::LogInfo("Successful saved.");
        return 0;
    }
    int saveAsFile(std::string file){  
        std::ofstream output;
        Messenger::LogInfo("Saving data to:", file);
        output.open(file, std::ios::out);
        if(output.fail()){
            Messenger::LogError("File failed to open");
            return 1;
        }
        root->print(output, 0);
        // output << src.rdbuf();
        output.close();
        Messenger::LogInfo("Successful saved.");
        return 0;
    }
    
    int closeFile(bool displayError = true){   
        if(!src.is_open() && displayError){
            Messenger::LogError("No file currently opened");
            return -1;
        } 
        if(src.is_open()){
            // root = nullptr;
            src.close();
        }
        return 0;
    }
};



