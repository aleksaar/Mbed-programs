#include "mbed.h"
#include "iostream"
#include <BLE.h>
#include <cstdint>
#include <cstdio>
#include <functional>


class CGap : private mbed::NonCopyable<CGap>, public ble::Gap::EventHandler {
    public:
        CGap(BLE& ble_interface, events::EventQueue& event_queue, const char* device_name) : ble_interface_(ble_interface), event_queue_(event_queue), led(LED1), adv_builder_(buffer), device_name_(device_name) {
            t1.start(callback(&event_queue_, &EventQueue::dispatch_forever));
        }

        void onDisconnectionComplete(const ble::DisconnectionCompleteEvent& event) override;

        void onConnectionComplete(const ble::ConnectionCompleteEvent& event) override;

        void set_init_callback(mbed::Callback<void()> cb);

        void start_advertisement();

        void toggle_led();

        void start_blink();

        void on_ble_init_complete(BLE::InitializationCompleteCallbackContext* context);


        void run();


    private:
        BLE& ble_interface_;
        events::EventQueue& event_queue_;
        DigitalOut led;
        ble::AdvertisingDataBuilder adv_builder_;
        events::EventQueue led_events;
        uint8_t buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
        const char* device_name_;
        mbed::Callback<void()> _on_ble_init_callback = nullptr;
        Thread t1;
};
