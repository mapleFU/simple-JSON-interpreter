//
// Created by xxx on 2017/2/23.
//

#ifndef JSON_INTERPRETER_JSON_H
#define JSON_INTERPRETER_JSON_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <iostream>

const int JSON_FALSE  = 0;
const int JSON_TRUE  = 1;
const int JSON_NULL  = 2;
const int JSON_NUMBER  = 3;
const int JSON_STRING  = 4;
const int JSON_ARRAY  = 5;
const int JSON_OBJECT  = 6;

struct JSON{
    using JSON_TYPE = int;
    JSON_TYPE type;
    std::string DataStr;
    int ValueInt;
    double ValueDouble;
    std::string VarName;
    JSON() = default;
    JSON(JSON_TYPE T): type(T) {}
    JSON(JSON_TYPE T, double num): type(T), ValueDouble(num) {}
    JSON(JSON_TYPE T, int num):type(T), ValueInt(num) {}
    JSON(const std::string &str): type(JSON_STRING), DataStr(str) {}

private:
    std::vector<std::shared_ptr<JSON> > VecObjs;  //保存底层的sVecs->对象
    std::map<std::string, std::shared_ptr<JSON>> VarAndArrs;        //保存var & arr
};

std::shared_ptr<JSON> ParseJSONFromFile(const std::string file_name);       //解析JSON文件,返回指向文件指针
void  PrintJSON(const std::shared_ptr<JSON> item, std::ostream &os = std::cout);      //将JSON输出到输出流


//creat
std::shared_ptr<JSON> CreateNULL();
std::shared_ptr<JSON> CreateTrue();
std::shared_ptr<JSON> CreateFalse();
std::shared_ptr<JSON> CreateBool(int b);
std::shared_ptr<JSON> CreateNumber(double num);
std::shared_ptr<JSON> CreateString(const string&);
std::shared_ptr<JSON> CreateArray();
std::shared_ptr<JSON> CreateObject();

std::shared_ptr<JSON> creatType(JSON::JSON_TYPE);
void AddItemToArray(std::shared_ptr<JSON> array, std::shared_ptr<JSON> item);

#endif //JSON_INTERPRETER_JSON_H
