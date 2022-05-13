#include "gap.hpp"


void CGap::onDisconnectionComplete(const ble::DisconnectionCompleteEvent& event) {
    printf("disconnection complete\n");
    start_advertisement();
    start_blink();
}

void CGap::onConnectionComplete(const ble::ConnectionCompleteEvent& event) {
    printf("connection complete\n");
    led_events.break_dispatch();
    led = 0;
}

void CGap::set_init_callback(mbed::Callback<void()> cb) {
    _on_ble_init_callback = cb;
}


void CGap::start_advertisement() {
    printf("starting advertising\n");
    ble::AdvertisingParameters adv_params (
        ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
        ble::adv_interval_t(ble::millisecond_t(100))
    );
    printf("Device name: %s\n", device_name_);
    adv_builder_.setFlags();
    adv_builder_.setName(device_name_);
    unsigned char advString = 'A';
    Span<unsigned char> advSpan = mbed::make_Span(&advString, 1);     
    adv_builder_.addData(ble::adv_data_type_t::MANUFACTURER_SPECIFIC_DATA, advSpan);


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

void CGap::toggle_led() {
    led = !led;
}

void CGap::start_blink() {
    printf("starting blink\n");
    led_events.call_every(500ms, callback(this, &CGap::toggle_led));
    led_events.dispatch_forever();
}

void CGap::on_ble_init_complete(BLE::InitializationCompleteCallbackContext* context) {
    if (context->error) {
        printf("init failed\n");
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

    printf("callback is: ");
    if (_on_ble_init_callback != nullptr) {
        printf("not null\n");
        _on_ble_init_callback();
    }
    else {
        printf("null\n");
    }

    start_advertisement();
    start_blink();
}


void CGap::run() {
    ble_interface_.init(this, &CGap::on_ble_init_complete);
    ble_interface_.gap().setEventHandler(this);
    printf("running\n");
    event_queue_.dispatch_forever();
}
