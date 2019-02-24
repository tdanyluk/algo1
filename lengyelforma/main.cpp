#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>

bool isOperand(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '=';
}

bool isLeftToRight(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char c)
{
    switch(c)
    {
        case '=': return 0;
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default:
        std::cout << "Program hiba" << std::endl;
        std::exit(1);
    }
}

int main()
{
    std::cout << "Irj be soronkent egy arithmetikai kifejezest, pl: a+b. Ures sor=kilepes." << std::endl;
    std::string line;
    while(std::getline(std::cin, line) && !line.empty())
    {
        std::stack<char> s;

        for(char c: line)
        {
            if(isOperand(c))
            {
                std::cout << c;
            }
            else if(c == '(')
            {
                s.push(c);
            }
            else if(c == ')')
            {
                while(!s.empty() && s.top() != '(')
                {
                    std::cout << s.top();
                    s.pop();
                }
                if(s.empty())
                {
                    std::cout << "Zarojelezesi hiba!" << std::endl;
                    break;
                }
                s.pop();
            }
            else if(isOperator(c))
            {
                if(isLeftToRight(c))
                {
                    while(!s.empty() && s.top() != '(' && precedence(s.top()) >= precedence(c))
                    {
                        std::cout << s.top();
                        s.pop();
                    }
                }
                else
                {
                    while(!s.empty() && s.top() != '(' && precedence(s.top()) > precedence(c))
                    {
                        std::cout << s.top();
                        s.pop();
                    }
                }
                s.push(c);

            }
            else if(c != ' ')
            {
                std::cout << "Ismeretlen karakter: " << c << std::endl;
                break;
            }
        }
        while(!s.empty())
        {
            std::cout << s.top();
            s.pop();
        }
        std::cout << std::endl;
    }
    return 0;
}
