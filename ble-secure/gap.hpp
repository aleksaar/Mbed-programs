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

        virtual void OnBleStackInit(BLE::InitializationCompleteCallbackContext* context);

        static void printDeviceAddress(ble::address_t& address) {
            printf("Device address ");
            for (int i=5; i>=1;i--) {
                printf("%02x:", address[i]);
            }
            printf("%02x\r\n", address[0]);
        }

        static void printError(ble_error_t error, const char *message) {
            //std::cout << message << std::endl;
            printf("%s\n",message);
            ble::BLE::errorToString(error);
        }


        void run();


    protected:
        BLE& ble_interface_;
        events::EventQueue led_events;
        DigitalOut led;
        mbed::Callback<void()> _on_ble_init_callback = nullptr;


    private:
        events::EventQueue& event_queue_;
        ble::AdvertisingDataBuilder adv_builder_;
        uint8_t buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
        const char* device_name_;
        Thread t1;
};

