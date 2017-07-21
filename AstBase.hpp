//ASTの種類
enum class AstID{
    BaseId
};
//ASTの基底クラス
class BaseAST{
    AstID ID;
    public:
    BaseAST(AstID id):ID(id){}
    virtual ~BaseAST(){}
    AstID getValueID()const{return ID;}
};

