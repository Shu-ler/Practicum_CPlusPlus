#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

struct Measur
{
	size_t count = 0;
	float summ = 0.f;
};

vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
	vector<float> avg_temp;				// Возвращаемый результат
	const auto days = measures.size();	// Количество дней наблюдения

	if (days > 0) {
		const auto per_day = measures[0].size();				// Количество измерений

		if (per_day > 0) {
			vector<Measur> measurs_avg(per_day, { 0, 0.f });	// Буфер подсчета средних измерений

			for (const auto& day : measures) {
				for (size_t i = 0; i < per_day; ++i) {
					measurs_avg[i].count += (day[i] > 0 ? 1 : 0);
					measurs_avg[i].summ += (day[i] > 0 ? day[i] : 0);
				}
			}

			avg_temp = vector<float>(per_day);
			for (auto i = 0; i < per_day; ++i) {
				avg_temp[i] = (measurs_avg[i].count > 0) ? measurs_avg[i].summ / measurs_avg[i].count : 0;
			}
		}
	}

	return avg_temp;
}

vector<float> GetRandomVector(int size) {
	static mt19937 engine;
	uniform_real_distribution<float> d(-100, 100);

	vector<float> res(size);
	for (int i = 0; i < size; ++i) {
		res[i] = d(engine);
	}

	return res;
}

int main() {
	vector<vector<float>> data;
	data.reserve(5000);

	for (int i = 0; i < 4; ++i) {
		data.push_back(GetRandomVector(3));
	}

	vector<float> avg;
	{
		LOG_DURATION("ComputeAvgTemp"s);
		avg = ComputeAvgTemp(data);
	}

	cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
}
