#if !defined(TDC_H)
#define TDC_H

#include <vector>

class TDC
{
private:
    static TDC instance_;

    std::vector<int> tdc_to_strip_;
    std::vector<int> strip_to_tdc_;
    
    double bcid_resolution_ns;
    double fine_time_resolution_ns;

public:
    static TDC & get();

    static int channel_to_strip(int channel);
    static int strip_to_channel(int strip);
    
    static double combined_time_ns(int bcid, int fine_time);

private:
    TDC();
    ~TDC();

    int channel_to_strip_imp(int channel) const;
    int strip_to_channel_imp(int strip) const;
    double combined_time_ns_imp(int bcid, int fine_time) const;


};


#endif // TDC_H
