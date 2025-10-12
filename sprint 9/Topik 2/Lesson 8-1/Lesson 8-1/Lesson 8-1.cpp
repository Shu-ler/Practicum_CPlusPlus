#include <iostream>
#include <string>

#include "input_reader.h"
#include "stat_reader.h"

using namespace std;

int main() {
    trans_cat::TransportCatalogue catalogue;
    InputReader reader;

    reader.LoadCatalog(std::cin, catalogue);
    reader.ProcessRequest(std::cin, std::cout, catalogue);

    return 0;
}
