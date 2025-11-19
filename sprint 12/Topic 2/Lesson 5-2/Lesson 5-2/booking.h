#pragma once

namespace raii {

    template <typename Provider>
    class Booking {
    private:
        using BookingId = typename Provider::BookingId;

        Provider* provider_ = nullptr;
        BookingId booking_id_;

    public:
        // Конструктор
        Booking(Provider* p, BookingId id)
            : provider_(p)
            , booking_id_(id) {
        }

        // Запрещаем копирование
        Booking(const Booking&) = delete;
        Booking& operator=(const Booking&) = delete;

        // Перемещение
        Booking(Booking&& other) noexcept
            : provider_(other.provider_)
            , booking_id_(other.booking_id_) {
            // Обнуляем исходный объект, чтобы деструктор не вызвал CancelOrComplete дважды
            other.provider_ = nullptr;
        }

        Booking& operator=(Booking&& other) noexcept {
            if (this != &other) {
                // Отменяем свою бронь (если была)
                if (provider_) {
                    provider_->CancelOrComplete(*this);
                }

                // Забираем чужую
                provider_ = other.provider_;
                booking_id_ = other.booking_id_;

                // Обнуляем чужой
                other.provider_ = nullptr;
            }
            return *this;
        }

        // Деструктор
        ~Booking() {
            if (provider_) {
                provider_->CancelOrComplete(*this);
            }
        }

        // Получение ID
        BookingId GetId() const {
            return booking_id_;
        }
    };

} // namespace raii