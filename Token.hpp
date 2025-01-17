#pragma once
#include <string>
#include <string.h>

namespace practicellvm
{

//token種別
enum struct TokenType
{
  TOK_IDENTIFIER, //識別子
  TOK_DIGIT,      //数字
  TOK_SYMBOL,     //記号
  TOK_INT,        //int
  TOK_RETURN,     //return
  TOK_EOF,        //eof
};

//個別トークン格納クラス
class Token
{
private:
  const TokenType type;
  const std::string tokenString;
  const int number; //typeがTOK_DIGITの時に数値が有効になる
  const int line;   //行数

public:
  Token(const std::string &string, TokenType _type, int _line)
      : tokenString(string),
        type(_type),
        line(_line),
        number(type == TokenType::TOK_DIGIT ? atoi(string.c_str()) : 0x7fffffff)
  {
  }
  ~Token() = default;
  //トークンの種別を取得
  TokenType GetTokenType() const { return type; }
  //トークンの文字列を表現
  std::string GetTokenString() const { return tokenString; }
  //トークンの数値を取得（種別が数字である場合に使用）
  int GetNumberValue() const { return number; }
  //トークンの出現した行数を取得
  int GetLine() const { return line; }
};
}