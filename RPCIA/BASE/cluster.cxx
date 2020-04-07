#include "cluster.h"

#include <algorithm>
#include <iostream>

#include "detector.h"

ClassImp(Cluster);

Int_t Cluster::id_counter_ = 0;

Cluster::Cluster()
 : unique_id_(Cluster::id_counter_++)
 , size_(0)
 , has_position_(false)
 , position_(-1, -1, -1)
 , position_error_(0, 0, 0)
{
}

bool Cluster::operator==(const Cluster &other) const
{
    if (unique_id_ == other.unique_id_)
    {
        if (digits_.size() != other.digits_.size())
        {
            std::cerr << "Clusters with same id have different sizes" << std::endl;
        }
        return true;
    }
    return false;
}

Int_t Cluster::trigger_id() const
{
    if (!digits_.empty())
    {
        return digits_.trigger_id();
    }
    return -1;
}

void Cluster::add_digit(Digit &digit)
{   
    digits_.add(digit);
    size_++;
    has_position_ = false;
    
    // Update tdcs
    if (std::find(tdcs_.begin(), tdcs_.end(), digit.tdc()) == tdcs_.end())
    {
        tdcs_.push_back(digit.tdc());
    }

    
}

Int_t Cluster::num_digits() const {
    return size_;
}

Iterator<Digit> Cluster::begin_digits() const
{
    return digits_.begin();
}

Iterator<Digit> Cluster::end_digits() const
{
    return digits_.end();
}

std::vector<int>::const_iterator Cluster::begin_tdcs() const
{
    return tdcs_.begin();
}

std::vector<int>::const_iterator Cluster::end_tdcs() const
{
    return tdcs_.end();
}

// void Cluster::update_num_tdcs()
// {
//     for (int i = 0; i < 2; i++)
//     {
//         if (digits_.back()->tdc() == tdcs_[i])
//         {
//             break; // already exists
//         }
//         if (tdcs_[i] == -1)
//         {
//             tdcs_[i] = digits_.back()->tdc();
//             num_tdcs_++; // does not exist
//             break;
//         }
//     }
// }

void Cluster::init()
{
    // if (num_digits() > 5)
    // {
    //     std::cout << "Trigger id: " << trigger_id() << "(" << num_digits() << ")" << std::endl;
    // }

    if (has_position_)
    {
        return;
    }
    Int_t width_sum[2] = {0, 0};
    TVector3 position_sum(0, 0, 0);
    for (auto it = begin_digits(); it != end_digits(); it++)
    {   
        position_sum += Detector::position(it->tdc(), it->strip()) * it->width();
        width_sum[it->direction()] += it->width();
    }
    if (width_sum[0]) position_sum[0] /= width_sum[0];
    else position_sum[0] = -1;
    if (width_sum[1]) position_sum[1] /= width_sum[1];
    else position_sum[1] = -1;
    position_sum[2] /= width_sum[0] + width_sum[1];
    position_ = position_sum;
    has_position_ = true;
}

TVector3 Cluster::position() const
{
    if (has_position_)
    {
        return position_;
    }
    return TVector3(0, 0, 0);
}

void Cluster::merge_with(const Cluster &other)
{
    for (auto dit = other.begin_digits(); dit != other.end_digits(); dit++)
    {
        auto testit = digits_.begin();
        while (testit != digits_.end())
        {
            if (testit == dit)
            {
                break;
            }
        }
        if (testit == digits_.end())
        {
            add_digit(*dit);

            if (!Detector::parallel(dit->tdc(), tdcs_[0]))
            {
                two_coords_ = true;
            }
        }
    }
}