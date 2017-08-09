#include "CodeGen.hpp"

namespace practicellvm{
    //コンストラクタ
    CodeGem:CodeGen(){
        builder=new llvm::IRBuilder<>(llvm::getGlobalContext());
        mod=nullptr;
    }
}