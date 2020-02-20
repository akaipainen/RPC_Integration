#if !defined(UTILS_HPP)
#define UTILS_HPP

// #include <sstream>
#include <iostream>
#include <string>

const int tdc_to_strip_mapping[32] = { 7, 15, 23, 31, 
                                       6, 14, 22, 30,
                                       5, 13, 21, 29,
                                       4, 12, 20, 28,
                                       3, 11, 19, 27,
                                       2, 10, 18, 26,
                                       1,  9, 17, 25,
                                       0,  8, 16, 24};

namespace utils 
{

inline unsigned long get_bits(unsigned long bits, int shift, int num_bits) {
    return bits >> shift & (1 << num_bits) - 1;
}

inline int tdc_to_strip(int i) {
    return tdc_to_strip_mapping[i];
}

class NoisyRolloverCounter {
private:
    int max;
    int last_value;
    int last_true;

public:
    NoisyRolloverCounter(int max) : max(max), last_value(0), last_true(0) { }

    int get(int value) {
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

    // Return the true counter value of value
    // int get(int value) {
    //     // If rollover occurred, count it
    //     if (!recently && value < max / 10) {
    //         recently = true;
    //         rollovers += 1;
    //     }
    //     // Reset the rollover flag
    //     if (recently && value > max / 2 && value < max / 2 + 2) {
    //         recently = false;
    //     }

    //     if (value > max / 2) {
    //         return (rollovers - recently) * max + value;
    //     }
    //     return rollovers * max + value;
    // }
};

template< class ...Args >
std::string sprintf( const char * f, Args && ...args ) {
    int size = snprintf( nullptr, 0, f, args... );
    std::string res;
    res.resize( size );
    snprintf( & res[ 0 ], size + 1, f, args... );
    return res;
}

}

#endif // UTILS_HPP