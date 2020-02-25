#if !defined(CLUSTER_H)
#define CLUSTER_H

#include <vector>

#include <TVector3.h>
#include <TObject.h>

#include "digit.h"

class Cluster : public TObject
{
private:
    Int_t unique_id_;
    static Int_t id_counter_;

    std::vector<Digit *> digits_;
    Int_t size_;
    
    Int_t num_tdcs_;
    std::vector<int> tdcs_;

    Bool_t has_position_; // true when computed
    Bool_t two_coords_;   // true after merged with another cluster
    TVector3 position_;
    TVector3 position_error_;

public:
    Cluster();

    bool operator==(const Cluster &other) const;

    // Get the trigger id of the digits in this cluster
    Int_t trigger_id() const;

    // Add a digit to the cluster
    void add_digit(Digit *digit);

    // Get the total number of digits
    Int_t num_digits();

    // Get begin iterator for digits
    std::vector<Digit*>::const_iterator digits_begin() const;

    // Get end iterator for digits
    std::vector<Digit*>::const_iterator digits_end() const;

    // Calculate the position of the cluster
    void init();

    // Get the width weighted average position of the cluster
    TVector3 position() const;

    // Merge with another cluster
    void merge_with(const Cluster &other);

private:
    // Update the number of tdcs this cluster contains
    void update_num_tdcs();

    ClassDef(Cluster, 1);
};

#endif // CLUSTER_H
