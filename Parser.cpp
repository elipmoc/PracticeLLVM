#include "Parser.hpp"
#include "Lexer.hpp"
#include <vector>
#include <algorithm>
#include "llvm/Support/Casting.h"

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
            //ここで再定義されていないか確認
            if(
                prototypeTable.count(proto->GetName())!=0 ||
                (
                    functionTable.count(proto->GetName())!=0 && 
                    functionTable[proto->GetName()]!=proto->GetParamNum()
                )
            ){
                //errorメッセージを出してNULLを返す
                fprintf(stderr,"Function:%s is redefined",proto->GetName().c_str());
                Safe_Delete(proto);
                return nullptr;
            }
            //関数名、引数のペアをプロトタイプ宣言テーブルに追加
            prototypeTable[proto->GetName()]=proto->GetParamNum();
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
        
        //ここでプロトタイプ宣言と違いないか、
        //すでに関数定義が行われていないか確認
        if(
            (
                prototypeTable.count(proto->GetName())!=0 &&
                prototypeTable[proto->GetName()]!=proto->GetParamNum() 
            )   ||
            functionTable.count(proto->GetName())!=0
        ){
            //errorメッセージを出してnullptrを返す
            fprintf(stderr,"Function: %s is redefined",proto->GetName().c_str());
            Safe_Delete(proto);
            return nullptr;
        }
        variableTable.clear();
        FunctionStmtAST* func_stmt=VisitFunctionStatement(proto);

        if(func_stmt){
            //ここで関数名、引数のペアを関数テーブルに追加
            functionTable[proto->GetName()]=proto->GetParamNum();
            return new FunctionAST(proto,func_stmt);
        }

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

                //引数の変数名に重複がないか確認
                if(
                    std::find(param_list.begin(),param_list.end(),tokens->GetCurString())
                    != param_list.end()
                ){
                    tokens->ApplyTokenIndex(bkup);
                    return nullptr;
                }

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
	VariableDeclAST *var_decl;
	BaseAST *stmt;
    BaseAST *last_stmt;
        
     //{statement_list}
	if(stmt=VisitStatement()){
		while(stmt){
			last_stmt=stmt;
			func_stmt->AddStatement(stmt);
			stmt=VisitStatement();
		}

	//variable_declaration_list
    }else if(var_decl=VisitVariableDeclaration()){
        while(var_decl){
            var_decl->SetDeclType(VariableDeclAST::DeclType::local);
            //変数に重複がないか確認
            if(
                std::find(variableTable.begin(),variableTable.end(),var_decl->GetName())
                != variableTable.end()
            ){
                Safe_Delete(var_decl);
                Safe_Delete(func_stmt);
                return nullptr;
            }
            //変数テーブルに新しく読み取った変数名を追加
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

            //最後のStatementがjump_statementであるか確認
            if(!last_stmt || !llvm::isa<JumpStmtAST>(last_stmt)){
                Safe_Delete(func_stmt);
                tokens->ApplyTokenIndex(bkup);
                return nullptr;

            }
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

    //AdditiveExpression用構文解析メソッド
    //<param>lhs(左辺) 初回呼び出し時はnullptr
    //<return>解析成功:BaseAST　解析失敗:nullptr
    BaseAST * Parser::VisitAdditiveExpression(BaseAST* lhs){
        int bkup=tokens->GetCurIndex();

        if(!lhs){
            lhs=VisitMultiplicativeExpression(nullptr);
            if(!lhs)
               return nullptr;
        }

        BaseAST* rhs;

        //+と-の解析
        if(tokens->GetCurType()==TokenType::TOK_SYMBOL &&
             (tokens->GetCurString()=="+" || tokens->GetCurString()=="-")  )
        {
            std::string symbol=tokens->GetCurString();
            tokens->GetNextToken();
            rhs=VisitMultiplicativeExpression(nullptr);
            if(rhs){
                return 
                    VisitAdditiveExpression(new BinaryExprAST(symbol,lhs,rhs));
            }
            Safe_Delete(lhs);
            tokens->ApplyTokenIndex(bkup);
            return nullptr;
        }
        return lhs;
    }

    //MultiplicativeExpression用構文解析メソッド
    //<param>lhs(左辺) 初回呼び出し時はnullptr
    //<return>解析成功:BaseAST　解析失敗:nullptr
    BaseAST * Parser::VisitMultiplicativeExpression(BaseAST* lhs){
        int bkup=tokens->GetCurIndex();

         if(!lhs){
            lhs=VisitPostfixExpression();
            if(!lhs)
               return nullptr;
        }
        BaseAST* rhs;

        //*と/の解析
        if(tokens->GetCurType()==TokenType::TOK_SYMBOL &&
             (tokens->GetCurString()=="*" || tokens->GetCurString()=="/")  )
        {
            std::string symbol=tokens->GetCurString();
            tokens->GetNextToken();
            rhs=VisitPostfixExpression();
            if(rhs){
                return 
                    VisitMultiplicativeExpression(new BinaryExprAST(symbol,lhs,rhs));
            }
            Safe_Delete(lhs);
            tokens->ApplyTokenIndex(bkup);
            return nullptr;
        }
        return lhs;
    }

    //ExpressionStatement用構文解析メソッド
    //<return>解析成功:BaseAST　解析失敗:nullptr
    BaseAST* Parser::VisitExpressionStatement(){
        BaseAST* assign_expr;

        //空文
        if(tokens->GetCurString()==";"){
            tokens->GetNextToken();
            return new NullExprAST();
        }
        if(assign_expr=VisitAssignmentExpression()){
            if(tokens->GetCurString()==";"){
                tokens->GetNextToken();
                return assign_expr;
            }
        }
        return nullptr;
    }

    BaseAST* Parser::VisitStatement(){
        BaseAST* stmt=nullptr;
        if(stmt=VisitExpressionStatement())
            return stmt;
        else if(stmt=VisitJumpStatement())
            return stmt;
        return nullptr;
    }

  // VariableDeclaration用構文解析メソッド
  // @return 解析成功：VariableDeclAST　解析失敗：nullptr
    VariableDeclAST *Parser::VisitVariableDeclaration(){
	    std::string name;

	    //INT
    	if(tokens->GetCurType()==TokenType::TOK_INT){
	    	tokens->GetNextToken();	
    	}else{
    		return nullptr;
    	}
    
	    //IDENTIFIER
    	if(tokens->GetCurType()==TokenType::TOK_IDENTIFIER){
    		name=tokens->GetCurString();    
    		tokens->GetNextToken();
    	}else{
	    	tokens->UnGetToken(1);
		    return nullptr;
    	}
	
	    //';'
    	if(tokens->GetCurString()==";"){
	    	tokens->GetNextToken();
    		return new VariableDeclAST(name);
    	}else{
	    	tokens->UnGetToken(2);	
	    	return nullptr;
	    }
    }

    /**
  * JumpStatement用構文解析メソッド
  * @return 解析成功：AST　解析失敗：NULL
  */
    BaseAST *Parser::VisitJumpStatement(){
	    int bkup=tokens->GetCurIndex();
	    BaseAST *expr;

	    if(tokens->GetCurType() == TokenType::TOK_RETURN){
	    	tokens->GetNextToken();	
	    	if(!(expr=VisitAssignmentExpression()) ){
	    		tokens->ApplyTokenIndex(bkup);
	    		return nullptr;
    		}
	    	if(tokens->GetCurString()==";"){
	    		tokens->GetNextToken();
	    		return new JumpStmtAST(expr);
            }
	    	tokens->ApplyTokenIndex(bkup);
	    	return nullptr;
	    }
	    return nullptr;
    }
}