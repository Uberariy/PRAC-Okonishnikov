

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

#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <set>

struct Printer {
    std::string str() const {
        return ss.str();
    }

    template<typename T>
    Printer& format(const T& t) {
        ss << t;
        return *this;
    }

    template<typename T>
    void content(const T& t) {
        bool first = false;
        for (auto it = t.begin(); it != t.end(); it++) {
            if (first) {
                ss << ", ";
            } else {
                first = true;
            }
            format(*it);
        }
    }

    template<typename T>
    Printer& format(std::vector<T> op) {
        ss << "[ ";
        content(op);
        ss << " ]";
        return *this;
    }

    template<typename T1, typename T2>
    Printer& format(std::pair<T1, T2> op) {
        ss << "( ";
        format(op.first);
        ss << ", ";
        format(op.second);
        ss << " )";
        return *this;
    }

    template<typename T>
    Printer& format(std::set<T> op) {
        ss << "{ ";
        content(op);
        ss << " }";
        return *this;
    }
    
    template<typename T>
    void additem_to_ss(T& t) {
        if (tuple_first) {
            ss << ", ";
        } else {
            tuple_first = true;
        }
        ss << Printer().format(t).str();
    }

    template<typename... T>
    Printer& format(std::tuple<T... >& op) {
        ss << "( ";
        tuple_first = false;
        std::apply([&](auto&... it) {(..., additem_to_ss(it));}, op);
        ss << " )";
        return *this;
    }

private:
    std::stringstream ss;
    bool tuple_first;
};

template<typename T>
std::string format(const T& t) {
    return Printer().format(t).str();
}

int main() {
    std::tuple<std::string, int, int> t = {"xyz", 1, 2};
    std::vector<std::pair<int, int> > v = {{1, 4}, {5, 6}};
    std::set<int> s = {1, 2, 3};

    std::string s1 = Printer().format("  vector:  ").format(v).str();
    std::cout << s1 << "\n";
    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
    std::cout << s2 << "\n";
    std::string s3 = Printer().format(s).str();
    std::cout << s3 << "\n";
}