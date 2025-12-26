#include<string>
#include<vector>

#include<expected>
#include<optional>

#include"parser.h"
#include"lexer.h"
#include"ast.h"
#include"error.h"


namespace tua{

Parser::Parser(Lexer&& lexer):_lexer(std::move(lexer)){ current_token=_lexer.get_token(); }
Parser::Parser(Lexer& lexer):_lexer(lexer){ current_token=_lexer.get_token(); }


std::expected<bool,Error*> Parser::consume_token(){
    if(match_token_kind(TokenKind::Err)){
        return std::unexpected(new ParseError("unrecognized token found",current_token->line));
    }
    if(match_token_kind(TokenKind::Eof)){
        return std::unexpected(new ParseError("end of token stream",current_token->line));
    }
    current_token=_lexer.get_token();
    return true;
}

std::expected<Program,Error*> Parser::parse(){
    auto stmts=std::vector<Stmt*>{};
    std::expected<Stmt*,Error*> stmt;
    while(!at_end()){
        stmt=parse_stmt();
        if (!stmt){
            return std::unexpected(stmt.error());
        }
        stmts.push_back(stmt.value());

        if(!match_token_kind(TokenKind::SEMICOLON)){
        return std::unexpected(new ParseError("; expected",current_token->line));
        }
        consume_token();
    };

    return Program(std::move(stmts));
}

std::expected<Stmt*,Error*> Parser::parse_stmt(){
    switch(current_token.value().kind){
        case tua::TokenKind::LEFT_BRACE: return parse_block();
        case tua::TokenKind::IF: return parse_if();
        case tua::TokenKind::WHILE: return parse_while();
        case tua::TokenKind::FUN: return parse_fct_decl();
        case tua::TokenKind::CLASS: return parse_class();
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

std::expected<Block*,Error*> Parser::parse_block(){
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
            return std::unexpected(new ParseError("; expected",current_token->line));
        }
        consume_token();
    };
    consume_token();

    return new Block(std::move(stmts));
}

std::expected<IfElse*,Error*> Parser:: parse_if(){
    consume_token();
    if(!match_token_kind(TokenKind::LEFT_PAREN)){
        return std::unexpected(new ParseError("( expected",current_token->line));
    }
    consume_token();
    auto condi=parse_expr();
    if(!condi){
        return std::unexpected(condi.error());
    }
    if(!match_token_kind(TokenKind::RIGHT_PAREN)){
        return std::unexpected(new ParseError(") expected",current_token->line));
    }
    consume_token();
    if(!match_token_kind(TokenKind::LEFT_BRACE)){
        return std::unexpected(new ParseError("{ expected",current_token->line));
    }
    auto if_block=parse_block();
    if(!if_block){
        return std::unexpected(if_block.error());
    }
    if(!match_token_kind(TokenKind::ELSE)){
        return new IfElse(condi.value(),if_block.value(),nullptr);
    }

    consume_token();
    if(!match_token_kind(TokenKind::LEFT_BRACE)){
        return std::unexpected(new ParseError("{ expected",current_token->line));
    }
    auto else_block=parse_block();
    if(!else_block){
        return std::unexpected(else_block.error());
    }
    return new IfElse(condi.value(),if_block.value(),else_block.value());

}

std::expected<FctDecl*,Error*> Parser::parse_fct_decl(){
    consume_token();
    if(!match_token_kind(TokenKind::IDENT)){
        return std::unexpected(new ParseError("return type identifier expected ",current_token->line));
    }
    auto ret_type=parse_type();
    if(!ret_type){
        return std::unexpected(ret_type.error());
    }
    if(!match_token_kind(TokenKind::IDENT)){
        return std::unexpected(new ParseError("function identifier expected",current_token->line));
    }

    auto ident=parse_symbol_assign();

    if(!match_token_kind(TokenKind::LEFT_PAREN)){
        return std::unexpected(new ParseError("( expected",current_token->line));
    }
    consume_token();
    Params params;
    uint16_t params_count=MAX_PARAMS;
    while(!match_token_kind(TokenKind::RIGHT_PAREN) && params_count){
        if(!match_token_kind(TokenKind::IDENT)){
            return std::unexpected(new ParseError("parameter identifier expected",current_token->line));
        }
        auto param=parse_symbol_assign();

        if(!match_token_kind(TokenKind::COLLON)){
            return std::unexpected(new ParseError(": expected",current_token->line));
        }
        consume_token();

        if(!match_token_kind(TokenKind::IDENT)){
            return std::unexpected(new ParseError("type identifier expected",current_token->line));
        }
        auto type=parse_type();
        if(!type){
            return std::unexpected(type.error());
        }
        auto tuple=std::make_tuple((Symbol*)param.value(),type.value());
        params.push_back(tuple);

        if(!match_token_kind(TokenKind::COMMA)){
            return std::unexpected(new ParseError(", expected",current_token->line));
        }
        consume_token();

        params_count--;
    }

    if(!params_count){
        return std::unexpected(new ParseError(") expected or overexceed params count",current_token->line));
    }
    consume_token();

    if(!match_token_kind(TokenKind::LEFT_BRACE)){
        return std::unexpected(new ParseError("{ expected",current_token->line));
    }

    auto block=parse_block();
    if(!block){
        return std::unexpected(block.error());
    }

    return new FctDecl(ret_type.value(),(Symbol*)(ident.value()),std::move(params),block.value());
}

std::expected<ClassStmt*,Error*> Parser::parse_class(){
    consume_token();
    if(!match_token_kind(TokenKind::IDENT)){
        return std::unexpected(new ParseError("class identifier expected",current_token->line));
    }
    auto ident=parse_symbol_assign();

    Type* type=nullptr;

    if(match_token_kind(TokenKind::COLLON)){
        consume_token();
        if(!match_token_kind(TokenKind::IDENT)){
            return std::unexpected(new ParseError("parent class identifier expected",current_token->line));
        }
        auto op_type=parse_type();
        type=op_type.value();
    }

    if(!match_token_kind(TokenKind::LEFT_BRACE)){
        return std::unexpected(new ParseError("{ expected",current_token->line));
    }

    auto block=parse_block();
    if(!block){
        auto msg=std::string("couldn't parse class body : ")+block.error()->_msg;
        return std::unexpected(new ParseError(std::move(msg),current_token->line));
    }
    Expr* value=ident.value();
    return new ClassStmt((Symbol*)(value),type,block.value());
}

std::expected<WhileStmt*,Error*> Parser::parse_while(){
    consume_token();
    if(!match_token_kind(TokenKind::LEFT_PAREN)){
        return std::unexpected(new ParseError("( expected",current_token->line));
    }
    consume_token();
    auto condi=parse_expr();
    if(!condi){
        return std::unexpected(condi.error());
    }
    if(!match_token_kind(TokenKind::RIGHT_PAREN)){
        return std::unexpected(new ParseError(") expected",current_token->line));
    }
    consume_token();
    if(!match_token_kind(TokenKind::LEFT_BRACE)){
        return std::unexpected(new ParseError("{ expected",current_token->line));
    }
    auto while_block=parse_block();
    if(!while_block){
        return std::unexpected(while_block.error());
    }
    return new WhileStmt(condi.value(),while_block.value());
}

std::expected<Type*,Error*> Parser::parse_type(){
    auto ident=current_token.value().lexeme;
    consume_token();
    return new Type(std::move(ident));
}

std::expected<VarDeclInit*,Error*> Parser::parse_vardeclinit(){
    consume_token();
    if(!match_token_kind(TokenKind::IDENT)){
        return std::unexpected(new ParseError("variable identifier expected",current_token->line));
    }

    auto ident=parse_symbol_assign();

    if(!match_token_kind(TokenKind::COLLON)){
        return std::unexpected(new ParseError(": expected",current_token->line));
    }
    consume_token();

    if(!match_token_kind(TokenKind::IDENT)){
        return std::unexpected(new ParseError("type identifier expected",current_token->line));
    }
    auto type=parse_type();
    if(!type){
        return std::unexpected(type.error());
    }

    if(!match_token_kind(TokenKind::EQUAL)){
        return new VarDeclInit((Symbol*)(ident.value()),nullptr,type.value());
    }

    consume_token();
    auto value=parse_expr();
    if(!value){
        return std::unexpected(value.error());
    }
    return new VarDeclInit((Symbol*)(ident.value()),value.value(),type.value());
}

std::expected<Return*,Error*> Parser::parse_return(){
    consume_token();
    auto value=parse_expr();
    if(!value){
        return std::unexpected(value.error());
    }
    return new Return(value.value());
}

std::expected<Expr*,Error*> Parser::parse_expr(){
    return  parse_term();
}

std::expected<Expr*,Error*> Parser::parse_term(){
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

std::expected<Expr*,Error*> Parser::parse_factor(){
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

std::expected<Expr*,Error*> Parser::parse_fctcalls(){
    auto expr=parse_terminals();
    if(expr){
        while(match_token_kind(TokenKind::LEFT_PAREN)){
            expr=parse_fctcall(expr.value());
        }
    }

    return expr;
}

std::expected<Expr*,Error*> Parser::parse_fctcall(Expr* expr){
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
                return std::unexpected(new ParseError(", expected",current_token->line));
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

std::expected<Expr*,Error*> Parser::parse_terminals(){
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
    auto token_lexeme=current_token.value().lexeme;
    std::string msg=std::string("unknown Terminal token : ")+token_lexeme;
    return std::unexpected(new ParseError(std::move(msg),current_token->line));
}

std::expected<Expr*,Error*> Parser::parse_group(){
    consume_token();
    auto expr=parse_expr();
    if(!expr){
        return expr;
    }
    if(!match_token_kind(TokenKind::RIGHT_PAREN)){
        return std::unexpected(new ParseError(") expected",current_token->line));
    }
    consume_token();
    return new Group(expr.value());
}

std::expected<Expr*,Error*> Parser::parse_int(){
    int value=std::atoi(current_token.value().lexeme.c_str());
    consume_token();
    return new Int(value);
}

std::expected<Expr*,Error*> Parser::parse_double(){
    double value=std::stod(current_token.value().lexeme.c_str());
    consume_token();
    return new Double(value);
}

std::expected<Expr*,Error*> Parser::parse_bool(){
    TokenKind tkind=current_token.value().kind;
    bool value=(tkind==TokenKind::TRUE)?true:false;
    consume_token();
    return new Bool(value);
}

std::expected<Expr*,Error*> Parser::parse_fct_expr(){
    consume_token();
    if(!match_token_kind(TokenKind::IDENT)){
        return std::unexpected(new ParseError("return type identifier expected ",current_token->line));
    }
    auto ret_type=parse_type();
    if(!ret_type){
        return std::unexpected(ret_type.error());
    }

    if(!match_token_kind(TokenKind::LEFT_PAREN)){
        return std::unexpected(new ParseError("( expected",current_token->line));
    }
    consume_token();
    Params params;
    uint16_t params_count=MAX_PARAMS;
    while(!match_token_kind(TokenKind::RIGHT_PAREN) && params_count){
        if(!match_token_kind(TokenKind::IDENT)){
            return std::unexpected(new ParseError("parameter identifier expected ",current_token->line));
        }
        auto param=parse_symbol_assign();

        if(!match_token_kind(TokenKind::COLLON)){
            return std::unexpected(new ParseError(": expected ",current_token->line));
        }
        consume_token();
        if(!match_token_kind(TokenKind::IDENT)){
            return std::unexpected(new ParseError("type identifier expected ",current_token->line));
        }

        auto type=parse_type();
        if(!type){
            return std::unexpected(type.error());
        }
        auto tuple=std::make_tuple((Symbol*)param.value(),type.value());
        params.push_back(tuple);

        if(!match_token_kind(TokenKind::COMMA)){
            return std::unexpected(new ParseError(", expected ",current_token->line));
        }
        consume_token();

        params_count--;
    }

    if(!params_count){
        return std::unexpected(new ParseError(") expected or overexceed params count",current_token->line));
    }
    consume_token();

    if(!match_token_kind(TokenKind::LEFT_BRACE)){
        return std::unexpected(new ParseError("{ expected ",current_token->line));
    }

    auto block=parse_block();
    if(!block){
        return std::unexpected(block.error());
    }

    return new FctExpr(ret_type.value(),std::move(params),block.value());
}

std::expected<Expr*,Error*> Parser::parse_str(){
    auto value=current_token.value().lexeme;
    consume_token();
    return new Str(value);
}

std::expected<Expr*,Error*> Parser::parse_symbol_assign(){
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

bool Parser::at_end() const noexcept {return match_token_kind(TokenKind::Eof);}

bool Parser::match_token_kind(TokenKind kind) const { return current_token.value().kind==kind; }

};
