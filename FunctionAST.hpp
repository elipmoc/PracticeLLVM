#pragma once

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
    };



    //ソースコードを表すAST
    class TranslationUnitAST{
        std::vector<PrototypeAST*> prototypes;
        std::vector<FunctionAST*> functions;
        public:
        TranslationUnitAST(){}
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
            prototypes.push_back(proto);
            return true;
        }

        //モジュールに関数を追加する
        bool AddFunction(FunctionAST* func){
            functions.push_back(func);
            return true;
        }

        //モジュールが空かどうか判定する
        bool IsEmpty()const;

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