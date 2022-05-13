#include "mbed.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "example_gatt_server.hpp"
#include "secure_gap.hpp"


events::EventQueue queue(32 * EVENTS_EVENT_SIZE);

void schedule_ble_processing(BLE::OnEventsToProcessCallbackContext* context) {
            queue.call(callback(&(context->ble), &BLE::processEvents));
}

int main(){
    BLE& ble_inst = BLE::Instance();
    ble_inst.onEventsToProcess(schedule_ble_processing);
    CSecureGap cgap(ble_inst, queue, "GATTServer Secure");
    CGattService serv(ble_inst);

    cgap.set_init_callback(callback(&serv, &CGattService::run));

    cgap.run();

    return 0;
}