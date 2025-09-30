#ifndef AST_H
#define AST_H

#include<string>
#include<vector>
#include<memory>

struct Stmt{
    virtual ~Stmt(){}
};

struct Assign:Stmt{
    virtual ~Assign(){}
};

struct Expr:Stmt{
    virtual ~Expr(){}
};

struct BinExpr:Expr{
    protected:
        BinExpr(Expr* left,Expr* right):left_(left),right_(right){}
        virtual ~BinExpr(){}
        Expr* left_;
        Expr* right_;
};

struct UnaryExpr:Expr{
    protected:
        UnaryExpr(Expr* expr):expr(expr){}
        virtual ~UnaryExpr(){}
        Expr* expr;
};

struct Add:public BinExpr{
    Add(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Sub:public BinExpr{
    Sub(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Mul:public BinExpr{
    Mul(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Div:public BinExpr{
    Div(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Equality:public BinExpr{
    Equality(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Less:public BinExpr{
    Less(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Lesser:public BinExpr{
    Lesser(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Greater:public BinExpr{
    Greater(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Less_eq:public BinExpr{
    Less_eq(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Great_eq:public BinExpr{
    Great_eq(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Bit_or:public BinExpr{
    Bit_or(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Bit_and:public BinExpr{
    Bit_and(Expr* left,Expr* right):BinExpr(left,right){}
};

struct Minus:UnaryExpr{
    Minus(Expr* expr):UnaryExpr(expr){}
};

struct Negate:UnaryExpr{
    Negate(Expr* expr):UnaryExpr(expr){}
};

struct Group:UnaryExpr{
    Group(Expr* expr):UnaryExpr(expr){}
};

template<typename T> struct Literal:Expr{
    Literal(T value):value(value){}
    Literal(const Literal&)=default;
    Literal(Literal&&)=default;
    Literal& operator=(const Literal&)=default;
    Literal& operator=(Literal&&)=default;
    T value;
    bool operator==(const Literal<T>& lhs)const noexcept {return value==lhs.value;}
};

using Double=Literal<double>;
using Int=Literal<int>;
using Str=Literal<std::string>;
using Bool=Literal<bool>;

using Stmts=std::vector<Stmt*>;

struct Program{
    Program(Stmts&& stmts):stmts(stmts){}
    Stmts stmts;
};

#endif 
