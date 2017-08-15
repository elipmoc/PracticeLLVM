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

    //check
	if(opt.getInputFileName().length()==0){
		fprintf(stderr,"入力ファイル名が指定されていません\n");
		exit(1);
	}

	//lex and parse
	Parser *parser=new Parser(opt.GetInputFileName());
	if(!parser->DoParse()){
		fprintf(stderr, "err at parser or lexer\n");
		SAFE_DELETE(parser);
		exit(1);
	}

	//get AST
	TranslationUnitAST &tunit=parser->GetAST();
	if(tunit.empty()){
		fprintf(stderr,"TranslationUnit is empty");
		SAFE_DELETE(parser);
		exit(1);
	}

	CodeGen *codegen=new CodeGen();
	if(!codegen->DoCodeGen(tunit, opt.GetInputFileName())){
		fprintf(stderr, "err at codegen\n");
		SAFE_DELETE(parser);
		SAFE_DELETE(codegen);
		exit(1);
	}

	//get Module
	llvm::Module& mod=codegen->GetModule();
	if(mod.empty()){
		fprintf(stderr,"Module is empty");
		SAFE_DELETE(parser);
		SAFE_DELETE(codegen);
		exit(1);
    }

  /*  TokenStream* tokenStream;
    tokenStream=LexicalAnalysis("sample.dc");
    if(tokenStream==nullptr){
        std::cout<<"NullPtr!!"<<std::endl;
        return 0;
    }
    tokenStream->PrintTokens();*/
    return 0;
}