#ifndef PARSER_H
#define PARSER_H

#include<string>
#include<expected>
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
            bool match_token_kind(TokenKind kind) const;
            std::expected<bool,std::string> consume_token();
            std::expected<Stmt*,std::string> parse_stmt();
            std::expected<Expr*,std::string> parse_expr();
            std::expected<Expr*,std::string> parse_term();
            std::expected<Expr*,std::string> parse_factor();
            std::expected<Expr*,std::string> parse_terminals();
            std::expected<Group*,std::string> parse_group();
            std::expected<Int*,std::string> parse_int();
            std::expected<Double*,std::string> parse_double();
            std::expected<Str*,std::string> parse_str();
            std::expected<Bool*,std::string> parse_bool();
            Lexer<T> _lexer;
            std::optional<Token> current_token;
    };

    template<typename T> Parser<T>::Parser(Lexer<T>&& lexer):_lexer(std::move(lexer)){ 
        current_token=_lexer.get_token();
    }

    template<typename T>  std::expected<bool,std::string> Parser<T>::consume_token(){
        if(match_token_kind(TokenKind::Err)){
            return std::unexpected("unrecognized token found");
        }
        if(match_token_kind(TokenKind::Eof)){
            return std::unexpected("end of tokens stream reached");
        }

        current_token=_lexer.get_token();
        return true;
    }

    template<typename T> std::expected<Program,std::string> Parser<T>::parse(){
        auto stmts=std::vector<Stmt*>{};
        while(!at_end()){
            auto stmt1=parse_stmt();
            if (stmt1){
                stmts.push_back(stmt1.value());
            }
            else{
                return std::unexpected(stmt1.error());
            }

            if(!match_token_kind(TokenKind::SEMICOLON)){
                return std::unexpected("; expected");
            }

            auto token=consume_token();
            if(!token){
                return std::unexpected(token.error());
            }
        };

        return Program(std::move(stmts));
    }

    template<typename T> std::expected<Stmt*,std::string> Parser<T>::parse_stmt(){
        auto expr= parse_expr();
        if(!expr){
            return std::unexpected(expr.error());
        }
        return expr.value();
    }

    template<typename T> std::expected<Expr*,std::string> Parser<T>::parse_expr(){
        return  parse_term();
    }

    template<typename T> std::expected<Expr*,std::string> Parser<T>::parse_term(){
        auto left_expr=parse_factor();
        if (!left_expr){
            return std::unexpected(left_expr.error());
        }
        switch(current_token.value().kind){
            case TokenKind::PLUS:{
                                     auto rslt=consume_token();
                                     auto right_expr=parse_expr();
                                     if (!right_expr){
                                         return std::unexpected(right_expr.error());
                                     }
                                     return new Add(left_expr.value(),right_expr.value());
                                 }

            case TokenKind::MINUS:{
                                      auto rslt=consume_token();
                                      auto right_expr=parse_expr();
                                      if (!right_expr){
                                          return std::unexpected(right_expr.error());
                                      }
                                      return new Sub(left_expr.value(),right_expr.value());
                                  }
            default:break;
        }
    return left_expr;
    }

template<typename T> std::expected<Expr*,std::string> Parser<T>::parse_factor(){
    auto left_expr=parse_terminals();
    if (!left_expr){
        return std::unexpected(left_expr.error());
    }
    switch(current_token.value().kind){
        case tua::TokenKind::STAR:{

                                      auto rslt=consume_token();
                                      auto right_expr=parse_expr();
                                      if (!right_expr){
                                          return std::unexpected(right_expr.error());
                                      }
                                      return new Mul(left_expr.value(),right_expr.value());
                                  }

        case TokenKind::SLASH :{
                                   auto rslt=consume_token();
                                   auto right_expr=parse_expr();
                                   if (!right_expr){
                                       return std::unexpected(right_expr.error());
                                   }
                                   return new Div(left_expr.value(),right_expr.value());
                               }
        default:break;
    }

    return left_expr;
}

template<typename T> std::expected<Expr*,std::string> Parser<T>::parse_terminals(){
    switch(current_token.value().kind){
        case TokenKind::DOUBLE: {
                                    auto rslt=parse_double();
                                    if (!rslt){
                                        return std::unexpected(rslt.error());
                                    }
                                    return rslt.value();
                                }

        case TokenKind::INT: {
                                 auto rslt=parse_int();
                                 if (!rslt){
                                     return std::unexpected(rslt.error());
                                 }
                                 return rslt.value();
                             }

        case TokenKind::STRING: {
                                    auto rslt=parse_str();
                                    if (!rslt){
                                        return std::unexpected(rslt.error());
                                    }
                                    return rslt.value();
                                }

        case TokenKind::FALSE: 
        case TokenKind::TRUE:{
                                 auto rslt=parse_bool();
                                 if (!rslt){
                                     return std::unexpected(rslt.error());
                                 }
                                 return rslt.value();
                             }

        case TokenKind::LEFT_PAREN:{
                                       auto rslt=parse_group();
                                       if (!rslt){
                                           return std::unexpected(rslt.error());
                                       }
                                       return rslt.value();
                                   }

        default: return std::unexpected("implemented");
    };
    return std::unexpected("implemented");
}

template<typename T>  std::expected<Group*,std::string> Parser<T>::parse_group(){
    auto rslt=consume_token();
    auto expr=parse_expr();
    if(!expr){
        return std::unexpected("couldn't parse group");
    }
    if(!match_token_kind(TokenKind::RIGHT_PAREN)){
        return std::unexpected("')' expected");
    }
    rslt=consume_token();
    return new Group(expr.value());
}

template<typename T>  std::expected<Int*,std::string> Parser<T>::parse_int(){
    int value=std::atoi(current_token.value().lexeme.c_str());
    auto rslt=consume_token();
    if(rslt){
        return new Int(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Double*,std::string> Parser<T>::parse_double(){
    double value=std::stod(current_token.value().lexeme.c_str());
    auto rslt=consume_token();
    if(rslt){
        return new Double(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Bool*,std::string> Parser<T>::parse_bool(){
    TokenKind tkind=current_token.value().kind;
    bool value=(tkind==TokenKind::TRUE)?true:false;
    auto rslt=consume_token();
    if(rslt){
        return new Bool(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Str*,std::string> Parser<T>::parse_str(){
    auto value=current_token.value().lexeme;
    auto rslt=consume_token();
    if(rslt){
        return new Str(value);
    }
    return std::unexpected(rslt.error());
}


template<typename T> bool Parser<T>::at_end() const noexcept {return match_token_kind(TokenKind::Eof);}

template<typename T> bool Parser<T>::match_token_kind(TokenKind kind) const { return current_token.value().kind==kind; }
}

#endif 
