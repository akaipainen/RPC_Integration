#include "digit.h"

#include "detector.h"
#include "tdc.h"

ClassImp(Digit);

Digit::Digit()
 : trigger_id_(0)
 , bcid_fpga_(0)
 , felix_counter_(0)
 , tdc_(0)
 , channel_(0)
 , width_(0)
 , bcid_tdc_(0)
 , fine_time_(0)
 , muon_(false)
{
}


Digit::Digit(Int_t trigger_id, 
             Int_t bcid_fpga, 
			 Int_t felix_counter,
             Int_t tdc, 
			 Int_t channel, 
			 Int_t width, 
             Int_t bcid_tdc, 
			 Int_t fine_time)
 : trigger_id_(trigger_id)
 , bcid_fpga_(bcid_fpga)
 , felix_counter_(felix_counter)
 , tdc_(tdc)
 , channel_(channel)
 , width_(width)
 , bcid_tdc_(bcid_tdc)
 , fine_time_(fine_time)
 , muon_(false)
{
	strip_ = TDC::channel_to_strip(channel_);
	direction_ = Detector::tdc_direction(tdc_);
}

Int_t Digit::trigger_id() const { return trigger_id_; }
Int_t Digit::bcid_fpga() const { return bcid_fpga_; }
Int_t Digit::felix_counter() const { return felix_counter_; }

Int_t Digit::tdc() const { return tdc_; }
Int_t Digit::channel() const { return channel_; }
Int_t Digit::strip() const { return strip_; }
Bool_t Digit::direction() const { return direction_; }
Int_t Digit::width() const { return width_; }
Int_t Digit::bcid_tdc() const { return bcid_tdc_; }
Int_t Digit::fine_time() const { return fine_time_; }

Bool_t Digit::muon() const { return muon_; }

void Digit::set_muon(Bool_t value) { muon_ = value; }