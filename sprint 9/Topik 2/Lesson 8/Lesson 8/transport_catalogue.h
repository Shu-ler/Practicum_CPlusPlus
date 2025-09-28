#pragma once

#include "geo.h"
#include <string>

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
		};

		// ���������� ���������� ���������.
		// @return ���������� ���������
		Coordinates GetCoordinates() const {
			return coordinates_;
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
