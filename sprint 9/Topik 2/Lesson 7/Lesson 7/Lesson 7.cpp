#include "libstat.h"

#include <iostream>

int main() {

    using namespace statistics;

    TestStatAggregSum();
    TestStatAggregMax();
    TestStatAggregMean();
    TestStatAggregStandardDeviation();
    TestStatAggregMode();
    TestStatAggregPrinter();

    std::cout << "Test passed!"sv << std::endl;
}
