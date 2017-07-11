#include <iostream>
#include "APP.hpp"
int main(){
    auto a=new int;
    std::cout<<a<<std::endl;
    Safe_Delete(a);
    std::cout<<a<<std::endl;
    return 0;
}