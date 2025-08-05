#include <vector>
#include <optional>
#include <string>
#include <algorithm>
#include <numeric>

struct Package {
    std::string sender{};
    std::string addressee{};
    double weight = 0;
};

inline double operator+(const Package& lhs, const Package& rhs) {
    return lhs.weight + rhs.weight;
}

inline bool operator<(const Package& lhs, const Package& rhs) {
    return lhs.weight < rhs.weight;
} 

class Post {
public:
    Post() = default;
    
    // У почтового отделения есть ограничение на количество посылок. 
    void SetPostCapacity(size_t capacity) {
        packages_.reserve(capacity);
        capacity_ = capacity;
    }
    
    // Функция добавления посылки.
    void AddPackage(const std::string& sender, const std::string& addressee, double weight) {
        if (GetFreePlace() <= 0) {
            return;
        }
        packages_.push_back(Package{
            .sender = sender,
            .addressee = addressee,
            .weight = weight
        });
    }
    
    double CalculateAverageWeight() {
        auto packages_size = static_cast<double>(packages_.size());
        return packages_size > 0 ? CalculateTotalWeight() / packages_size : 0.0;
    }
    
    std::optional<Package> FindHeaviestPackage() {
        auto HeaviestPackage = std::max_element(packages_.begin(), packages_.end());
        if (HeaviestPackage == packages_.end()) {
            return std::nullopt;
        }
        return *HeaviestPackage;
    }
    
    std::optional<Package> FindLightestPackage() {
        auto LightestPackage = std::min_element(packages_.begin(), packages_.end());
        if (LightestPackage == packages_.end()) {
            return std::nullopt;
        }
        return *LightestPackage;
    }
    
    double CalculateTotalWeight() {
        return std::accumulate(packages_.begin(), packages_.end(), 0.0, [](double val, const Package& p){
            return val + p.weight;
        });
    }
    
    std::optional<Package> FindFirstSenderByAddress(const std::string& address) {
        auto res = std::find_if(packages_.begin(), packages_.end(), [&](const Package& t){
            return t.addressee == address;
        });
        if (res == packages_.end()) {
            return std::nullopt;
        } else {
            return *res;
        }
    }
    
    int CalculatePackageForAddress(const std::string& address) {
        return std::count_if(packages_.begin(), packages_.end(), [&](const Package& t){
            return t.addressee == address;
        });
    }
    
private:
    int GetFreePlace() {
        return capacity_ - HowManyPackages();
    }
    
    size_t HowManyPackages() {
        return packages_.size();
    }
    
private:
    std::vector<Package> packages_;
    size_t capacity_ = 0;
};
