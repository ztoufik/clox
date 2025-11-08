#ifndef PARSER_H
#define PARSER_H

#include<string>
#include<expected>
#include<vector>
#include<optional>

#include"lexer.h"
#include"ast.h"
#include"error.h"

namespace tua{

    template<typename T>class Parser{
        public:
            Parser(Lexer<T>&& lexer);
            std::expected<Program,Error*> parse();
        private:
            bool at_end() const noexcept;
            bool match_token_kind(TokenKind kind) const;
            std::expected<bool,Error*> consume_token();
            std::expected<Stmt*,Error*> parse_stmt();
            std::expected<Expr*,Error*> parse_expr();
            std::expected<Expr*,Error*> parse_term();
            std::expected<Expr*,Error*> parse_factor();
            std::expected<Expr*,Error*> parse_terminals();
            std::expected<Expr*,Error*> parse_group();
            std::expected<Expr*,Error*>parse_int();
            std::expected<Expr*,Error*> parse_double();
            std::expected<Expr*,Error*>parse_str();
            std::expected<Expr*,Error*> parse_bool();
            std::expected<Expr*,Error*> parse_symbol();
            std::expected<Expr*,Error*> parse_fctcalls();
            std::expected<Expr*,Error*> parse_fctcall(Expr* expr);
            Lexer<T> _lexer;
            std::optional<Token> current_token;
    };

    template<typename T> Parser<T>::Parser(Lexer<T>&& lexer):_lexer(std::move(lexer)){ 
        current_token=_lexer.get_token();
    }

    template<typename T>  std::expected<bool,Error*> Parser<T>::consume_token(){
        if(match_token_kind(TokenKind::Err)){
            return std::unexpected(new ParseError("unrecognized token found",_lexer.get_line()));
        }
        if(match_token_kind(TokenKind::Eof)){
            return std::unexpected(new ParseError("unrecognized token found",_lexer.get_line()));
        }
        current_token=_lexer.get_token();
        return true;
    }

    template<typename T> std::expected<Program,Error*> Parser<T>::parse(){
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
                return std::unexpected(new ParseError("; expected",0));
            }

            auto token=consume_token();
            if(!token){
                return std::unexpected(token.error());
            }
        };

        return Program(std::move(stmts));
    }

    template<typename T> std::expected<Stmt*,Error*> Parser<T>::parse_stmt(){
        auto expr= parse_expr();
        if(!expr){
            return std::unexpected(expr.error());
        }
        return expr.value();
    }

    template<typename T> std::expected<Expr*,Error*> Parser<T>::parse_expr(){
        return  parse_term();
    }

    template<typename T> std::expected<Expr*,Error*> Parser<T>::parse_term(){
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

template<typename T> std::expected<Expr*,Error*> Parser<T>::parse_factor(){
    auto left_expr=parse_fctcalls();
    if (left_expr){
    switch(current_token.value().kind){
        case tua::TokenKind::STAR:{

                                      auto rslt=consume_token();
                                      auto right_expr=parse_expr();
                                      if (!right_expr){
                                          return right_expr;
                                      }
                                      return new Mul(left_expr.value(),right_expr.value());
                                  }

        case TokenKind::SLASH :{
                                   auto rslt=consume_token();
                                   auto right_expr=parse_expr();
                                  if (!right_expr){
                                      return right_expr;
                                  }
                                   return new Div(left_expr.value(),right_expr.value());
                               }
        default:break;
    }
    }
    return left_expr;
}

template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_fctcalls(){
    auto expr=parse_terminals();
    if(expr){
        while(match_token_kind(TokenKind::LEFT_PAREN)){
            expr=parse_fctcall(expr.value());
        }
    }

    return expr;
}

template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_fctcall(Expr* expr){
    auto args=std::vector<Expr*>();
    auto token=consume_token();
    if(match_token_kind(TokenKind::RIGHT_PAREN)){
        token=consume_token();
        return new FctCall(expr,std::move(args));
    }
    auto arg=parse_expr();
    if(!arg){
        return arg;
    }
    args.push_back(arg.value());
    while(!match_token_kind(TokenKind::RIGHT_PAREN)){
        if(!match_token_kind(TokenKind::COMMA)){
            return std::unexpected(new ParseError(std::string(", expected"),_lexer.get_line()));
        }
        auto rslt=consume_token();
        auto arg=parse_expr();
        if(!arg){
            return arg;
        }
        args.push_back(arg.value());
    }
    auto rslt=consume_token();
    return new FctCall(expr,std::move(args));
}

template<typename T> std::expected<Expr*,Error*> Parser<T>::parse_terminals(){
    switch(current_token.value().kind){
        case TokenKind::DOUBLE:  return parse_double();

        case TokenKind::INT: return parse_int();

        case TokenKind::STRING: return parse_str();

        case TokenKind::FALSE: 
        case TokenKind::TRUE: return parse_bool();

        case TokenKind::LEFT_PAREN: return parse_group();
        case TokenKind::IDENT: return parse_symbol();

        default: break;
    };
        return std::unexpected(new ParseError("unimplemented",_lexer.get_line()));
}

template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_group(){
    auto rslt=consume_token();
    auto expr=parse_expr();
    if(!expr){
        return expr;
    }
    if(!match_token_kind(TokenKind::RIGHT_PAREN)){
        return std::unexpected(new ParseError(") expected",_lexer.get_line()));
    }
    rslt=consume_token();
    return new Group(expr.value());
}

template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_int(){
    int value=std::atoi(current_token.value().lexeme.c_str());
    auto rslt=consume_token();
    if(rslt){
        return new Int(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_double(){
    double value=std::stod(current_token.value().lexeme.c_str());
    auto rslt=consume_token();
    if(rslt){
        return new Double(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_bool(){
    TokenKind tkind=current_token.value().kind;
    bool value=(tkind==TokenKind::TRUE)?true:false;
    auto rslt=consume_token();
    if(rslt){
        return new Bool(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_str(){
    auto value=current_token.value().lexeme;
    auto rslt=consume_token();
    if(rslt){
        return new Str(value);
    }
    return std::unexpected(rslt.error());
}

template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_symbol(){
    auto sym_name=current_token.value().lexeme;
    auto rslt=consume_token();
    if(!rslt){
        return std::unexpected(rslt.error());
    }
    return new Symbol(std::move(sym_name));
}



template<typename T> bool Parser<T>::at_end() const noexcept {return match_token_kind(TokenKind::Eof);}

template<typename T> bool Parser<T>::match_token_kind(TokenKind kind) const { return current_token.value().kind==kind; }
}

#endif 
