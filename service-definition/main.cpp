#include "mbed.h"
#include <cstdint>

#include "gap.hpp"


#define CHAR_UUID1 "a7bc5116-ad5d-40d8-9463-b7f91a715b9f"
#define CHAR_UUID2 "8836cb4d-0e25-4fa3-a7bb-91827398f399"
#define CHAR_UUID3 "6160dc64-2c88-4a5f-bcaa-34eb7161281e"
#define SERVICE_UUID "50e8e9a0-5636-4419-83cb-6cf5dc12b30e"
#define CHARACTERISTIC_DESCRIPTION "Characteristic 1"


class CGattService {
public:
    CGattService(BLE& ble_interface) : ble_interface_(ble_interface),
    
                            userDescription(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHARACTERISTIC_DESCRIPTION,
                            sizeof(CHARACTERISTIC_DESCRIPTION),
                            sizeof(CHARACTERISTIC_DESCRIPTION),
                            false), 
                            
                            characteristic1(CHAR_UUID1,
                            nullptr,
                            0,
                            0,
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE,
                            char1Descs,
                            1,
                            false), 
                            
                            characteristic2(CHAR_UUID2,
                            nullptr,
                            0,
                            0,
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE,
                            nullptr,
                            0,
                            false),
                            
                            characteristic3(CHAR_UUID3,
                            nullptr,
                            0,
                            0,
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE,
                            nullptr,
                            0,
                            false),

                            service(SERVICE_UUID, characteristics, 3) 
                            
                            {
                                char1Descs[0] = &userDescription;
                                characteristics[0] = &characteristic1;
                                characteristics[1] = &characteristic2;
                                characteristics[2] = &characteristic3;
                            }
    
    void run(void) {
        printf("service started\n");
        ble_interface_.gattServer().addService(service);
    }

private:
    GattAttribute userDescription;
    GattAttribute* char1Descs[1];
    GattCharacteristic characteristic1;
    GattCharacteristic characteristic2;
    GattCharacteristic characteristic3;
    GattCharacteristic* characteristics[3];
    GattService service;
    BLE& ble_interface_;
  
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