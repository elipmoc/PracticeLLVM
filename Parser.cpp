#include "Parser.hpp"
#include "Lexer.hpp"
#include <vector>

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

    //TranslationUnit用構文解析メソッド
    //<return>解析成功 :true 解析失敗:false
    bool Parser::VisitTranslationUnit(){
        tu=new TranslationUnitAST();

        //ExternalDecl
        while(true){
            if(VisitExternalDeclaration(tu)==false){
                Safe_Delete(tu);
                return false;
            }
            if(tokens->GetCurType()==TokenType::TOK_EOF)
                break;
        }
        return true;
    }

    //ExternalDeclaration用構文解析クラス
    //解析したPrototypeASTとFunctionASTをTranslationUnitに追加
    bool Parser::VisitExternalDeclaration(TranslationUnitAST* tunit){
        //FunctionDeclaration
        PrototypeAST* proto =VisitFunctionDeclaration();
        if(proto){
            tunit->AddPrototype(proto);
            return true;
        }

        //FunctionDefinition
        FunctionAST* funcDef=VisitFunctionDefinition();
        if(funcDef){
            tunit->AddFunction(funcDef);
            return true;
        }
        return false;
    }
    
    //FunctionDeclaration用構文解析メソッド
    //<return>解析成功;PrototypeAST　解析失敗:nullptr
    PrototypeAST* Parser::VisitFunctionDeclaration(){
        int bkup=tokens->GetCurIndex();
        PrototypeAST* proto=VisitPrototype();
        if(proto==nullptr)
            return nullptr;
        //文の終わりが;で終わってるか確認
        if(tokens->GetCurString()==";"){
            //本来であれば、ここで再定義されていないか確認
            tokens->GetNextToken();
            return proto;
        }
        else{
            Safe_Delete(proto);
            tokens->ApplyTokenIndex(bkup);
            return nullptr;
        }
    }

    //FunctionDefinition用構文解析メソッド
    //<return> 解析成功:FunctionAST　解析失敗:nullptr
    FunctionAST* Parser::VisitFunctionDefinition(){
        PrototypeAST* proto=VisitPrototype();
        if(proto==nullptr)
            return nullptr;
        
        //本来であれば、ここで再定義されていないか確認

        FunctionStmtAST* func_stmt=VisitFunctionStatement(proto);

        //あとで追加。。。。。
    }

    //Prototype用構文解析メソッド
    //<return>解析成功:PrototypeAST　解析失敗:nullptr
    PrototypeAST* Parser::VisitPrototype(){
        int bkup=tokens->GetCurIndex();
        //あとでついか
        //parameter_list
        bool is_first_param=true;
        std::vector<std::string> param_list;
        while(true){
            //区切り文字「,」をスキップ
            if(!is_first_param && tokens->GetCurType()==TokenType::TOK_SYMBOL && 
                tokens->GetCurString()==","){
                    tokens->GetNextToken();
                }
            if(tokens->GetCurType()==TokenType::TOK_INT)
                tokens->GetNextToken();
            else 
                break;
            if(tokens->GetCurType()==TokenType::TOK_IDENTIFIER){
                //本来であれば、ここで再定義されていないか確認

                param_list.push_back(tokens->GetCurString());
                tokens->GetNextToken();
            }
            else{
                tokens->ApplyTokenIndex(bkup);
                return nullptr;
            }
        }
        //省略
    }



}