#include "CodeGen.hpp"

namespace practicellvm{
    //コンストラクタ
    CodeGen::CodeGen(){
        builder=new llvm::IRBuilder<>(m_context);
        m_mod=nullptr;
    }

    //コード生成実行
    //<param> TranslationUnitAST Module名（入力ファイル名）
    //<return> 成功時：true　失敗時:false
    bool CodeGen::DoCodeGen(TranslationUnitAST& tunit,const std::string& name){
        return GenerateTranslationUnit(tunit,name);
    }

    //Module取得
    llvm::Module &CodeGen::GetModule(){
        return m_mod? *m_mod : *(new llvm::Module("null",m_context));
    }

    //Module生成メソッド
    //<param> TranslationUnitAST Module名（入力ファイル名）
    //<return> 成功時：true　失敗時:false
    bool CodeGen::GenerateTranslationUnit(TranslationUnitAST& tunit,const std::string& name){
        
        //Moduleを生成
        m_mod=new llvm::Module(name,m_context);

        //function declaration
        for(int i=0;;i++){
            auto proto=tunit.GetPrototype(i);
            if(!proto)break;
            if(!GeneratePrototype(proto,m_mod)){
                Safe_Delete(m_mod);
                return false;
            }
        }

        //function definition
        for(int i=0;;i++){
            FunctionAST* func=tunit.GetFunction(i);
            if(!func)break;
            if(!GenerateFunctionDefinition(func,m_mod)){
                Safe_Delete(m_mod);
                return false;
            }
        }
        return true;
    }

    //関数宣言生成メソッド
    //<param> PrototypeAST ,Module
    //<return> 生成したllvm::Functionのポインタ
    llvm::Function* CodeGen::GeneratePrototype(PrototypeAST* proto,llvm::Module* mod){
        //既に宣言済みかチェック
        llvm::Function* func=mod->getFunction(proto->GetName());
        if(func){
            if(func->arg_size()==proto->GetParamNum() && func->empty())
                return func;
            fprintf(stderr,"error::function %s is redefined",proto->GetName().c_str());
            return nullptr;
        }

        //引数を生成
        std::vector<llvm::Type*> int_types(
                proto->GetParamNum(),llvm::Type::getInt32Ty(m_context)
            );
        //関数のシグネチャ生成
        llvm::FunctionType* func_type=
            llvm::FunctionType::get(
                llvm::Type::getInt32Ty(m_context),
                int_types,false
            );

        //関数生成
        func=
            llvm::Function::Create(
                func_type,llvm::Function::ExternalLinkage,proto->GetName(),mod
            );
        
        //引数の名前を設定する
        auto itr=func->arg_begin();
        for(int i=0;i<proto->GetParamNum();i++){
            itr->setName(proto->GetParamName(i).append("_arg"));
            ++itr;
        }
        return func;

    }

    //関数定義生成メソッド
    //<param> FunctionAST Module
    //<return> 生成したFunctionのポインタ
    llvm::Function* CodeGen::GenerateFunctionDefinition(FunctionAST* func_ast,llvm::Module* mod){
        llvm::Function* func=GeneratePrototype(func_ast->GetPrototype(),mod);
        if(!func) return nullptr;
        curFunc=func;
        auto block= llvm::BasicBlock::Create(m_context,"entry",func);
        builder->SetInsertPoint(block);
        //Functionのボディを生成
        GenerateFunctionStatement(func_ast->GetBody());
        return func;
    }

    //関数のボディ生成メソッド
    //変数宣言、ステートメントの順に生成
    //<param> FunctionStmtAST
    //<return> 最後に生成したValueのポインタ

    llvm::Value* CodeGen::GenerateFunctionStatement(FunctionStmtAST* func_stmt){
        VariableDeclAST* vdecl;
        llvm::Value* v=nullptr;
        for(int i=0;;i++){
            if(!func_stmt->GetVariableDecl(i))
                break;
            
            vdecl=func_stmt->GetVariableDecl(i);
            v=GenerateVariableDeclaration(vdecl);
        }

        BaseAST* stmt;
        for(int i=0;;i++){
            stmt=func_stmt->GetStatement(i);
            if(!stmt) break;
            if(!llvm::isa<NullExprAST>(stmt))
                v=GenerateStatement(stmt);
        }
        return v;
    }

    //変数宣言（alloca命令）生成メソッド
    //<param> VariableDeclAST
    //<return>生成したValueのポインタ
    llvm::Value* CodeGen::GenerateVariableDeclaration(VariableDeclAST* vdecl){
        //create alloca
        auto alloca=
            builder->CreateAlloca(llvm::Type::getInt32Ty(m_context),0,vdecl->GetName());
        
        //引数の宣言だったときは、引数受け取りのコードを挿入する
        if(vdecl->GetType()==VariableDeclAST::DeclType::param){
            llvm::ValueSymbolTable* arg_value_table=curFunc->getValueSymbolTable();
            builder->CreateStore(arg_value_table->lookup(vdecl->GetName().append("_arg")),alloca);
        }
        return alloca;
    }

    //ステートメント生成メソッド
    //実際にはASTの種類を確認して各種生成メソッドを呼び出し
    //<param> JumpStmtAST
    //<return> 生成したValueのポインタ
    llvm::Value* CodeGen::GenerateStatement(BaseAST* stmt){
        if(llvm::isa<BinaryExprAST>(stmt))
            return GenerateBinaryExpression(llvm::dyn_cast<BinaryExprAST>(stmt));
        if(llvm::isa<CallExprAST>(stmt))
            return GenerateCallExpression(llvm::dyn_cast<CallExprAST>(stmt));
        if(llvm::isa<JumpStmtAST>(stmt))
            return GenerateJumpStatement(llvm::dyn_cast<JumpStmtAST>(stmt));
        return nullptr;
    }

    //二項演算子生成メソッド
    //<param> JumpStmtAST
    //<return> 生成したValueのポインタ
    llvm::Value* CodeGen::GenerateBinaryExpression(BinaryExprAST* bin_expr){
        BaseAST* lhs=bin_expr->GetLHS();
        BaseAST* rhs=bin_expr->GetRHS();
        llvm::Value* lhs_v;
        llvm::Value* rhs_v;

        //assignment
        if(bin_expr->GetOp()=="="){
            //lhsは変数とみなせる
            VariableAST* lhs_var=llvm::dyn_cast<VariableAST>(lhs);
            auto vs_table=curFunc->getValueSymbolTable();
            lhs_v=vs_table->lookup(lhs_var->GetName());
        //other operand
	    }else{
		    //lhs=?
	    	//Binary?
		    if(llvm::isa<BinaryExprAST>(lhs))
		    	lhs_v=GenerateBinaryExpression(llvm::dyn_cast<BinaryExprAST>(lhs));
    		//Variable?
            else if(llvm::isa<VariableAST>(lhs))
		    	lhs_v=GenerateVariable(llvm::dyn_cast<VariableAST>(lhs));
    		//Number?
            else if(llvm::isa<NumberAST>(lhs)){
		    	NumberAST *num=llvm::dyn_cast<NumberAST>(lhs);
		    	lhs_v=GenerateNumber(num->GetNumberValue());
		    }
	    }
	    //create rhs value
	    if(llvm::isa<BinaryExprAST>(rhs))
	    	rhs_v=GenerateBinaryExpression(llvm::dyn_cast<BinaryExprAST>(rhs));
    	//CallExpr?
        else if(llvm::isa<CallExprAST>(rhs))
	    	rhs_v=GenerateCallExpression(llvm::dyn_cast<CallExprAST>(rhs));
	    //Variable?
        else if(llvm::isa<VariableAST>(rhs))
	    	rhs_v=GenerateVariable(llvm::dyn_cast<VariableAST>(rhs));
    	//Number?
        else if(llvm::isa<NumberAST>(rhs)){
	    	NumberAST *num=llvm::dyn_cast<NumberAST>(rhs);
    		rhs_v=GenerateNumber(num->GetNumberValue());
    	}
	    if(bin_expr->GetOp()=="="){
    		//store
	    	return builder->CreateStore(rhs_v, lhs_v);
	    }else if(bin_expr->GetOp()=="+"){
	    	//add
	    	return builder->CreateAdd(lhs_v, rhs_v, "add_tmp");
	    }else if(bin_expr->GetOp()=="-"){
	    	//sub
	    	return builder->CreateSub(lhs_v, rhs_v, "sub_tmp");
    	}else if(bin_expr->GetOp()=="*"){
	    	//mul
	    	return builder->CreateMul(lhs_v, rhs_v, "mul_tmp");
	    }else if(bin_expr->GetOp()=="/"){
		    //div
		    return builder->CreateSDiv(lhs_v, rhs_v, "div_tmp");
        }
        return nullptr;

    }

    /**
  * 関数呼び出し(Call命令)生成メソッド
  * @param CallExprAST
  * @return 生成したValueのポインタ　
  */
    llvm::Value *CodeGen::GenerateCallExpression(CallExprAST *call_expr){
    	std::vector<llvm::Value*> arg_vec;
    	BaseAST *arg;
    	llvm::Value *arg_v;
    	auto vs_table = curFunc->getValueSymbolTable();
    	for(int i=0; ; i++){
    		if(!(arg=call_expr->GetArgs(i)))
	    		break;

	    	//isCall
	    	if(llvm::isa<CallExprAST>(arg))
			    arg_v=GenerateCallExpression(llvm::dyn_cast<CallExprAST>(arg));

		    //isBinaryExpr
		    else if(llvm::isa<BinaryExprAST>(arg)){
		    	BinaryExprAST *bin_expr = llvm::dyn_cast<BinaryExprAST>(arg);

	    		//二項演算命令を生成
		    	arg_v=GenerateBinaryExpression(llvm::dyn_cast<BinaryExprAST>(arg));

		    	//代入の時はLoad命令を追加
		    	if(bin_expr->GetOp()=="="){
		    		VariableAST *var= llvm::dyn_cast<VariableAST>(bin_expr->GetLHS());
		    		arg_v=builder->CreateLoad(vs_table->lookup(var->GetName()), "arg_val");
		    	}
	    	}

	    	//isVar
    		else if(llvm::isa<VariableAST>(arg))
	    		arg_v=GenerateVariable(llvm::dyn_cast<VariableAST>(arg));
		
	    	//isNumber
	    	else if(llvm::isa<NumberAST>(arg)){
	    		NumberAST *num=llvm::dyn_cast<NumberAST>(arg);
	    		arg_v=GenerateNumber(num->GetNumberValue());
	    	}
	    	arg_vec.push_back(arg_v);
	    }
	    return builder->CreateCall( m_mod->getFunction(call_expr->GetCallee()),
	    									arg_vec,"call_tmp" );
    }

    /**
     * ジャンプ(今回はreturn命令のみ)生成メソッド
     * @param  JumpStmtAST
    * @return 生成したValueのポインタ
    */
    llvm::Value* CodeGen::GenerateJumpStatement(JumpStmtAST *jump_stmt){
    	BaseAST *expr=jump_stmt->GetExpr();
	    llvm::Value *ret_v;
	    if(llvm::isa<BinaryExprAST>(expr)){
	    	ret_v=GenerateBinaryExpression(llvm::dyn_cast<BinaryExprAST>(expr));
	    }else if(llvm::isa<VariableAST>(expr)){
	    	VariableAST *var=llvm::dyn_cast<VariableAST>(expr);
	    	ret_v = GenerateVariable(var);
	    }else if(llvm::isa<NumberAST>(expr)){
	    	NumberAST *num=llvm::dyn_cast<NumberAST>(expr);
	    	ret_v=GenerateNumber(num->GetNumberValue());

	    }
	    builder->CreateRet(ret_v);
    }

}