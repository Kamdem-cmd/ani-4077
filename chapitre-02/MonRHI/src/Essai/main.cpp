#include "MonRhi/MonRhi.h"
#include <iostream>

int main() {
    long long result = MonRhi::ComputeLongSum();

    std::cout << "Somme calculee : " << result << std::endl;

    return 0;
}