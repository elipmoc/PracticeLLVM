#include <iostream>
#include "APP.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
using practicellvm::Token;
using practicellvm::TokenType;
int main(){
    Token token("114514",TokenType::TOK_DIGIT,15);
    std::cout<<static_cast<int>(token.GetTokenType())<<std::endl;
    std::cout<<token.GetTokenString()<<std::endl;
    std::cout<<token.GetNumberValue()<<std::endl;
    std::cout<<token.GetLine()<<std::endl;
    return 0;
}