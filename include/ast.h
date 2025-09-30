#ifndef AST_H
#define AST_H

#include<string>
#include<vector>

#include"lexer.h"

namespace tua{
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
        public:
            const Expr* get_left_expr()const noexcept {return left_;}
            const Expr* get_right_expr()const noexcept {return right_;}
        protected:
            BinExpr(Expr* left,Expr* right):left_(left),right_(right){}
            virtual ~BinExpr(){}
            Expr* left_;
            Expr* right_;
    };

    struct UnaryExpr:Expr{
        public:
            const Expr* get_expr()const noexcept {return expr;}
        protected:
            UnaryExpr(Expr* expr):expr(expr){}
            virtual ~UnaryExpr(){}
            Expr* expr;
    };

    template<TokenKind kind> struct Unar_Expr:UnaryExpr{
        Unar_Expr(Expr* expr):UnaryExpr(expr){}
    };

    template<TokenKind kind> struct Bin_Expr:BinExpr{
        Bin_Expr(Expr* left,Expr* right):BinExpr(left,right){}
    };

    using Add=Bin_Expr<TokenKind::PLUS>;
    using Sub=Bin_Expr<TokenKind::MINUS>;
    using Mul=Bin_Expr<TokenKind::STAR>;
    using Div=Bin_Expr<TokenKind::SLASH>;

    using Equality=Bin_Expr<TokenKind::EQUAL_EQUAL>;
    using Less=Bin_Expr<TokenKind::LESS>;
    using Great=Bin_Expr<TokenKind::GREATER>;
    using LessEq=Bin_Expr<TokenKind::LESS_EQUAL>;
    using GreatEq=Bin_Expr<TokenKind::GREATER_EQUAL>;

    using BitOr=Bin_Expr<TokenKind::BIT_OR>;
    using BitAnd=Bin_Expr<TokenKind::BIT_AND>;
    using RShift=Bin_Expr<TokenKind::BIT_RSHIFT>;
    using LShift=Bin_Expr<TokenKind::BIT_LSHIFT>;

    using Minus=Unar_Expr<TokenKind::MINUS>;
    using Negate=Unar_Expr<TokenKind::BANG>;
    using Group=Unar_Expr<TokenKind::LEFT_PAREN>;

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
};

#endif 
