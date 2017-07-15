#include "APP.hpp"
#include "TokenStream.hpp"
#include <iostream>
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

    //格納されたトークン一覧を表示する
    const bool TokenStream::PrintTokens(){
        for(auto&& item:tokens){
            std::cout<<static_cast<int>(item->GetTokenType())<<":";
            if(item->GetTokenType()!=TokenType::TOK_EOF)
                std::cout<<item->GetTokenString();
            std::cout<<std::endl;
        }
        return true;
    }
}