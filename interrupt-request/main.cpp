/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "iostream"


#define SLEEP_RATE 500ms

DigitalOut led(LED1);
DigitalIn button(BUTTON2);
InterruptIn interrupt(BUTTON2);
bool intr = false;


void button_isr(void) {
    led = interrupt.read();
    intr = true;
}


int main()
{
    interrupt.rise(&button_isr);
    interrupt.fall(&button_isr);
    //std::cout << "hello" << std::endl;
    while (true) {
        if(intr) {
            std::cout << (led ? "OFF" : "ON") << std::endl;
            intr = false;
        }
        ThisThread::sleep_for(SLEEP_RATE);
    }
}
