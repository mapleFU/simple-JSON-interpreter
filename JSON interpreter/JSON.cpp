//
// Created by 付旭炜 on 2017/2/23.
//
#include "JSON.h"

#define DEFAULT
#undef DEFAULT
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
static inline bool isnum(const std::string &str, double &dnum){
    std::istringstream inS(str);
    if(inS >> dnum)
        return true;
    else
        return false;
}
static inline bool StringCmp(const std::string& s1, const std::string& s, unsigned int beg){
    auto len = s1.size();
    if(!s.compare(beg, len, s1))
        return true;
    else
        return false;
}

std::shared_ptr<JSON> ParseJSON(const std::string &BeParsed) {
    //BeParsed为需要解析的字符串,函数返回指向解析后的JSON的指针
    //如果遇到[,{etc是否考虑连续调用
    using namespace std;
    std::shared_ptr<JSON> JsonNew = nullptr;
    string msgFirst, mesSec;
    if(BeParsed.empty())
        JsonNew = nullptr;
    if(BeParsed == "{" || BeParsed == "{\r") {    //start
        JsonNew = CreateObject();
    }
    else{       //json is a var or array,eg "author": "Giuem <i@giuem.com>",

        bool OnlyOne(true);
        unsigned int pos(0),mesPos(0);        //需要处理的位置
        while(BeParsed[mesPos] == ' '){
            mesPos++;
        }       //mesPos指向第一个非空格

        if(BeParsed[mesPos] != '\"'){ //first not a string?
            //if it's a number
            double tmpNum;
            pos = BeParsed.find_first_not_of(num_of_str, mesPos);
            if(isnum(BeParsed.substr(mesPos, pos - mesPos), tmpNum)){
                mesPos = pos;
                JsonNew = CreateNumber(tmpNum);
            }
            //if it's true
            else if(StringCmp("true", BeParsed, mesPos)){
                mesPos += 4;
                JsonNew = CreateTrue();
            }
            //false
            else if(StringCmp("false", BeParsed, mesPos)){
                mesPos += 5;
                JsonNew = CreateFalse();
            }
            else if(StringCmp("null", BeParsed, mesPos)){// null
                mesPos += 4;
                JsonNew = CreateNULL();
            }
            else{
                OnlyOne = false;
                pos = BeParsed.find_first_of(": ");
                msgFirst = BeParsed.substr(mesPos, pos);
                if(BeParsed[pos] == ':')
                    mesPos = pos + 1;
                else
                    mesPos = BeParsed.find_first_of(":", pos) + 1;
            }
        }
        else{       //有"开头
            auto right = BeParsed.find(":");
            if(right < BeParsed.size()){
                OnlyOne = false;
            }
            auto r = BeParsed.find_first_of("\"", mesPos + 1);
            msgFirst = BeParsed.substr(mesPos + 1, r - mesPos - 1);
            mesPos = static_cast<unsigned int>(right + 1);
        }

        if(OnlyOne){
            if(JsonNew == nullptr){     //is a string and only one
                JsonNew = CreateString(msgFirst);
            }
        }
        else{                           //with: and not only one
            JsonNew = make_shared<JSON>();
            JsonNew->VarName = msgFirst;
            while(BeParsed[mesPos] == ' '){     //会超出么
                mesPos++;
            }

            double tmpNum;
            pos = BeParsed.find_first_not_of(num_of_str, mesPos + 1);

            if(BeParsed[mesPos] == '{'){
                JsonNew->type = JSON_OBJECT;
            }
            else if(BeParsed[mesPos] == '['){
                JsonNew->type = JSON_ARRAY;
            }
            else if(BeParsed[mesPos] == '\"'){
                pos = BeParsed.find_first_of("\"", mesPos + 1);
                mesSec = BeParsed.substr(mesPos + 1, pos - mesPos - 1);
                JsonNew->type = JSON_STRING;//TODO:哪有问题
                JsonNew->DataStr = mesSec;
            }
                //if it's num
            else if(isnum(BeParsed.substr(mesPos, pos - mesPos), tmpNum)){
                mesPos = pos;
                JsonNew->type = JSON_NUMBER;
                if(ceil(tmpNum) == floor(tmpNum))
                    JsonNew->ValueInt = static_cast<int>(tmpNum);
                else
                    JsonNew->ValueDouble = tmpNum;
            }
                //if it's true
            else if(StringCmp("true", BeParsed, mesPos)){
                mesPos += 4;
                JsonNew->type = JSON_TRUE;
            }
                //false
            else if(StringCmp("false", BeParsed, mesPos)){
                mesPos += 5;
                JsonNew->type = JSON_FALSE;
            }
            else if(StringCmp("null", BeParsed, mesPos)){// null
                mesPos += 4;
                JsonNew->type = JSON_NULL;
            }
        }
    }

//    if(!msgFirst.empty())
//        cout<<msgFirst<<"  ->testFirst"<<endl;
//    if(!mesSec.empty())
//        cout<<mesSec<<"  ->testSec"<<endl;
//    putchar('\n');

    return JsonNew;
}

std::shared_ptr<JSON> ParseJSONFromFile(const std::string &file_name){  //解析JSON文件,返回指向JSON指针
    using namespace std;
    ifstream FileIn(file_name);
    string msg;
    shared_ptr<JSON> JsonFile;  //json文件
    while(FileIn>>msg) {        //gettype
        JsonFile = ParseJSON(msg);
        if(JsonFile == nullptr)
            continue;
        else
            break;
    }
    std::stack<std::shared_ptr<JSON>> Pre;        //保存var & obj
    while (getline(FileIn,msg)){    //我不晓得是否有必要(T T)
//        puts("SHIT");
        if(msg.empty() || msg == "\r")      //Mac '\r'
           continue;
        if(msg.find("}") <= msg.size() || msg.find("]") <= msg.size()){
            if(Pre.empty())
                break;          //显然,完了
            else{
                Pre.pop();      //弹出
                continue;
            }
        }

        auto PartJson = ParseJSON(msg);
        if(PartJson == nullptr)
            continue;

        auto tmpJson = JsonFile;
        bool StackEmpty(true);
        if(!Pre.empty()){       //若栈非空
            StackEmpty = false;
            JsonFile = Pre.top();
        }
        JsonFile->VecObjs.push_back(PartJson);      //保持顺序,将解析得到的parseJson放入vector
//        JsonFile->VarAndArrs[PartJson->VarName] = PartJson; //arr & var

        if(PartJson->type == JSON_OBJECT || PartJson->type == JSON_ARRAY){
            Pre.push(PartJson);
        }
        if(!StackEmpty)
            JsonFile = tmpJson;
    }
    return JsonFile;
}


inline void showVecs(std::vector<std::shared_ptr<JSON> >  Jvec, std::ostream &os){
    for (int i = 0; i < Jvec.size() - 1; ++i) {
        PrintJSON(Jvec[i], os);
        os<<',';
    }
    PrintJSON(Jvec.back(), os);
}
void PrintJSON(const std::shared_ptr<JSON> item, std::ostream &os){
    switch (item->type){
        case JSON_NULL:
        case JSON_FALSE:
        case JSON_TRUE:
            os<<item->DataStr;
        case JSON_NUMBER:
            if(item->ValueInt != 0)
                os<<item->ValueInt;
            else
                os<<item->ValueDouble;
            break;
        case JSON_STRING:
            os<<"\""<<item->DataStr<<"\"";
            break;
        case JSON_ARRAY:
            os<<'[';
            showVecs(item->VecObjs, os);
            os<<']';
            break;
        case JSON_OBJECT:
            os<<'{';
            showVecs(item->VecObjs, os);
            os<<'}';
            break;
    }
}
