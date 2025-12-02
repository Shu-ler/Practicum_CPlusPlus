#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

/**
 * @brief Система стимулирования чтения — отслеживает прогресс пользователей и выдаёт мотивационные доли.
 *
 * Для каждого пользователя сохраняется последняя прочитанная страница.
 * При запросе CHEER вычисляется доля пользователей (не считая самого пользователя),
 * которые прочитали меньше страниц.
 *
 * Реализация оптимизирована под ограничения:
 * - до 100 000 пользователей
 * - до 1000 страниц
 * - до 10⁶ запросов
 */
class ReadingManager {
private:
    static const int MAX_USER_COUNT = 100'000;  ///< Максимально возможный идентификатор пользователя
    static const int MAX_PAGE_COUNT = 1'000;    ///< Максимальное количество страниц в книге

    /// Хранит последнюю прочитанную страницу для каждого пользователя.
    /// Индекс — user_id, значение — номер страницы. -1 означает, что пользователь не читал.
    std::vector<int> user_page_;

    /// Счётчик пользователей, находящихся на каждой странице.
    /// Используется для быстрого подсчёта количества пользователей с прогрессом < X.
    std::vector<int> page_count_;

    /**
     * @brief Подсчитывает общее количество активных пользователей (считая всех с READ).
     * @return Количество пользователей, сделавших хотя бы один READ.
     */
    int GetTotalUserCount() const {
        int total = 0;
        for (int count : page_count_) {
            total += count;
        }
        return total;
    }

public:
    /**
     * @brief Конструктор. Инициализирует внутренние структуры данных.
     *
     * Все пользователи изначально считаются неактивными (прогресс = -1).
     * Счётчики страниц обнуляются.
     */
    ReadingManager()
        : user_page_(MAX_USER_COUNT + 1, -1)  // индексы 1..100'000
        , page_count_(MAX_PAGE_COUNT + 1, 0)  // индексы 1..1000
    {
    }

    /**
     * @brief Обновляет прогресс пользователя: он дочитал до указанной страницы.
     *
     * Если пользователь уже читал, его предыдущий прогресс заменяется.
     * Гарантируется, что номера страниц для одного пользователя возрастают.
     *
     * @param user_id Идентификатор пользователя (1 <= user_id <= 100'000)
     * @param page Номер страницы, до которой дочитал (1 <= page <= 1000)
     */
    void Read(int user_id, int page) {
        // Защита от некорректных данных (на всякий случай)
        if (user_id < 1 || user_id > MAX_USER_COUNT || page < 1 || page > MAX_PAGE_COUNT) {
            return;
        }

        // Если пользователь уже читал — уменьшаем счётчик старой страницы
        if (user_page_[user_id] != -1) {
            page_count_[user_page_[user_id]]--;
        }

        // Обновляем прогресс
        user_page_[user_id] = page;
        page_count_[page]++;
    }

    /**
     * @brief Возвращает долю пользователей, прочитавших меньше страниц, чем данный пользователь.
     *
     * Поведение:
     * - Если у пользователя не было READ — возвращает 0.0
     * - Если пользователь единственный — возвращает 1.0
     * - Иначе: (количество пользователей с page < его_page) / (общее_количество - 1)
     *
     * @param user_id Идентификатор пользователя
     * @return Доля в диапазоне [0.0, 1.0]
     */
    double Cheer(int user_id) const {
        // Проверка диапазона и активности
        if (user_id < 1 || user_id > MAX_USER_COUNT || user_page_[user_id] == -1) {
            return 0.0;
        }

        int total_users = GetTotalUserCount();
        if (total_users == 1) {
            return 1.0;
        }

        int my_page = user_page_[user_id];
        int less_count = 0;

        // Считаем, сколько пользователей на страницах строго меньше my_page
        for (int p = 1; p < my_page; ++p) {
            less_count += page_count_[p];
        }

        return static_cast<double>(less_count) / (total_users - 1);
    }
};

/**
 * @brief Главная функция — обрабатывает ввод и управляет системой чтения.
 *
 * Читает количество запросов, затем обрабатывает каждый:
 * - "READ user page" → обновляет прогресс пользователя
 * - "CHEER user"     → выводит долю пользователей с меньшим прогрессом
 *
 * Формат вывода: вещественные числа с 6 значащими цифрами (setprecision(6)).
 */
int main() {
    // Оптимизация ввода/вывода для скорости (важно при 10^6 запросов)
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Создаём менеджер чтения — он будет хранить и обрабатывать все данные
    ReadingManager manager;

    // Считываем количество запросов
    int query_count;
    cin >> query_count;

    // Обрабатываем каждый запрос
    for (int i = 0; i < query_count; ++i) {
        string command;
        cin >> command;

        int user_id;
        cin >> user_id;

        if (command == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        }
        else if (command == "CHEER") {
            // Выводим результат с точностью 6 знаков
            cout << setprecision(6) << manager.Cheer(user_id) << '\n';
        }
        // Если команда неизвестна — игнорируем (по условию не требуется)
    }

    return 0;
}