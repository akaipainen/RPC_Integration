#include "tdc_strip_map.h"

std::vector<int> TDCStripMap::inverted_cables_tdc_to_strip_ {
    7, 15, 23, 31,  6, 14, 22, 30,
    5, 13, 21, 29,  4, 12, 20, 28,
    3, 11, 19, 27,  2, 10, 18, 26,
    1,  9, 17, 25,  0,  8, 16, 24
};

std::vector<int> TDCStripMap::inverted_cables_strip_to_tdc_ {
    24, 20, 16, 12,  8,  4,  0, 28,
    25, 21, 17, 13,  9,  5,  1, 29,
    26, 22, 18, 14, 10,  6,  2, 30,
    27, 23, 19, 15, 11,  7,  3, 31
};


int TDCStripMap::get_strip(int channel) {
    return TDCStripMap::inverted_cables_tdc_to_strip_[channel];
}

int TDCStripMap::get_channel(int strip) {
    return TDCStripMap::inverted_cables_strip_to_tdc_[strip];
}

