#pragma once

#include <bitset>
#include <iostream>
#include <sstream>

enum Permissions {READ = 1, WRITE = 2, EXECUTE = 4};

class FileAccess {
public:
    FileAccess() : permissions_(0) {}

    // Устанавливает указанный флаг.
    void SetPermission(Permissions perm);
   
    // Проверяет, есть ли указанный флаг.
    bool CheckPermission(Permissions perm);
    
    // Удаляет флаг.
    void RemovePermission(Permissions perm);
    
    // Инвертирует флаг.
    void TogglePermission(Permissions perm);
    
    // Преобразует набор прав в строку вида 0000.
    std::string ToString() {
        std::ostringstream s;
        s << std::bitset<4>(permissions_);
        return s.str();
    }
private:
    int permissions_;
};

inline void FileAccess::SetPermission(Permissions perm) {
    permissions_ = permissions_ | static_cast<int>(perm);
}

inline bool FileAccess::CheckPermission(Permissions perm) {
    return permissions_ & static_cast<int>(perm);
}

inline void FileAccess::RemovePermission(Permissions perm) {
    permissions_ = permissions_ & ~static_cast<int>(perm);
}

inline void FileAccess::TogglePermission(Permissions perm) {
    permissions_ = permissions_ ^ static_cast<int>(perm);
}
