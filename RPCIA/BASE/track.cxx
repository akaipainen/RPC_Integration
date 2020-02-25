#include "track.h"

#include <TLinearFitter.h>

ClassImp(Track);

Int_t Track::id_counter_ = 0;

Track::Track()
 : unique_id_(Track::id_counter_++)
 , num_clusters_(0)
 , two_coords_(false)
 , direction_(false)
 , has_plane_(false)
 , plane_(0, 0, 0)
{
}

Int_t Track::trigger_id() const
{
    if (!clusters_.empty())
    {
        return clusters_[0]->trigger_id();
    }
    return -1;
}

Int_t Track::direction() const
{
    if (two_coords_)
    {
        return -1;
    }
    return direction_;
}

void Track::add(Cluster *cluster)
{
    for (auto &c : clusters_)
    {
        if (cluster == c) {
            return;
        }
    }
    
    bool cluster_dir = (*cluster->digits_begin())->direction();
    if (!clusters_.empty() && cluster_dir != direction_)
    {
        two_coords_ = true;
    }
    else
    {
        direction_ = cluster_dir;
    }
    
    clusters_.push_back(cluster);
    num_clusters_++;
    has_plane_ = false;
    
    data_x_.push_back(cluster->position()[2]);
    data_z_.push_back(cluster->position()[direction_]);
}

void Track::init()
{
    TLinearFitter *lf = new TLinearFitter();
    lf->StoreData(false);
    lf->SetFormula("hyp1");
    lf->AssignData(data_z_.size(), 1, data_x_.data(), data_z_.data());
    lf->Eval();
    TVectorD params;
    lf->GetParameters(params);
    plane_[direction_] = params[0];
    plane_[1 - direction_] = 0;
    plane_[2] = params[1];
    delete lf;
    has_plane_ = true;
}

std::vector<Cluster*>::iterator Track::begin_clusters()
{
    return clusters_.begin();
}

std::vector<Cluster*>::iterator Track::end_clusters()
{
    return clusters_.end();
}


TVector3 Track::plane() const
{
    if (has_plane_)
    {
        return plane_;
    }
    return TVector3(0, 0, 0);
}

void Track::merge(const Track &track)
{
    for (auto &cluster: track.clusters_)
    {
        add(cluster);
    }
    init();
}