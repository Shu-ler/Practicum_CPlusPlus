#include <iostream>
#include <vector>
#include <sstream>

class WarehouseStorage {
public:
    WarehouseStorage(const std::vector<int>& items)
      : items_{items} {}
    
    WarehouseStorage operator+(const std::vector<int>& vec) const {
        WarehouseStorage stock(items_);
        for (auto sku : vec) {
            stock.items_.push_back(sku);
        }
        return stock;
    };

    bool CheckContains(const std::vector<int>& first, int val) const {
        bool cont = false;
        for (auto sku : first) {
            if (sku == val) {
                cont = true;
                break;
            }
        }
        return cont;
    };

    WarehouseStorage operator-(const std::vector<int>& vec) const {
        WarehouseStorage stock{{}};

        for (auto sku : items_) {
            if (!CheckContains(vec, sku)) {
                stock.items_.push_back(sku);
            }
        }
        return stock;
    };
    
    void Print(std::ostream &stream, const std::string& name) const {
        stream << name << ": ";
        int i{};
        for (auto &item : items_) {
            if (i++) {
                stream << " ";
            }
            stream << item;
        }
        stream << std::endl;
    }

private:
    std::vector<int> items_;
};

std::vector<int> Read(std::istream &stream) {
    std::string line;
    std::getline(stream, line);
    std::istringstream line_stream(line);

    std::vector<int> ret;
    int val;
    while(line_stream >> val) {
        ret.push_back(val);
    }

    return ret;
}

int main() {
    auto origin = Read(std::cin);
    auto items = Read(std::cin);
    
    const WarehouseStorage storage{origin};
    storage.Print(std::cout, "Origin");
    (storage + items).Print(std::cout, "Supply");
    (storage - items).Print(std::cout, "Disposal");
}
