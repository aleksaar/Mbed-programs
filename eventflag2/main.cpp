/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "iostream"
#include <cstdint>
#include <cstdio>
#include <stdio.h>


#define bit0 (1UL << 0)
#define bit1 (1UL << 1)


DigitalOut led(LED1);
DigitalIn button(BUTTON1);

Thread task1(osPriorityNormal);
Thread task2(osPriorityAboveNormal);
EventFlags flag;
bool buttonState = false;

void buttonFunc() {
    if (!button) {
        std::cout << "button down" << std::endl;
        flag.set(bit1);
    }
    else {
        std::cout << "button up" << std::endl;
        flag.set(bit0);
    }
}

void task1_func(){
    std::cout << "task1" << std::endl;
    uint32_t flag_read = 0;
    uint32_t current_flag = 0;
    while (true) {
        current_flag = flag.wait_any(bit0 | bit1);
        if (current_flag & bit1) {
            led = 0;
        }
        else {
            led = 1;
        }
    }
}

void task2_func(){
    std::cout << "task2" << std::endl;
    while (true) {
        if (button != buttonState) {
            buttonState = button;
            buttonFunc();
            ThisThread::sleep_for(2ms);
        }
    }
}


int main(){
    std::cout << "eventflag" << std::endl;
    std::cout << "testing" << std::endl;

    task1.start(task1_func);
    ThisThread::sleep_for(5ms);
    task2.start(task2_func);

}