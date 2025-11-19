#include <iostream>
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

int main()
{
    //std::string controls;
    //std::getline(std::cin, controls);

    //while (!validateControls(controls)) {
    //    std::cout << "Invalid controls. Controls are 'C' and 'R' only." << '\n';
    //    std::cout << "Try again" << std::endl;
    //    std::getline(std::cin, controls);
    //}

    //for (size_t i = 0; i < controls.size(); i++) {
    //    
    //}

    std::string input;
    std::getline(std::cin, input);
    try {
        Expression ex = Expression(input);
    }
    catch (const std::invalid_argument& e) {
        std::cout << e.what();
    }
}
