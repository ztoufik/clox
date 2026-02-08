#include<optional>

#include "ast.h"
#include "checker.h"

namespace tua{
    const std::optional<const TType> TypeFrame::get_type(const std::string& ident) const{
        auto value=types.find(ident);
        if(value==types.end()){
            return value->second;
        }
        return {}; 
    }
    void TypeFrame::add_type(const std::string& ident, const TType& type){
        types.insert(std::make_pair(ident,type));
    }

    const std::optional<const TVar> VarFrame::get_var(const std::string& ident) const{
        auto value=vars.find(ident);
        if(value==vars.end()){
            return value->second;
        }
        return {}; 
    }
    void VarFrame::add_var(const std::string& ident, const TVar& var){
        vars.insert(std::make_pair(ident,var));
    }

    void AssemblerVisitor::visit_program(Program* program) {
        for(const auto& stmt: program->stmts){
            stmt->visit(this);
        }
    }
    void AssemblerVisitor::visit_accexpr(AccExpr*){}
    void AssemblerVisitor::visit_fctexpr(FctExpr*){}
    void AssemblerVisitor::visit_fctcall(FctCall*){}
    void AssemblerVisitor::visit_assign(Assign*){}
    void AssemblerVisitor::visit_literal(Double*){}
    void AssemblerVisitor::visit_literal(Int*){}
    void AssemblerVisitor::visit_literal(Str*){}
    void AssemblerVisitor::visit_literal(Bool*){}
    void AssemblerVisitor::visit_binexpr(BinExpr*){}
    void AssemblerVisitor::visit_bin_expr(Add*){}
    void AssemblerVisitor::visit_bin_expr(Sub*){}
    void AssemblerVisitor::visit_bin_expr(Mul*){}
    void AssemblerVisitor::visit_bin_expr(Div*){}
    void AssemblerVisitor::visit_bin_expr(Less*){}
    void AssemblerVisitor::visit_bin_expr(Great*){}
    void AssemblerVisitor::visit_bin_expr(LessEq*){}
    void AssemblerVisitor::visit_bin_expr(GreatEq*){}
    void AssemblerVisitor::visit_bin_expr(BitOr*){}
    void AssemblerVisitor::visit_bin_expr(BitAnd*){}
    void AssemblerVisitor::visit_bin_expr(RShift*){}
    void AssemblerVisitor::visit_bin_expr(LShift*){}
    void AssemblerVisitor::visit_unaryexpr(UnaryExpr*){}
    void AssemblerVisitor::visit_unar_expr(Minus*){}
    void AssemblerVisitor::visit_unar_expr(Negate*){}
    void AssemblerVisitor::visit_unar_expr(Group*){}
    void AssemblerVisitor::visit_return(Return*){}
    void AssemblerVisitor::visit_whilestmt(WhileStmt*){}
    void AssemblerVisitor::visit_ifelse(IfElse*){}
    void AssemblerVisitor::visit_fctdecl(FctDecl*){}
    void AssemblerVisitor::visit_vardeclinit(VarDeclInit*){}
    void AssemblerVisitor::visit_classstmt(ClassStmt*){}
    void AssemblerVisitor::visit_block(Block*){}
    void AssemblerVisitor::visit_expr(Expr*){}
    void AssemblerVisitor::visit_symbol(Symbol*){}
    void AssemblerVisitor::visit_stmt(Stmt*){}

};
