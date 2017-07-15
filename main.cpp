#include <iostream>
#include "APP.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
using practicellvm::Token;
using practicellvm::TokenType;
using practicellvm::TokenStream;
int main(){
    Token* token =new Token("114514",TokenType::TOK_DIGIT,15);
    Token* token2 =new Token("810",TokenType::TOK_DIGIT,15);
    Token* token3 =new Token("github",TokenType::TOK_SYMBOL,15);
    Token* token4 =new Token("phpはくそ",TokenType::TOK_EOF,15);
    TokenStream tokenStream;
    tokenStream.PushToken(token);
    tokenStream.PushToken(token2);
    tokenStream.PushToken(token3);
    tokenStream.PushToken(token4);
    tokenStream.PrintTokens();
    return 0;
}