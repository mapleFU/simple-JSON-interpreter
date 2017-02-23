//
// Created by 付旭炜 on 2017/2/23.
//
#include "JSON.h"

static std::shared_ptr<JSON> creatType(JSON::JSON_TYPE T){
    std::shared_ptr<JSON> pJ = std::make_shared<JSON>(JSON(T));
    return pJ;
}

//creat JSON
std::shared_ptr<JSON> CreateObject(){
    return creatType(JSON_OBJECT);
}
std::shared_ptr<JSON> CreateNULL(){
    return creatType(JSON_NULL);
}
std::shared_ptr<JSON> CreateTrue(){
    return creatType(JSON_TRUE);
}
std::shared_ptr<JSON> CreateFalse(){
    return creatType(JSON_FALSE);
}
std::shared_ptr<JSON> CreateBool(int b){
    if(b == 0)
        return CreateFalse();
    else
        return CreateTrue();
}
std::shared_ptr<JSON> CreateNumber(double num){
    std::shared_ptr<JSON> pJ = std::make_shared<JSON>(JSON(JSON_NUMBER, num));
    return pJ;
}
std::shared_ptr<JSON> CreateString(const std::string& str){
    std::shared_ptr<JSON> pJ = std::make_shared<JSON>(JSON(str));
    return pJ;
}
std::shared_ptr<JSON> CreateArray(){
    return creatType(JSON_ARRAY);
}


//parse & print
//指定格式:如果JSON为object或者array类型，那么首尾的 '{' 或 '[' 需要各占一行，其余的 '{'  或 '['  需要在 ':' 后一个空格输出

std::string GetNameStr(const std::string& tmpMsg){  //获取变量名
    using namespace std;
    string strRet;
    if(tmpMsg.empty() || tmpMsg == ":")
        return strRet;
    if(tmpMsg[0] == '\"');  //"varname",不知道这里有没有好的处理orz
    {
        bool getFront(false);       //有前引号
        for (auto &ch:tmpMsg){
            if(ch == '\"' || ch == ':'){
                if(getFront || ch == ':')
                    break;
                else
                    getFront = true;
            } else{
                strRet += ch;
            }
        }
    }
    return strRet;
}

std::shared_ptr<JSON> ParseJSON(const std::string &BeParsed){
    //BeParsed为需要解析的字符串,函数返回指向解析后的JSON的指针
    //如果遇到[,{etc是否考虑连续调用
    using namespace std;
    std::shared_ptr<JSON> JsonNew;
    if(BeParsed.empty())
        JsonNew = nullptr;
    if(BeParsed[0] == '{') {    //start
        JsonNew = CreateObject();
    }
    else{       //json is a var or array,eg "author": "Giuem <i@giuem.com>",
        JsonNew = make_shared<JSON>();      //empty type;
        istringstream strIn(BeParsed);
        string tmpMsg;
        bool getVarName(false);               //是否获取名称；
        while (strIn >> tmpMsg){
            if(tmpMsg.empty)
                break;
            if(!getVarName){
                JsonNew->VarName = GetNameStr(tmpMsg);
                getVarName = true;
            }
            //TODO:正常人都看得出来这里有问题啊啊啊

            else if(tmpMsg[0] == '\"' || isalpha(tmpMsg[0])){       //已经getVarName,是一个对应的str,num
                string content = GetNameStr(tmpMsg);
                bool isdig(true);               //判断是否是数字...
                if(isalpha(tmpMsg[0])){
                    auto PosEnd = tmpMsg.find_first_not_of("+-01234567890.");
                    if(PosEnd <= tmpMsg.size() - 1)
                        isdig = false;
                }
                if(!isdig){
                    JsonNew->type = JSON_STRING;
                    JsonNew->DataStr = GetNameStr(content);
                } else{
                    JsonNew->type = JSON_NUMBER;
                    if(content.find('.') >= content.size())   //变成数字
                        JsonNew->ValueDouble = stod(content);
                    else
                        JsonNew->ValueInt = stoi(content, 0);
                }
                break;
            }
            else if(tmpMsg[0] == '[')
                JsonNew->type = JSON_ARRAY;
            else if(tmpMsg[0] == '{')
                JsonNew->type = JSON_OBJECT;
        }
        //TODO:
    }
    return JsonNew;
}

std::shared_ptr<JSON> ParseJSONFromFile(const std::string &file_name){  //解析JSON文件,返回指向文件指针
    using namespace std;
    ifstream FileIn(file_name);
    string msg;
    shared_ptr<JSON> JsonFile;  //json文件
    while(FileIn>>msg) {        //gettype
        JsonFile = ParseJSON(msg);
        if(JsonFile == nullptr)
            continue;
    }

    while (getline(FileIn,msg)){    //我不晓得是否有必要(T T)
        auto PartJson = ParseJSON(msg);
        if(PartJson->type == JSON_OBJECT)
            JsonFile->VecObjs.push_back(PartJson);      //如果是obj,将指针存入vec(是否可以考虑用bst?)
        else{
            JsonFile->VecObjs.push_back(PartJson);      //保持顺序...
            JsonFile->VarAndArrs[PartJson->VarName] = PartJson; //arr & var
        }
    }
    return JsonFile;
}

