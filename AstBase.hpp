#pragma once
#include <string>

namespace practicellvm{

//ASTの種類
enum class AstID{
    BaseId,
    VariableDeclID,
    BinaryExprID,
    CallExprID,
    JumpStmtID,
    VariableID,
    NumberID
};

//ASTの基底クラス
class BaseAST{
    AstID ID;
    public:
    BaseAST(AstID id):ID(id){}
    virtual ~BaseAST(){}
    AstID GetValueID()const{return ID;}
};

//変数宣言を表すAST
class VariableDeclAST:public BaseAST{
    public:
    //変数の宣言型識別
    enum class DeclType{
        param,//引数
        local//ローカル変数
    };
    private:
    DeclType type;
    std::string name;
    public:
    VariableDeclAST(const std::string& _name):BaseAST(AstID::VariableID),name(_name){

    }

    static bool classof(VariableDeclAST const*){return true;}
    static bool classof(BaseAST const* base){
        return base->GetValueID()==AstID::VariableDeclID;
    }
    ~VariableDeclAST(){}

    //変数名を取得する
    std::string GetName(){return name;}

    //変数の宣言種別を設定する
    bool SetDeclType(DeclType _type){type=_type;return true;}

    //変数の宣言種別を取得する
    DeclType GetType(){return type;}

};

}