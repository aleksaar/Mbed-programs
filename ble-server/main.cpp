#include "mbed.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "gap.hpp"
#include "characteristic.hpp"


#define CHAR_UUID1 "a7bc5116-ad5d-40d8-9463-b7f91a715b9f"
#define CHAR_UUID2 "8836cb4d-0e25-4fa3-a7bb-91827398f399"
#define CHAR_UUID3 "6160dc64-2c88-4a5f-bcaa-34eb7161281e"
#define SERVICE_UUID "50e8e9a0-5636-4419-83cb-6cf5dc12b30e"
#define CHAR1_DESC "LED2"
#define CHAR2_DESC "DummyConfig"
#define CHAR3_DESC "TickCount"


class CGattService : public GattServer::EventHandler {
public:
    CGattService(BLE& ble_interface) :  ble_interface_(ble_interface),
                                        led2(LED2),
    
                            userDescription1(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR1_DESC,
                            sizeof(CHAR1_DESC),
                            sizeof(CHAR1_DESC),
                            false),

                            userDescription2(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR2_DESC,
                            sizeof(CHAR2_DESC),
                            sizeof(CHAR2_DESC),
                            false),

                            userDescription3(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR3_DESC,
                            sizeof(CHAR3_DESC),
                            sizeof(CHAR3_DESC),
                            false), 

                            service(SERVICE_UUID, characteristics, 3) 
                            
                            {
                                char1Descs[0] = &userDescription1;
                                char2Descs[0] = &userDescription2;
                                char3Descs[0] = &userDescription3;
                                characteristics[0] = &characteristic1;
                                characteristics[1] = &characteristic2;
                                characteristics[2] = &characteristic3;
                            }
    
    
    virtual void onDataWritten (const GattWriteCallbackParams &e) override {
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

    virtual void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) override {
        printf("Updates enabled\n");
        event_queue_.call_every(1000ms, callback(this, &CGattService::readTime));
        t1.start(callback(&event_queue_, &EventQueue::dispatch_forever));
    }

    void readTime() {
        uint64_t tickCount = Kernel::Clock::now().time_since_epoch().count();
        unsigned lowest4 = tickCount % 10000;
        //printf("Lowest 4 bytes of time: %u\n", lowest4);
        characteristic3.setAttributeValue(ble_interface_.gattServer(), characteristic3, lowest4);
    }
    
    void run(void) {
        printf("service started\n");
        ble_interface_.gattServer().setEventHandler(this);
        ble_interface_.gattServer().addService(service);
    }


    virtual void onDataRead (const GattReadCallbackParams &e) override {
        uint16_t value;
        memcpy(&value, e.data, sizeof(uint16_t));
        printf("Data read: %u\n", value);
    }

private:
    GattAttribute userDescription1;
    GattAttribute userDescription2;
    GattAttribute userDescription3;
    GattAttribute* char1Descs[1];
    GattAttribute* char2Descs[1];
    GattAttribute* char3Descs[1];
    CCharacteristic<uint8_t> characteristic1 {CHAR_UUID1, // UUID
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE, // properties
                            1, // initial value
                            char1Descs, // descriptors
                            1}; // num of descriptors
    CCharacteristic<uint16_t> characteristic2 {CHAR_UUID2,
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE, 
                            0,
                            char2Descs,
                            1};
    CCharacteristic<uint32_t> characteristic3 {CHAR_UUID3,
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY |
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE,
                            0,
                            char3Descs,
                            1};

    GattCharacteristic* characteristics[3];
    GattService service;
    BLE& ble_interface_;
    DigitalOut led2;
    events::EventQueue event_queue_;
    Thread t1;

};


events::EventQueue queue(32 * EVENTS_EVENT_SIZE);

void schedule_ble_processing(BLE::OnEventsToProcessCallbackContext* context) {
            queue.call(callback(&(context->ble), &BLE::processEvents));
}

int main(){
    BLE& ble_inst = BLE::Instance();
    ble_inst.onEventsToProcess(schedule_ble_processing);
    CGap cgap(ble_inst, queue, "GATTService Test");
    CGattService serv(ble_inst);

    cgap.set_init_callback(callback(&serv, &CGattService::run));

    cgap.run();

    return 0;
}