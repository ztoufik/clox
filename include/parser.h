#ifndef PARSER_H
#define PARSER_H

#include<memory>
#include<string>
#include<vector>
#include<optional>

#include"lexer.h"
#include"ast.h"
 

template<typename T>class Parser{
    public:
        Parser(Lexer<T>&& lexer);
        std::expected<Program,std::string> parse();
    private:
        bool at_end() const noexcept;
        bool match_token_kind(Token& token, TokenKind kind) const noexcept;
        std::expected<bool,std::string> consume_token();
        std::expected<std::unique_ptr<Int>,std::string> parse_int();
        std::expected<std::unique_ptr<Double>,std::string> parse_double();
        std::expected<std::unique_ptr<Str>,std::string> parse_str();
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

template<typename T>  std::expected<std::unique_ptr<Int>,std::string> Parser<T>::parse_int(){
        double value=std::atoi(current_token.value().lexeme.c_str());
        auto rslt=std::move(consume_token());
        if(rslt){
            return std::make_unique<Int>(value);
        }
        return std::unexpected(rslt.error());
}

template<typename T>  std::expected<std::unique_ptr<Double>,std::string> Parser<T>::parse_double(){
        double value=std::stod(current_token.value().lexeme.c_str());
        auto rslt=std::move(consume_token());
        if(rslt){
            return std::make_unique<Double>(value);
        }
        return std::unexpected(rslt.error());
}

template<typename T>  std::expected<std::unique_ptr<Str>,std::string> Parser<T>::parse_str(){
        auto value=current_token.value().lexeme.c_str();
        auto rslt=std::move(consume_token());
        if(rslt){
            return std::make_unique<Str>(value);
        }
        return std::unexpected(rslt.error());
}

template<typename T> std::expected<Program,std::string> Parser<T>::parse(){
    auto stmts=std::vector<std::shared_ptr<Stmt>>{};
    std::unique_ptr<Stmt> stmt;

    while(!at_end()){
        switch(current_token.value().kind){
            case TokenKind::NUMBER: {
                                        auto rslt=std::move(parse_double());
                                        if (rslt){
                                            stmt=std::move(rslt.value());
                                        }
                                        else{
                                            return std::unexpected(rslt.error());
                                        }
                                        break;
                                    }
            case TokenKind::INT: {
                                        auto rslt=std::move(parse_int());
                                        if (rslt){
                                            stmt=std::move(rslt.value());
                                        }
                                        else{
                                            return std::unexpected(rslt.error());
                                        }
                                        break;
                                    }
        default: return std::unexpected("implemented");
        };
    };

    stmts.push_back(std::move(stmt));
    return std::move(Program(std::move(stmts)));
}

template<typename T> bool Parser<T>::at_end() const noexcept { return current_token.value().kind==TokenKind::Eof; }

template<typename T> bool Parser<T>::match_token_kind(Token& token, TokenKind kind) const noexcept { return token.kind==kind; }

#endif 
