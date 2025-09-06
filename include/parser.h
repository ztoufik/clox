#ifndef PARSER_H
#define PARSER_H

#include<memory>
#include<string>
#include<vector>
#include<optional>

#include"lexer.h"
#include"ast.h"

namespace tua{

template<typename T>class Parser{
    public:
        Parser(Lexer<T>&& lexer);
        std::expected<Program,std::string> parse();
    private:
        bool at_end() const noexcept;
        bool match_token_kind(Token& token, TokenKind kind) const noexcept;
        std::expected<bool,std::string> consume_token();
        std::expected<std::shared_ptr<Stmt>,std::string> parse_stmt();
        std::expected<Int,std::string> parse_int();
        std::expected<Double,std::string> parse_double();
        std::expected<Str,std::string> parse_str();
        std::expected<Bool,std::string> parse_bool();
        Lexer<T> _lexer;
        std::optional<Token> current_token;
};

template<typename T> Parser<T>::Parser(Lexer<T>&& lexer):_lexer(std::move(lexer)){ 
    current_token=_lexer.get_token().value();
}

template<typename T>  std::expected<bool,std::string> Parser<T>::consume_token(){
    if(!at_end()){ 
        auto rslt=_lexer.get_token();
        if(rslt){
            current_token=rslt.value();
            return true;
        }
        else{
            return std::unexpected("unrecognized token found");
        } 
    }
    return std::unexpected("end of tokens stream reached");
}

template<typename T> std::expected<std::shared_ptr<Stmt>,std::string> Parser<T>::parse_stmt(){
    while(!at_end()){
        switch(current_token.value().kind){
            case TokenKind::DOUBLE: {
                                        auto rslt=std::move(parse_double());
                                        if (rslt){
                                            return std::make_shared<Double>(rslt.value());
                                        }
                                        else{
                                            return std::unexpected(rslt.error());
                                        }
                                        break;
                                    }

            case TokenKind::INT: {
                                     auto rslt=std::move(parse_int());
                                     if (rslt){
                                         return std::make_shared<Int>(rslt.value());
                                     }
                                     else{
                                         return std::unexpected(rslt.error());
                                     }
                                     break;
                                 }

            case TokenKind::STRING: {
                                        auto rslt=std::move(parse_str());
                                        if (rslt){
                                            return std::make_shared<Str>(rslt.value());
                                        }
                                        else{
                                            return std::unexpected(rslt.error());
                                        }
                                        break;
                                    }

            case TokenKind::FALSE: 
            case TokenKind::TRUE:{
                                     auto rslt=std::move(parse_bool());
                                     if (rslt){
                                         return std::make_shared<Bool>(rslt.value());
                                     }
                                     else{
                                         return std::unexpected(rslt.error());
                                     }
                                     break;
                                 }

            default: return std::unexpected("implemented");
        };
    };
    return std::unexpected("implemented");
}

template<typename T>  std::expected<Int,std::string> Parser<T>::parse_int(){
    int value=std::atoi(current_token.value().lexeme.c_str());
    auto rslt=std::move(consume_token());
    if(rslt){
        return value;
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Double,std::string> Parser<T>::parse_double(){
    double value=std::stod(current_token.value().lexeme.c_str());
    auto rslt=std::move(consume_token());
    if(rslt){
        return Double(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Bool,std::string> Parser<T>::parse_bool(){
    TokenKind tkind=current_token.value().kind;
    bool value=(tkind==TokenKind::TRUE)?true:false;
    auto rslt=std::move(consume_token());
    if(rslt){
        return Bool(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Str,std::string> Parser<T>::parse_str(){
    auto value=current_token.value().lexeme;
    auto rslt=std::move(consume_token());
    if(rslt){
        return Str(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T> std::expected<Program,std::string> Parser<T>::parse(){
    auto stmts=std::vector<std::shared_ptr<Stmt>>{};
    while(!at_end()){
        auto stmt1=parse_stmt();
        if (stmt1){
            stmts.push_back(stmt1.value());
        }
        else{
            return std::unexpected(stmt1.error());
        }
        
    if(!match_token_kind(current_token.value(), TokenKind::SEMICOLON)){
            return std::unexpected("; expected");
        }
    if(!consume_token()){
            return std::unexpected("unexpected end of file");
    }
    };

    return std::move(Program(std::move(stmts)));
}

template<typename T> bool Parser<T>::at_end() const noexcept { return current_token.value().kind==TokenKind::Eof; }

template<typename T> bool Parser<T>::match_token_kind(Token& token, TokenKind kind) const noexcept { return token.kind==kind; }

}

#endif 
