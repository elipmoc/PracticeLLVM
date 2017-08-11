#include "CodeGen.hpp"

namespace practicellvm{
    //コンストラクタ
    CodeGen::CodeGen(){
        builder=new llvm::IRBuilder<>(m_context);
        mod=nullptr;
    }

    //コード生成実行
    //<param> TranslationUnitAST Module名（入力ファイル名）
    //<return> 成功時：true　失敗時:false
    bool CodeGen::DoCodeGen(TranslationUnitAST& tunit,const std::string& name){
        return GenerateTranslationUnit(tunit,name);
    }

    //Module取得
    llvm::Module &CodeGen::GetModule(){
        return mod? *mod : *(new llvm::Module("null",m_context));
    }

    //Module生成メソッド
    //<param> TranslationUnitAST Module名（入力ファイル名）
    //<return> 成功時：true　失敗時:false
    bool CodeGen::GenerateTranslationUnit(TranslationUnitAST& tunit,const std::string& name){
        
        //Moduleを生成
        mod=new llvm::Module(name,m_context);

        //function declaration
        for(int i=0;;i++){
            auto proto=tunit.GetPrototype(i);
            if(!proto)break;
            if(!GeneratePrototype(proto,mod)){
                Safe_Delete(mod);
                return false;
            }
        }

        //function definition
        for(int i=0;;i++){
            FunctionAST* func=tunit.GetFunction(i);
            if(!func)break;
            if(!GenerateFunctionDefinition(func,mod)){
                Safe_Delete(mod);
                return false;
            }
        }
        return true;
    }
}