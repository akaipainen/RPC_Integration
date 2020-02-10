#include <iostream>
#include <string>

#include "hit.h"
#include "utils.h"

ClassImp(Hit);

// Constructor
Hit::Hit() : bcid_tdc(0), tdc(0), channel(0), strip(0), width(0), fine_time(0) { }
Hit::Hit(Int_t tdc, Int_t channel, Int_t width, Int_t bcid_tdc, Int_t fine_time)
: tdc(tdc), channel(channel), width(width), bcid_tdc(bcid_tdc), fine_time(fine_time) { }

// Print
void Hit::Print(Option_t *) const {
    printf("TDC: %2d | Channel: %2d | Width: %3d | BCID: %3d | Fine Time: %4d\n", 
           get_tdc(), get_channel(), get_width(), get_bcid_tdc(), get_fine_time());
}

std::istream& operator>>(std::istream &stream, Hit &hit) {

    unsigned long b1, b2, b3, b4;
    if (stream >> std::hex >> b1 >> b2 >> b3 >> b4) {
        unsigned long dataword = b1 << 24 | b2 << 16 | b3 << 8 | b4;

        hit.tdc = utils::get_bits(dataword, 24, 4);
        hit.channel = utils::get_bits(dataword, 19, 5);
        hit.strip = utils::tdc_to_strip(hit.channel);
        hit.width = utils::get_bits(dataword, 12, 7);
        hit.bcid_tdc = utils::get_bits(dataword, 7, 5);
        hit.fine_time = utils::get_bits(dataword, 0, 7);
        
    }
    return stream;
}

// Getters
Int_t Hit::get_tdc() const { return tdc; }
Int_t Hit::get_channel() const { return channel; }
Int_t Hit::get_strip() const { return strip; }
Int_t Hit::get_width() const { return width; }
Int_t Hit::get_bcid_tdc() const { return bcid_tdc; }
Int_t Hit::get_fine_time() const { return fine_time; }
Int_t Hit::get_time() const { return bcid_tdc << 5 | fine_time; }
Bool_t Hit::get_noise() const { return noise; }

void Hit::set_bcid_fpga(int value) { bcid_fpga = value; }
void Hit::set_noise(bool value) { noise = value; }