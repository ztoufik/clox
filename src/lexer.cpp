#include<string>
#include <sstream>
#include <unordered_map>

#include"lexer.h"
namespace tua{

    const void Lexer::consume(){
        iter++;
    }

    const bool Lexer::at_end() const noexcept{
        return iter==iter_end;
    }

    const Token Lexer::tokenize_string(){
        std::stringstream ss;
        while(iter!=iter_end && *iter!='"') {
            ss<<*iter;
            iter++;
        };
        iter++;
        return Token(TokenKind::STRING,std::move(ss.str()),current_line);
    }


    const Token Lexer::get_token(){
        while(iter!=iter_end && isspace(*iter) ){
            if(*iter=='\n') current_line++; 
            consume();
        };

        if(iter==iter_end) { return Token(TokenKind::Eof,current_line);}
        switch(*iter){
            case '(':{consume();return Token(TokenKind::LEFT_PAREN,current_line);}
            case ')':{consume();return Token(TokenKind::RIGHT_PAREN,current_line);}
            case '[':{consume();return Token(TokenKind::LEFT_BRACKET,current_line);}
            case ']':{consume();return Token(TokenKind::RIGHT_BRACKET,current_line);}
            case '{':{consume();return Token(TokenKind::LEFT_BRACE,current_line);}
            case '}':{consume();return Token(TokenKind::RIGHT_BRACE,current_line);}
            case '|':{consume();return Token(TokenKind::BIT_OR,current_line);}
            case '&':{consume();return Token(TokenKind::BIT_AND,current_line);}
            case ',':{consume();return Token(TokenKind::COMMA,current_line);}
            case ':':{consume();return Token(TokenKind::COLLON,current_line);}
            case ';':{consume();return Token(TokenKind::SEMICOLON,current_line);}
            case '.':{consume();return Token(TokenKind::DOT,current_line);}
            case '+':{consume();return Token(TokenKind::PLUS,current_line);}
            case '-':{consume();return Token(TokenKind::MINUS,current_line);}
            case '*':{consume();return Token(TokenKind::STAR,current_line);}
            case '"':{consume();return tokenize_string();}
            case '/':{
                         consume();
                         if (at_end() || *iter!='/' ){return Token(TokenKind::SLASH,current_line);}
                         consume();
                         while(*iter!='\n' ) {
                             consume();
                             if(at_end()) {return Token(TokenKind::Eof,current_line);}
                         }
                         consume();
                         return get_token();
                     }
            case '=':{
                         consume();
                         if (at_end() || *iter!='=' ){return Token(TokenKind::EQUAL,current_line);}
                         consume();
                         return Token(TokenKind::EQUAL_EQUAL,current_line);
                     }
            case '<':{
                         consume();
                         if (!at_end() && *iter=='='){consume();return Token(TokenKind::LESS_EQUAL,current_line);}
                         if (!at_end() && *iter=='<'){consume();return Token(TokenKind::BIT_LSHIFT,current_line);}
                         return Token(TokenKind::LESS,current_line);
                     }
            case '>':{
                         consume();
                         if (!at_end() && *iter=='='){consume();return Token(TokenKind::GREATER_EQUAL,current_line);}
                         if (!at_end() && *iter=='>'){consume();return Token(TokenKind::BIT_RSHIFT,current_line);}
                         return Token(TokenKind::GREATER,current_line);
                     }
            case '!':{
                         consume();
                         if (at_end() || *iter!='=' ){return Token(TokenKind::BANG,current_line);}
                         consume();
                         return Token(TokenKind::BANG_EQUAL,current_line);
                     }
            default: break;
        };

        if(iter==iter_end) { return Token(TokenKind::Eof,current_line);}
        if(std::isalpha(*iter)){
            return tokenize_ident();
        }

        if(iter==iter_end) { return Token(TokenKind::Eof,current_line);}
        if(std::isdigit(*iter)){
            return tokenize_numeric();
        }
        return Token(TokenKind::Err,current_line);
    }
    const std::uint16_t Lexer::get_line() const noexcept{
        return current_line;
    }

    const Token Lexer::tokenize_ident(){
        std::stringstream ss;
        while(iter!=iter_end && std::isalnum(*iter)){
            ss<<*iter;
            iter++;
        }

        auto word=std::move(ss.str());
        auto tkind_ptr=Lexer::key_words.find(word);
        if(tkind_ptr==Lexer::key_words.end())
            return Token(TokenKind::IDENT,std::move(word),current_line);
        return Token(tkind_ptr->second,current_line);
    }

    const Token Lexer::tokenize_numeric(){
        std::stringstream ss;
        TokenKind tkind=TokenKind::INT;
        bool digit_pt=false;
        while(iter!=iter_end && (std::isdigit(*iter) || *iter=='.')) {
            if (*iter=='.'){
                if (digit_pt) {
                    break;
                }
                else{
                    digit_pt=true;
                    tkind=TokenKind::DOUBLE;
                    ss<<*iter;
                }
            }
            else{
                ss<<*iter;
            }
            iter++;
        }
        return Token(tkind,std::move(ss.str()),current_line);
    }

};

