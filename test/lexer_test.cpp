#include <string>
#include <gtest/gtest.h>

#include "lexer.h"


using namespace tua;


TEST(LexerTest, simple_token) {
    auto lexer=Lexer<std::string_view>("(){}[],.;*/+-===!!=|&");
    auto tokens=std::vector{
            Token(TokenKind::LEFT_PAREN,0),
            Token(TokenKind::RIGHT_PAREN,0),
            Token(TokenKind::LEFT_BRACE,0),
            Token(TokenKind::RIGHT_BRACE,0),
            Token(TokenKind::LEFT_BRACKET,0),
            Token(TokenKind::RIGHT_BRACKET,0),
            Token(TokenKind::COMMA,0),
            Token(TokenKind::DOT,0),
            Token(TokenKind::SEMICOLON,0),
            Token(TokenKind::STAR,0),
            Token(TokenKind::SLASH,0),
            Token(TokenKind::PLUS,0),
            Token(TokenKind::MINUS,0),
            Token(TokenKind::EQUAL_EQUAL,0),
            Token(TokenKind::EQUAL,0),
            Token(TokenKind::BANG,0),
            Token(TokenKind::BANG_EQUAL,0),
            Token(TokenKind::BIT_OR,0),
            Token(TokenKind::BIT_AND,0),
            Token(TokenKind::Eof,0),
    };
    for(const auto& expected_token: tokens){
        auto token=lexer.get_token();
        EXPECT_EQ(expected_token,token);
    }
}

// Define a test fixture class
class LexerFixture : public ::testing::TestWithParam<std::tuple<std::string, Token>> {
};
// Define the test case using TEST_P
TEST_P(LexerFixture, LexerTest) {
    std::string src = std::get<0>(GetParam());
    auto expected_token = std::get<1>(GetParam());


    auto lexer(Lexer<std::string>(std::move(src)));
    auto token=lexer.get_token();
    ASSERT_EQ(expected_token, token);
}


INSTANTIATE_TEST_SUITE_P(
        linetest,
        LexerFixture,
        ::testing::Values(
            std::tuple(std::string("\n"),Token(TokenKind::Eof,1))
            )
        );

INSTANTIATE_TEST_SUITE_P(
        NumericToken,
        LexerFixture,
        ::testing::Values(
            std::tuple(std::string("3"),Token(TokenKind::INT,"3",0)),
            std::tuple(std::string("33"),Token(TokenKind::INT,"33",0)),
            std::tuple(std::string("313"),Token(TokenKind::INT,"313",0)),
            std::tuple(std::string("00001"),Token(TokenKind::INT,"00001",0)),
            std::tuple(std::string("0.1"),Token(TokenKind::DOUBLE,"0.1",0)),
            std::tuple(std::string("0.2101"),Token(TokenKind::DOUBLE,"0.2101",0)),
            std::tuple(std::string("1.1"),Token(TokenKind::DOUBLE,"1.1",0)),
            std::tuple(std::string("13.3"),Token(TokenKind::DOUBLE,"13.3",0)),
            std::tuple(std::string("\n0.1"),Token(TokenKind::DOUBLE,"0.1",1)),
            std::tuple(std::string("\n\n0.2101"),Token(TokenKind::DOUBLE,"0.2101",2))

            )
        );

INSTANTIATE_TEST_SUITE_P(
        StringToken,
        LexerFixture,
        ::testing::Values(
            std::tuple(std::string("\"toufik\""),Token(TokenKind::STRING,"toufik",0)),
            std::tuple(std::string("\"zoubir\n\""),Token(TokenKind::STRING,"zoubir\n",0))
            )
        );

INSTANTIATE_TEST_SUITE_P(
        identkeyword,
        LexerFixture,
        ::testing::Values(
            std::tuple(std::string("toufik"),Token(TokenKind::IDENT,"toufik",0)),
            std::tuple(std::string("if"),Token(TokenKind::IF,0)),
            std::tuple(std::string("class"),Token(TokenKind::CLASS,0)),
            std::tuple(std::string("and") ,Token(TokenKind::AND,0)),
            std::tuple(std::string("else") ,Token(TokenKind::ELSE,0)),
            std::tuple(std::string("false") ,Token(TokenKind::FALSE,0)),
            std::tuple(std::string("fun") ,Token(TokenKind::FUN,0)),
            std::tuple(std::string("for") ,Token(TokenKind::FOR,0)),
            std::tuple(std::string("if") ,Token(TokenKind::IF,0)),
            std::tuple(std::string("nil") ,Token(TokenKind::NIL,0)),
            std::tuple(std::string("or") ,Token(TokenKind::OR,0)),
            std::tuple(std::string("return") ,Token(TokenKind::RETURN,0)),
            std::tuple(std::string("super") ,Token(TokenKind::SUPER,0)),
            std::tuple(std::string("this") ,Token(TokenKind::THIS,0)),
            std::tuple(std::string("true") ,Token(TokenKind::TRUE,0)),
            std::tuple(std::string("var") ,Token(TokenKind::VAR,0)),
            std::tuple(std::string("while") ,Token(TokenKind::WHILE,0))
            )
        );

INSTANTIATE_TEST_SUITE_P(
        comment,
        LexerFixture,
        ::testing::Values(
            std::tuple(std::string("//toufik"),Token(TokenKind::Eof,0)),
            std::tuple(std::string("\n//toufik"),Token(TokenKind::Eof,1))
            )
        );
