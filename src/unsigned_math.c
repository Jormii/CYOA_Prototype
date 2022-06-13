#include "unsigned_math.h"

u32_t umath_substract(u32_t minuend, u32_t subtrahend) {
    if (subtrahend >= minuend) {
        return 0;
    }

    return minuend - subtrahend;
}