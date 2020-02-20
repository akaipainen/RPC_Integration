#if !defined(DIGIT_H)
#define DIGIT_H

#include <TObject.h>

class Digit : public TObject {
private:
    Int_t trigger_id_;
    Int_t bcid_fpga_;
    Int_t felix_counter_;

	Int_t tdc_;
	Int_t channel_;
    Int_t strip_;
	Int_t width_;
    Int_t bcid_tdc_;
	Int_t fine_time_;

public:
    Digit();
    ~Digit();

    Digit(const Digit& digit);

    Digit(Int_t trigger_id, Int_t bcid_fpga, Int_t felix_counter,
          Int_t tdc, Int_t channel, Int_t strip,
          Int_t width, Int_t bcid_tdc, Int_t fine_time);

    // Getters
    Int_t get_trigger_id() const;
    Int_t get_bcid_fpga() const;
    Int_t get_felix_counter() const;

    Int_t get_tdc() const;
    Int_t get_channel() const;
    Int_t get_strip() const;
    Int_t get_width() const;
    Int_t get_bcid_tdc() const;
    Int_t get_fine_time() const;

    // Int_t get_unique_id() const;

private:
    ClassDef(Digit, 1); // Digit structure
};

#endif // DIGIT_H
