#include "noisy_rollover_counter.h"

NoisyRolloverCounter::NoisyRolloverCounter(int max)
 : max(max), last_value(0), last_true(0) { }

int NoisyRolloverCounter::get(int value) {
    if (value < last_value) { // new value is smaller
        if (last_value - value > max / 2) { // rolled over
            last_true += max + value - last_value;
            last_value = value;
            return last_true;
        } else { // just decreased normally
            last_true += value - last_value;
            last_value = value;
            return last_true;
        }
    } else { // new value is larger
        if (value - last_value > max / 2) { // rolled back
            last_true += value - last_value - max;
            last_value = value;
            return last_true;
        } else { // just decreased normally
            last_true += value - last_value;
            last_value = value;
            return last_true;
        }
    }
}