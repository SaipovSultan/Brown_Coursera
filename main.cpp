#include <iostream>
#include <map>
#include <string>
#include <queue>


using namespace std;

class HotelManager {
public:
    void Book(string hotel_name, int64_t time, int client_id, int room_count) {
        current_time = time;
        hotels[hotel_name].Book({time, client_id, room_count});
    }

    int ComputeClientsCount(string hotel_name) {
        return hotels[hotel_name].ComputeClientsCount(current_time);
    }

    int ComputeRoomsCount(string hotel_name) {
        return hotels[hotel_name].ComputeRoomsCount(current_time);
    }

private:
    struct Booking {
        int64_t time;
        int client_id;
        int room_count;
    };

    class HotelInfo {
    public:
        void Book(const Booking& booking) {
            last_bookings.push(booking);
            rooms_count += booking.room_count;
            client_booking_count[booking.client_id] += 1;
        }

        int ComputeClientsCount(int64_t current_time) {
            RemoveOldBookings(current_time);
            return client_booking_count.size();
        }

        int ComputeRoomsCount(int64_t current_time) {
            RemoveOldBookings(current_time);
            return rooms_count;
        }
    private:
        queue<Booking> last_bookings;
        map<int, int> client_booking_count;
        int rooms_count = 0;
        static const int TIME_WINDOW_SIZE = 86400;

        void RemoveOldBookings(int64_t current_time) {
            while(!last_bookings.empty() && current_time - last_bookings.front().time >= TIME_WINDOW_SIZE) {
                PopBooking();
            }
        }

        void PopBooking() {
            const Booking& booking = last_bookings.front();
            rooms_count -= booking.room_count;
            const auto cliet_stat_it = client_booking_count.find(booking.client_id);
            if(--cliet_stat_it->second == 0) {
                client_booking_count.erase(booking.client_id);
            }
            last_bookings.pop();
        }
    };

    int64_t current_time;
    map<string, HotelInfo> hotels;
};

int main() {
    int count_query;
    cin >> count_query;
    string type_query;
    HotelManager hotelManager;
    for(int i = 0;i < count_query;++i) {
        cin >> type_query;
        if(type_query == "BOOK") {
            int64_t time, client_id;
            short room_count;
            string hotel_name;
            cin >> time >> hotel_name >> client_id >> room_count;
            hotelManager.Book(hotel_name, time, client_id, room_count);
        } else if(type_query == "CLIENTS") {
            string hotel_name;
            cin >> hotel_name;
            cout << hotelManager.ComputeClientsCount(hotel_name) << "\n";
        } else if(type_query == "ROOMS") {
            string hotel_name;
            cin >> hotel_name;
            cout << hotelManager.ComputeRoomsCount(hotel_name) << "\n";
        }
    }
    cout.flush();
    return 0;
}
