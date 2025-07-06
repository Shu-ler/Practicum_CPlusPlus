#pragma once

#include "universe_listener.h"
#include "email_sender.h"
#include <format>

inline std::string GetAddressByType(UniverseSignalType type) {
    using namespace std::literals;
    switch(type) {
    case UniverseSignalType::UFO :
        return "ufo@secret-committee.gov"s;
    case UniverseSignalType::SUPERNOVA:
        return "evacuation@transport-department.gov"s;
    case UniverseSignalType::DYSON_SPHERE:
        return "alien-contacts@secret-committee.gov"s;
    case UniverseSignalType::GRAVITATIONAL_LENS:
        return "astronomy@university.edu"s;
    }
    abort();
}

class SignalProcessor {
public:
    explicit SignalProcessor(UniverseListener& listener, EmailSender& sender) {
        using namespace std::literals;
        // Порог силы сигнала. Нужно игнорировать сигнал, если 
        // его сила ниже этого числа.
        static const double threshold = 0.8;

        // Создайте шаблонную лямбда-функцию,
        // получающую сигнал и отправляющую электронное 
        // письмо.
        auto sendmail = [&]<UniverseSignalType type>(const UniverseSignal<type>& signal) { 
            if (signal.info.signal_strength >= threshold) {
                std::string subject = "Обнаружен объект: "s + signal.payload.name;
                std::string body = std::format("В области неба {};{} обнаружен указанный объект. Сила сигнала: {}.", 
                    signal.info.coordinates.ra, 
                    signal.info.coordinates.dec, 
                    signal.info.signal_strength); 
                std::string address = GetAddressByType(type);
                auto attachment = signal.payload; 

                sender.Send(subject, body, address, attachment);
            }
        };
        
        // Передайте эту функцию во все четыре метода
        // SetCallback*.
        listener.SetCallbackUfo(sendmail);
        listener.SetCallbackSupernova(sendmail);
        listener.SetCallbackDysonSphere(sendmail);
        listener.SetCallbackGravitationalLens(sendmail);
    }
};
