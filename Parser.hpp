#pragma once
#include "APP.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "Lexer.hpp"
#include "AstBase.hpp"
#include "FunctionAST.hpp"

namespace practicellvm{
    //構文解析・意味解析クラス
    class Parser{
        TokenStream* tokens;
        TranslationUnitAST* tu;
        public:
        Parser(const std::string& filename);
        ~Parser(){
            Safe_Delete(tu);
            Safe_Delete(tokens);
        } 
        bool DoParse();
        TranslationUnitAST& GetAST()const;
        private:
        //各種構文解析メソッド
        bool VisitTranslationUnit();
        bool VisitExternalDeclaration(TranslationUnitAST* tunit);
        PrototypeAST* VisitFunctionDeclaration();
        FunctionAST* VisitFunctionDefinition();
        PrototypeAST* VisitPrototype();
        FunctionStmtAST* VisitFunctionStatement(PrototypeAST* proto);
        VariableDeclAST* VisitVariableDeclaration();
        BaseAST* VisitStatement();
        BaseAST* VisitExpressionStatement();
        BaseAST* VisitJumpStatement();
        BaseAST* VisitAssignmentExpression();
        BaseAST* VisitAdditiveExpression(BaseAST* lhs);
        BaseAST* VisitMultiplicativeExpression(BaseAST* lhs);
        BaseAST* VisitPrimaryExpression();
    };


}