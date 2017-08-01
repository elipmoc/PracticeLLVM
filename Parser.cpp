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

    //FunctionStatement用構文解析メソッド
    //<param> 関数名や引数を格納したPrototypeクラスのインスタンス
    //<return>解析成功:FunctionStmtAST　解析失敗:nullptr
    FunctionStmtAST* Parser::VisitFunctionStatement(PrototypeAST* proto){
        int bkup=tokens->GetCurIndex();
        if(tokens->GetCurString()=="{"){
            tokens->GetNextToken();
        }
        else 
            return nullptr;
        FunctionStmtAST* func_stmt=new FunctionStmtAST();

        //引数をfunc_stmtの変数宣言リストに追加
        for(int i=0;i<proto->GetParamNum();i++){
            VariableDeclAST* vdecl=new VariableDeclAST(proto->GetParamName(i));
            vdecl->SetDeclType(VariableDeclAST::DeclType::param);
            func_stmt->AddVariableDeclaration(vdecl);
            variableTable.push_back(vdecl->GetName());
        }
        //省略
        //仮においてるｉｆ文
        if(false){

        }else if(var_decl=VisitVariableDeclaration()){
            while(var_decl){
                var_decl->SetDeclType(VariableDeclAST::DeclType::local);
                //本来ならここで変数が二重宣言されていないことを確認

                func_stmt->AddVariableDeclaration(var_decl);
                variableTable.push_back(var_decl->GetName());
                var_decl=VisitVariableDeclaration();
            }

            if(stmt=VisitStatement()){
                while(stmt){
                    last_stmt=stmt;
                    func_stmt->AddStatement(stmt);
                    stmt=VisitStatement();
                }
            }
            else{
                Safe_Delete(func_stmt);
                tokens->ApplyTokenIndex(bkup);
                return nullptr;
            }

            //本来ならここで戻り値の確認

            if(tokens->GetCurString()=="}"){
                tokens->GetNextToken();
                return func_stmt;
            }
            else{
                Safe_Delete(func_stmt);
                tokens->ApplyTokenIndex(bkup);
                return nullptr;
            }

        }
    }

    //AssignmentExpression用構文解析メソッド
    //<return>解析成功:BaseAST　解析失敗:nullptr
    BaseAST* Parser::VisitAssignmentExpression(){
        int bkup=tokens->GetCurIndex();
        BaseAST* lhs;
        if(tokens->GetCurType()==TokenType::TOK_IDENTIFIER){
            //本来はここで変数の宣言確認
            lhs=new VariableAST(tokens->GetCurString());
            tokens->GetNextToken();
            BaseAST* rhs;
            if(tokens->GetCurType()==TokenType::TOK_SYMBOL && tokens->GetCurString()=="="){
                tokens->GetNextToken();
                if(rhs=VisitAdditiveExpression(nullptr))
                    return new BinaryExprAST("=",lhs,rhs);
                else{
                    Safe_Delete(lhs);
                    tokens->ApplyTokenIndex(bkup);
                }
            }
            else{
                Safe_Delete(lhs);
                tokens->ApplyTokenIndex(bkup);
            }
        }
        BaseAST* add_expr=VisitAdditiveExpression(nullptr);
        if(add_expr){
            return add_expr;
        }
        return nullptr;
    }

    //PrimaryExpression用構文解析メソッド
    //<return> 解析成功:BaseAST　解析失敗:nullptr

    BaseAST* Parser::VisitPrimaryExpression(){

        
        int bkup=tokens->GetCurIndex();
        //変数
        if(tokens->GetCurType()==TokenType::TOK_IDENTIFIER){
            //本来なここで変数宣言確認
            std::string var_name=tokens->GetCurString();
            tokens->GetNextToken();
            return new VariableAST(var_name);            
        }
        //整数リテラル
        if(tokens->GetCurType()==TokenType::TOK_DIGIT){
            int val=tokens->GetCurNumVal();
            tokens->GetNextToken();
            return new NumberAST(val);
        }
        if(tokens->GetCurType()==TokenType::TOK_SYMBOL && 
            tokens->GetCurString()=="-"){
                //省略


                return nullptr;

        }
    }

    //PostfixExpression用構文解析メソッド
    //<return>解析成功:BaseAST　解析失敗:nullptr
    BaseAST* Parser::VisitPostfixExpression(){
        int bkup=tokens->GetCurIndex();
        //省略　
        //関数呼び出し
        if(tokens->GetCurType()==TokenType::TOK_IDENTIFIER){
            //本来ならここで関数の宣言確認

            //関数名取得
            std::string callee=tokens->GetCurString();
            tokens->GetNextToken();

            //関数の（の部分
            if(tokens->GetCurType()!=TokenType::TOK_SYMBOL ||
                tokens->GetCurString()!="("){
                    tokens->ApplyTokenIndex(bkup);
                    return nullptr;
            }

            //引数の解析
            tokens->GetNextToken();
            std::vector<BaseAST*> args;
            BaseAST * assign_expr=VisitAssignmentExpression();
            if(assign_expr){
                args.push_back(assign_expr);
                //","が続く限り繰り返す
                while(tokens->GetCurType()==TokenType::TOK_SYMBOL &&
                    tokens->GetCurString()==",")
                {
                    tokens->GetNextToken();
                    assign_expr=VisitAssignmentExpression();
                    if(assign_expr)
                        args.push_back(assign_expr);
                    else
                        break;

                }
            }

            //本来ならここで引数の数を確認

            //関数の)の部分
            if(tokens->GetCurType()==TokenType::TOK_SYMBOL &&
                tokens->GetCurString()==")")
            {
                tokens->GetNextToken();
                return new CallExprAST(callee,args);
            }
            else{
                for(auto&& item:args)
                    Safe_Delete(item);
                tokens->ApplyTokenIndex(bkup);
                return nullptr;
            }
        }
        else {
            return nullptr;
        }
    }

}