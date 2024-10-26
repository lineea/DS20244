#include <iostream>
#include <stack>
#include <cctype>
#include <sstream>
#include <string>
#include <stdexcept>

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void apply_operator(std::stack<int>& values, char op) {
    int right = values.top(); values.pop();
    int left = values.top(); values.pop();
    
    switch (op) {
        case '+': values.push(left + right); break;
        case '-': values.push(left - right); break;
        case '*': values.push(left * right); break;
        case '/':
            if (right == 0) throw std::runtime_error("division by zero");
            values.push(left / right); break;
    }
}

int evaluate_expression(const std::string& expression) {
    std::stack<int> values;
    std::stack<char> operators;

    for (size_t i = 0; i < expression.length(); ++i) {
        if (isspace(expression[i])) continue;

        if (isdigit(expression[i])) {
            int num = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                ++i;
            }
            values.push(num);
            --i; // Adjust for the loop increment
        } else if (expression[i] == '(') {
            operators.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                apply_operator(values, operators.top());
                operators.pop();
            }
            operators.pop(); // Remove '('
        } else if (precedence(expression[i]) > 0) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(expression[i])) {
                apply_operator(values, operators.top());
                operators.pop();
            }
            operators.push(expression[i]);
        }
    }

    while (!operators.empty()) {
        apply_operator(values, operators.top());
        operators.pop();
    }

    return values.top();
}

int main() {
    std::string expression = "3 + 5 * (2 - 8)";
    try {
        int result = evaluate_expression(expression);
        std::cout << "结果: " << result << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    }
    return 0;
}

