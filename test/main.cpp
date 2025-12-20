#include<iostream>
#include<optional>
#include<fstream>
#include<string>

#include "lexer.h"
#include "parser.h"

std::optional<std::string> readfilecontent(std::string_view path){
    std::string myText;
    std::string ouput;

    std::ifstream MyReadfile(path.data());
    if(!MyReadfile.is_open()){
        return std::nullopt;
    }

    while (getline (MyReadfile, myText)) {
        std::cout<<myText<<std::endl;
        ouput+=myText+'\n';
    }
    MyReadfile.close();
    return ouput;
}

int main(){
    auto op_source=readfilecontent("C:\\Users\\toufik\\Documents\\cpp_projects\\lox_cpp\\build\\test\\source.txt");
    if(!op_source){
        std::cout<<"no source code available"<<std::endl;
        return 1;
    }
    auto source=std::move(op_source.value());
    auto lexer=tua::Lexer(std::move(source));
    auto parser=tua::Parser(lexer);
    auto output=parser.parse();
    if(!output){
        std::cout<<output.error()->_msg<<std::endl;
        std::cout<<output.error()->_lnum<<std::endl;
        return 1;
    }
    else{
        std::cout<<"OK"<<std::endl;
        return 1;
    }
}

