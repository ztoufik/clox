#include"lexer.h"
#include <cassert>
#include<sstream>
#include<iostream>


const std::unordered_map<std::string,TokenKind> Lexer::key_words={
    {std::string("class"),TokenKind::CLASS},
    {std::string("and") ,TokenKind::AND},
    {std::string("else") ,TokenKind::ELSE},
    {std::string("false") ,TokenKind::FALSE},
    {std::string("fun") ,TokenKind::FUN},
    {std::string("for") ,TokenKind::FOR},
    {std::string("if") ,TokenKind::IF},
    {std::string("nil") ,TokenKind::NIL},
    {std::string("or") ,TokenKind::OR},
    {std::string("return") ,TokenKind::RETURN},
    {std::string("super") ,TokenKind::SUPER},
    {std::string("this") ,TokenKind::THIS},
    {std::string("true") ,TokenKind::TRUE},
    {std::string("var") ,TokenKind::VAR},
    {std::string("while") ,TokenKind::WHILE},
};

Token Lexer::tokenize_ident(){
    std::stringstream ss;
    while(std::isalnum(src[current_char])){
        ss<<src[current_char];
        current_char++;
    }
    std::cout<<"***********************"<<std::endl;
    //assert(Lexer::key_words.size()==15);
    //if(Lexer::key_words.find("if")==Lexer::key_words.end())
    //    std::cout<<"no if key word found"<<std::endl;
    //else
    //    std::cout<<"if key word found"<<std::endl;
    //std::cout<<"***********************"<<std::endl;
    auto word=std::move(ss.str());
    auto tkind_ptr=Lexer::key_words.find(word);
    std::cout<<(tkind_ptr==Lexer::key_words.end())<<std::endl;
    if(tkind_ptr==Lexer::key_words.end())
        return Token(TokenKind::IDENTIFIER,word,current_line);
    return Token(tkind_ptr->second,current_line);
}

void Lexer::tokenize_comment(){

}

Token Lexer::tokenize_numeric(){
    std::stringstream ss;
    TokenKind tkind=TokenKind::INT;
    bool digit_pt=false;
    while(std::isdigit(src[current_char]) || src[current_char]=='.') {
        if (src[current_char]=='.'){
            if (digit_pt) {
                break;
            }
            else{
                digit_pt=true;
                tkind=TokenKind::NUMBER;
                ss<<src[current_char];
            }
        }
        else{
            ss<<src[current_char];
        }
    current_char++;
    }
    return Token(tkind,ss.str(),current_line);
}

Token Lexer::tokenize_string(){
    std::stringstream ss;
    while(src[current_char]!='"') {
        ss<<src[current_char];
        current_char++;
    };
    current_char++;
    return Token(TokenKind::STRING,ss.str(),current_line);
}

Token Lexer::get_token(){

    while(isspace(src[current_char])){
        if(src[current_char]=='\n') current_line++; 
        current_char++;
    };

    switch(src[current_char]){
        case '(':{current_char++;return Token(TokenKind::LEFT_PAREN,current_line);}
        case ')':{current_char++;return Token(TokenKind::RIGHT_PAREN,current_line);}
        case '[':{current_char++;return Token(TokenKind::LEFT_BRACE,current_line);}
        case ']':{current_char++;return Token(TokenKind::RIGHT_BRACE,current_line);}
        case ',':{current_char++;return Token(TokenKind::COMMA,current_line);}
        case ';':{current_char++;return Token(TokenKind::SEMICOLON,current_line);}
        case '.':{current_char++;return Token(TokenKind::DOT,current_line);}
        case '+':{current_char++;return Token(TokenKind::PLUS,current_line);}
        case '-':{current_char++;return Token(TokenKind::MINUS,current_line);}
        case '*':{current_char++;return Token(TokenKind::STAR,current_line);}
        case '!':{current_char++;return Token(TokenKind::BANG,current_line);}
        case '"':{current_char++;return tokenize_string();}
        case '/':{
                     current_char++;
                     if (src[current_char]!='/' ){return Token(TokenKind::SLASH,current_line);}
                     current_char++;
                     while(src[current_char]!='\n'&& current_char < src.length()) {current_char++;}

                 }
        default: break;
    };

    if(std::isalpha(src[current_char])){
        return tokenize_ident();
    }
    if(std::isdigit(src[current_char])){
        return tokenize_numeric();
    }
    return Token(TokenKind::Eof,current_line);
}
