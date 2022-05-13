#pragma once
#include "mbed.h"
#include <BLE.h>
#include <algorithm>
#include <cstdint>

/**
  * Characteristic declaration helper class.
  *
  * @tparam T type of data of the characteristic.
  */
 template <typename T>
 class CCharacteristic : public GattCharacteristic {
     public:
        /**
        * Construct a characteristic that can be read or written and emit
        * notification or indication.
        *
        * @param[in] uuid The UUID of the characteristic.
        * @param[in] properties The characteristic properties
        * @param[in] initialValue Initial value contained by the characteristic.
        * @param[in] descriptors characteristic descriptors
        * @param[in] numOfDescriptors number of descriptors
        */
        CCharacteristic(const UUID &uuid,
                 uint8_t properties,
                 const T &initialValue,
                 GattAttribute *descriptors[] = NULL,
                 int numOfDescriptors = 0) : 
                 GattCharacteristic(uuid, buffer, sizeof(T), sizeof(T), 
                 properties,
                 descriptors,
                 numOfDescriptors,
                 true)
                 {
                    buffer[0] = initialValue;
                    //  for (int i=0;i<sizeof(T);i++) {
                    //      buffer[i] = initialValue[i];
                    //  }
                 }

        /**
        * Get the value of a characteristic.
        *
        * @param[in] server GattServer instance that contains the characteristic value.
        * @param[in] characteristic The characteristic
        * @param[in] dst Variable that will receive the characteristic value.
        *
        * @return BLE_ERROR_NONE in case of success or an appropriate error code.
        */
        ble_error_t getAttributeValue(const GattServer &server,
                                    const GattCharacteristic& characteristic,
                                    T &dst) const {
            uint16_t valueSize = sizeof(T);
            return server.read(characteristic.getValueHandle(), &dst, &valueSize);
        }

        /**
        * Assign a new value to this characteristic.
        *
        * @param[in] server GattServer instance that will receive the new value.
        * @param[in] characteristic The characteristic
        * @param[in] value The new value to set.
        * @param[in] localOnly Flag that determine if the change should be kept
        *                      locally or forwarded to subscribed clients.
        */
        ble_error_t setAttributeValue(GattServer& server,
                                    const GattCharacteristic& characteristic,
                                    const T &value,
                                    bool localOnly = false) const {
        return server.write(characteristic.getValueHandle(),
                            reinterpret_cast<const uint8_t*>(&value),
                            sizeof(value),
                            localOnly);
        }

    private:
        T buffer[sizeof(T)];
 };