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
}