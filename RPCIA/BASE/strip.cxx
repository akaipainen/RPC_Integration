#include "strip.h"

// #include "tdc_strip_map.h"

Strip::Strip()
{
}

// Strip::Strip(Int_t tdc, Int_t channel, Int_t width_raw)
//  : tdc_(tdc)
//  , channel_(channel)
//  , width_raw_(width)
// {
//     layer_ = ChamberConfig::tdc_to_layer(tdc_);
//     strip_ = ChamberConfig::channel_to_strip(channel_);
//     width_ = ChamberConfig::width_precision(width_raw_);
//     direction_ = ChamberConfig::tdc_to_direction(tdc_);
// }

// Strip::Strip(const Digit &digit)
//  : tdc_(digit.get_tdc())
//  , channel_(digit.get_channel())
//  , strip_(digit.get_strip())
//  , width_raw_(digit.get_width())
// {
//     layer_ = ChamberConfig::tdc_to_layer(tdc_);
//     width_ = ChamberConfig::width_precision(width_raw_);
//     direction_ = ChamberConfig::tdc_to_direction(tdc_);
// }

Bool_t Strip::are_neighbors(const Strip &first, const Strip &second) {
    if (first.tdc() == second.tdc()
        && abs(first.strip() - second.strip()) == 1)
    {
        return true;
    }
    return false;
}

Bool_t are_parallel(const Strip &first, const Strip &second) {
    return first.direction() == second.direction();
}