#include "mbed.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "gatt_server.hpp"
#include "secure_gap.hpp"
#include "ble_utils.h"



int main(){
    BLE& ble_inst = BLE::Instance();
    ble_inst.onEventsToProcess(ble_utils::schedule_ble_processing);
    CSecureGap cgap(ble_inst, ble_utils::queue, "MTC-BLE");
    CGattService serv(ble_inst);

    cgap.set_init_callback(callback(&serv, &CGattService::run));

    cgap.run();

    return 0;
}