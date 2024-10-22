#pragma once
#include <iostream>

namespace practicellvm{

    //関数宣言を表すAST
    class PrototypeAST{
        std::string name;
        std::vector<std::string> params;
        public:
        PrototypeAST(const std::string& _name,const std::vector<std::string>& _params)
            :name(_name),params(_params){}
    
        //関数名を取得する
        std::string GetName()const{return name;}

        //i番目の引数名を取得する
        std::string GetParamName(int i)const{
            if(i<params.size())
                return params[i];
            return nullptr;
        }        

        //引数の数を取得する
        int GetParamNum()const{return params.size();}
        void DebugPrint(size_t nest)const{
            for(size_t i=0;i<nest;i++)
                std::cout<<"    ";
            std::cout<<"PrototypeAST"<<"{"<<name<<"}"<<std::endl;
        }
    };


    //関数定義（ボディ）を表すAST
    class FunctionStmtAST{
        std::vector<VariableDeclAST*> variableDecls;
        std::vector<BaseAST*> stmtLists;

        public:
        FunctionStmtAST(){}
        ~FunctionStmtAST(){
            for(auto&& item:variableDecls)
                Safe_Delete(item);
            for(auto&& item:stmtLists)
                Safe_Delete(item);
            variableDecls.clear();
            stmtLists.clear();
        }

        //関数に変数を追加する
        bool AddVariableDeclaration(VariableDeclAST* vdecl){
            variableDecls.push_back(vdecl);
            return true;
        }

        //関数にステートメントを追加する
        bool AddStatement(BaseAST* stmt){stmtLists.push_back(stmt);}

        //i番目の変数を取得する
        VariableDeclAST* GetVariableDecl(int i)const{
            if(i<variableDecls.size())
                return variableDecls[i];
            else
                return nullptr;
        }

        //i番目のステートメントを取得する
        BaseAST* GetStatement(int i)const{
            if(i<stmtLists.size())
                return stmtLists[i];
            else
                return nullptr;
        }

          void DebugPrint(size_t nest)const{
            for(size_t i=0;i<nest;i++)
                std::cout<<"    ";
            std::cout<<"FunctionStmtAST"<<std::endl;
            for(auto item:variableDecls)
                item->DebugPrint(nest+1);
            for(auto item:stmtLists)
                item->DebugPrint(nest+1);
        }

    };

    //関数定義（ボディ&宣言）を表すAST
    class FunctionAST{
        PrototypeAST* proto;
        FunctionStmtAST* body;
        public:
        FunctionAST(PrototypeAST* _proto,FunctionStmtAST* _body)
            :proto(_proto),body(_body){}
        ~FunctionAST(){
            	Safe_Delete(proto);
                Safe_Delete(body);
        }

        //関数名を取得する
        std::string GetName()const{return proto->GetName();}

        //この関数のプロトタイプ宣言を取得する
        PrototypeAST* GetPrototype()const{return proto;}

        //この関数のボデイを取得する
        FunctionStmtAST* GetBody()const{return body;}

        void DebugPrint(size_t nest)const{
            for(size_t i=0;i<nest;i++)
                std::cout<<"    ";
            std::cout<<"FunctionAST"<<std::endl;
            proto->DebugPrint(nest+1);
            body->DebugPrint(nest+1);
        }

    };



    //ソースコードを表すAST
    class TranslationUnitAST{
        std::vector<PrototypeAST*> prototypes;
        std::vector<FunctionAST*> functions;
        public:

        void DebugPrint()const{
            std::cout<<"TranslationUnitAST"<<std::endl;
            for(auto item:prototypes)
                item->DebugPrint(1);
            for(auto item:functions)
                item->DebugPrint(1);
        }
        TranslationUnitAST(){
            std::cout<<"Create TranslationUnitAST"<<std::endl;
        }
        ~TranslationUnitAST(){
            for(auto&& item:prototypes)
               Safe_Delete(item);
            for(auto&& item:functions)
               Safe_Delete(item);
            prototypes.clear();
            functions.clear();
            
        }

        //モジュールにプロトタイプ宣言を追加する
        bool AddPrototype(PrototypeAST* proto){
            std::cout<<"AddPrototype"<<proto->GetName()<<std::endl;
            prototypes.push_back(proto);
            return true;
        }

        //モジュールに関数を追加する
        bool AddFunction(FunctionAST* func){
            std::cout<<"AddFunction"<<func->GetName()<<std::endl;
            functions.push_back(func);
            return true;
        }

        //モジュールが空かどうか判定する
        bool IsEmpty()const{
            return (prototypes.size()==0 && functions.size()==0);
        }

        //i番目のプロトタイプ宣言をしゅとくする
        PrototypeAST* GetPrototype(int i)const{
            if(i<prototypes.size())
                return prototypes[i];
            else
                return nullptr;
        }

        //i番目の関数を取得する
        FunctionAST* GetFunction(int i)const{
            if(i<functions.size())
                return functions[i];
            else
                return nullptr;
        }
    };

}