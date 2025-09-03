#ifndef PARSER_H
#define PARSER_H

#include<vector>

#include"lexer.h"
#include"ast.h"
 

template<typename T>class Parser{
    public:
        Parser(Lexer<T>&& lexer):lexer(std::move(lexer)){}
         Program parse(){
            auto stmt=std::vector<std::shared_ptr<Stmt>>{};
            stmt.emplace_back(std::make_unique<Double>(3.3));
            auto program=Program(std::move(stmt));
            return program;
        }

    private:
        Lexer<T> lexer;
};




#endif 
