✅ Шаг 2: Создание структуры проекта
Откройте PowerShell и выполните:

POWERSHELL
# Создайте папку проекта
mkdir hello
cd hello

# Создайте папку для исходников
mkdir src

✅ Шаг 3: Создание src/main.cpp
Выполните:

POWERSHELL
cat > src/main.cpp << 'EOL'
// Подключим библиотеку Boost.Optional, чтобы убедиться, что Boost подключен успешно
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <iostream>

int main() {
    // Шаблон boost::optional — прообраз std::optional, используем его здесь для примера
    boost::optional<int> opt;
    std::cout << opt << std::endl;  // Выведет "not initialized"
}
EOL

Или просто создайте файл вручную.

✅ Шаг 4: Создание conanfile.txt
POWERSHELL
cat > conanfile.txt << 'EOL'
[requires]
boost/1.78.0

[generators]
cmake
EOL

Этот файл говорит Conan:


Нужен Boost 1.78.0
Сгенерировать conanbuildinfo.cmake для CMake

✅ Шаг 5: Создание CMakeLists.txt
POWERSHELL
cat > CMakeLists.txt << 'EOL'
cmake_minimum_required(VERSION 3.11)

project(Hello CXX)
set(CMAKE_CXX_STANDARD 20)

# Подключаем скрипт от Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Поддержка потоков (на всякий случай)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Основной исполняемый файл
add_executable(hello src/main.cpp)

# Подключаем библиотеки: потоки + все от Conan (включая Boost)
target_link_libraries(hello PRIVATE Threads::Threads ${CONAN_LIBS})
EOL

✅ Шаг 6: Настройка профиля Conan (важно!)
Conan должен знать, что вы используете Visual Studio 2022.

Откройте профиль по умолчанию:
POWERSHELL
notepad $env:USERPROFILE\.conan\profiles\default
Замените содержимое на:

INI
[settings]
os=Windows
arch=x86_64
compiler=Visual Studio
compiler.version=17
compiler.runtime=MD
build_type=Release

[options]
[build_requires]
[env]

⚠️ compiler.version=17 — это VS 2022

Если у вас VS 2019 — поставьте 16

✅ Шаг 7: Создание папки сборки и установка зависимостей
POWERSHELL
# Создаём папку build
mkdir build
cd build

# Устанавливаем зависимости через Conan
conan install .. --output-folder=. --build=missing
# Устанавливаем зависимости через Conan для папки out/build/x64-Debug для Visual Studio
conan install ..\..\.. --output-folder=. --build=missing
 
✅ После этого в папке build появятся:

conanbuildinfo.cmake
conaninfo.txt
Информация о Boost
✅ Шаг 8: Генерация проекта через CMake
POWERSHELL
cmake -S .. -B . -DCONAN_DISABLE_CHECK_COMPILER=ON

⚠️ Флаг -DCONAN_DISABLE_CHECK_COMPILER=ON временно нужен, если версия MSVC не совпадает (например, Conan думает, что у вас 16, а у вас 17).

Как только профиль обновлён — этот флаг можно убрать.


✅ Шаг 9: Сборка проекта
POWERSHELL
cmake --build . --config Release
✅ После сборки:

В папке build/Release/ будет hello.exe
Или в build/ (если используется single-config генератор)

✅ Шаг 10: Запуск
POWERSHELL
.\Release\hello.exe
Ожидаемый вывод:

not initialized

Это значит: boost::optional<int> не инициализирован → всё работает!


📌 Итоговая структура проекта
hello/
│
├── conanfile.txt
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── build/
    ├── conanbuildinfo.cmake
    ├── Hello.sln
    ├── hello.vcxproj
    └── Release/
        └── hello.exe

🔁 Что делать при пересоздании?
Просто удалите папку build и повторите шаги 7–10:

POWERSHELL
rm -Recurse -Force build
mkdir build
cd build
conan install ..
cmake -S .. -B .
cmake --build . --config Release
