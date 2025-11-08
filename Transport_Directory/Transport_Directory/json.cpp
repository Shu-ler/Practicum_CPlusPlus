#include "json.h"
#include <cctype>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <cmath>

namespace json {

    namespace detail {

        // Предварительные объявления
        struct PrintContext;
        void PrintValue(const Value&, const PrintContext&);
        void PrintArray(const Array&, const PrintContext&);
        void PrintDict(const Dict&, const PrintContext&);

        /**
         * @brief Контекст вывода JSON с поддержкой отступов.
         *
         * Используется для красивого форматирования JSON-документа.
         * Хранит ссылку на выходной поток и текущий уровень отступа.
         * Позволяет:
         * - Выводить отступы с помощью PrintIndent()
         * - Создавать вложенный контекст с увеличенным отступом через Indented()
         *
         * Пример использования:
         * @code
         * PrintContext ctx{std::cout, 2}; // шаг отступа = 2 пробела
         * ctx.PrintIndent(); // выведет 0 пробелов (текущий уровень = 0)
         *
         * auto child = ctx.Indented(); // создаёт контекст с отступом 2
         * child.PrintIndent(); // выведет 2 пробела
         * @endcode
         */
        struct PrintContext {
            std::ostream& out;           ///< Ссылка на поток вывода
            int indent_step = 4;         ///< Шаг увеличения отступа (по умолчанию 4 пробела)
            int indent = 0;              ///< Текущий уровень отступа в пробелах

            /**
             * @brief Выводит текущее количество пробелов в поток.
             *
             * Использует создание строки нужной длины — более эффективно,
             * чем посимвольная запись.
             */
            void PrintIndent() const {
                if (indent > 0) {
                    out << std::string(indent, ' ');
                }
            }

            /**
             * @brief Создаёт новый контекст с увеличенным отступом.
             *
             * Используется при переходе на следующий уровень вложенности:
             * - при входе в массив
             * - при входе в объект (Dict)
             *
             * @return Новый PrintContext с уровнем отступа: indent + indent_step
             */
            PrintContext Indented() const {
                return { out, indent_step, indent + indent_step };
            }
        };

        /**
         * @brief Рекурсивно выводит значение JSON в поток с красивым форматированием.
         *
         * Использует std::visit для обработки всех типов variant.
         * Для Array и Dict вызывает отдельные функции с поддержкой отступов.
         *
         * @param value Значение JSON
         * @param ctx Контекст вывода (поток, отступы)
         */
        void PrintValue(const Value& value, const PrintContext& ctx);

        /**
         * @brief Выводит массив в формате JSON с красивым форматированием.
         *
         * Пустой массив → "[]"
         * Непустой массив → многострочный формат с отступами.
         *
         * @param arr Массив JSON-узлов
         * @param ctx Контекст вывода
         */
        void PrintArray(const Array& arr, const PrintContext& ctx);

        /**
         * @brief Выводит словарь (объект JSON) в формате с красивым форматированием.
         *
         * Если словарь пуст, выводит "{}".
         * Иначе выводит каждый ключ-значение на новой строке с отступами.
         * Между парами ставится запятая.
         *
         * Ключ всегда — строка, выводится в двойных кавычках.
         * Значение выводится рекурсивно через PrintValue.
         *
         * @param dict Словарь узлов JSON
         * @param ctx Контекст вывода (поток и текущий уровень отступа)
         */
        void PrintDict(const Dict& dict, const PrintContext& ctx);

        // Реализации
        void PrintValue(const Value& value, const PrintContext& ctx) {
            std::visit([&ctx](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                std::ostream& out = ctx.out;

                if constexpr (std::is_same_v<T, std::nullptr_t>) {
                    out << "null";
                }
                else if constexpr (std::is_same_v<T, int>) {
                    out << v;
                }
                else if constexpr (std::is_same_v<T, double>) {
                    std::ostringstream tmp;
                    // tmp << std::setprecision(std::numeric_limits<double>::max_digits10) << v;
                    tmp << std::fixed << std::setprecision(6) << v;
                    std::string str = tmp.str();
                    if (str.find('.') != std::string::npos) {
                        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                        if (!str.empty() && str.back() == '.') {
                            str.pop_back();
                        }
                    }
                    out << str;
                }
                else if constexpr (std::is_same_v<T, bool>) {
                    out << (v ? "true" : "false");
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    out << '"';
                    for (char c : v) {
                        switch (c) {
                        case '"':  out << "\\\""; break;
                        case '\\': out << "\\\\"; break;
                        case '\r': out << "\\r";  break;
                        case '\n': out << "\\n";  break;
                        case '\t': out << "\\t";  break;
                        default:   out << c;      break;
                        }
                    }
                    out << '"';
                }
                else if constexpr (std::is_same_v<T, Array>) {
                    PrintArray(v, ctx);
                }
                else if constexpr (std::is_same_v<T, Dict>) {
                    PrintDict(v, ctx);
                }
                }, value);
        }

        void PrintArray(const Array& arr, const PrintContext& ctx) {
            std::ostream& out = ctx.out;

            if (arr.empty()) {
                out << "[]";
                return;
            }

            out << "[\n";
            PrintContext indented = ctx.Indented();
            bool first = true;
            for (const auto& item : arr) {
                if (!first) {
                    out << ",\n";
                }
                first = false;
                indented.PrintIndent();
                PrintValue(item.GetValue(), indented);
            }
            out << '\n';
            ctx.PrintIndent();
            out << ']';
        }

        void PrintDict(const Dict& dict, const PrintContext& ctx) {
            std::ostream& out = ctx.out;

            if (dict.empty()) {
                out << "{}";
                return;
            }

            out << "{\n";
            PrintContext indented = ctx.Indented();
            bool first = true;
            for (const auto& [key, value] : dict) {
                if (!first) {
                    out << ",\n";
                }
                first = false;
                indented.PrintIndent();
                PrintValue(key, PrintContext{ out });
                out << ": ";
                PrintValue(value.GetValue(), indented);
            }
            out << '\n';
            ctx.PrintIndent();
            out << '}';
        }

    } // namespace detail

    // =============================================================================
    // Реализация Node
    // =============================================================================

    Node::Node(int value) : value_(value) {}
    Node::Node(double value) : value_(value) {}
    Node::Node(bool value) : value_(value) {}
    Node::Node(const std::string& value) : value_(value) {}
    Node::Node(std::string&& value) : value_(std::move(value)) {}
    Node::Node(std::nullptr_t) : value_(nullptr) {}
    Node::Node(const Array& array) : value_(array) {}
    Node::Node(Array&& array) : value_(std::move(array)) {}
    Node::Node(const Dict& map) : value_(map) {}
    Node::Node(Dict&& map) : value_(std::move(map)) {}

    bool Node::IsInt() const { return std::holds_alternative<int>(value_); }
    bool Node::IsPureDouble() const { return std::holds_alternative<double>(value_); }
    bool Node::IsDouble() const { return IsInt() || IsPureDouble(); }
    bool Node::IsBool() const { return std::holds_alternative<bool>(value_); }
    bool Node::IsString() const { return std::holds_alternative<std::string>(value_); }
    bool Node::IsNull() const { return std::holds_alternative<std::nullptr_t>(value_); }
    bool Node::IsArray() const { return std::holds_alternative<Array>(value_); }
    bool Node::IsMap() const { return std::holds_alternative<Dict>(value_); }

    int Node::AsInt() const {
        if (!IsInt()) throw std::logic_error("Not an int");
        return std::get<int>(value_);
    }

    bool Node::AsBool() const {
        if (!IsBool()) throw std::logic_error("Not a bool");
        return std::get<bool>(value_);
    }

    double Node::AsDouble() const {
        if (IsInt()) return static_cast<double>(AsInt());
        if (IsPureDouble()) return std::get<double>(value_);
        throw std::logic_error("Not a number");
    }

    const std::string& Node::AsString() const {
        if (!IsString()) throw std::logic_error("Not a string");
        return std::get<std::string>(value_);
    }

    const Array& Node::AsArray() const {
        if (!IsArray()) throw std::logic_error("Not an array");
        return std::get<Array>(value_);
    }

    const Dict& Node::AsMap() const {
        if (!IsMap()) throw std::logic_error("Not a map");
        return std::get<Dict>(value_);
    }

    bool Node::operator==(const Node& rhs) const {
        return value_ == rhs.value_;
    }

    bool Node::operator!=(const Node& rhs) const {
        return !(*this == rhs);
    }

    const Value& Node::GetValue() const {
        return value_;
    }

    Value& Node::GetValueRef() {
        return value_;
    }

    // =============================================================================
    // Реализация Document
    // =============================================================================

    Document::Document() : root_(nullptr) {}

    Document::Document(Node root) : root_(std::move(root)) {}

    const Node& Document::GetRoot() const { return root_; }

    bool Document::operator==(const Document& other) const {
        return root_ == other.root_;
    }

    bool Document::operator!=(const Document& other) const {
        // Используем перегруженный выше оператор равенства
        return !(*this == other);
    }

    // =============================================================================
    // Ввод / вывод
    // =============================================================================

    void Print(const Document& doc, std::ostream& output) {
        detail::PrintValue(doc.GetRoot().GetValue(), detail::PrintContext{ output });
    }

    std::istream& operator>>(std::istream& is, json::Document& doc) {
        try {
            doc = json::Load(is);
        }
        catch (const json::ParsingError& e) {
            is.setstate(std::ios::failbit);  // Устанавливаем флаг ошибки
        }
        catch (const std::exception&) {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const json::Document& doc) {
        json::Print(doc, os);
        return os;
    }

    // =============================================================================
    // Парсинг
    // =============================================================================
    namespace {

        bool IsDigit(char c);

        char PeekChar(std::istream& input);
        void CheckLiteral(std::istream& input, std::string_view literal);
        void ExpectChar(std::istream& input, char expected);

        std::string ParseString(std::istream& input);
        Node ParseNumber(std::istream& input);
        Node ParseArray(std::istream& input);
        Node ParseDict(std::istream& input);
        Node ParseNode(std::istream& input);

        /**
         * @brief Проверяет, является ли символ цифрой ('0'–'9').
         *
         * Безопасная альтернатива std::isdigit, не вызывающая неопределённого поведения
         * при отрицательных значениях char.
         *
         * @param c Символ для проверки
         * @return true если c — цифра от '0' до '9', иначе false
         *
         * @note Используется вместо std::isdigit для безопасности.
         *       Подходит для парсинга ASCII-цифр в JSON.
         */
        bool IsDigit(char c) {
            return c >= '0' && c <= '9';
        }

        /**
         * @brief Возвращает следующий непробельный символ из потока без извлечения.
         *
         * Пропускает все пробельные символы (используя std::ws), затем смотрит следующий символ.
         * Если поток достиг EOF, выбрасывает ParsingError.
         *
         * @param input Входной поток
         * @return char Следующий непробельный символ
         * @throws ParsingError Если достигнут конец потока
         *
         * @note Используется как первый шаг при определении типа JSON-значения.
         */
        char PeekChar(std::istream& input) {
            std::ws(input);
            int c = input.peek();
            if (c == EOF) {
                throw ParsingError("Unexpected end of input");
            }
            return static_cast<char>(c);
        }

        /**
         * @brief Проверяет, что в потоке находится ожидаемая строка.
         *
         * Извлекает из потока символы, соответствующие строке `literal`.
         * Если символы не совпадают или поток заканчивается раньше — выбрасывает ошибку.
         *
         * Используется для распознавания литералов: true, false, null.
         *
         * @param input Входной поток
         * @param literal Ожидаемая строка (например, "true")
         * @throws ParsingError Если строка не совпадает или поток обрывается
         *
         * @example
         * CheckLiteral(input, "null"); // ожидаем, что дальше идёт "null"
         */
        void CheckLiteral(std::istream& input, std::string_view literal) {
            for (char expected : literal) {
                char c;
                if (!input.get(c) || c != expected) {
                    throw ParsingError("Invalid literal: expected '" + std::string(literal) + "'");
                }
            }
        }

        /**
         * @brief Ожидает и проверяет наличие конкретного символа в потоке.
         *
         * Считывает следующий непробельный символ из потока и проверяет,
         * совпадает ли он с ожидаемым. Если нет — выбрасывает ParsingError.
         *
         * Используется для разбора синтаксических элементов JSON:
         * - открывающие/закрывающие скобки: [ ] { }
         * - разделители: , :
         * - начало строк: "
         *
         * @param input Входной поток
         * @param expected Ожидаемый символ
         * @throws ParsingError Если символ отсутствует или не совпадает
         *
         * @example
         * ExpectChar(input, '{'); // убедится, что текущий токен — начало объекта
         */
        void ExpectChar(std::istream& input, char expected) {
            char c;
            if (!(input >> c) || c != expected) {
                throw ParsingError("Expected '" + std::string(1, expected) + "'");
            }
        }

        /**
         * @brief Парсит строку JSON, начиная с открывающей кавычки.
         *
         * Ожидается, что первый символ (кавычка) уже считан перед вызовом.
         * Функция читает символы до следующей незащищённой кавычки.
         * Поддерживает escape-последовательности:
         * - \\" → "
         * - \\\\ → \\
         * - \\n → \n
         * - \\r → \r
         * - \\t → \t
         * - \\/ → /
         * - \\b, \\f → соответствующие управляющие символы
         *
         * Управляющие символы (кроме разрешённых) недопустимы в JSON.
         *
         * @param input Входной поток (ожидается, что первый '"' уже прочитан)
         * @return std::string Распарсенная строка без кавычек и escape-последовательностей
         * @throws ParsingError При незавершённой строке, неизвестном escape-коде или управляющем символе
         *
         * @note Не добавляет в строку символ закрывающей кавычки — он просто завершает чтение.
         */
        std::string ParseString(std::istream& input) {
            std::string result;
            char c;

            while (input.get(c)) {
                if (c == '"') {
                    return result;
                }
                else if (c == '\\') {
                    if (!input.get(c)) {
                        throw ParsingError("Unexpected end of string after '\\'");
                    }
                    switch (c) {
                    case 'r':  result += '\r'; break;
                    case 'n':  result += '\n'; break;
                    case 't':  result += '\t'; break;
                    case '"':  result += '"';  break;
                    case '\\': result += '\\'; break;
                    case '/':  result += '/';  break;
                    case 'b':  result += '\b'; break;
                    case 'f':  result += '\f'; break;
                    default:
                        throw ParsingError("Invalid escape sequence: \\" + std::string(1, c));
                    }
                }
                else if (c >= 32 && c < 127) {
                    result += c;
                }
                else {
                    throw ParsingError("Invalid control character in string");
                }
            }

            throw ParsingError("Unterminated string");
        }

        /**
         * @brief Парсит число JSON: целое или с плавающей точкой.
         *
         * Поддерживает:
         * - Целые: 123, -456
         * - Дробные: 3.14, -0.5
         * - Экспоненциальную запись: 1e10, -2.5E-4
         *
         * Число распознаётся как int, если оно не содержит '.' или 'e'/'E'.
         * В противном случае — как double.
         * Если целое число выходит за пределы int, оно автоматически интерпретируется как double.
         *
         * @param input Входной поток
         * @return Node Узел типа int или double
         * @throws ParsingError При неверном формате, отсутствии цифр, переполнении или NaN/inf
         *
         * @note Использует std::stoll и std::stod с проверками на out_of_range и конечность значения.
         */
        Node ParseNumber(std::istream& input) {
            std::string num;
            char c;

            // Считываем первый символ (пропуская пробелы)
            if (!(input >> c)) {
                throw ParsingError("Unexpected end of input");
            }

            if (c == '-') {
                num += c;
                if (!(input >> c)) {
                    throw ParsingError("Unexpected end of input");
                }
            }

            if (!IsDigit(c)) {
                throw ParsingError("Invalid number format");
            }

            // Целая часть
            do {
                num += c;
                if (!input.get(c)) {
                    break;
                }
            } while (IsDigit(c)); // Проверяем уже следующий символ

            // Возвращаем последний символ, если это не цифра
            input.putback(c);

            // Дробная часть
            if (input.peek() == '.') {
                if (!input.get(c)) {
                    throw ParsingError("Unexpected end of input");
                }
                num += c;

                if (!IsDigit(input.peek())) {
                    throw ParsingError("Invalid number format: missing digits after '.'");
                }

                while (IsDigit(input.peek())) {
                    input.get(c);
                    num += c;
                }
            }

            // Экспонента
            if (input.peek() == 'e' || input.peek() == 'E') {
                input.get(c);
                num += c;

                // Знак экспоненты
                if (input.peek() == '+' || input.peek() == '-') {
                    input.get(c);
                    num += c;
                }

                if (!IsDigit(input.peek())) {
                    throw ParsingError("Invalid number format: missing digits after exponent");
                }

                while (IsDigit(input.peek())) {
                    input.get(c);
                    num += c;
                }
            }

            // Парсинг
            try {
                if (num.find_first_of(".eE") != std::string::npos) {
                    double value = std::stod(num);
                    if (!std::isfinite(value)) {
                        throw ParsingError("Invalid number: infinity or NaN");
                    }
                    return Node(value);
                }
                else {
                    long long int_value = std::stoll(num);
                    if (int_value > std::numeric_limits<int>::max() ||
                        int_value < std::numeric_limits<int>::min()) {
                        return Node(static_cast<double>(int_value));
                    }
                    return Node(static_cast<int>(int_value));
                }
            }
            catch (const std::out_of_range&) {
                throw ParsingError("Number out of range");
            }
            catch (const std::exception&) {
                throw ParsingError("Invalid number format");
            }
        }


        /**
         * @brief Парсит массив JSON, начиная с '['.
         *
         * Ожидается, что открывающая скобка '[' уже прочитана перед вызовом.
         * Элементы разделяются запятыми, пробелы игнорируются.
         * Пустой массив: [] — допустим.
         *
         * @param input Входной поток
         * @return Node Узел, содержащий Array
         * @throws ParsingError При отсутствии ']', неправильных элементах или отсутствии запятых
         *
         * @note Вызывает ParseNode для каждого элемента (рекурсивно).
         */
        Node ParseArray(std::istream& input) {
            ExpectChar(input, '[');

            Array arr;
            bool first = true;
            while (PeekChar(input) != ']') {
                if (!first) {
                    ExpectChar(input, ',');
                }
                arr.push_back(ParseNode(input));
                first = false;
            }
            ExpectChar(input, ']');
            return Node(std::move(arr));
        }

        /**
         * @brief Парсит объект JSON (словарь), начиная с '{'.
         *
         * Ожидается, что открывающая фигурная скобка '{' уже прочитана.
         * Каждая пара "ключ: значение" разделяется запятой.
         * Ключ всегда — строка в кавычках.
         * Значение — любой валидный JSON-узел.
         *
         * @param input Входной поток
         * @return Node Узел, содержащий Dict
         * @throws ParsingError При синтаксических ошибках, дубликатах ключей или отсутствии '}'.
         *
         * @note Дублирование ключей не разрешено — выбрасывается ошибка.
         *       Это строже стандарта, но помогает избежать ошибок.
         */
        Node ParseDict(std::istream& input) {
            ExpectChar(input, '{');

            Dict dict;
            bool first = true;
            while (PeekChar(input) != '}') {
                if (!first) {
                    ExpectChar(input, ',');
                }
                ExpectChar(input, '"');
                std::string key = ParseString(input);
                ExpectChar(input, ':');
                auto [it, inserted] = dict.emplace(std::move(key), ParseNode(input));
                if (!inserted) {
                    throw ParsingError("Duplicate key in object: " + it->first);
                }
                first = false;
            }
            ExpectChar(input, '}');
            return Node(std::move(dict));
        }

        /**
         * @brief Главная функция парсинга: определяет тип следующего узла по первому символу.
         *
         * Выполняет разбор в зависимости от текущего токена:
         * - " → строка
         * - { → объект
         * - [ → массив
         * - t → true
         * - f → false
         * - n → null
         * - цифра или '-' → число
         *
         * Пробелы перед токеном автоматически пропускаются.
         *
         * @param input Входной поток
         * @return Node Распарсенный узел любого типа
         * @throws ParsingError При неизвестном символе, неполном значении или синтаксической ошибке
         *
         * @note Использует рекурсию для вложенных структур (массивы, объекты).
         */
        Node ParseNode(std::istream& input) {
            char c = PeekChar(input);

            switch (c) {
            case '[':
                return ParseArray(input);
            case '{':
                return ParseDict(input);
            case '"':
                ExpectChar(input, '"');
                return Node(ParseString(input));
            case 't':
                CheckLiteral(input, "true");
                return Node(true);
            case 'f':
                CheckLiteral(input, "false");
                return Node(false);
            case 'n':
                CheckLiteral(input, "null");
                return Node(nullptr);
            case '-':
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                return ParseNumber(input);
            default:
                throw ParsingError("Unexpected character: " + std::string(1, c));
            }
        }

    } // anonymous namespace

    Document Load(std::istream& input) {
        try {
            Node root = ParseNode(input);
            std::ws(input);
            if (input.peek() != EOF) {
                throw ParsingError("Unexpected content after JSON");
            }

            // В тренажере не проходит return Document(std::move(root))
            return Document(root);
        }
        catch (const std::ios_base::failure&) {
            throw ParsingError("IO error");
        }
    }

    // =============================================================================
    // Реализация Builder
    // =============================================================================

    Builder& Builder::AddNull() {
        AddNode(nullptr);
        return *this;
    }

    Builder& Builder::AddBool(bool value) {
        Node node(value);
        AddNode(node);
        return *this;
    }

    Builder& Builder::AddNumber(int value) {
        Node node(value);
        AddNode(node);
        return *this;
    }

    Builder& Builder::AddNumber(double value) {
        Node node(value);
        AddNode(node);
        return *this;
    }

    Builder& Builder::AddString(std::string value) {
        Node node(std::move(value));
        AddNode(node);
        return *this;
    }

    Builder& Builder::StartArray() {
        CheckClosed();

        // Создаём Array, перемещаем его в node
        Array arr;
        Node node(std::move(arr));

        // Добавляем ноду, получаем адрес созданного массива
        Node* array_node = AddNode(std::move(node));

        // и заносим в стек - теперь это текущий контекст для вставки элементов
        nodes_stack_.push_back(array_node);
        return *this;
    }

    Builder& Builder::EndArray() {

        // Первая проверка - в стеке вложенных контейнеров есть хоть что-то
        // Предупреждает вызов back() на пустом векторе
        if (nodes_stack_.empty()) {
            throw BuildError("No array to end: builder is empty");
        }

        // Вторая проверка - закрываем именно Array
        const Node& top = *nodes_stack_.back();
        if (!top.IsArray()) {
            throw BuildError("EndArray() called on non-array context");
        }

        // При пройденных проверках - удаляем Array из стека
        nodes_stack_.pop_back();
        return *this;
    }

    Builder& Builder::StartDict() {
        CheckClosed();

        // Создаём Dict, перемещаем его в node
        Dict dict;
        Node node(std::move(dict));

        // Добавляем ноду, получаем адрес созданного словаря
        Node* dict_node = AddNode(std::move(node));

        // и заносим в стек - теперь это текущий контекст для вставки элементов
        nodes_stack_.push_back(dict_node);
        return *this;
    }

    Builder& Builder::EndDict() {

        // Первая проверка - в стеке вложенных контейнеров есть хоть что-то
        // Предупреждает вызов back() на пустом векторе
        if (nodes_stack_.empty()) {
            throw BuildError("No dict to end: builder is empty");
        }

        // Вторая проверка - закрываем именно Dict
        const Node& top = *nodes_stack_.back();
        if (!top.IsMap()) {
            throw BuildError("EndDict() called on non-dict context");
        }

        // При пройденных проверках - удаляем Dict из стека
        nodes_stack_.pop_back();
        return *this;
    }

    Builder& Builder::Key(std::string key) {

        // Первая проверка - в стеке вложенных контейнеров есть хоть что-то
        // Предупреждает вызов back() на пустом векторе
        if (nodes_stack_.empty()) {
            throw BuildError("Key() called outside of dict: builder is empty");
        }

        // Вторая проверка - добавляем Key именно в Dict
        Node& parent = *nodes_stack_.back();
        if (!parent.IsMap()) {
            throw BuildError("Key() called on non-dict context");
        }

        Dict& map = std::get<Dict>(parent.GetValueRef());
        map[key] = nullptr;  // map["name"] = null
        nodes_stack_.pop_back();
        nodes_stack_.push_back(&map[key]);
        return *this;
    }

    Node Builder::Build() {
        CheckClosed();
        return std::move(root_);
    }

    void Builder::CheckClosed() const {
        if (!nodes_stack_.empty()) {
            throw BuildError("Container is not closed");
        }
    }

    Node* Builder::AddNode(Node node) {
        if (nodes_stack_.empty()) {

            // Стек пуст - заносим в root_
            root_ = std::move(node);
            return &root_;
        }
        else {

            // Стек не пуст
            // Заменяем значение, на которое указывает вершина стека
            Node& target = *nodes_stack_.back();
            target = std::move(node);
            return &target;
        }
    }

} // namespace json
