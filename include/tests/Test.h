/**
 * @file Test.h
 * @brief Interface for all test classes. 
 */
#ifndef TEST_H
#define TEST_H

class Test {

public: 
    virtual ~Test() = default;
    virtual bool test() const = 0;
};

#endif
