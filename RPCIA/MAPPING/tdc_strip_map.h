#if !defined(TDC_STRIP_MAP_H)
#define TDC_STRIP_MAP_H

#include <vector>

class TDCStripMap {
private:
    static std::vector<int> inverted_cables_tdc_to_strip_;

    static std::vector<int> inverted_cables_strip_to_tdc_;

public:
    static int get_strip(int channel);
    static int get_channel(int strip);
};


#endif // TDC_STRIP_MAP_H
