#include "parser.hpp"

int main()
{
    parser parse("code.txt");
    parse.expr();
    std::cout << '\n';

    return 0;
}