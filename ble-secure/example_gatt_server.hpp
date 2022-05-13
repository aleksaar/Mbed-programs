#include "mbed.h"
#include <BLE.h>
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

                                characteristic1.setReadSecurityRequirement(ble::att_security_requirement_t::UNAUTHENTICATED);
                                characteristic1.setWriteSecurityRequirement(ble::att_security_requirement_t::AUTHENTICATED);
                                characteristic1.setUpdateSecurityRequirement(ble::att_security_requirement_t::UNAUTHENTICATED);

                                characteristic2.setReadSecurityRequirement(ble::att_security_requirement_t::UNAUTHENTICATED);
                                characteristic2.setWriteSecurityRequirement(ble::att_security_requirement_t::AUTHENTICATED);
                                characteristic2.setUpdateSecurityRequirement(ble::att_security_requirement_t::UNAUTHENTICATED);

                                characteristic3.setReadSecurityRequirement(ble::att_security_requirement_t::UNAUTHENTICATED);
                                characteristic3.setWriteSecurityRequirement(ble::att_security_requirement_t::UNAUTHENTICATED);
                                characteristic3.setUpdateSecurityRequirement(ble::att_security_requirement_t::AUTHENTICATED);

                            }
    
    
    virtual void onDataWritten (const GattWriteCallbackParams &e) override;

    virtual void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) override;

    void readTime();
    
    void run(void);

    virtual void onDataRead(const GattReadCallbackParams &e) override;


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