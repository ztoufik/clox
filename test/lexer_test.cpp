#include <gtest/gtest.h>

#include "lexer.h"


// Define a test fixture class
class LexerFixture : public ::testing::TestWithParam<std::tuple<std::string, Token>> {
};
// Define the test case using TEST_P
TEST_P(LexerFixture, Tokens) {
    auto src = std::get<0>(GetParam());
    auto expected_token = std::get<1>(GetParam());

    auto lexer=Lexer(src);
    auto token=lexer.get_token();
    ASSERT_EQ(expected_token.value, token.value); 
    ASSERT_EQ(expected_token.kind, token.kind);
    ASSERT_EQ(expected_token.line, token.line);
}

INSTANTIATE_TEST_SUITE_P(
        MyValues,
        LexerFixture,
        ::testing::Values(
            std::tuple(std::string(""),Token(TokenKind::Eof,"",0)),
            std::tuple(std::string("\n"),Token(TokenKind::Eof,"",1)),
            std::tuple(std::string("\r"),Token(TokenKind::Eof,"",1)),
            std::tuple(std::string(")"),Token(TokenKind::RIGHT_PAREN,"",0)),
            std::tuple(std::string("("),Token(TokenKind::LEFT_PAREN,"",0)),
            std::tuple(std::string("["),Token(TokenKind::LEFT_BRACE,"",0)),
            std::tuple(std::string("]"),Token(TokenKind::RIGHT_BRACE,"",0)),
            std::tuple(std::string(","),Token(TokenKind::COMMA,"",0)),
            std::tuple(std::string("."),Token(TokenKind::DOT,"",0)),
            std::tuple(std::string(";"),Token(TokenKind::SEMICOLON,"",0)),
            std::tuple(std::string("*"),Token(TokenKind::STAR,"",0)),
            std::tuple(std::string("/"),Token(TokenKind::SLASH,"",0)),
            std::tuple(std::string("+"),Token(TokenKind::PLUS,"",0)),
            std::tuple(std::string("-"),Token(TokenKind::MINUS,"",0)),
            std::tuple(std::string("!"),Token(TokenKind::BANG,"",0))
            )
        );
