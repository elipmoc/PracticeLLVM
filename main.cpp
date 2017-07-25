#include <iostream>
#include "APP.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "Lexer.hpp"
#include "AstBase.hpp"
#include "FunctionAST.hpp"
#include "Parser.hpp"
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