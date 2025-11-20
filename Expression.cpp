#include "Expression.h"
#include <stack>
#include <queue>
#include <cmath>
#include <stdexcept>
#include <string>
#include "Vector.h"

static double stringToDouble(const String& str) { return std::stod(str.c_str()); }
static String doubleToString(double val) { return std::to_string(val).c_str(); }
static String intToString(int val) { return std::to_string(val).c_str(); }
static bool isDigit(char c) { return c >= '0' && c <= '9'; }
static bool isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static bool isSpace(char c) { return c == ' '; }
static bool isOperator(char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^'; }
static bool isFunction(const String& s) { return s == "MIN" || s == "MAX" || s == "SUM" || s == "AVG" || s == "PRD"; }
static int getPrecedence(const String& op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}
static double applyOp(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return b != 0 ? a / b : 0;
    case '^': return std::pow(a, b);
    }
    return 0;
}

static double applyFunc(const String& f, const Vector<double>& args) {
    if (args.empty()) return 0;
    if (f == "SUM") { double s = 0; for (size_t i = 0; i < args.size(); i++) s += args[i]; return s; }
    if (f == "PRD") { double p = 1; for (size_t i = 0; i < args.size(); i++) p *= args[i]; return p; }
    if (f == "AVG") { double s = 0; for (size_t i = 0; i < args.size(); i++) s += args[i]; return s / args.size(); }
    if (f == "MIN") { double m = args[0]; for (size_t i = 1; i < args.size(); i++) if (args[i] < m)m = args[i]; return m; }
    if (f == "MAX") { double m = args[0]; for (size_t i = 1; i < args.size(); i++) if (args[i] > m)m = args[i]; return m; }
    return 0;
}

unsigned Expression::ID_COUNTER = 1;

Expression::Expression(const String& input)
    : id(ID_COUNTER++)
{
    int assignPos = input.find('=');
    if (assignPos == -1) throw std::invalid_argument("Invalid. Missing '='.");

    int varCount = 0;
    var = '\0';
    for (int i = 0; i < assignPos; i++) {
        char c = input[i];
        if (isSpace(c)) continue;
        varCount++;
        if (isAlpha(c)) var = c;
    }
    if (varCount != 1) throw std::invalid_argument("Variable must be a single character.");

    String rightSide;
    for (size_t i = assignPos + 1; i < input.size(); i++) rightSide += input[i];
    rpn = toRPN(rightSide);
}

std::queue<String> Expression::toRPN(const String& input)
{
    std::queue<String> output;
    std::stack<String> opStack;
    std::stack<int> argCounts;

    for (size_t i = 0; i < input.size(); i++) {
        char el = input[i];

        if (isSpace(el)) continue;

        if (isDigit(el) || el == '.') {
            String current;
            if (el == '.') {
                current += '0';
            }
            while (i < input.size() && (isDigit(input[i]) || input[i] == '.')) {
                current += input[i];
                i++;
            }
            i--;
            output.push(current);
        }
        else if (el == '-' && i + 1 < input.size() && isDigit(input[i + 1])) {
            String current(el);
            i++;
            while (i < input.size() && (isDigit(input[i]) || input[i] == '.')) {
                current += input[i];
                i++;
            }
            i--;
            output.push(current);
        }
        else if (isAlpha(el)) {
            String name;
            while (i < input.size() && isAlpha(input[i])) {
                name += input[i];
                i++;
            }
            i--;

            if (isFunction(name)) {
                opStack.push(name);
                argCounts.push(1);
            }
            else {
                if (name.size() != 1) {
                    throw std::invalid_argument("Unknown function.");
                }
                output.push(name);
            }
        }
        else if (el == ';') {
            while (!opStack.empty() && opStack.top() != "(") {
                output.push(opStack.top());
                opStack.pop();
            }
            if (!argCounts.empty()) {
                argCounts.top()++;
            }
        }
        else if (el == '(') {
            opStack.push("(");
        }
        else if (el == ')') {
            while (!opStack.empty() && opStack.top() != "(") {
                output.push(opStack.top());
                opStack.pop();
            }

            if (!opStack.empty()) opStack.pop();

            if (!opStack.empty() && isFunction(opStack.top())) {
                String funcName = opStack.top();
                opStack.pop();

                int args = 1;
                if (!argCounts.empty()) {
                    args = argCounts.top();
                    argCounts.pop();
                }

                String finalToken = funcName;
                finalToken += "_";
                finalToken += intToString(args);
                output.push(finalToken);
            }
        }
        else if (isOperator(el)) {
            String opStr(el);

            while (!opStack.empty() && opStack.top() != "(") {
                String top = opStack.top();

                if (getPrecedence(top) >= getPrecedence(opStr)) {
                    if (opStr == "^" && getPrecedence(top) == getPrecedence(opStr)) break;

                    output.push(top);
                    opStack.pop();
                }
                else {
                    break;
                }
            }
            opStack.push(opStr);
           
        }
    }

    while (!opStack.empty()) {
        if (opStack.top() == "(" || opStack.top() == ")") {
            throw std::invalid_argument("Mismatched parentheses");
        }
        output.push(opStack.top());
        opStack.pop();
    }

    return output;
}

CalcResult Expression::calculate(VariableStore& store) {
    std::stack<double> stack;

    std::queue<String> rpnCopy = rpn;
    while (!rpnCopy.empty()) {
        String token = rpnCopy.front();
        rpnCopy.pop();

        if (isDigit(token[0]) || (token.size() > 1 && token[0] == '-' && isDigit(token[1]))) {
            stack.push(std::stod(token.c_str()));
        }
        else if (isOperator(token[0]) && token.size() == 1) {
            if (stack.size() < 2) return CalcResult::ERROR;

            double right = stack.top(); stack.pop();
            double left = stack.top(); stack.pop();

            double res = applyOp(left, right, token[0]);
            stack.push(res);
        }
        else if (token.find('_') != -1) {
            int underscorePos = token.find('_');
            String funcName = token.substr(0, underscorePos);
            int argsCount = std::stoi(token.substr(underscorePos + 1, token.size()).c_str());

            if (stack.size() < argsCount) return CalcResult::ERROR;

            Vector<double> args;
            for (size_t k = 0; k < argsCount; k++) {
                args.push_back(stack.top());
                stack.pop();
            }
            double res = applyFunc(funcName, args);
            stack.push(res);
        }
        else {
            char varName = token[0];
            if (store.hasValue(varName)) {
                int index = store.getId(varName);
                stack.push(store.values[index]);
            }
            else {
                return CalcResult::POSTPONED;
            }
        }
    }

    if (stack.size() != 1) return CalcResult::ERROR;

    store.setValue(var, stack.top());
    return CalcResult::CALCULATED;
}
