/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "iostream"


DigitalOut led(LED1);

int function1(int a, int b) {
    return a + b + 1;
}

int function2(int a, int b) {
    return (a * b) + 1;
}


int main(){
    class MyClass {
        public:
            int function1(int a, int b) {
                return a + b;
            }

            int function2(int a, int b) {
                return a * b;
            }
    };


    // declare an instance of Callback.
    // the variable name is cb.
    MyClass mObj;
    Callback<int(int, int)> cb;

    
    // assign cb to global function1
    cb = &function1;
    std::cout << "callback(1, 2) with global function1 returns "
              << cb(1,2)
              << std::endl;

    // assign cb to global function2
    cb = &function2;
    std::cout << "callback(1, 2) with global function2 returns "
              << cb(1, 2)
              << std::endl;


    // assign cb to MyClass::function1
    cb = &MyClass::function1;
    std::cout << "callback(1, 2) with MyClass::function1 returns "
              << cb(1, 2)
              << std::endl;

    // assign cb to MyClass::function2
    cb = &MyClass::function2;
    std::cout << "callback(1, 2) with MyClass::function2 returns "
              << cb(1, 2)
              << std::endl;

    while (true) {

        thread_sleep_for(1000);
    }
}