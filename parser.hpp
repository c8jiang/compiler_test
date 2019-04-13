#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <fstream>
#include <iostream>
#include <stdexcept>

class syntax_error : std::exception
{
public:
    syntax_error() : std::exception() {}
    const char* what() const noexcept {
        return "syntax error";
    }
};

class parser
{
    static int lookahead;
    static std::fstream fs;

public:
    parser(const char* file);
    ~parser();

    void expr();
    void term();
    void match(int t);

private:
    static bool is_digit(char c);
};

#include "parser_impl.hpp"

#endif