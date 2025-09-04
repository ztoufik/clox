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
