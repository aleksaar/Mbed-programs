#pragma once

#include "mbed.h"
#include <BLE.h>
#include <cstdint>
#include "ble_gatt_immediate_alert_service.hpp"
#include "ble_gatt_alert_notification_service.hpp"


class CGattService : public GattServer::EventHandler {
public:
    CGattService(BLE& ble_interface) :  ble_interface_(ble_interface), led3(LED3), button(BUTTON1) {
        led3 = 1;
        t1.start(callback(&event_queue_, &EventQueue::dispatch_forever));
        button.rise(callback(this, &CGattService::buttonPress));
        event_queue_.call_every(500ms, callback(this, &CGattService::setUnreadAlerts));
    }
    
    
    virtual void onDataWritten (const GattWriteCallbackParams &e) override;

    void buttonPress() {
            event_queue_.call(callback(this, &CGattService::incrementButtonPresses));
        }


    void incrementButtonPresses() {
        button_presses ++;
        if(printButtonPresses) {
            printf("button presses: %d\n", button_presses);
        }
    }

    void setUnreadAlerts() {
        // displayed as hexadecimal on phone
        alertNotificationService.getChar(4)->setAttributeValue
        (ble_interface_.gattServer(), *alertNotificationService.getChar(4), (uint8_t)button_presses);
    }

    
    void run(void);



private:
    ImmediateAlertService immediateAlertService;
    AlertNotificationService alertNotificationService;
    
    BLE& ble_interface_;
    PwmOut led3;
    InterruptIn button;
    uint32_t button_presses = 0;

    events::EventQueue event_queue_;
    Thread t1;
    bool printButtonPresses = false;

};
