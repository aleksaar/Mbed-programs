/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "iostream"


DigitalOut led(LED1);
DigitalOut led2(LED2);

DigitalIn button(BUTTON1);
DigitalIn button2(BUTTON2);
DigitalIn button3(BUTTON3);

Thread task1(osPriorityNormal);
Thread task2(osPriorityNormal);
Thread task3(osPriorityNormal);
Semaphore sem(2);


void task1_func(){
    std::cout << "task1" << std::endl;
    while (true) {
        if (!button) {
            if (sem.try_acquire()) {
                std::cout << "task 1 acquired sem" << std::endl;
                if (!led) {
                    led2 = 0;
                    thread_sleep_for(1000);
                    led2 = 1;
                }
                else {
                    led = 0;
                    thread_sleep_for(1000);
                    led = 1;
                }
                sem.release();
            }
        }
    }
}

void task2_func(){
    std::cout << "task2" << std::endl;
    while (true) {
        if (!button2) {
            if (sem.try_acquire()) {
                std::cout << "task 2 acquired sem" << std::endl;
                if (!led) {
                    led2 = 0;
                    thread_sleep_for(1000);
                    led2 = 1;
                }
                else {
                    led = 0;
                    thread_sleep_for(1000);
                    led = 1;
                }
                sem.release();
            }
        }
    }
}

void task3_func(){
    std::cout << "task3" << std::endl;
    while (true) {
        if (!button3) {
            if (sem.try_acquire()) {
                std::cout << "task 3 acquired sem" << std::endl;
                if (!led) {
                    led2 = 0;
                    thread_sleep_for(1000);
                    led2 = 1;
                }
                else {
                    led = 0;
                    thread_sleep_for(1000);
                    led = 1;
                }
                sem.release();
            }
        }
    }
}

int main(){
    std::cout << "start" << std::endl;
    led = 1;
    led2 = 1;
    task1.start(task1_func);
    task2.start(task2_func);
    task3.start(task3_func);

}