#pragma once

#include <new> // ��� ���������� bad_alloc
#include <vector>
#include "ptrvector.h"
#include "scopedptr.h"

// ��������
class Tentacle {
public:
	explicit Tentacle(int id) noexcept
		: id_(id) {
	}

	int GetId() const noexcept {
		return id_;
	}

	Tentacle* GetLinkedTentacle() const noexcept {
		return linked_tentacle_;
	}
	void LinkTo(Tentacle& tentacle) noexcept {
		linked_tentacle_ = &tentacle;
	}
	void Unlink() noexcept {
		linked_tentacle_ = nullptr;
	}

private:
	int id_ = 0;
	Tentacle* linked_tentacle_ = nullptr;
};

// ��������
class Octopus {
public:
    Octopus()
        : Octopus(8) {
    }

    explicit Octopus(int num_tentacles) {
        for (int i = 1; i <= num_tentacles; ++i) {
            AddTentacle();
        }
    }

    // ����������� ����������� ������ PtrVector ��������� ��������� ��������,
    // ������� ��������������� ������������ ����������� ����������� ��� ������ �������

    Tentacle& AddTentacle() {
        ScopedPtr<Tentacle> t(new Tentacle(tentacles_.Size() + 1));
        tentacles_.GetItems().push_back(t.GetRawPtr());
        t.Release();
        return *tentacles_.GetItems().back();
    }

    int GetTentacleCount() const noexcept {
        return static_cast<int>(tentacles_.Size());
    }

    const Tentacle& GetTentacle(size_t index) const {
        return *tentacles_.at(index);
    }
    Tentacle& GetTentacle(size_t index) {
        return *tentacles_.at(index);
    }

private:
    // ������ ������ ��������� �� ��������. ���� ������� ������� ��������� � ����
    PtrVector<Tentacle> tentacles_;
};