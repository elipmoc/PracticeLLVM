#include <iostream>
#include "CodeGen.hpp"
#include "dcc.hpp"
using practicellvm::Token;
using practicellvm::TokenType;
using practicellvm::TokenStream;
using practicellvm::LexicalAnalysis;
using practicellvm::OptionParser;
int main(int argc,char** argv){
    llvm::InitializeNativeTarget();
    llvm::sys::PrintStackTraceOnErrorSignal(llvm::StringRef());
    llvm::PrettyStackTraceProgram X(argc,argv);
    llvm::EnableDebugBuffering=true;
    OptionParser opt(argc,argv);
    if(!opt.ParseOption())
        exit(1);
    std::cout<<"aaa";
    /*TokenStream* tokenStream;
    tokenStream=LexicalAnalysis("test.txt");
    if(tokenStream==nullptr){
        std::cout<<"NullPtr!!"<<std::endl;
        return 0;
    }
    tokenStream->PrintTokens();*/
    return 0;
}