#pragma once
#include "mbed.h"
#include "BLE.h"


namespace ble_utils {
    events::EventQueue queue(32 * EVENTS_EVENT_SIZE);

    inline void schedule_ble_processing(BLE::OnEventsToProcessCallbackContext* context) {
            queue.call(callback(&(context->ble), &BLE::processEvents));
    }

}