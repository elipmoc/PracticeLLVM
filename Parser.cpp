#include "Parser.hpp"
#include "Lexer.hpp"

namespace practicellvm{
    //コンストラクタ
    Parser::Parser(const std::string& filename){
        tokens=LexicalAnalysis(filename);
    }

    //構文解析実行
    //<return>解析成功：true 解析失敗:false
    bool Parser::DoParse(){
        if(!tokens){
            fprintf(stderr,"erro at lexer\n");
            return false;
        }
        else 
            return VisitTranslationUnit(); 
    }

    //AST取得
    //<return>TranslationUnitへの参照
    TranslationUnitAST& Parser::GetAST()const{
        if(tu){
            return *tu;
        }
        else return *(new TranslationUnitAST());
    }
}