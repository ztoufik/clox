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
    BinExpr(Expr&& left,Expr&& right):left_(std::move(left)),right_(std::move(right)){}
    virtual ~BinExpr(){}
    Expr left_,right_;
};

struct UnaryExpr:Expr{
    protected:
        UnaryExpr(Expr&& expr):expr(std::move(expr)){}
        virtual ~UnaryExpr(){}
        Expr expr;
};

struct Add:public BinExpr{
    Add(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Sub:public BinExpr{
    Sub(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Mul:public BinExpr{
    Mul(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Div:public BinExpr{
    Div(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Equality:public BinExpr{
    Equality(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Less:public BinExpr{
    Less(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Lesser:public BinExpr{
    Lesser(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Greater:public BinExpr{
    Greater(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Less_eq:public BinExpr{
    Less_eq(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Great_eq:public BinExpr{
    Great_eq(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Bit_or:public BinExpr{
    Bit_or(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Bit_and:public BinExpr{
    Bit_and(Expr&& left,Expr&& right):BinExpr(std::move(left),std::move(right)){}
};

struct Minus:UnaryExpr{
    Minus(Expr&& expr):UnaryExpr(std::move(expr)){}
};

struct Negate:UnaryExpr{
    Negate(Expr&& expr):UnaryExpr(std::move(expr)){}
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

using Stmts=std::vector<std::shared_ptr<Stmt>>;

struct Program{
    Program(Stmts&& stmts):stmts(std::move(stmts)){}
    Stmts stmts;
};



#endif 
