#include <iostream>
#include <queue>
#include "String.h"
#include <string>
#include "Expression.h"

bool validateControls(const std::string& controls) {
	for (size_t i = 0; i < controls.size(); i++) {
		if (controls[i] != 'R' && controls[i] != 'C') {
			return false;
		}
	}

	return true;
}

void handleCalcResult(CalcResult result, const Expression& expr, std::queue<Expression>& queue, const VariableStore& varStore) {
	if (result == CalcResult::CALCULATED) {
		int varIndex = varStore.getId(expr.getVar());
		double value = varStore.values[varIndex];
		std::cout << "Expression (" << expr.getId() << ") was calculated. The value of " << expr.getVar() << " is now " << value << "." << std::endl;
	}
	else if (result == CalcResult::POSTPONED) {
		std::cout << "Expression (" << expr.getId() << ") was postponed." << std::endl;
		queue.push(expr);
	}
	else {
		std::cout << "Error: Could not calculate expression (" << expr.getId() << ")." << std::endl;
	}
}

int main(int argc, char* argv[]) {
	std::cout << "Enter controls ('R' and 'C' only): ";
	std::string inputControls;
	std::getline(std::cin, inputControls);

	while (!validateControls(inputControls) || inputControls.empty()) {
	    std::cout << "Invalid. Controls are 'C' and 'R' only." << '\n';
	    std::cout << "Try again: ";
	    std::getline(std::cin, inputControls);
	}

	String controls(inputControls.c_str());

	std::queue<Expression> queue;
	VariableStore varStore;

	for (size_t i = 0; i < controls.size(); i++) {
		char command = controls[i];

		if (command == 'R') {
			std::cout << "Enter expression: ";
			std::string expression;
			std::getline(std::cin, expression);

			if (expression.empty()) {
				std::getline(std::cin, expression);
			}

			try {
				String expressionString(expression.c_str());
				Expression expr = Expression(expressionString);
				queue.push(expr);
				std::cout << "Expression (" << expr.getId() << ") was read." << std::endl;
			}
			catch (const std::exception& e) {
				std::cout << "Error reading expression: " << e.what() << std::endl;
			}
		}
		else if (command == 'C') {
			if (queue.empty()) {
				std::cout << "Queue is empty, cannot calculate." << std::endl;
				continue;
			}

			Expression expr = queue.front();
			queue.pop();

			CalcResult result = expr.calculate(varStore);

			handleCalcResult(result, expr, queue, varStore);
		}
		else {
			std::cout << "Invalid command '" << command << "'. Skipping." << std::endl;
		}
	}

	if (!queue.empty()) {
		std::cout << "WARNING: There are " << queue.size() << " expressions remaining in the queue. Should they be calculated? (y/n): " << std::endl;
		char input;
		std::cin >> input;

		if (input == 'Y' || input == 'y') {
			while (!queue.empty()) {
				Expression expr = queue.front();
				queue.pop();
				CalcResult result = expr.calculate(varStore);

				handleCalcResult(result, expr, queue, varStore);
			}
		}
	}
}