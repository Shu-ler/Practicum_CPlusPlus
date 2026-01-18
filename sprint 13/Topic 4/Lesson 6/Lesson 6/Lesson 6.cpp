#include <cassert>
#include <cstddef>
#include <cstdint>

struct Nucleotide {
    char symbol;
    size_t position;
    int chromosome_num;
    int gene_num;
    bool is_marked;
    char service_info;
};

struct CompactNucleotide {
    uint32_t position;  // 32 бита

    uint32_t symbol : 2;           // 2 бита (A=0, T=1, G=2, C=3)
    uint32_t chromosome_num : 6;   // 6 бит (1–46)
    uint32_t gene_num : 15;        // 15 бит (0–32767)
    uint32_t is_marked : 1;        // 1 бит
    uint32_t service_info : 8;     // 8 бит (char)
    // Итого: 32 + 32 = 64 бита = 8 байт
};

CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide cn;
    cn.position = static_cast<uint32_t>(n.position);

    switch (n.symbol) {
    case 'A': cn.symbol = 0; break;
    case 'T': cn.symbol = 1; break;
    case 'G': cn.symbol = 2; break;
    case 'C': cn.symbol = 3; break;
    default: cn.symbol = 0; // fallback
    }

    cn.chromosome_num = static_cast<uint32_t>(n.chromosome_num);
    cn.gene_num = static_cast<uint32_t>(n.gene_num);
    cn.is_marked = n.is_marked ? 1 : 0;
    cn.service_info = static_cast<uint32_t>(n.service_info);

    return cn;
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide n;
    n.position = cn.position;

    switch (cn.symbol) {
    case 0: n.symbol = 'A'; break;
    case 1: n.symbol = 'T'; break;
    case 2: n.symbol = 'G'; break;
    case 3: n.symbol = 'C'; break;
    default: n.symbol = '?';
    }

    n.chromosome_num = static_cast<int>(cn.chromosome_num);
    n.gene_num = static_cast<int>(cn.gene_num);
    n.is_marked = cn.is_marked;
    n.service_info = static_cast<char>(cn.service_info);

    return n;
}

// Строковые литералы без 's' — только C-style строки
static_assert(sizeof(CompactNucleotide) <= 8, "Your CompactNucleotide is not compact enough");
static_assert(alignof(CompactNucleotide) == 4, "Don't use '#pragma pack'!");

// Оператор сравнения
bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.symbol == rhs.symbol) &&
        (lhs.position == rhs.position) &&
        (lhs.chromosome_num == rhs.chromosome_num) &&
        (lhs.gene_num == rhs.gene_num) &&
        (lhs.is_marked == rhs.is_marked) &&
        (lhs.service_info == rhs.service_info);
}

// Тесты
void TestSize() {
    assert(sizeof(CompactNucleotide) <= 8);
}

void TestCompressDecompress() {
    Nucleotide source;
    source.symbol = 'T';
    source.position = 1'000'000'000;
    source.chromosome_num = 48;
    source.gene_num = 1'000;
    source.is_marked = true;
    source.service_info = '!';

    CompactNucleotide compressed = Compress(source);
    Nucleotide decompressed = Decompress(compressed);

    assert(source == decompressed);
}

int main() {
    TestSize();
    TestCompressDecompress();
}
