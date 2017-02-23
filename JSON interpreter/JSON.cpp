//
// Created by 付旭炜 on 2017/2/23.
//
#include "JSON.h"

static std::shared_ptr<JSON> creatType(JSON::JSON_TYPE T){
    std::shared_ptr<JSON> pJ = std::make_shared<JSON>(JSON(T));
    return pJ;
}
//creat JSON
std::shared_ptr<JSON> CreatObject(){
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
    
}

