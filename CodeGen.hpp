#include "llvm/Module.h"
#include "llvm/IRBuilder.h"

namespace practicellvm{
//コード生成クラス
    class CodeGen{
        private:
        //現在コード生成中のFunction
        llvm::Function *curFunc;
        //生成したModuleを格納
        llvm::Module *mod;
        //LLVM-IRを生成するIRBuilderクラス
        llvm::IRBuilder<> *builder;
        public:
        CodeGen();
        ~CodeGen();
        bool DoCodeGen(TranslationUnitAST &tunit, const std::string& name,const std::string& link_file, bool with_jit);
        llvm::Module &GetModule();
        private:
		bool GenerateTranslationUnit(TranslationUnitAST &tunit,const std::string& name);
		llvm::Function *GenerateFunctionDefinition(FunctionAST *func, llvm::Module *mod);
		llvm::Function *GeneratePrototype(PrototypeAST *proto, llvm::Module *mod);
		llvm::Value *GenerateFunctionStatement(FunctionStmtAST *func_stmt);
		llvm::Value *GenerateVariableDeclaration(VariableDeclAST *vdecl);
		llvm::Value *GenerateStatement(BaseAST *stmt);
		llvm::Value *GenerateBinaryExpression(BinaryExprAST *bin_expr);
		llvm::Value *GenerateCallExpression(CallExprAST *call_expr);
		llvm::Value *GenerateJumpStatement(JumpStmtAST *jump_stmt);
		llvm::Value *GenerateVariable(VariableAST *var);
		llvm::Value *GenerateNumber(const int value);
        bool LinkModule(llvm::Module *dest,const std::string& file_name);
    };
}