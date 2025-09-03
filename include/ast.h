#ifndef AST_H
#define AST_H

#include<string>
#include<vector>
#include<memory>

struct Stmt{

    virtual ~Stmt(){}
};

struct Expr:Stmt{

};

template<typename T> struct Literal:Expr{
    Literal(T value):value(value){}
    bool operator==(const Literal<T>& lhs){return this->value==lhs.value;} const
    T value;
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
