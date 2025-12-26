#ifndef PARSER_H
#define PARSER_H

#include<expected>
#include<optional>

#include"lexer.h"
#include"ast.h"
#include"error.h"

namespace tua{

    const uint16_t MAX_PARAMS=128;

    class Parser{
        public:
            Parser(Lexer&& lexer);
            Parser(Lexer& lexer);
            std::expected<Program,Error*> parse();
        private:
            bool at_end() const noexcept;
            bool match_token_kind(TokenKind kind) const;
            std::expected<bool,Error*> consume_token();
            std::expected<Stmt*,Error*> parse_stmt();
            std::expected<Block*,Error*> parse_block();
            std::expected<IfElse*,Error*> parse_if();
            std::expected<FctDecl*,Error*> parse_fct_decl();
            std::expected<ClassStmt*,Error*> parse_class();
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
            Lexer _lexer;
            std::optional<Token> current_token;
    };

}

#endif 
