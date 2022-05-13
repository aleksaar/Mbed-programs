/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "iostream"
#include <cstdio>
#include <stdio.h>


Thread task(osPriorityNormal);

void eventHandler(int id) {
    //std::cout << id << std::endl;
    printf("%d \n", id);
}

EventQueue q;
Event<void(int)> event(&q, eventHandler);


void task_func(){
    event.post(1);
    event.post(2);
    event.post(3);

}

int main(){
    std::cout << "start" << std::endl;
    event.delay(100ms);
    event.period(200ms);

    task.start(task_func);

    q.dispatch_for(1000ms);

    task.join();
    std::cout << "end" << std::endl;
}