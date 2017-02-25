#include <iostream>
#include <cstdio>
#include "JSON.h"
bool isnum(const std::string &str, double &dnum){
    std::istringstream inS(str);
    if(inS >> dnum)
        return true;
    else
        return false;
}
int main() {
    auto ptrJ = ParseJSONFromFile("/Users/fuasahi/Desktop/PackageJSON.json");
    PrintJSON(ptrJ);

//    double test = 0;
//    if(isnum("72.364",test)){
//        std::cout<<test<<std::endl;
//    }
    return 0;
}