#include "cluster.h"

#include <algorithm>
#include <iostream>

#include "detector.h"

ClassImp(Cluster);

Int_t Cluster::id_counter_ = 0;

Cluster::Cluster()
 : unique_id_(Cluster::id_counter_++)
 , size_(0)
 , num_tdcs_(0)
 , has_position_(false)
 , position_(-1, -1, -1)
 , position_error_(0, 0, 0)
{
    tdcs_.resize(4, -1); // initialize the tdcs array with 0s
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
        return digits_.front().trigger_id();
    }
    return -1;
}

void Cluster::add_digit(const Digit &digit)
{
    digits_.push_back(digit);
    size_++;
    update_num_tdcs();
    has_position_ = false;
}

std::vector<Digit>::const_iterator Cluster::digits_begin() const
{
    return digits_.begin();
}

std::vector<Digit>::const_iterator Cluster::digits_end() const
{
    return digits_.end();
}

void Cluster::update_num_tdcs()
{
    for (int i = 0; i < 2; i++)
    {
        if (digits_.back().tdc() == tdcs_[i])
        {
            break; // already exists
        }
        if (tdcs_[i] == -1)
        {
            tdcs_[i] = digits_.back().tdc();
            num_tdcs_++; // does not exist
            break;
        }
    }
}

void Cluster::init()
{
    if (has_position_)
    {
        return;
    }
    Int_t width_sum[2] = {0, 0};
    TVector3 position_sum(0, 0, 0);
    for (auto it = digits_begin(); it != digits_end(); it++)
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
    for (auto dit = other.digits_begin(); dit != other.digits_end(); dit++)
    {
        add_digit(*dit);
        if (!Detector::parallel(dit->tdc(), tdcs_[0]))
        {
            two_coords_ = true;
        }
    }
}