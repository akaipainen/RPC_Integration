#if !defined(DIGIT_H)
#define DIGIT_H

#include <TObject.h>

class Digit : public TObject 
{
private:
    Int_t trigger_id_;
    Int_t bcid_fpga_;
    Int_t felix_counter_;

	Int_t tdc_;
	Int_t channel_;
    Int_t strip_;
    Bool_t direction_; // eta = true, phi = false
	Int_t width_;
    Int_t bcid_tdc_;
	Int_t fine_time_;

public:
    Digit();

    Digit(Int_t trigger_id, Int_t bcid_fpga, Int_t felix_counter,
          Int_t tdc, Int_t channel, Int_t width, 
          Int_t bcid_tdc, Int_t fine_time);

    // Getters
    Int_t trigger_id() const;
    Int_t bcid_fpga() const;
    Int_t felix_counter() const;

    Int_t tdc() const;
    Int_t channel() const;
    Int_t strip() const;
    Bool_t direction() const;
    Int_t width() const;
    Int_t bcid_tdc() const;
    Int_t fine_time() const;

private:
    ClassDef(Digit, 2); // Digit structure
};

#endif // DIGIT_H
