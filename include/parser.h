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
            std::expected<Block*,Error*> parse_block();
            std::expected<IfElse*,Error*> parse_if();
            std::expected<WhileStmt*,Error*> parse_while();
            std::expected<VarDeclInit*,Error*> parse_vardeclinit();
            std::expected<Type*,Error*> parse_type();
            std::expected<Return*,Error*> parse_return();
            std::expected<Expr*,Error*> parse_expr();
            std::expected<Expr*,Error*> parse_term();
            std::expected<Expr*,Error*> parse_factor();
            std::expected<Expr*,Error*> parse_terminals();
            std::expected<Expr*,Error*> parse_group();
            std::expected<Expr*,Error*>parse_int();
            std::expected<Expr*,Error*> parse_double();
            std::expected<Expr*,Error*>parse_str();
            std::expected<Expr*,Error*> parse_bool();
            std::expected<Expr*,Error*> parse_fct_expr();
            std::expected<Expr*,Error*> parse_symbol_assign();
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
        std::expected<Stmt*,Error*> stmt;
        while(!at_end()){
            stmt=parse_stmt();
            if (!stmt){
                return std::unexpected(stmt.error());
            }
            stmts.push_back(stmt.value());

            if(!match_token_kind(TokenKind::SEMICOLON)){
                return std::unexpected(new ParseError("; expected",_lexer.get_line()));
            }
            consume_token();
        };

        return Program(std::move(stmts));
    }

    template<typename T> std::expected<Stmt*,Error*> Parser<T>::parse_stmt(){
        switch(current_token.value().kind){
            case tua::TokenKind::LEFT_BRACE: return parse_block();
            case tua::TokenKind::IF: return parse_if();
            case tua::TokenKind::WHILE: return parse_while();
            case tua::TokenKind::LET: return parse_vardeclinit();
            case tua::TokenKind::RETURN: return parse_return();
            default:{
                        auto expr= parse_expr();
                        if(!expr){
                            return std::unexpected(expr.error());
                        }
                        return expr.value();
                    }
        }
    }

    template<typename T> std::expected<Block*,Error*> Parser<T>::parse_block(){
        consume_token();
        auto stmts=std::vector<Stmt*>{};
        std::expected<Stmt*,Error*> stmt;
        while(!match_token_kind(TokenKind::RIGHT_BRACE)){
            stmt=parse_stmt();
            if (!stmt){
                return std::unexpected(stmt.error());
            }
            stmts.push_back(stmt.value());

            if(!match_token_kind(TokenKind::SEMICOLON)){
                return std::unexpected(new ParseError("; expected",_lexer.get_line()));
            }
            consume_token();
        };
        consume_token();

        return new Block(std::move(stmts));
    }

    template<typename T> std::expected<IfElse*,Error*> Parser<T>:: parse_if(){
        consume_token();
        if(!match_token_kind(TokenKind::LEFT_PAREN)){
            return std::unexpected(new ParseError("( expected",_lexer.get_line()));
        }
        consume_token();
        auto condi=parse_expr();
        if(!condi){
            return std::unexpected(condi.error());
        }
        if(!match_token_kind(TokenKind::RIGHT_PAREN)){
            return std::unexpected(new ParseError(") expected",_lexer.get_line()));
        }
        consume_token();
        auto if_block=parse_block();
        if(!if_block){
            return std::unexpected(if_block.error());
        }
        if(!match_token_kind(TokenKind::ELSE)){
            return new IfElse(condi.value(),if_block.value(),nullptr);
        }

        consume_token();
        auto else_block=parse_block();
        if(!else_block){
            return std::unexpected(else_block.error());
        }
        return new IfElse(condi.value(),if_block.value(),else_block.value());

    }

    template<typename T> std::expected<WhileStmt*,Error*> Parser<T>::parse_while(){
        consume_token();
        if(!match_token_kind(TokenKind::LEFT_PAREN)){
            return std::unexpected(new ParseError("( expected",_lexer.get_line()));
        }
        consume_token();
        auto condi=parse_expr();
        if(!condi){
            return std::unexpected(condi.error());
        }
        if(!match_token_kind(TokenKind::RIGHT_PAREN)){
            return std::unexpected(new ParseError(") expected",_lexer.get_line()));
        }
        consume_token();
        auto while_block=parse_block();
        if(!while_block){
            return std::unexpected(while_block.error());
        }
        return new WhileStmt(condi.value(),while_block.value());
    }

    template<typename T> std::expected<Type*,Error*> Parser<T>::parse_type(){
        auto ident=current_token.value().lexeme;
        consume_token();
        return new Type(std::move(ident));
    }
    template<typename T> std::expected<VarDeclInit*,Error*> Parser<T>::parse_vardeclinit(){
        consume_token();
        if(!match_token_kind(TokenKind::IDENT)){
            return std::unexpected(new ParseError("Ident expected",_lexer.get_line()));
        }

        auto ident=current_token.value().lexeme;
        consume_token();

        if(!match_token_kind(TokenKind::COLLON)){
            return std::unexpected(new ParseError(": expected",_lexer.get_line()));
        }
        consume_token();

        auto type=parse_type();
        if(!type){
            return std::unexpected(type.error());
        }

        if(!match_token_kind(TokenKind::EQUAL)){
            return new VarDeclInit(std::move(ident),nullptr,type.value());
        }

        consume_token();
        auto value=parse_expr();
        if(!value){
            return std::unexpected(value.error());
        }
        return new VarDeclInit(std::move(ident),value.value(),type.value());
    }

    template<typename T> std::expected<Return*,Error*> Parser<T>::parse_return(){
        consume_token();
        auto value=parse_expr();
        if(!value){
            return std::unexpected(value.error());
        }
        return new Return(value.value());
    }

    template<typename T> std::expected<Expr*,Error*> Parser<T>::parse_expr(){
        return  parse_term();
    }

    template<typename T> std::expected<Expr*,Error*> Parser<T>::parse_term(){
        auto left_expr=parse_factor();
        if (left_expr){
            switch(current_token.value().kind){
                case TokenKind::PLUS:{
                                         consume_token();
                                         auto right_expr=parse_expr();
                                         if (!right_expr){
                                             return right_expr;
                                         }
                                         return new Add(left_expr.value(),right_expr.value());
                                     }

                case TokenKind::MINUS:{
                                          consume_token();
                                          auto right_expr=parse_expr();
                                          if (!right_expr){
                                              return right_expr;
                                          }
                                          return new Sub(left_expr.value(),right_expr.value());
                                      }
                default:break;
            }
        }
        return left_expr;
    }

    template<typename T> std::expected<Expr*,Error*> Parser<T>::parse_factor(){
        auto left_expr=parse_fctcalls();
        if (left_expr){
            switch(current_token.value().kind){
                case tua::TokenKind::STAR:{

                                              consume_token();
                                              auto right_expr=parse_expr();
                                              if (!right_expr){
                                                  return right_expr;
                                              }
                                              return new Mul(left_expr.value(),right_expr.value());
                                          }

                case TokenKind::SLASH :{
                                           consume_token();
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
        std::expected<Expr*,Error*> arg;
        consume_token();
        if(!match_token_kind(TokenKind::RIGHT_PAREN)){
            arg=parse_expr();
            if(!arg){
                return arg;
            }
            args.push_back(arg.value());
            while(!match_token_kind(TokenKind::RIGHT_PAREN)){
                if(!match_token_kind(TokenKind::COMMA)){
                    return std::unexpected(new ParseError(std::string(", expected"),_lexer.get_line()));
                }
                consume_token();
                arg=parse_expr();
                if(!arg){
                    return arg;
                }
                args.push_back(arg.value());
            }
        }
        consume_token();
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
            case TokenKind::IDENT: return parse_symbol_assign();
            case TokenKind::LAMBDA: return parse_fct_expr();

            default: break;
        };
        return std::unexpected(new ParseError("unknown token",_lexer.get_line()));
    }

    template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_group(){
        consume_token();
        auto expr=parse_expr();
        if(!expr){
            return expr;
        }
        if(!match_token_kind(TokenKind::RIGHT_PAREN)){
            return std::unexpected(new ParseError(") expected",_lexer.get_line()));
        }
        consume_token();
        return new Group(expr.value());
    }

    template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_int(){
        int value=std::atoi(current_token.value().lexeme.c_str());
        consume_token();
        return new Int(value);
    }

    template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_double(){
        double value=std::stod(current_token.value().lexeme.c_str());
        consume_token();
        return new Double(value);
    }

    template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_bool(){
        TokenKind tkind=current_token.value().kind;
        bool value=(tkind==TokenKind::TRUE)?true:false;
        consume_token();
        return new Bool(value);
    }

    template<typename T> std::expected<Expr*,Error*> Parser<T>::parse_fct_expr(){
        consume_token();
        auto ret_type=parse_type();
       if(!ret_type){
            return std::unexpected(ret_type.error());
        }

        if(!match_token_kind(TokenKind::LEFT_PAREN)){
            return std::unexpected(new ParseError("( expected",_lexer.get_line()));
        }
        consume_token();
        Params params;
        uint16_t params_count=128;
        while(!match_token_kind(TokenKind::RIGHT_PAREN) && params_count){
            auto ident=current_token.value().lexeme;
            consume_token();

            if(!match_token_kind(TokenKind::COLLON)){
                return std::unexpected(new ParseError("missing : ",_lexer.get_line()));
            }
            consume_token();

            auto type=parse_type();
            if(!type){
                return std::unexpected(type.error());
            }
            auto tuple=std::make_tuple(std::move(ident),std::move(*type.value()));
            params.push_back(tuple);

            if(!match_token_kind(TokenKind::COMMA)){
                return std::unexpected(new ParseError("missing , ",_lexer.get_line()));
            }
            consume_token();

            params_count--;
        }

        if(!params_count){
            return std::unexpected(new ParseError("missing ) or overexceed params count",_lexer.get_line()));
        }
        consume_token();

        return new FctExpr(ret_type.value(),std::move(params));
    }

    template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_str(){
        auto value=current_token.value().lexeme;
        consume_token();
        return new Str(value);
    }

    template<typename T>  std::expected<Expr*,Error*> Parser<T>::parse_symbol_assign(){
        auto ident=current_token.value().lexeme;
        consume_token();
        if(!match_token_kind(TokenKind::EQUAL)){
            return new Symbol(std::move(ident));
        }
        consume_token();
        auto value=parse_expr();
        if(!value){
            return value;
        }
        return new Assign(std::move(ident),value.value());

    }

    template<typename T> bool Parser<T>::at_end() const noexcept {return match_token_kind(TokenKind::Eof);}

    template<typename T> bool Parser<T>::match_token_kind(TokenKind kind) const { return current_token.value().kind==kind; }
}

#endif 
