#pragma once
#include "mbed.h"
#include <BLE.h>
#include "gap.hpp"


class CSecureGap : public ble::SecurityManager::EventHandler, public CGap {
    public:
        using CGap::CGap;

        void OnBleStackInit(BLE::InitializationCompleteCallbackContext* context) override;

        void onConnectionComplete(const ble::ConnectionCompleteEvent& event) override;

        virtual void pairingRequest(ble::connection_handle_t connectionHandle) override {
            std::cout << "Pairing requested - authorising" << std::endl;
            ble_interface_.securityManager().acceptPairingRequest(connectionHandle);
        }

        virtual void linkEncryptionResult(ble::connection_handle_t connectionHandle, ble::link_encryption_t result)
         override {
            if(result == ble::link_encryption_t::ENCRYPTED) {
                std::cout << "Link ENCRYPTED" << std::endl;
            } 
            else if(result == ble::link_encryption_t::ENCRYPTED_WITH_MITM) {
                std::cout << "Link ENCRYPTED_WITH_MITM" << std::endl;
            } 
            else if(result == ble::link_encryption_t::NOT_ENCRYPTED) {
                std::cout << "Link NOT_ENCRYPTED" << std::endl;
            }
        }

        virtual void passkeyDisplay(ble::connection_handle_t connectionHandle, const SecurityManager::Passkey_t passkey)
         override {
            std::cout << "Input passKey: ";
            for(unsigned i = 0; i < ble::SecurityManager::PASSKEY_LEN; i++) {
            printf("%c ", passkey[ble::SecurityManager::PASSKEY_LEN - 1 - i]);
            }
            std::cout << std::endl;
        }

        virtual void confirmationRequest(ble::connection_handle_t connectionHandle) override {
            std::cout << "Conf required!" << std::endl;
        }

        virtual void passkeyRequest(ble::connection_handle_t connectionHandle) override {
            std::cout << "passkeyRequest" << std::endl; 
        }

        virtual void keypressNotification(ble::connection_handle_t connectionHandle, SecurityManager::Keypress_t keypress) 
        override {
            std::cout << "keypressNotification" << std::endl;
        }

        virtual void signingKey(ble::connection_handle_t connectionHandle,
        const ble::csrk_t* csrk, bool authenticated) override {
            std::cout << "signingKey" << std::endl;
        }

        virtual void pairingResult(ble::connection_handle_t connectionHandle,
        SecurityManager::SecurityCompletionStatus_t result) override {
            printf("Security status 0x%02x\r\n", result);
            if(result == SecurityManager::SEC_STATUS_SUCCESS) {
                std::cout << "Security success" << std::endl;
            } 
            else {
                std::cout << "Security failed" << std::endl;
            }
        }


    private:
        ble::peripheral_privacy_configuration_t privacyConfiguration;
};