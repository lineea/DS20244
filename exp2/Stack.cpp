#define _CRT_SECURE_NO_WARNINGS
#ifndef M_PI  
#define M_PI 3.14159265358979323846  
#endif
#include "Vector.hpp"
#include <stdio.h> 
#include <string.h> 
#include <iostream>  
#include <string>  
#include <cctype>  
#include <cmath>  
#include <stdexcept>  
#include <sstream> 
#include <unordered_map>

template <typename T> class Stack : public Vector<T> {
public:
    void push(T const& e) { Vector<T>::insert(Vector<T>::size(), e); }
    T pop() { return Vector<T>::remove(Vector<T>::size() - 1); }
    T& top() { return(*this)[Vector<T>::size() - 1]; }
};

#define N_OPTR 11 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE, SIN, LOG,} Operator; //运算符集合
//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符

const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [当前]
    /* |-------------------- 当 前 运 算 符 --------------------| */
             /* +     -    *    /    ^    !    (    )   \0   sin  log*/
    /* --  + */ '>', '>', '<', '<', '<', '<', '<', '>', '>', '<', '<',
    /* |   - */ '>', '>', '<', '<', '<', '<', '<', '>', '>', '<', '<',
    /* 栈  * */ '>', '>', '>', '>', '<', '<', '<', '>', '>', '<', '<',
    /* 顶  / */ '>', '>', '>', '>', '<', '<', '<', '>', '>', '<', '<',
    /* 运  ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>', '>', '>',
    /* 算  ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>',
    /* 符  ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ', '<', '<',
    /* |   ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* -- \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '=', '<', '<',
    /* --sin */ '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '<',
    /* --log */ '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '<'
};

Operator optr2rank(char op)
{
    if (op == '+') return ADD;
    else if (op == '-') return SUB;
    else if (op == '*') return MUL;
    else if (op == '/') return DIV;
    else if (op == '^') return POW;
    else if (op == '!') return FAC;
    else if (op == '(') return L_P;
    else if (op == ')') return R_P;
    else if (op == 's' || op == 'c' || op == 't') return SIN; // 处理多个字符映射到 SIN  
    else if (op == 'l') return LOG;
    else if (op == '\0') return EOE;
    else  exit(-1); // 未知字符  
   
} 

char orderBetween(char op1, char op2) //比较两个运算符乀间癿优先级
{
    return pri[optr2rank(op1)][optr2rank(op2)];
}

void convert(Stack<char>& S, __int64 n, int base)
{
    static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    while (n > 0)
    {
        int remainder = (int)(n % base);
        S.push(digit[remainder]);
        n /= base;
    }
}

bool paren(const char exp[], Rank lo, Rank hi) { //表达式括号匹配检查，可兼顾三种括号
    Stack<char> S; //使用栈记录已发现但尚未匹配的左括号
    for (Rank i = lo; i <= hi; i++) /* 逐一检查当前字符 */
        switch (exp[i]) { //左括号直接进栈；右括号若与栈顶失配，则表达式必不匹配
        case '(': case '[': case '{': S.push(exp[i]); break;
        case ')': if ((S.empty()) || ('(' != S.pop())) return false; break;
        case ']': if ((S.empty()) || ('[' != S.pop())) return false; break;
        case '}': if ((S.empty()) || ('{' != S.pop())) return false; break;
        default: break; //非括号字符一律忽略
        }
    return S.empty(); //最终栈空，当且仅当匹配
}

bool paren(const char exp[])
{
    return paren(exp, 0, strlen(exp) - 1);
}

// 判断字符是否为数字  
bool isDigit(char c) {
    return isdigit(c);
}

void append(char*& R, float a)
{
    if (R == nullptr)
    {
        R = new char[1];
        R[0] = '\0';
    }
    char* b = new char[50];
    sprintf(b, "%.2f", a); // 将float类型转化为char型
    size_t ol = strlen(R);
    size_t nl = ol + strlen(b) + 1;
    char* nS = new char[nl];
    strcpy(nS, R);
    strcat(nS, b);
    R = nS;
}
// 将字符追加到字符串中（考虑字符串的增长）  
void append(char*& RPN, char c) {
    static int len = 0; // 静态变量，用于跟踪当前RPN字符串的长度  
    static char* buffer = nullptr; // 静态缓冲区，用于存储RPN字符串  
    buffer = (char*)realloc(buffer, len + 2); // 重新分配内存，+2是为了'\0'和可能的下一个字符  
    buffer[len++] = c;
    buffer[len] = '\0';
    RPN = buffer;
}

void readNumber(char*& p, Stack<float>& stk) {
    std::ostringstream oss;

    // 读取整数部分  
    while (isdigit(*p)) {
        oss << *p++;
    }

    // 读取小数部分  
    if (*p == '.') {
        oss << '.';
        while (isdigit(*(++p))) {
            oss << *p;
        }
    }

    // 尝试将字符串转换为浮点数  
    try {
        stk.push(std::stof(oss.str()));
    }
    catch (const std::invalid_argument&) {
        throw std::runtime_error("Invalid number format.");
    }
    catch (const std::out_of_range&) {
        throw std::runtime_error("Number out of range.");
    }

    // 检查是否为函数  
    if (*p == 's' || *p == 'c' || *p == 't' || *p == 'l') {
        std::string funcName;
        while (isalnum(*(p + 1))) { // 跳过函数名的第一个字符，因为它已经被检查过  
            funcName += *(++p);
        }

        // 处理函数  
        if (funcName == "sin") {
            if (stk.empty()) throw std::runtime_error("Stack is empty for sin.");
            stk.push(sin(stk.pop()));
        }
        else if (funcName == "cos") {
            if (stk.empty()) throw std::runtime_error("Stack is empty for cos.");
            stk.push(cos(stk.pop()));
        }
        else if (funcName == "tan") {
            if (stk.empty()) throw std::runtime_error("Stack is empty for tan.");
            stk.push(tan(stk.pop()));
        }
        else if (funcName == "log") {
            if (stk.empty()) throw std::runtime_error("Stack is empty for log.");
            float operand = stk.pop();
            if (operand <= 0) throw std::runtime_error("Logarithm of non-positive number.");
            stk.push(log(operand));
        }
        else {
            throw std::runtime_error("Unknown function name.");
        }
    }
}

float d_transform(float x) { return x * (M_PI / 180.00); }

float calcu(float operand1, char op, float operand2) {
    switch (op) {
    case '+': return operand1 + operand2;
    case '-': return operand1 - operand2;
    case '*': return operand1 * operand2;
    case '/': return operand2 != 0.0f ? operand1 / operand2 : INFINITY; // 注意使用 0.0f 来确保浮点比较  
    case '^': return pow(operand1, operand2);
    default: cout << "未知操作符: " << op << endl; exit(-1);
    }
}

float calcu(char op, float operand) {
    switch (op)
    {
    case ('!'):
    {
        float r = 1.0;
        while (operand > 1)
        {
            r *= operand--;
        }
        return r;
    }
    case ('s'):
        return sin(d_transform(operand));
    case ('c'):
        return cos(d_transform(operand));
    case ('t'):
        return tan(d_transform(operand));
    case ('l'):
        return logf(operand);
    default:
        cout << "未知操作符: " << op << endl; exit(-1);
    }
}



float evaluate(char* S, char*& RPN)
{
    if (!paren(S)) { cerr << "错误：括号不匹配" << endl; exit(-1); }
    Stack<float> opnd;
    Stack<char> optr;
    optr.push('\0');
    // cout<<opnd.top()<<endl;
    while (!optr.empty())
    {
        if (isdigit(*S))
        {
            readNumber(S, opnd);
            append(RPN, opnd.top());
        }
        else
            switch (orderBetween(optr.top(), *S))
            {
            case '<':
                optr.push(*S);
                if (isalpha(static_cast<Rank>(*S)))
                {
                    while (isalpha(static_cast<Rank>(*S)))// 如果S指向三角函数或者log
                        S++;
                }
                else
                    S++;
                break;
            case '=':
                optr.pop();
                if (*S) S++;
                break;
            case '>':
            {
                char op = optr.pop();
                append(RPN, op);
                if ('!' == op|| 's' == op || 'c' == op || 't' == op || 'l' == op)
                {
                    float p0pnd = opnd.pop();
                    opnd.push(calcu(op, p0pnd));
                }
                else
                {
                    float p0pnd2 = opnd.pop(), p0pnd1 = opnd.pop();
                    opnd.push(calcu(p0pnd1, op, p0pnd2));
                }
                break;
            }
            default:exit(-1);
            }
    }
    return opnd.pop();
}


