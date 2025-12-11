#ifndef AST_H
#define AST_H

#include<string>
#include<vector>

#include"lexer.h"

namespace tua{

    using Type=std::string;

    struct Stmt{
        virtual ~Stmt(){}
    };

    using Stmts=std::vector<Stmt*>;

    struct Expr:Stmt{
        virtual ~Expr(){}
    };

    struct VarDeclInit:Stmt{
        VarDeclInit(std::string&& ident,Expr* value,Type* type):ident_(std::move(ident)),value_(value),type_(type){}
        const Expr* get_value()const noexcept {return value_;}
        const Type* get_type()const noexcept {return type_;}
        const std::string& get_ident()const noexcept {return ident_;}
        virtual ~VarDeclInit(){
            if(!value_) delete value_;
            if(!type_) delete type_;
        }
        protected:
            Expr* value_;
            Type* type_;
            std::string ident_;
    };

    struct Block:Stmt{
        public:
            Block(Stmts&& stmts):stmts(std::move(stmts)){}
            const Stmts& get_stmts() const noexcept {return stmts;}
            virtual ~Block(){}
        protected:
            Stmts stmts;
    };

    struct IfElse:Stmt{
        public:
            const Stmt* get_if_stmt()const noexcept {return if_;}
            const Stmt* get_else_stmt()const noexcept {return else_;}
            const Expr* get_condtion_expr()const noexcept {return condition_;}
            IfElse(Expr* condition,Block* left,Block* right):condition_(condition),if_(left),else_(right){}
            virtual ~IfElse(){
                if(!if_){
                    delete if_;
                }

                if(!else_){
                    delete else_;
                }

                if(!condition_){
                    delete condition_;
                }
            }
        protected:
            Expr* condition_;
            Block* if_;
            Block* else_;
    };

    struct WhileStmt:Stmt{
            const Stmt* get_block_stmt()const noexcept {return block_;}
            const Expr* get_condtion_expr()const noexcept {return condition_;}
            WhileStmt(Expr* condition,Block* block):condition_(condition),block_(block){}
            virtual ~WhileStmt(){
                if(!block_ ){
                    delete block_;
                }
                if(!condition_){
                    delete condition_;
                }
            }
        protected:
            Expr* condition_;
            Block* block_;
    };

    struct Return:Stmt{
            const Expr* get_value()const noexcept {return value_;}
            Return(Expr* value):value_(value){}
            virtual ~Return(){
                if(!value_){
                    delete value_;
                }
            }
        protected:
            Expr* value_;
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

    struct Assign:Expr{
        Assign(std::string&& ident,Expr* value):ident_(std::move(ident)),value_(value){}
        Assign(const Assign&)=default;
        Assign(Assign&&)=default;
        Assign& operator=(const Assign&)=default;
        Assign& operator=(Assign&&)=default;
        std::string ident_;
        Expr* value_;
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


    struct Program{
        Program(Stmts&& stmts):stmts(stmts){}
        Stmts stmts;
    };
};

#endif 
