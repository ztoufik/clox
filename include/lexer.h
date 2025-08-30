#ifndef LEXER_H
#define  LEXER_H

#include <string>
#include <unordered_map>

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

class Lexer {
    private:
        std::uint16_t current_char;
        std::uint16_t current_line;
        std::string src;

    public:
        Lexer(std::string src):src(src),current_char(0),current_line(0){}
        Token tokenize_ident();
        Token tokenize_numeric();
        Token tokenize_string();
        Token get_token();

        static const std::unordered_map<std::string,TokenKind> key_words;
};

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
