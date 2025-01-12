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
        return clusters_.trigger_id();
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
        if (*cluster == c) {
            return;
        }
    }
    
    bool cluster_dir = cluster->begin_digits()->direction();
    if (!clusters_.empty() && cluster_dir != direction_)
    {
        two_coords_ = true;
    }
    else
    {
        direction_ = cluster_dir;
    }
    
    clusters_.add(*cluster);
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

Iterator<Cluster> Track::begin_clusters() const
{
    return clusters_.begin();
}

Iterator<Cluster> Track::end_clusters() const
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
        add(&cluster);
        // // Check if could merge new track's cluster with existing cluster
        // auto cit = clusters_.begin();
        // while (cit != clusters_.end())
        // {
        //     // If clusters are on same layer, merge them
        //     if (cluster.position()[2] == cit->position()[2])
        //     {
        //         cit->merge_with(cluster);

        //     }
        //     cit++;
        // }
        // if (cit == clusters_.end())
        // {
        //     add(&cluster);
        // }
        
    }
    init();
}