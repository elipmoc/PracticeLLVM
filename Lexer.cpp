#include "TokenStream.hpp"
#include "APP.hpp"
#include "Lexer.hpp"
#include <string>
#include <fstream>
#include <ctype.h>

namespace practicellvm{

//トークン切り出し関数
//<param>字句解析対象のファイル名
//<return>切り出したトークンを格納したTokenStream
TokenStream* LexicalAnalysis(std::string input_filename){
    TokenStream* tokens=new TokenStream();
    std::ifstream ifs;
    std::string cur_line;
    std::string token_str;
    int line_num=0;
    bool isComment=false;
    ifs.open(input_filename,std::ios::in);
    if(!ifs)
        return nullptr;
    while(ifs&&getline(ifs,cur_line)){
        char next_char;
        std::string line;
        Token* next_token;
        int index=0;
        int length=cur_line.length();

        while(index<length){
            next_char=cur_line.at(index++);

            //コメント読み飛ばし
            if(isComment){
                if(
                    length>index &&
                    next_char=='*' &&
                    cur_line.at(index)=='/'
                )
                {
                    index++;
                    isComment=false;
                    continue;
                }
                continue;
            }

            //EOF
            if(next_char==EOF){
                token_str=EOF;
                next_token=new Token(token_str,TokenType::TOK_EOF,line_num);
            }
            else if(isspace(next_char))
                continue;
            //IDENTIFIER
            else if(isalpha(next_char)){
                token_str+=next_char;
                next_char=cur_line.at(index++);
                while(isalnum(next_char)){
                    token_str+=next_char;
                    next_char=cur_line.at(index++);
                    if(index==length)
                        break;
                }
                index--;

                if(token_str=="int")
                    next_token=new Token(token_str,TokenType::TOK_INT,line_num);
                else if(token_str=="return")
                    next_token=new Token(token_str,TokenType::TOK_RETURN,line_num);
                else
                    next_token=new Token(token_str,TokenType::TOK_IDENTIFIER,line_num);
            }
            //数字
            else if(isdigit(next_char)){
                if(next_char=='0'){
                    token_str+=next_char;
                    next_token=new Token(token_str,TokenType::TOK_DIGIT,line_num);
                }
                else{
                    token_str+=next_char;
                    next_char=cur_line.at(index++);
                    while(isdigit(next_char)){
                        token_str+=next_char;
                        next_char=cur_line.at(index++);
                    }
                    next_token=new Token(token_str,TokenType::TOK_DIGIT,line_num);
                    index--;
                }
            }
            //コメントまたは除算演算子
            else if(next_char=='/'){
                next_char=cur_line.at(index++);

                //コメントの場合
                if(next_char=='/')
                    break;
                //コメントの場合
                else if(next_char=='*'){
                    isComment=true;
                    continue;
                }
                //除算演算子
                else{
                    token_str+='/';
                    index--;
                    next_token=new Token(token_str,TokenType::TOK_SYMBOL,line_num);
                }
            }
            //それ以外（記号）
            else{
                if(
                    next_char=='*'||
                    next_char=='+'||
                    next_char=='-'||
                    next_char=='='||
                    next_char==';'||
                    next_char==','||
                    next_char=='('||
                    next_char==')'||
                    next_char=='{'||
                    next_char=='}'
                ){
                    token_str+=next_char;
                    next_token=new Token(token_str,TokenType::TOK_SYMBOL,line_num);
                }
                //解析不可能字句
                else{
                    fprintf(stderr,"unclear token:%c",next_char);
                    Safe_Delete(tokens);
                    return nullptr;
                }
            }
            //Tokenstに追加
            tokens->PushToken(next_token);
            token_str.clear();
        }
        token_str.clear();
        line_num++;
    }
    //EOFの確認
    if(ifs.eof()){
        tokens->PushToken(new Token(token_str,TokenType::TOK_EOF,line_num));
    }
    //クローズ
    ifs.close();
    return tokens;
}
}