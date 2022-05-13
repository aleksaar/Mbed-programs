#pragma once
#include "characteristic.hpp"
#include <cstdint>
#include <string>


#define CHAR_UUID "b32e0301-73fa-4d62-8ba3-bba67877269e"
#define CHAR_DESCRIPTION "Alert Level"
#define IA_SERVICE_UUID 0x1802


class ImmediateAlertService : public GattService {
    public:
        ImmediateAlertService() : GattService(IA_SERVICE_UUID, characteristics, 1),

                            userDescription(BLE_UUID_DESCRIPTOR_CHAR_USER_DESC,
                            (uint8_t *)CHAR_DESCRIPTION,
                            sizeof(CHAR_DESCRIPTION),
                            sizeof(CHAR_DESCRIPTION),
                            false)
        {
            charDescs[0] = &userDescription;
            characteristics[0] = &characteristic1;
            characteristic1.setWriteSecurityRequirement(ble::att_security_requirement_t::AUTHENTICATED);
        }

    protected:
        CCharacteristic<uint8_t> characteristic1 {CHAR_UUID, // UUID
                            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE, // properties
                            0, // initial value
                            charDescs, // descriptors
                            1}; // num of descriptors
        GattAttribute userDescription;
        GattAttribute* charDescs[1];
        GattCharacteristic* characteristics[1];

};
