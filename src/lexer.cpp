#include"lexer.h"
#include<sstream>
#include<print>

//std::ostream &operator<<(std::ostream &os, const Token token) {
//    switch (token.kind) {
//        case TokenKind::Eof: {
//                                 os << "EOF" << std::endl;
//                                 break;
//                             }
//        case TokenKind::Identifier:
//        case TokenKind::Number: {
//                                    os << "identifier: " << token.value << std::endl;
//                                    break;
//                                }
//    }
//    return os;
//}
//

Token Lexer::tokenize_ident(){
    std::stringstream ss;
    while(std::isalnum(src[current_char])){
        ss<<src[current_char];
        current_char++;
    }
    return Token(TokenKind::IDENTIFIER,ss.str(),current_line);
}

Token Lexer::tokenize_numeric(){
    std::stringstream ss;
    TokenKind tkind=TokenKind::INT;
    bool digit_pt=false;
    do{
        if (src[current_char]=='.'){
            if (digit_pt) {
                break;
            }
            else{
                digit_pt=true;
                tkind=TokenKind::NUMBER;
                ss<<src[current_char];
            }
        }
        ss<<src[current_char];
    }while(std::isdigit(src[current_char]) || src[current_char]=='.');
    return Token(tkind,ss.str(),current_line);
}

Token Lexer::get_token(){
    std::print("hello");

    while(isspace(src[current_char])){
        if(src[current_char]=='\n' |src[current_char]=='\r') current_line++; 
        current_char++;
    };

    switch(src[current_char]){
        case '(':{current_char++;return Token(TokenKind::LEFT_PAREN,current_line);}
        case ')':{current_char++;return Token(TokenKind::RIGHT_PAREN,current_line);}
        case '[':{current_char++;return Token(TokenKind::LEFT_BRACE,current_line);}
        case ']':{current_char++;return Token(TokenKind::RIGHT_BRACE,current_line);}
        case ',':{current_char++;return Token(TokenKind::COMMA,current_line);}
        case ';':{current_char++;return Token(TokenKind::SEMICOLON,current_line);}
        case '.':{current_char++;return Token(TokenKind::DOT,current_line);}
        case '+':{current_char++;return Token(TokenKind::PLUS,current_line);}
        case '-':{current_char++;return Token(TokenKind::MINUS,current_line);}
        case '/':{current_char++;return Token(TokenKind::SLASH,current_line);}
        case '*':{current_char++;return Token(TokenKind::STAR,current_line);}
        case '!':{current_char++;return Token(TokenKind::BANG,current_line);}
        default: break;
    };

    if(std::isalpha(src[current_char])){
        return tokenize_ident();
    }
    if(std::isdigit(src[current_char])){
        return tokenize_numeric();
    }
    return Token(TokenKind::Eof,current_line);
}
