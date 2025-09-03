#include<memory>
#include <gtest/gtest.h>

#include "lexer.h"
#include "parser.h"

TEST(Parser, simpleTest) {
    auto expected_ast_node=std::make_unique<Double>(3.3);
    auto parser=Parser(Lexer<std::string_view>("3.3"));
    Program program=parser.parse();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=std::dynamic_pointer_cast<Double>(stmt.at(0));
    ASSERT_TRUE(ast_node);
    ASSERT_EQ(ast_node->value,expected_ast_node->value);
}

