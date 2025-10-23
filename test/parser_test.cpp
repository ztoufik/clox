
#include <gtest/gtest.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"

using namespace tua;


//Define a test fixture class
class ParseIntFixt : public ::testing::TestWithParam<std::tuple<std::string_view, Int>> {
};
 
// Define the test case using TEST_P
TEST_P(ParseIntFixt, ParserTest) {
    auto expected_ast_node = std::get<1>(GetParam());

    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(std::move(lexer));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Int*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
    ASSERT_EQ(expected_ast_node,*ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseIntTest,
        ParseIntFixt,
        ::testing::Values(
            std::tuple(std::string_view("0;"),Int(0)),
            std::tuple(std::string_view("1;"),Int(1)),
            std::tuple(std::string_view("2;"),Int(2)),
            std::tuple(std::string_view("3;"),Int(3)),
            std::tuple(std::string_view("4;"),Int(4)),
            std::tuple(std::string_view("5;"),Int(5)),
            std::tuple(std::string_view("6;"),Int(6)),
            std::tuple(std::string_view("7;"),Int(7)),
            std::tuple(std::string_view("8;"),Int(8)),
            std::tuple(std::string_view("9;"),Int(9)),
            std::tuple(std::string_view("10;"),Int(10)),
            std::tuple(std::string_view("15;"),Int(15)),
            std::tuple(std::string_view("100;"),Int(100)),
            std::tuple(std::string_view("999;"),Int(999)),
            std::tuple(std::string_view("1234;"),Int(1234)),
            std::tuple(std::string_view("9999;"),Int(9999))
            )
        );

class ParseDoubleFixt : public ::testing::TestWithParam<std::tuple<std::string_view, Double>> {
};
// Define the test case using TEST_P
TEST_P(ParseDoubleFixt, ParserTest) {
    auto expected_ast_node = std::get<1>(GetParam());

    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(std::move(lexer));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Double*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
    ASSERT_EQ(expected_ast_node,*ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseDouble,
        ParseDoubleFixt,
        ::testing::Values(
            std::tuple(std::string_view("0.0;"),Double(0.0)),
            std::tuple(std::string_view("1.1;"),Double(1.1)),
            std::tuple(std::string_view("2.2;"),Double(2.2)),
            std::tuple(std::string_view("3.3;"),Double(3.3)),
            std::tuple(std::string_view("4.4;"),Double(4.4)),
            std::tuple(std::string_view("5.5;"),Double(5.5)),
            std::tuple(std::string_view("6.6;"),Double(6.6)),
            std::tuple(std::string_view("7.7;"),Double(7.7)),
            std::tuple(std::string_view("8.8;"),Double(8.8)),
            std::tuple(std::string_view("9.9;"),Double(9.9)),
            std::tuple(std::string_view("1.100;"),Double(1.100)),
            std::tuple(std::string_view("1.155;"),Double(1.155)),
            std::tuple(std::string_view("1.10000;"),Double(1.10000)),
            std::tuple(std::string_view("9.99999;"),Double(9.99999)),
            std::tuple(std::string_view("1.1234234;"),Double(1.1234234)),
            std::tuple(std::string_view("9.9999999;"),Double(9.9999999))
            )
        );

class ParseStrFixt : public ::testing::TestWithParam<std::tuple<std::string_view, Str>> {
};
// Define the test case using TEST_P
TEST_P(ParseStrFixt, ParserTest) {
    auto expected_ast_node = std::get<1>(GetParam());

    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(std::move(lexer));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Str*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
    ASSERT_EQ(expected_ast_node,*ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseStr,
        ParseStrFixt,
        ::testing::Values(
            std::tuple(std::string_view("\"0.0\";"),Str("0.0")),
            std::tuple(std::string_view("\"1.1\";"),Str("1.1")),
            std::tuple(std::string_view("\"2.2\";"),Str("2.2")),
            std::tuple(std::string_view("\"3.3\";"),Str("3.3")),
            std::tuple(std::string_view("\"4.4\";"),Str("4.4")),
            std::tuple(std::string_view("\"5.5\";"),Str("5.5")),
            std::tuple(std::string_view("\"6.6\";"),Str("6.6")),
            std::tuple(std::string_view("\"7.7\";"),Str("7.7")),
            std::tuple(std::string_view("\"8.8\";"),Str("8.8")),
            std::tuple(std::string_view("\"9.9\";"),Str("9.9")),
            std::tuple(std::string_view("\"1.100\";"),Str("1.100")),
            std::tuple(std::string_view("\"1.155\";"),Str("1.155")),
            std::tuple(std::string_view("\"1.10000\";"),Str("1.10000")),
            std::tuple(std::string_view("\"9.99999\";"),Str("9.99999")),
            std::tuple(std::string_view("\"1.1234234\";"),Str("1.1234234")),
            std::tuple(std::string_view("\"9.9999999\";"),Str("9.9999999"))
            )
        );

class ParseBoolFixt : public ::testing::TestWithParam<std::tuple<std::string_view, Bool>> {
};

TEST_P(ParseBoolFixt, ParserTest) {
    auto expected_ast_node = std::get<1>(GetParam());

    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(Parser(std::move(lexer)));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Bool*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
    ASSERT_EQ(expected_ast_node,*ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseBool,
        ParseBoolFixt,
        ::testing::Values(
            std::tuple(std::string_view("true;"),Bool(true)),
            std::tuple(std::string_view("false;"),Bool(false))
            )
        );

class ParseGroupFixt : public ::testing::TestWithParam<std::tuple<std::string_view>> {
};

TEST_P(ParseGroupFixt, ParserTest) {
    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(Parser(std::move(lexer)));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Group*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseGroup,
        ParseGroupFixt,
        ::testing::Values(
            std::tuple(std::string_view("(3);")),
            std::tuple(std::string_view("(false);")),
            std::tuple(std::string_view("((3+3));"))
            )
        );

class ParseAddFixt : public ::testing::TestWithParam<std::tuple<std::string_view>> {
};

TEST_P(ParseAddFixt, ParserTest) {
    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(Parser(std::move(lexer)));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Add*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseAdd,
        ParseAddFixt,
        ::testing::Values(
            std::tuple(std::string_view("3+3;")),
            std::tuple(std::string_view("3+0.3*4;")),
            std::tuple(std::string_view("(3)+0;"))
            )
        );

class ParseSubFixt : public ::testing::TestWithParam<std::tuple<std::string_view>> {
};

TEST_P(ParseSubFixt, ParserTest) {
    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(Parser(std::move(lexer)));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Sub*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseSub,
        ParseSubFixt,
        ::testing::Values(
            std::tuple(std::string_view("3-3;")),
            std::tuple(std::string_view("3-0.3*4;")),
            std::tuple(std::string_view("(3)-0;"))
            )
        );

class ParseMulFixt : public ::testing::TestWithParam<std::tuple<std::string_view>> {
};

TEST_P(ParseMulFixt, ParserTest) {
    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(Parser(std::move(lexer)));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Mul*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseMul,
        ParseMulFixt,
        ::testing::Values(
            std::tuple(std::string_view("3*3;")),
            std::tuple(std::string_view("(3+4)*3;")),
            std::tuple(std::string_view("3*3*0;")),
            std::tuple(std::string_view("3*3/0;"))
            )
        );

class ParseDivFixt : public ::testing::TestWithParam<std::tuple<std::string_view>> {
};

TEST_P(ParseDivFixt, ParserTest) {
    auto src = std::get<0>(GetParam());
    auto lexer=Lexer<std::string_view>(std::move(src));
    Parser parser(Parser(std::move(lexer)));
    auto rslt=parser.parse();
    ASSERT_TRUE(rslt);
    Program& program=rslt.value();
    auto& stmt=program.stmts;
    ASSERT_EQ(stmt.size(),1);
    auto ast_node=dynamic_cast<Div*>(stmt.at(0));
    ASSERT_TRUE(ast_node);
}

INSTANTIATE_TEST_SUITE_P(
        ParseDiv,
        ParseDivFixt,
        ::testing::Values(
            std::tuple(std::string_view("3/3;")),
            std::tuple(std::string_view("(3+4)/3;")),
            std::tuple(std::string_view("3/3*0;"))
            )
        );
