//
// Created by 付旭炜 on 2017/2/23.
//

#ifndef JSON_INTERPRETER_JSON_H
#define JSON_INTERPRETER_JSON_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

const int JSON_FALSE  = 0;
const int JSON_TRUE  = 1;
const int JSON_NULL  = 2;
const int JSON_NUMBER  = 3;
const int JSON_STRING  = 4;
const int JSON_ARRAY  = 5;
const int JSON_OBJECT  = 6;

struct JSON{
    friend std::shared_ptr<JSON> ParseJSONFromFile(const std::string &file_name);

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


//parse & print
extern std::shared_ptr<JSON> ParseJSONFromFile(const std::string &file_name);       //解析JSON文件,返回指向文件指针
extern void  PrintJSON(const std::shared_ptr<JSON> item, std::ostream &os = std::cout);      //将JSON输出到输出流
extern std::shared_ptr<JSON> ParseJSON(const std::string &BeParsed);               //BeParsed为需要解析的字符串,函数返回指向解析后的JSON的指针
extern std::string GetNameStr(const std::string& tmpMsg);


//creat
extern std::shared_ptr<JSON> CreateNULL();
extern std::shared_ptr<JSON> CreateTrue();
extern std::shared_ptr<JSON> CreateFalse();
extern std::shared_ptr<JSON> CreateBool(int b);
extern std::shared_ptr<JSON> CreateNumber(double num);
extern std::shared_ptr<JSON> CreateString(const std::string&);
extern std::shared_ptr<JSON> CreateArray();
extern std::shared_ptr<JSON> CreateObject();

void AddItemToArray(std::shared_ptr<JSON> array, std::shared_ptr<JSON> item);

#endif //JSON_INTERPRETER_JSON_H
