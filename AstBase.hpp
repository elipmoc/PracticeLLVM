#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace practicellvm{

//ASTの種類
enum class AstID{
    BaseId,
    VariableDeclID,
    BinaryExprID,
    NullExprID,
    CallExprID,
    JumpStmtID,
    VariableID,
    NumberID
};

//ASTの基底クラス
class BaseAST{ 
    AstID ID;
    protected:
    virtual void DebugPrint_(size_t nest)const{
        std::cout<<"unknown"<<std::endl;
    }
    public:
    BaseAST(AstID id):ID(id){}
    virtual ~BaseAST(){}
    AstID GetValueID()const{return ID;}
    void DebugPrint(size_t nest)const{
        for(size_t i=0;i<nest;i++)
            std::cout<<"    ";
        DebugPrint_(nest);
    }
};

//空文を表すAST
class NullExprAST:public BaseAST{
        public:
    NullExprAST():BaseAST(AstID::NullExprID){}

    static bool classof(NullExprAST const*){return true;}
    static bool classof(BaseAST const* base){
        return base->GetValueID()==AstID::NullExprID;
    }
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
    protected:
    virtual void DebugPrint_(size_t nest)const override{
        std::cout<<"VariableDeclAST"<<"{"<<name<<"}"<<std::endl;  
    }
    public:
    VariableDeclAST(const std::string& _name):BaseAST(AstID::VariableID),name(_name){
    }

    static bool classof(VariableDeclAST const*){return true;}
    static bool classof(BaseAST const* base){
        return base->GetValueID()==AstID::VariableDeclID;
    }
    ~VariableDeclAST(){}

    //変数名を取得する
    std::string GetName()const{return name;}

    //変数の宣言種別を設定する
    bool SetDeclType(DeclType _type){type=_type;return true;}

    //変数の宣言種別を取得する
    DeclType GetType()const{return type;}

};

//二項演算を表すAST
class BinaryExprAST:public BaseAST{
    std::string op;
    BaseAST *lhs,*rhs;
    protected:
    virtual void DebugPrint_(size_t nest)const override{
        std::cout<<"BinaryExprAST"<<"{"<<op<<"}"<<std::endl;
        lhs->DebugPrint(nest+1);        
        rhs->DebugPrint(nest+1);
    }
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

//関数呼び出しを表すAST
class CallExprAST:public BaseAST{
    std::string callee;
    std::vector<BaseAST*> args;
    protected:
    virtual void DebugPrint_(size_t nest)const override{
        std::cout<<"CallExprAST"<<"{"<<callee<<"}"<<std::endl;
    }
    public:
    CallExprAST(const std::string& _callee,const std::vector<BaseAST*>& _args)
        :BaseAST(AstID::CallExprID),
        callee(_callee),
        args(_args){}
    ~CallExprAST(){
        for(auto&& item:args)
            Safe_Delete(item);
    };
    static bool classof(CallExprAST const*){return true;}
    static bool classof(BaseAST const* base){
        return base->GetValueID()==AstID::CallExprID;
    }

    //呼び出す関数名を取得する
    std::string GetCallee()const{return callee;}

    //i番目の引数を取得する
    BaseAST* GetArgs(int i)const{
        if(i<args.size())
            return args[i];
        else
            return nullptr;
    }
};

//ジャンプ（ここではreturn)を表すAST
class JumpStmtAST:public BaseAST{
    BaseAST* expr;
    protected:
    virtual void DebugPrint_(size_t nest)const override{
        std::cout<<"JumpStmtAST"<<std::endl;
        expr->DebugPrint(nest+1);
    }
    public:
    JumpStmtAST(BaseAST* _expr):BaseAST(AstID::JumpStmtID),expr(_expr){}
    ~JumpStmtAST(){Safe_Delete(expr);}

    static bool classof(JumpStmtAST const*){return true;}
    static bool classof(BaseAST const* base){
        return base->GetValueID()==AstID::JumpStmtID;
    }

    //returnで返すExpressionを取得する
    BaseAST* GetExpr()const{return expr;}
};

//変数参照を表すAST
class VariableAST:public BaseAST{
    std::string name;
    protected:
    virtual void DebugPrint_(size_t nest)const override{
        std::cout<<"VariableAST"<<"{"<<name<<"}"<<std::endl;
    }
    public:
    VariableAST(const std::string &_name):BaseAST(AstID::VariableID),name(_name){}
    ~VariableAST(){}

    static bool classof(VariableAST const*){return true;}
    static bool classof(BaseAST const* base){
        return base->GetValueID()==AstID::VariableID;
    }

    //変数名を取得する
    std::string GetName()const{return name;}
};

//整数を表すAST
class NumberAST:public BaseAST{
    int val;
    protected:
    virtual void DebugPrint_(size_t nest)const override{
        std::cout<<"NumberAST"<<"{"<<val<<"}"<<std::endl;
    }
    public:
    NumberAST(int _val):BaseAST(AstID::NumberID),val(_val){}
    ~NumberAST(){}
    static bool classof(NumberAST const*){return true;}
    static bool classof(BaseAST const* base){
        return base->GetValueID()==AstID::NumberID;
    }

    //このASTが表現する数値を取得する
    int GetNumberValue()const{return val;}   
};
}