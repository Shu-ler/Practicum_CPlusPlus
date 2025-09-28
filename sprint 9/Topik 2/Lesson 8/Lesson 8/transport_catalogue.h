#pragma once

#include "geo.h"
#include <string>
#include <iostream>

namespace stop {

	/*
	 * ����� ������������ ����� ������ ��������� ������������� ����������.
	 * �������� ���������� � �������� ��������� � � �������������� �����������.
	 */
	class Stop {
	public:

		// ����������� �� ���������
		Stop() = default;

		// �������������� ����� ��������� � ��������� ������ � ������������.
		// @param name �������� ���������
		// @param lat ������
		// @param lon �������
		Stop(const std::string& name, const double lat, const double lon) :
			name_(name),
			coordinates_{ lat, lon } {
		};

		// ���������� �������� ���������.
		// @return �������� ���������
		std::string GetName() const {
			return name_;
		}

		// ���������� ���������� ���������.
		// @return ���������� ���������
		Coordinates GetCoordinates() const {
			return coordinates_;
		}

		// ����� ��������� ���������
		// @param lat ������
		// @param lon �������
		void SetCoordinates(const double lat, const double lon) {
			coordinates_ = { lat, lon };
		}

		// ����� �������� ��������� ���������
		// @param other ������ ���������
		bool operator==(const Stop& other) const {
			return name_ == other.name_ && coordinates_ == other.coordinates_;
		}

		// ����� ���������� �� ���������
		// @param os ����� ������
		// @param stop ���������
		friend std::ostream& operator<<(std::ostream& os, const Stop& stop) {
			using namespace std::literals;
			os << "Stop: "s << stop.name_ 
				<< ", Coordinates: ("s << stop.coordinates_.lat 
				<< ", "s << stop.coordinates_.lng 
				<< ")"s;
			return os;
		}

	private:
		std::string name_{};					// �������� ���������.
		Coordinates coordinates_{ 0.0, 0.0 };	// �������������� ���������� ���������.
	};

} // namespace stop

class TransportCatalogue {

	// TODO: ����� ���������� �������� � ����
	// TODO: ����� ���������� ��������� � ����
	// TODO: ����� ������ �������� �� �����
	// TODO: ����� ������ ��������� �� �����
	// TODO: ����� ��������� ���������� � ��������

private:
	// TODO: ����� ������
};
