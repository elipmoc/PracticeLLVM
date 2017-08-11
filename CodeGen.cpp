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

}