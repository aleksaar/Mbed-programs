/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "iostream"


DigitalOut led(LED1);
DigitalOut led2(LED2);
DigitalIn button(BUTTON1);

Thread task1(osPriorityNormal);
Thread task2(osPriorityAboveNormal);


void task1_func(){
    while (true) {
        led = button;
    }   
}

void task2_func(){
    while (true) {
        led2 = button;
    }   
}


int main(){
    std::cout << "multitask" << std::endl;
    
    task1.start(task1_func);
    task2.start(task2_func);
}