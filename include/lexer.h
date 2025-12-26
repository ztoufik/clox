#ifndef LEXER_H
#define  LEXER_H

#include <string>
#include <unordered_map>

namespace tua{

    enum class TokenKind {
        Err,
        Eof ,

        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        LEFT_BRACE,
        RIGHT_BRACE,

        SLASH,
        STAR,
        MINUS,
        PLUS,

        COMMA,
        COLLON,
        DOT,
        EQUAL,
        SEMICOLON,

        BIT_OR,
        BIT_AND,
        BIT_RSHIFT,
        BIT_LSHIFT,

        BANG_EQUAL,
        BANG,

        GREATER,
        LESS,
        EQUAL_EQUAL,
        GREATER_EQUAL,
        LESS_EQUAL,

        IDENT,
        STRING,
        INT,
        DOUBLE,

        OR ,
        AND ,
        CLASS ,
        SUPER ,
        THIS ,
        FALSE ,
        TRUE ,
        FOR ,
        WHILE ,
        IF ,
        ELSE ,
        NIL ,
        FUN ,
        LAMBDA ,
        RETURN ,
        LET ,
    };

    struct Token {
        TokenKind kind;
        std::string lexeme;
        std::uint16_t line;
        char* begin,end;

        Token(TokenKind kind, std::string&& lexeme,std::uint16_t line) : kind(kind), lexeme(lexeme),line(line) {}
        Token(TokenKind kind,std::uint16_t line) : kind(kind),lexeme(""),line(line){}
        Token(const Token&)=default;
        Token(Token&&)=default;
        Token& operator=(const Token&)=default;
        Token& operator=(Token&&)=default;

        friend bool operator==(const Token& Rhs, const Token& Lhs){
            return Rhs.kind==Lhs.kind && Rhs.lexeme==Lhs.lexeme && Rhs.line==Lhs.line;
        }

        friend bool operator!=(const Token& Rhs, const Token& Lhs){
            return Rhs != Lhs;
        }
    };

    class Lexer {
        public:
            Lexer(std::string&& source):src(std::move(source)),iter(src.begin()),iter_end(src.end()),current_line(0){ }
            const Token get_token();
            const std::uint16_t get_line() const noexcept;

        private:
            std::string src;
            std::string::iterator iter,iter_end;
            std::uint16_t current_line;

            const void consume(); 
            const bool at_end() const noexcept;
            const Token tokenize_ident();
            const Token tokenize_numeric();
            const Token tokenize_string();

            static const std::unordered_map<std::string,TokenKind> key_words;
    };

    
};
#endif
