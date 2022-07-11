#pragma once

namespace RAII {
    template <typename Provider>
    class Booking {
    public:
        Booking(Provider* provider_ptr, int booking_id) : provider_ptr(provider_ptr), booking_id(booking_id) {}

        Booking(const Booking&) = delete;

        Booking& operator = (const Booking&) = delete;

        Booking(Booking&& other) {
            provider_ptr = other.provider_ptr;
            booking_id = other.booking_id;
            other.provider_ptr = nullptr;
            other.booking_id = -1;
        }

        Booking& operator = (Booking&& other) {
            if(this != &other) {
                Cancel();
                provider_ptr = other.provider_ptr;
                booking_id = other.booking_id;
                other.provider_ptr = nullptr;
                other.booking_id = -1;
            }
        }

        ~Booking() {
            Cancel();
        }
    private:
        void Cancel() {
            if(provider_ptr) provider_ptr->CancelOrComplete(*this);
        }

        Provider* provider_ptr;
        int booking_id;
    };
}