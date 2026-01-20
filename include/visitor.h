#ifndef VISITOR_H
#define VISITOR_H


class Visitor{
    public:
    virtual void visit_program()=0;
    virtual void visit_accexpr()=0;
    virtual void visit_fctexpr()=0;
    virtual void visit_vardeclinit()=0;
    virtual void visit_fctcall()=0;
    virtual void visit_assign()=0;
    virtual void visit_literal()=0;
    virtual void visit_binexpr()=0;
    virtual void visit_bin_expr()=0;
    virtual void visit_unaryexpr()=0;
    virtual void visit_unarexpr()=0;
    virtual void visit_return()=0;
    virtual void visit_whilestmt()=0;
    virtual void visit_ifelse()=0;
    virtual void visit_fctdecl()=0;
    virtual void visit_classstmt()=0;
    virtual void visit_block()=0;
    virtual void visit_expr()=0;
    virtual void visit_symbol()=0;
    virtual void visit_stmt()=0;
};



#endif 
