#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <array>

// напишите эту функцию
inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name) {
	bool res = false;
	std::ifstream src_file(src_name, std::ios::binary);

	if (src_file) {
		static const int max_block_size = 128;
	//	unsigned char header{ ' ' };
		std::ofstream dst_file(dst_name, std::ios::binary | std::ios::trunc);
		bool is_series_block = false;
		size_t block_size = 0;

		do {
			unsigned char header = static_cast<unsigned char>(src_file.get());	// Чтение блока
			if (src_file.eof()) {
				break;
			}

			is_series_block = (header & 1) == 1;					// Извлечение типа блока
			block_size = (static_cast<size_t>(header >> 1)) + 1;	// Извлечение размера данных

			if (is_series_block) {
				char data;
				src_file.get(data);
				for (size_t i = 0; i < block_size; ++i) {
					dst_file.put(data);
				}
			}
			else {
				std::array<char, max_block_size> buf_write;
				auto& b1 = src_file.read(buf_write.data(), block_size);
				auto read_size = b1.gcount();
				dst_file.write(buf_write.data(), read_size);
			}
		} while (src_file.good() && !src_file.eof());

		res = true;
	}

	return res;
}

