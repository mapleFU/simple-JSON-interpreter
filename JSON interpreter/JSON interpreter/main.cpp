#include <iostream>
#include <cstdio>
#include "JSON.h"
int main() {
//    auto fileCon = ParseJSONFromFile("/Users/fuasahi/Desktop/testdata/PackageJSON.json");
//    auto ptr_json = ParseJSON("\"author\":");
    std::string tmp("\"author\": ");
    std::cout<<GetNameStr(tmp);
    auto pjson = CreateObject();
//    if(pjson->type == JSON_OBJECT)
//        puts("true");
    return 0;
}