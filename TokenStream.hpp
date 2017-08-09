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

    bool UnGetToken(const int times=1);
    bool GetNextToken();
    bool PushToken(Token* token){
        tokens.push_back(token);
        return true;
    }
    Token GetToken()const;

    //トークンの種類を取得
    TokenType GetCurType()const{
        return tokens[curIndex]->GetTokenType();
    }
    //トークンの文字表現を取得
    std::string GetCurString()const{
        return tokens[curIndex]->GetTokenString();
    }
    //トークンの数値を取得
    int GetCurNumVal()const{
        return tokens[curIndex]->GetNumberValue();
    }
    //現在のインデックスを取得
    int GetCurIndex()const{return curIndex;}
    //インデックス指定した値に設定
    bool ApplyTokenIndex(const int index){
        curIndex=index;
        return true;
    }

    bool PrintTokens();
};
}