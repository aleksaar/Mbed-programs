#include "example_gatt_server.hpp"


void CGattService::onDataWritten (const GattWriteCallbackParams &e) {
    printf("Data written\n");
    if (e.handle == characteristic1.getValueHandle()) {
        if ((uint8_t)e.data[0] % 2 == 0) {
            led2 = 0;
        }
        else {
            led2 = 1;
        }
        printf("Characteristic 1 written value: %c = ascii %d\n", (uint8_t)e.data[0], (uint8_t)e.data[0]);
    }
    else if (e.handle == characteristic2.getValueHandle()) {
        uint16_t value;
        memcpy(&value, e.data, sizeof(uint16_t));
        printf("Characteristic 2 written value: %c = ascii %d\n", value, value);
    }
    else {
        printf("Unknown characteristic\n");
    }
}

 void CGattService::onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) {
    printf("Updates enabled\n");
    event_queue_.call_every(1000ms, callback(this, &CGattService::readTime));
    t1.start(callback(&event_queue_, &EventQueue::dispatch_forever));
}

void CGattService::readTime() {
    uint64_t tickCount = Kernel::Clock::now().time_since_epoch().count();
    unsigned lowest4 = tickCount % 10000;
    //printf("Lowest 4 bytes of time: %u\n", lowest4);
    characteristic3.setAttributeValue(ble_interface_.gattServer(), characteristic3, lowest4);
}

void CGattService::run(void) {
    printf("service started\n");
    ble_interface_.gattServer().setEventHandler(this);
    ble_interface_.gattServer().addService(service);
}


 void CGattService::onDataRead (const GattReadCallbackParams &e) {
    uint16_t value;
    memcpy(&value, e.data, sizeof(uint16_t));
    printf("Data read: %u\n", value);
}
