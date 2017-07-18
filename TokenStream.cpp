#include "APP.hpp"
#include "TokenStream.hpp"
#include <iostream>
#include <iomanip>
namespace practicellvm{

    //デストラクタ
    TokenStream::~TokenStream(){
        for(auto&& item:tokens)
            Safe_Delete(item);
        tokens.clear();
    }

    //トークン取得
    //<return>curIndex番目のToken
    const Token TokenStream::GetToken()const{
        return *tokens[curIndex];
    }

    //インデックスを一つ増やして次のトークンにすすめる
    //<return>成功時:true 失敗時:false
    const bool TokenStream::GetNextToken(){
        return 
            (tokens.size()-1<=curIndex)?false:(curIndex++,true);
    }

    //インデックスtimes回戻す
    const bool TokenStream::UnGetToken(const int times){
        for(size_t i=0;i<times;++i){
            if(curIndex==0)
                return false;
            curIndex--;
        }
        return true;
    }

    const std::string TokenTypeToString(TokenType tokenType){
        std::string str="";
        switch(tokenType){
            case TokenType::TOK_IDENTIFIER:
                str="識別子      ";
                break;
            case TokenType::TOK_DIGIT:
                str="数値リテラル";
                break;
            case TokenType::TOK_SYMBOL:
                str="記号        ";
                break;
            case TokenType::TOK_INT:
                str="int         ";
                break;
            case TokenType::TOK_RETURN:
                str="return      ";
                break;
            case TokenType::TOK_EOF:
                str="eof         ";
                break;
            default:
                str="ErrorType ";
                break;
        }
        return str;
    }

    //格納されたトークン一覧を表示する
    const bool TokenStream::PrintTokens(){
        for(auto&& item:tokens){
            std::cout<<TokenTypeToString(item->GetTokenType())<<":";
            if(item->GetTokenType()!=TokenType::TOK_EOF)
                std::cout<<item->GetTokenString();
            std::cout<<std::endl;
        }
        return true;
    }
}