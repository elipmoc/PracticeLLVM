#pragma once

#include "Token.hpp"
#include <vector>

namespace practicellvm{

//切り出したToken格納用クラス
class TokenStream{
    private:
    std::vector<Token*> tokens;
    size_t curIndex;

    public:
    TokenStream():curIndex(0){}
    ~TokenStream();

    const bool UngetToken(const int times=1);
    const bool GetNextToken();
    const bool PushToken(Token* token){
        tokens.push_back(token);
        return true;
    }
    Token GetToken();

    //トークンの種類を取得
    const TokenType GetCurType()const{
        return tokens[curIndex]->GetTokenType();
    }
    //トークンの文字表現を取得
    const std::string GetCurString()const{
        return tokens[curIndex]->GetTokenString();
    }
    //トークンの数値を取得
    const int GetCurNumVal()const{
        return tokens[curIndex]->GetNumberValue();
    }
    //現在のインデックスを取得
    const int GetCurIndex()const{return curIndex;}
    //インデックス指定した値に設定
    const bool ApplyTokenIndex(const int index){
        curIndex=index;
        return true;
    }

    const bool PrintTokens();
};
}