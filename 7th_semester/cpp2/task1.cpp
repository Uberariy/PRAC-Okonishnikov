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
    std::vector<int> v {1, 2, 3};
    std::vector<int> v2 {1, 2, 3};
    std::vector<int> v3 {1, 2, 3};

    // 1
    for (auto x : v ) {
        x = 5;

    }

    for (auto x : v ) {
        std::cout << x;

    }

    std::cout << "\n";

    // 2
    for (auto& x : v2 ) {
        x = 5;

    }

    for (auto x : v2 ) {
        std::cout << x;

    }

    std::cout << "\n";

    // 3
    for (auto&& x : v3 ) {
        x = 5;

    }

    for (auto x : v3 ) {
        std::cout << x;

    }

    std::cout << "\n";
}