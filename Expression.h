#pragma once
#include "String.h"
#include "Vector.h"

struct VariableStore {
	double values[26];
	bool isSet[26]{};

	int getId(char c) const {
		if (c >= 'a' && c <= 'z') return c - 'a';
		if (c >= 'A' && c <= 'Z') return c - 'A';
		return -1;
	}

	void setValue(char c, double val) {
		int idx = getId(c);
		if (idx != -1) {
			values[idx] = val;
			isSet[idx] = true;
		}
	}

	bool hasValue(char c) const {
		int idx = getId(c);
		if (idx != -1 && isSet[idx]) {
			return true;
		}
		return false;
	}
};

enum class CalcResult {
	CALCULATED,
	POSTPONED,
	ERROR
};

class Expression {
public:
	Expression(const String& input);

	CalcResult calculate(VariableStore& store);

	unsigned getId() const { return id; }
	char getVar() const { return var; }

private:
	Vector<String> toRPN(const String& input);

private:
	static unsigned ID_COUNTER;
	const unsigned id;

	char var;
	Vector<String> rpn;
};