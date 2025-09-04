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
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program program=rslt.value();
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
            std::tuple(std::string_view("0"),Double(0)),
            std::tuple(std::string_view("1"),Double(1)),
            std::tuple(std::string_view("2"),Double(2)),
            std::tuple(std::string_view("3"),Double(3)),
            std::tuple(std::string_view("4"),Double(4)),
            std::tuple(std::string_view("5"),Double(5)),
            std::tuple(std::string_view("6"),Double(6)),
            std::tuple(std::string_view("7"),Double(7)),
            std::tuple(std::string_view("8"),Double(8)),
            std::tuple(std::string_view("9"),Double(9)),
            std::tuple(std::string_view("10"),Double(10)),
            std::tuple(std::string_view("15"),Double(15)),
            std::tuple(std::string_view("100"),Double(100)),
            std::tuple(std::string_view("999"),Double(999)),
            std::tuple(std::string_view("1234"),Double(1234)),
            std::tuple(std::string_view("9999"),Double(9999))
            )
        );
