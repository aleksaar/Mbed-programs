/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "iostream"


DigitalOut led(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

DigitalIn button(BUTTON1);
DigitalIn button2(BUTTON2);

Thread task1(osPriorityNormal);
Thread task2(osPriorityNormal);
Mutex mutex;


void task1_func(){
    std::cout << "task1" << std::endl;
    while (true) {
        led = button;
            if (!button){
                if (mutex.trylock()) {
                    led3 = 0;
                    thread_sleep_for(1000);
                    led3 = 1;
                    mutex.unlock();
                }
            }
    }
}

void task2_func(){
    std::cout << "task2" << std::endl;
    while (true) {
        led2 = button2;
            if (!button2){
                if (mutex.trylock()) {
                    led3 = 0;
                    thread_sleep_for(1000);
                    led3 = 1;
                    mutex.unlock();
                }
            }
    }
}


int main(){
    std::cout << "mutex" << std::endl;

    task1.start(task1_func);
    ThisThread::sleep_for(5ms);
    task2.start(task2_func);

}