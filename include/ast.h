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
            virtual ~BinExpr(){
                if(!left_ && !right_){
                    delete left_;
                    delete right_;
                }
            }
            Expr* left_;
            Expr* right_;
    };

    struct UnaryExpr:Expr{
        public:
            const Expr* get_expr()const noexcept {return expr_;}
        protected:
            UnaryExpr(Expr* expr):expr_(expr){}
            virtual ~UnaryExpr(){
                if(!expr_){
                    delete expr_;
                }
            }
            Expr* expr_;
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

    struct Symbol:Expr{
        Symbol(std::string&& ident):ident_(std::move(ident)){}
        Symbol(const Symbol&)=default;
        Symbol(Symbol&&)=default;
        Symbol& operator=(const Symbol&)=default;
        Symbol& operator=(Symbol&&)=default;
        std::string ident_;
        bool operator==(const Symbol lhs)const noexcept {return ident_==lhs.ident_;}
    };

    struct FctCall:Expr{
        FctCall(Expr* expr,std::vector<Expr*>&& exprs):expr_(expr),exprs_(std::move(exprs)){}
        FctCall(const FctCall&)=default;
        FctCall(FctCall&&)=default;
        FctCall& operator=(const FctCall&)=default;
        FctCall& operator=(FctCall&&)=default;
        Expr* expr_;
        std::vector<Expr*> exprs_;
    };

    using Stmts=std::vector<Stmt*>;

    struct Program{
        Program(Stmts&& stmts):stmts(stmts){}
        Stmts stmts;
    };
};

#endif 
