#include "parser.hpp"
#include "basic.hpp"

int parser::lookahead = 0;
std::fstream parser::fs;

parser::parser(const char* file)
{
    fs.open(file, std::ios_base::in);
    if (!fs.is_open())
        throw std::exception();

    lookahead = fs.get();
}

parser::~parser()
{
}

void parser::expr()
{
    term();
    while (true)
    {
        if (lookahead == '+')
        {
            match('+');
            term();
            std::cout << '+';
        }
        else if (lookahead == '-')
        {
            match('-');
            term();
            std::cout << '-';
        }
        else
            return;
    }
}

void parser::term()
{
    if (is_digit((char)lookahead))
    {
        std::cout << (char)lookahead;
        match(lookahead);
    }
    else
        throw syntax_error();
}

void parser::match(int t)
{
    if (lookahead == t)
        lookahead = fs.get();
    else
        throw syntax_error();
}

// bool parser::is_digit(char c) { return '0' <= c && c <= '9'; }