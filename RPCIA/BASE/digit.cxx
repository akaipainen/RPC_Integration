#include "digit.h"

ClassImp(Digit);

Digit::Digit() { }
Digit::~Digit() { }

Digit::Digit(const Digit &digit)
 : trigger_id_(digit.trigger_id_), bcid_fpga_(digit.bcid_fpga_), felix_counter_(digit.felix_counter_),
   tdc_(digit.tdc_), channel_(digit.channel_), strip_(digit.strip_),
   width_(digit.width_), bcid_tdc_(digit.bcid_tdc_), fine_time_(digit.fine_time_) { }

Digit::Digit(Int_t trigger_id, Int_t bcid_fpga, Int_t felix_counter,
             Int_t tdc, Int_t channel, Int_t strip,
             Int_t width, Int_t bcid_tdc, Int_t fine_time)
    : trigger_id_(trigger_id), bcid_fpga_(bcid_fpga), felix_counter_(felix_counter),
      tdc_(tdc), channel_(channel), strip_(strip),
      width_(width), bcid_tdc_(bcid_tdc), fine_time_(fine_time) {}

Int_t Digit::get_trigger_id() const { return trigger_id_; }
Int_t Digit::get_bcid_fpga() const { return bcid_fpga_; }
Int_t Digit::get_felix_counter() const { return felix_counter_; }

Int_t Digit::get_tdc() const { return tdc_; }
Int_t Digit::get_channel() const { return channel_; }
Int_t Digit::get_strip() const { return strip_; }
Int_t Digit::get_width() const { return width_; }
Int_t Digit::get_bcid_tdc() const { return bcid_tdc_; }
Int_t Digit::get_fine_time() const { return fine_time_; }

// Int_t Digit::get_unique_id() const {
//     return trigger_id_ << 20 | tdc_ << 12 | channel_ << 7 | fine_time_;
// }