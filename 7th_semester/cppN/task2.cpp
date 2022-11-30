#include <exception>
#include <string>
#include <iostream>

#include "gtest/gtest.h"

class CustomException : std::exception {
private:
    std::string message;

public:
    CustomException() : message("Custom Exception") {}
    explicit CustomException(const char* msg) : message(msg) {}
    explicit CustomException(const std::string& msg) : message(msg) {}
    const char* what () {
        std::cout << message << "\n";
        return message.data();
    }
    friend CustomException& operator<< (CustomException& E, const std::string& msg);
};

class SonException : public CustomException {

};

class SonSecondException : public CustomException {

};

CustomException& operator<< (CustomException& E, const std::string& msg) {
    E.message += msg.c_str();
    return E;
}

/* Tests */

class Custom_test:public testing::Test {};

TEST(Custom_test, exception)
{
    CustomException Cust;
    Cust << "abc";
    EXPECT_THROW(throw Cust, CustomException);
    SonException Son;
    Son << "abcd" << "ef";
    EXPECT_THROW(throw Son, SonException);
    SonSecondException SonSecond;
    SonSecond << "abcde";
    EXPECT_THROW(throw SonSecond, SonSecondException);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}