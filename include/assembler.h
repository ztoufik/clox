#ifndef CHECKER_H
#define CHECKER_H

#include<vector>
#include<any>
#include<map>
#include<string>
#include<optional>

#include "ast.h"

namespace tua{

    class TType;
    class TVar;


    class TType{
        public:
            std::vector<FctDecl> fcts;
            std::vector<TVar> vars;
            std::string ident;
    };

    class TVar{
        public:
            std::string ident;
            TType* type;
            std::any value;
    };


    class TypeFrame{
        public:
            TypeFrame(std::map<std::string,TType>&& types_map):types(std::move(types_map)){}
            const std::optional<const TType> get_type(const std::string& ident) const;
            void add_type(const std::string& ident, const TType& type);
        private:
            std::map<std::string,TType> types;
            TypeFrame* parent;
    };


    class VarFrame{
        public:
            VarFrame(std::map<std::string,TVar>&& vars_map):vars(std::move(vars_map)){}
            const std::optional<const TVar> get_var(const std::string& ident) const;
            void add_var(const std::string& ident, const TVar& var);
        private:
            std::map<std::string,TVar> vars;
            VarFrame* parent;
    };

    class AssemblerVisitor:Visitor{
        public:
             void visit_program(Program*) override;
             void visit_accexpr(AccExpr*) override;
             void visit_fctexpr(FctExpr*) override;
             void visit_fctcall(FctCall*) override;
             void visit_assign(Assign*) override;
             void visit_literal(Double*) override;
             void visit_literal(Int*) override;
             void visit_literal(Str*) override;
             void visit_literal(Bool*) override;
             void visit_binexpr(BinExpr*) override;
             void visit_bin_expr(Add*) override;
             void visit_bin_expr(Sub*) override;
             void visit_bin_expr(Mul*) override;
             void visit_bin_expr(Div*) override;
             void visit_bin_expr(Less*) override;
             void visit_bin_expr(Great*) override;
             void visit_bin_expr(LessEq*) override;
             void visit_bin_expr(GreatEq*) override;
             void visit_bin_expr(BitOr*) override;
             void visit_bin_expr(BitAnd*) override;
             void visit_bin_expr(RShift*) override;
             void visit_bin_expr(LShift*) override;
             void visit_unaryexpr(UnaryExpr*) override;
             void visit_unar_expr(Minus*) override;
             void visit_unar_expr(Negate*) override;
             void visit_unar_expr(Group*) override;
             void visit_return(Return*) override;
             void visit_whilestmt(WhileStmt*) override;
             void visit_ifelse(IfElse*) override;
             void visit_fctdecl(FctDecl*) override;
             void visit_vardeclinit(VarDeclInit*) override;
             void visit_classstmt(ClassStmt*) override;
             void visit_block(Block*) override;
             void visit_expr(Expr*) override;
             void visit_symbol(Symbol*) override;
             void visit_stmt(Stmt*) override;
    };

};

#endif 
