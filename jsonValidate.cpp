  int GenerateDataTree(){
        std::string key;
        std::string value;
        bool inData = false;
        bool inValue = false;
        for(int i =0 ;i<data.size();i++){
            if(inValue  && (data[i] == ',' || data[i]== '}' || data[i] == '{')){
                 std::cout << key << ":" << value << std::endl;
                    key = "";
                    value = "";
                    inData= false;
                    inValue = false;
                    continue;
            }
            if(data[i] == '"'  && inValue == false){
                inData = !inData;
                continue;
            }
           
            if(data[i] == ':'){
                inValue = true;
                inData= true;
                continue;
            }
            if(inData){
                if(inValue){
                    value+=data[i];
                }else{
                    key+=data[i];
                }
            }
        }




        return 0;
    }