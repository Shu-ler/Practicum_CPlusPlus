#pragma once
#include "flight_provider.h"
#include "hotel_provider.h"

#include <string>
#include <vector>
#include <memory>
#include <utility>

class Trip {
public:
    
    // Конструктор: принимает ссылки на провайдеров
    Trip(HotelProvider & hp, FlightProvider & fp) 
        : hotel_provider_(hp)
        , flight_provider_(fp) {
    }

    // Удаляем конструктор копирования — нельзя копировать владение бронями
    Trip(const Trip&) = delete;

    // Конструктор перемещения
    Trip(Trip&& other) noexcept
        : hotel_provider_(other.hotel_provider_)
        , flight_provider_(other.flight_provider_)
        , hotels_(std::move(other.hotels_))
        , flights_(std::move(other.flights_)) {
        // Обнуляем, чтобы деструктор other не отменил наши брони
        other.hotels_.clear();
        other.flights_.clear();
    }

    // Запрещаем копирование
    Trip& operator=(const Trip&) = delete;

    // Перемещение 
    Trip& operator=(Trip&& other) noexcept {
        if (this != &other) {
            // Отменяем свои брони
            Cancel();

            // Забираем чужие
            hotels_ = std::move(other.hotels_);
            flights_ = std::move(other.flights_);

            // Чужие обнуляем
            other.hotels_.clear();
            other.flights_.clear();
        }
        return *this;
    };

    void Cancel() {
        for (auto it = flights_.rbegin(); it != flights_.rend(); ++it) {
            flight_provider_.Cancel(*it);
        }
        for (auto it = hotels_.rbegin(); it != hotels_.rend(); ++it) {
            hotel_provider_.Cancel(*it);
        }
        flights_.clear();
        hotels_.clear();
    }

    void BookFlight(const std::string& from, const std::string& to, const std::string& date) {
        FlightProvider::BookingData data;
        data.city_from = from;
        data.city_to = to;
        data.date = date;
        flights_.push_back(flight_provider_.Book(data));
    }

    void BookHotel(const std::string& city, const std::string& date_from, const std::string& date_to) {
        HotelProvider::BookingData data;
        data.city = city;
        data.date_from = date_from;
        data.date_to = date_to;
        hotels_.push_back(hotel_provider_.Book(data));
    }
    
    ~Trip() {
        Cancel();
    };

private:
    HotelProvider& hotel_provider_;
    FlightProvider& flight_provider_;

    std::vector<HotelProvider::BookingId> hotels_;
    std::vector<FlightProvider::BookingId> flights_;
};

class TripManager {
public:
    using BookingId = std::string;
    struct BookingData {
        std::string city_from;
        std::string city_to;
        std::string date_from;
        std::string date_to;
    };

    Trip Book(const BookingData& data) {
        Trip trip(hotel_provider_, flight_provider_);

        trip.BookFlight(data.city_from, data.city_to, data.date_from);
        trip.BookHotel(data.city_to, data.date_from, data.date_to);
        trip.BookFlight(data.city_to, data.city_from, data.date_to);

        return trip;
    }

    void Cancel(Trip& trip) {
        trip.Cancel();
    }

private:
    HotelProvider hotel_provider_;
    FlightProvider flight_provider_;
};
