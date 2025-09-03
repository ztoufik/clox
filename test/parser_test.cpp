#include<memory>
#include <gtest/gtest.h>

#include "lexer.h"
#include "parser.h"

// Define a test fixture class
class ParserFixture : public ::testing::TestWithParam<std::tuple<std::string_view, Double>> {
};
// Define the test case using TEST_P
TEST_P(ParserFixture, Tokens) {
    auto expected_ast_node = std::get<1>(GetParam());

    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    auto parser=Parser(std::move(lexer));
    Program program=parser.parse();
    auto& stmt=program.stmts;

    ASSERT_EQ(stmt.size(),1);
    auto ast_node=std::dynamic_pointer_cast<Double>(stmt.at(0));
    ASSERT_TRUE(ast_node);
    ASSERT_EQ(expected_ast_node,*ast_node);
}


INSTANTIATE_TEST_SUITE_P(
        Literals,
        ParserFixture,
        ::testing::Values(
            std::tuple(std::string_view("3.4"),Double(3.3))
            )
        );
