#include "CodeGen.hpp"

namespace practicellvm{
    //コンストラクタ
    CodeGen::CodeGen(){
        builder=new llvm::IRBuilder<>(m_context);
        mod=nullptr;
    }
}