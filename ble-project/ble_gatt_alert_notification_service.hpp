#pragma once
#include "characteristic.hpp"
#include <cstdint>
#include <string>


#define CHAR_UUID1 "b32e0301-73fa-4d62-8ba3-bba67877269e"
#define CHAR_UUID2 "8836cb4d-0e25-4fa3-a7bb-91827398f399"
#define CHAR_UUID3 "6160dc64-2c88-4a5f-bcaa-34eb7161281e"
#define CHAR_UUID4 "50e8e9a0-5636-4419-83cb-6cf5dc12b30e"
#define CHAR_UUID5 "7712ec49-c7ad-4b0f-bf26-164565157a6d"
#define CHAR1_DESCRIPTION "Supported New Alert Category"
#define CHAR2_DESCRIPTION "New Alert"
#define CHAR3_DESCRIPTION "Supported Unread Alert Category"
#define CHAR4_DESCRIPTION "Unread Alert Status"
#define CHAR5_DESCRIPTION "Alert Notification Control Point"

#define AN_SERVICE_UUID 0x1811



class AlertNotificationService : public GattService {
    public:
        AlertNotificationService() : GattService(AN_SERVICE_UUID, characteristics, 5),

                            userDescription1(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR1_DESCRIPTION,
                            sizeof(CHAR1_DESCRIPTION),
                            sizeof(CHAR1_DESCRIPTION),
                            false),

                            userDescription2(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR2_DESCRIPTION,
                            sizeof(CHAR2_DESCRIPTION),
                            sizeof(CHAR2_DESCRIPTION),
                            false),

                            userDescription3(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR3_DESCRIPTION,
                            sizeof(CHAR3_DESCRIPTION),
                            sizeof(CHAR3_DESCRIPTION),
                            false),

                            userDescription4(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR4_DESCRIPTION,
                            sizeof(CHAR4_DESCRIPTION),
                            sizeof(CHAR4_DESCRIPTION),
                            false),

                            userDescription5(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR5_DESCRIPTION,
                            sizeof(CHAR5_DESCRIPTION),
                            sizeof(CHAR5_DESCRIPTION),
                            false)
        {
            char1Descs[0] = &userDescription1;
            char2Descs[0] = &userDescription2;
            char3Descs[0] = &userDescription3;
            char4Descs[0] = &userDescription4;
            char5Descs[0] = &userDescription5;
            characteristics[0] = &characteristic1;
            characteristics[1] = &characteristic2;
            characteristics[2] = &characteristic3;
            characteristics[3] = &characteristic4;
            characteristics[4] = &characteristic5;

            characteristic1.setReadSecurityRequirement(ble::att_security_requirement_t::AUTHENTICATED);
            characteristic3.setReadSecurityRequirement(ble::att_security_requirement_t::AUTHENTICATED);
            characteristic5.setWriteSecurityRequirement(ble::att_security_requirement_t::AUTHENTICATED);
        }

        CCharacteristic<uint8_t>* getChar(int charNumber) {
            if (charNumber == 1) {
                return &characteristic1;
            }
            else if (charNumber == 2) {
                return &characteristic2;
            }
            else if (charNumber == 3) {
                return &characteristic3;
            }
            else if (charNumber == 4) {
                return &characteristic4;
            }
            else if (charNumber == 5) {
                return &characteristic5;
            }
            else {
                return nullptr;
            }
        }

    protected:
        CCharacteristic<uint8_t> characteristic1 {CHAR_UUID1, // UUID
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ, // properties
                            0, // initial value
                            char1Descs, // descriptors
                            1}; // num of descriptors
        CCharacteristic<uint8_t> characteristic2 {CHAR_UUID2, // UUID
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, // properties
                            0, // initial value
                            char2Descs, // descriptors
                            1}; // num of descriptors
        CCharacteristic<uint8_t> characteristic3 {CHAR_UUID3, // UUID
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ, // properties
                            0, // initial value
                            char3Descs, // descriptors
                            1}; // num of descriptors
        CCharacteristic<uint8_t> characteristic4 {CHAR_UUID4, // UUID
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, // properties
                            0, // initial value
                            char4Descs, // descriptors
                            1}; // num of descriptors
        CCharacteristic<uint8_t> characteristic5 {CHAR_UUID5, // UUID
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE, // properties
                            0, // initial value
                            char5Descs, // descriptors
                            1}; // num of descriptors

        GattAttribute userDescription1;
        GattAttribute userDescription2;
        GattAttribute userDescription3;
        GattAttribute userDescription4;
        GattAttribute userDescription5;
        GattAttribute* char1Descs[1];
        GattAttribute* char2Descs[1];
        GattAttribute* char3Descs[1];
        GattAttribute* char4Descs[1];
        GattAttribute* char5Descs[1];
        GattCharacteristic* characteristics[5];

};
