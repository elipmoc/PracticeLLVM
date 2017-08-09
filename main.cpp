#include <iostream>
#include "CodeGen.hpp"
using practicellvm::Token;
using practicellvm::TokenType;
using practicellvm::TokenStream;
using practicellvm::LexicalAnalysis;
int main(){
    TokenStream* tokenStream;
    tokenStream=LexicalAnalysis("test.txt");
    if(tokenStream==nullptr){
        std::cout<<"NullPtr!!"<<std::endl;
        return 0;
    }
    tokenStream->PrintTokens();
    return 0;
}