#include "MonRhi.h"

namespace MonRhi {
    long long ComputeLongSum() {
        long long sum = 0;
        for (long long i = 1; i <= 100000000; ++i) {
            sum += i;
        }
        return sum;
    }
}