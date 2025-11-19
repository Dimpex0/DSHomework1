#include "Expression.h"
#include <iostream>
#include <stdexcept>

unsigned Expression::ID_COUNTER = 0;

Expression::Expression(const std::string& input)
	: id(ID_COUNTER++)
{
	int expressionStart = 1;
	int varSize = 0;
	for (size_t i = 0; i < input.size() && input[i] != '='; i++) {
		expressionStart++;
		if (input[i] == ' ') {
			continue;
		}
		varSize++;
		variable = input[i];
	}
	if (varSize != 1) {
		throw std::invalid_argument("Expression variable must be a single character.");
	}

	std::string expression = input.substr(expressionStart, input.size());
	if (expression.empty()) {
		throw std::invalid_argument("Expression is empty.");
	}

	this->expression = simplifyExpression(expression);
}

std::string Expression::simplifyExpression(const std::string& expression) const
{
	for (size_t i = 0; i < expression.size(); i++) {
		std::cout << expression[i];
	}
	return expression;
}

//std::stack<std::string> Expression::toRPN(const std::string& expression) const
//{
//	for (size_t i = 0; i < expression.size(); i++) {
//
//	}
//}
