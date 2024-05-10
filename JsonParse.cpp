#pragma one
#include <string>
#include <iostream>

class DataSave{
    private:
    std::string fileData;
    std::string filePath;

    public:

    int openFile(){   
        return -1;
    }
    int saveFile(){   
        return -1;
    }
    int saveAsFile(){   
        return -1;
    }
    int closeFile(){   
        return -1;
    }



};



class JsonObject{
    enum valueType{
        tString,
        tNumber,
        tObject
    };
    class dataClass{
        public:
        std::string key;
        valueType ValueType;
    
        virtual void setValue(std::string data);
        virtual void setValue(dataClass data);
        virtual std::string getValue();
        virtual dataClass getValue();
    };

    class dataClassString : public dataClass{
        public:
        std::string value;
        void setValue(std::string data ) override{
            this->value = data;
            this->ValueType = tString;
        }
        std::string getValue() override{
            return value;
        }
    };
    class dataClassNumber : public dataClass{
        public:
        float value;
        void setValue(std::string data ) override{
            this->value = stof(data);
            this->ValueType = tNumber;
        }
        std::string getValue() override{
            return std::to_string(value);
        }
    };
    class dataClassNode : public dataClass{
        public:
        dataClass value;
        void setValue(dataClass data ) override{
            this->value = data;
            this->ValueType = tNumber;
        }
        dataClass getValue() override{
            return std::to_string(value);
        }
    };

    dataClass *data;

    JsonObject(){
        data = &dataClassString();
   
    }


};


