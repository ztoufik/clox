#ifndef LEXER_H
#define  LEXER_H

#include <string>
#include <expected>
#include <sstream>
#include <unordered_map>

struct TokenError{
    std::string msg;
};

enum class TokenKind {
    Eof ,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    BANG,
    EQUAL,
    GREATER,
    LESS,
    BANG_EQUAL,
    EQUAL_EQUAL,
    GREATER_EQUAL,
    LESS_EQUAL,
    IDENTIFIER,
    STRING,
    INT,
    NUMBER,
    AND ,
    CLASS ,
    ELSE ,
    FALSE ,
    FUN ,
    FOR ,
    IF ,
    NIL ,
    OR ,
    RETURN ,
    SUPER ,
    THIS ,
    TRUE ,
    VAR ,
    WHILE ,
};

struct Token {
    TokenKind kind;
    std::string value;
    std::uint16_t line;

    Token(TokenKind kind, std::string&& value,std::uint16_t line) : kind(kind), value(value),line(line) {}
    Token(TokenKind kind,std::uint16_t line) : kind(kind),value(""),line(line){}

    //friend std::ostream &operator<<(std::ostream &os, const Token token);
};

//std::ostream &operator<<(std::ostream &os, const Token token);

template<typename T> class Lexer {
    public:
        Lexer(T&& source):src(source),iter(src.begin()),end(src.end()),current_line(0){}
        Token tokenize_ident();
        Token tokenize_numeric();
        Token tokenize_string();
        std::expected<Token,TokenError> get_token();

    private:
        T src;
        T::iterator iter,end;
        std::uint16_t current_line;

        static const std::unordered_map<std::string,TokenKind> key_words;
};

template<typename T>
const std::unordered_map<std::string,TokenKind> Lexer<T>::key_words={
    {std::string("class"),TokenKind::CLASS},
    {std::string("and") ,TokenKind::AND},
    {std::string("else") ,TokenKind::ELSE},
    {std::string("false") ,TokenKind::FALSE},
    {std::string("fun") ,TokenKind::FUN},
    {std::string("for") ,TokenKind::FOR},
    {std::string("if") ,TokenKind::IF},
    {std::string("nil") ,TokenKind::NIL},
    {std::string("or") ,TokenKind::OR},
    {std::string("return") ,TokenKind::RETURN},
    {std::string("super") ,TokenKind::SUPER},
    {std::string("this") ,TokenKind::THIS},
    {std::string("true") ,TokenKind::TRUE},
    {std::string("var") ,TokenKind::VAR},
    {std::string("while") ,TokenKind::WHILE},
};

template<typename T> Token Lexer<T>::tokenize_ident(){
    std::stringstream ss;
    while(iter!=end && std::isalnum(*iter)){
        ss<<*iter;
        iter++;
    }

    auto word=std::move(ss.str());
    auto tkind_ptr=Lexer::key_words.find(word);
    if(tkind_ptr==Lexer::key_words.end())
        return Token(TokenKind::IDENTIFIER,std::move(word),current_line);
    return Token(tkind_ptr->second,current_line);
}

template<typename T> Token Lexer<T>::tokenize_numeric(){
    std::stringstream ss;
    TokenKind tkind=TokenKind::INT;
    bool digit_pt=false;
    while(iter!=end && (std::isdigit(*iter) || *iter=='.')) {
        if (*iter=='.'){
            if (digit_pt) {
                break;
            }
            else{
                digit_pt=true;
                tkind=TokenKind::NUMBER;
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

template<typename T> Token Lexer<T>::tokenize_string(){
    std::stringstream ss;
    while(iter!=end && *iter!='"') {
        ss<<*iter;
        iter++;
    };
    iter++;
    return Token(TokenKind::STRING,std::move(ss.str()),current_line);
}

template<typename T> std::expected<Token,TokenError>Lexer<T>::get_token(){
    while(iter!=end && isspace(*iter) ){
        if(*iter=='\n') current_line++; 
        iter++;
    };

    if(iter==end) { return Token(TokenKind::Eof,current_line);}
    switch(*iter){
        case '(':{iter++;return Token(TokenKind::LEFT_PAREN,current_line);}
        case ')':{iter++;return Token(TokenKind::RIGHT_PAREN,current_line);}
        case '[':{iter++;return Token(TokenKind::LEFT_BRACE,current_line);}
        case ']':{iter++;return Token(TokenKind::RIGHT_BRACE,current_line);}
        case ',':{iter++;return Token(TokenKind::COMMA,current_line);}
        case ';':{iter++;return Token(TokenKind::SEMICOLON,current_line);}
        case '.':{iter++;return Token(TokenKind::DOT,current_line);}
        case '+':{iter++;return Token(TokenKind::PLUS,current_line);}
        case '-':{iter++;return Token(TokenKind::MINUS,current_line);}
        case '*':{iter++;return Token(TokenKind::STAR,current_line);}
        case '"':{iter++;return tokenize_string();}
        case '/':{
                     iter++;
                     if (iter ==end || *iter!='/' ){return Token(TokenKind::SLASH,current_line);}
                     iter++;
                     while( iter !=end && *iter!='\n' ) {iter++;}
                     break;
                 }
        case '=':{
                     iter++;
                     if (iter ==end || *iter!='=' ){return Token(TokenKind::EQUAL,current_line);}
                     iter++;
                     return Token(TokenKind::EQUAL_EQUAL,current_line);
                 }
        case '!':{
                     iter++;
                     if (iter ==end || *iter!='=' ){return Token(TokenKind::BANG,current_line);}
                     iter++;
                     return Token(TokenKind::BANG_EQUAL,current_line);
                 }
        default: break;
    };

    if(iter==end) { return Token(TokenKind::Eof,current_line);}
    if(std::isalpha(*iter)){
        return tokenize_ident();
    }

    if(iter==end) { return Token(TokenKind::Eof,current_line);}
    if(std::isdigit(*iter)){
        return tokenize_numeric();
    }
    return std::unexpected(TokenError{.msg = "unrecognized char"});
}

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

#endif
