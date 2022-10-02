/**
 * @file task1.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <iostream>
#include <vector>

int main() {
    std::vector<bool> v {false, false, false};
    std::vector<bool> v2 {false, false, false};

    // 2
    // for (auto& x : v ) {         --- Not compiling
    //     x = true;

    // }

    // for (auto x : v ) {
    //     std::cout << x;

    // }

    std::cout << "\n";

    // 2
    for (auto&& x : v2 ) {
        x = true;

    }

    for (auto x : v2 ) {
        std::cout << x;

    }

    std::cout << "\n";
}