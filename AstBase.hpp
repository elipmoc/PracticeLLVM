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

//二項演算を表すAST
class BinaryExprAST:public BaseAST{
    std::string op;
    BaseAST *lhs,*rhs;
    public:
    BinaryExprAST(std::string _op,BaseAST * _lhs,BaseAST * _rhs)
        :BaseAST(AstID::BinaryExprID),op(_op),lhs(_lhs),rhs(_rhs){}
    ~BinaryExprAST(){
        Safe_Delete(lhs),Safe_Delete(rhs);
    }

    static bool classof(BinaryExprAST const*){return true;}
    static bool classof(BaseAST const* base){
        return base->GetValueID()==AstID::BinaryExprID;
    }

    //演算子を取得する
    std::string GetOp()const{return op;}

    //左辺値をしゅとくする
    BaseAST* GetLHS()const{return lhs;}
    //右辺値をしゅとくする
    BaseAST* GetRHS()const{return rhs;}

};

}