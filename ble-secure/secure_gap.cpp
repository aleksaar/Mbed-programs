#include "secure_gap.hpp"

void CSecureGap::OnBleStackInit(BLE::InitializationCompleteCallbackContext *context) {
    ble_error_t error;
        if(context->error) {
            std::cout << "Error during the initialisation" << std::endl;
            return;
        }
        // initialize security manager
        error =
          ble_interface_.securityManager().init(false /* Enable bonding*/,
                                                      true /*Require MITM protection*/,
                                                      SecurityManager::IO_CAPS_DISPLAY_ONLY/*IO capabilities*/,
                                                      NULL /*Passkey*/,
                                                      false /*Support data signing*/);
        CGap::printError(error, "_ble.securityManager().init() ");
        if(error != BLE_ERROR_NONE) {
          return;
        }
        // enable legacy pairing
        ble_interface_.securityManager().allowLegacyPairing(true);
        // set the event handler to this object
        ble_interface_.securityManager().setSecurityManagerEventHandler(this);
        //
        ble_interface_.securityManager().setPairingRequestAuthorisation(true);

        // start advertisement using CGAP instance of this class
        CGap::start_advertisement();

        ble::own_address_type_t addr_type;
        ble::address_t address;
        auto& a_t = addr_type;
        auto& a = address;
        context->ble.gap().getAddress(a_t, a);
        CGap::printDeviceAddress(address);

        printf("callback is: ");
        if (_on_ble_init_callback != nullptr) {
            printf("not null\n");
            _on_ble_init_callback();
        }
        else {
            printf("null\n");
        }

        // Enable privacy
        error = ble_interface_.gap().enablePrivacy(true);

        CGap::printError(error, "_ble.gap().enablePrivacy() ");
        // Configure privacy settings
        privacyConfiguration.use_non_resolvable_random_address = false;
        privacyConfiguration.resolution_strategy = ble::peripheral_privacy_configuration_t::REJECT_NON_RESOLVED_ADDRESS;
        ble_interface_.gap().setPeripheralPrivacyConfiguration(&privacyConfiguration);

        CGap::start_blink();
}

void CSecureGap::onConnectionComplete(const ble::ConnectionCompleteEvent& event) {
    CGap::printError(event.getStatus(), "onConnectionComplete() ");
    ble::connection_handle_t handle = event.getConnectionHandle();
    ble_error_t error = ble::BLE::Instance().
                          securityManager().
                          setLinkSecurity(handle,
                                          SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM);
    CGap::printError(error, "_ble.securityManager().setLinkSecurity() ");
    led_events.break_dispatch();
    led = 0;
}
