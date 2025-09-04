#include<memory>

#include <gtest/gtest.h>

#include "lexer.h"
#include "parser.h"

// Define a test fixture class
class ParserFixture : public ::testing::TestWithParam<std::tuple<std::string_view, Int>> {
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
    auto ast_node=std::dynamic_pointer_cast<Int>(stmt.at(0));
    ASSERT_TRUE(ast_node);
    ASSERT_EQ(expected_ast_node,*ast_node);
}


INSTANTIATE_TEST_SUITE_P(
        Literals,
        ParserFixture,
        ::testing::Values(
            std::tuple(std::string_view("0"),Int(0)),
            std::tuple(std::string_view("1"),Int(1)),
            std::tuple(std::string_view("2"),Int(2)),
            std::tuple(std::string_view("3"),Int(3)),
            std::tuple(std::string_view("4"),Int(4)),
            std::tuple(std::string_view("5"),Int(5)),
            std::tuple(std::string_view("6"),Int(6)),
            std::tuple(std::string_view("7"),Int(7)),
            std::tuple(std::string_view("8"),Int(8)),
            std::tuple(std::string_view("9"),Int(9)),
            std::tuple(std::string_view("10"),Int(10)),
            std::tuple(std::string_view("15"),Int(15)),
            std::tuple(std::string_view("100"),Int(100)),
            std::tuple(std::string_view("999"),Int(999)),
            std::tuple(std::string_view("1234"),Int(1234)),
            std::tuple(std::string_view("9999"),Int(9999))
            )
        );
