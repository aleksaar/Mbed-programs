/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "iostream"
#include <BLE.h>
#include <cstdint>
#include <cstdio>


class GAPButton : private mbed::NonCopyable<GAPButton>, public ble::Gap::EventHandler {
    public:
        GAPButton(BLE& ble_interface, events::EventQueue& event_queue) : ble_interface_(ble_interface), event_queue_(event_queue), button(BUTTON1), led(LED1), adv_builder_(buffer) {
            t1.start(callback(&event_queue_, &EventQueue::dispatch_forever));
            button.rise(callback(this, &GAPButton::interrupt));
        }

        void onDisconnectionComplete(const ble::DisconnectionCompleteEvent& event) override {
            printf("disconnection complete");
            start_advertisement();
            start_blink();
        }

        void onConnectionComplete(const ble::ConnectionCompleteEvent& event) override {
            printf("connection complete\n");
            led_events.break_dispatch();
        }

        void replace_adv_data() {
            button_presses_int++;
            button_presses_char = button_presses_int;
            printf("button presses %d\n", button_presses_char);
            printf("replacing data\n");
            Span<unsigned char> button_span = mbed::make_Span(&button_presses_char, 1);
            adv_builder_.replaceData(ble::adv_data_type_t::MANUFACTURER_SPECIFIC_DATA,
                           button_span);
        }

        void print_interrupt() {
            printf("--------\ninterrupt\n");
        }

        void interrupt() {
            event_queue_.call(callback(this, &GAPButton::print_interrupt));
            event_queue_.call(callback(this, &GAPButton::replace_adv_data));
        }

        void start_advertisement() {
            printf("starting advertising\n");
            ble::AdvertisingParameters adv_params (
                ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
                ble::adv_interval_t(ble::millisecond_t(100))
            );

            //unsigned char adv_data = 'a';          
            //Span<unsigned char> button_span = mbed::make_Span(&adv_data, 1);     

            Span<unsigned char> button_span = mbed::make_Span(&button_presses_char, 1);     
           
            adv_builder_.setFlags();
            adv_builder_.setName("BLEButton");
            adv_builder_.addData(ble::adv_data_type_t::MANUFACTURER_SPECIFIC_DATA,
                           button_span);

            ble_error_t error = ble_interface_.gap().setAdvertisingParameters(
                ble::LEGACY_ADVERTISING_HANDLE,
                adv_params
            );

            if (error != 0U) {
                std::cout << ble::BLE::errorToString(error) << std::endl;
                return;
            }
            // then, the payload
            error = ble_interface_.gap().setAdvertisingPayload(
                ble::LEGACY_ADVERTISING_HANDLE,
                adv_builder_.getAdvertisingData()
            );

            if (error != 0U) {
                std::cout << ble::BLE::errorToString(error) << std::endl;
                return;
            }

            /* Start advertising */
            error = ble_interface_.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
            printf("advertising\n");
            if (error != 0U) {
                std::cout << ble::BLE::errorToString(error) << std::endl;
                return;
            }
        }

        void toggle_led() {
            led = !led;
        }

        void start_blink() {
            printf("starting blink\n");
            led_events.call_every(500ms, callback(this, &GAPButton::toggle_led));
            led_events.dispatch_forever(); // forever?
        }

        void on_ble_init_complete(BLE::InitializationCompleteCallbackContext* context) {
            if (context->error) {
                printf("init failed");
                return;
            }
            printf("init done\n");
            ble::own_address_type_t addr_type;
            ble::address_t address;
            auto& a_t = addr_type;
            auto& a = address;
            context->ble.gap().getAddress(a_t, a);
            printf("Device address ");
            for (int i=5; i>=1;i--) {
                printf("%02x:", address[i]);
            }
            printf("%02x\r\n", address[0]);

            start_advertisement();
            start_blink();
        }

        

        void run() {
            ble_interface_.init(this, &GAPButton::on_ble_init_complete);
            printf("running\n");
            event_queue_.dispatch_forever();
        }

        


    private:
        BLE& ble_interface_;
        events::EventQueue& event_queue_;
        InterruptIn button;
        DigitalOut led;
        ble::AdvertisingDataBuilder adv_builder_;
        events::EventQueue led_events;
        uint32_t button_presses_int = 0;
        unsigned char button_presses_char = '0';
        uint8_t buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
        Thread t1;
};


events::EventQueue queue(32 * EVENTS_EVENT_SIZE);


void schedule_ble_processing(BLE::OnEventsToProcessCallbackContext* context) {
            queue.call(callback(&(context->ble), &BLE::processEvents));
        }

int main(){
    BLE& ble_inst = BLE::Instance();
    ble_inst.onEventsToProcess(schedule_ble_processing);
    GAPButton gap_button (ble_inst, queue);
    gap_button.run();

    return 0;
}