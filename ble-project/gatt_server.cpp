#include "gatt_server.hpp"
#include <iostream>
#include <ostream>


void CGattService::onDataWritten (const GattWriteCallbackParams &e) {
    printf("Data written\n");
    
    if (e.handle == immediateAlertService.getCharacteristic(0)->getValueHandle()) {
        if (e.data[0] == 0) {
            printf("Alert Level:\n");
            printf("NO ALERT\n");
            printf("LED3 set to 1\n");
            led3 = 1;
        }
        else if (e.data[0] == 1) {
            printf("Alert Level:\n");
            printf("MEDIUM\n");
            printf("LED3 set to 0.75\n");
            led3 = 0.75;
        }
        else if (e.data[0] == 2) {
            printf("Alert Level:\n");
            printf("HIGH\n");
            printf("LED3 set to 0.1\n");
            led3 = 0.1;
        }
    }
    else if (e.handle == alertNotificationService.getCharacteristic(4)->getValueHandle()) {
        if (e.data[0] == 0) {
            printButtonPresses = false;
            printf("Updates disabled\n");
        }
        else if (e.data[0] == 1) {
            printButtonPresses = true;
            printf("Updates enabled\n");
        } 
    }


    else {
        printf("Unknown characteristic\n");
    }
}

void CGattService::run(void) {
    printf("service started\n");
    ble_interface_.gattServer().setEventHandler(this);
    ble_interface_.gattServer().addService(immediateAlertService);
    ble_interface_.gattServer().addService(alertNotificationService);
}
