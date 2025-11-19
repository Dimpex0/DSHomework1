#pragma once
#include <string>
#include <stack>

class Expression
{
public:
	Expression(const std::string& input);

private:
	std::string simplifyExpression(const std::string& expression) const;
	std::stack<std::string> toRPN(const std::string& expression) const;

private:
	static unsigned ID_COUNTER;
	const unsigned id;
	char variable;
	std::string expression;
};

