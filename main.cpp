#include <iostream>
#include "CodeGen.hpp"
#include "dcc.hpp"
using practicellvm::Token;
using practicellvm::TokenType;
using practicellvm::TokenStream;
using practicellvm::LexicalAnalysis;
using practicellvm::OptionParser;
using practicellvm::Parser;
using practicellvm::TranslationUnitAST;
using practicellvm::CodeGen;


int main(int argc,char** argv){
    llvm::InitializeNativeTarget();
    llvm::sys::PrintStackTraceOnErrorSignal(llvm::StringRef());
    llvm::PrettyStackTraceProgram X(argc,argv);
    llvm::EnableDebugBuffering=true;
	std::cout<<"targetセット完了"<<std::endl;
    OptionParser opt(argc,argv);
    if(!opt.ParseOption()){
        fprintf(stderr,"オプションがおかしい");
        exit(1);
        }

    //check
	if(opt.GetInputFileName().length()==0){
		fprintf(stderr,"入力ファイル名が指定されていません\n");
		exit(1);
	}

	//lex and parse
	Parser *parser=new Parser(opt.GetInputFileName());
	if(!parser->DoParse()){
		fprintf(stderr, "err at parser or lexer\n");
		Safe_Delete(parser);
		exit(1);
	}

	//get AST
	TranslationUnitAST &tunit=parser->GetAST();
	if(tunit.IsEmpty()){
		fprintf(stderr,"TranslationUnit is empty");
		Safe_Delete(parser);
		exit(1);
	}

	CodeGen *codegen=new CodeGen();
	if(!codegen->DoCodeGen(tunit, opt.GetInputFileName())){
		fprintf(stderr, "err at codegen\n");
		Safe_Delete(parser);
		Safe_Delete(codegen);
		exit(1);
	}

	//get Module
	llvm::Module& mod=codegen->GetModule();
	if(mod.empty()){
		fprintf(stderr,"Module is empty");
		Safe_Delete(parser);
		Safe_Delete(codegen);
		exit(1);
    }

    llvm::legacy::PassManager pm;

	//出力
	std::error_code error;
	llvm::raw_fd_ostream raw_stream(opt.GetOutputFileName().c_str(), error, llvm::sys::fs::OpenFlags::F_RW);
	pm.add(llvm::createPrintModulePass(raw_stream));
	pm.run(mod);
	raw_stream.close();

	//delete
	Safe_Delete(parser);
    Safe_Delete(codegen);

  /*  TokenStream* tokenStream;
    tokenStream=LexicalAnalysis("sample.dc");
    if(tokenStream==nullptr){
        std::cout<<"NullPtr!!"<<std::endl;
        return 0;
    }
    tokenStream->PrintTokens();*/
    return 0;
}