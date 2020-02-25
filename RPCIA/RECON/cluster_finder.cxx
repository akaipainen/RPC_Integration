#include "cluster_finder.h"

#include <iostream>

#include "detector.h"
#include "tdc.h"

ClusterFinder::ClusterFinder()
{
}

ClusterFinder::~ClusterFinder()
{
}


void ClusterFinder::make_preclusters(Store<Digit> *digit_store, Store<Cluster> *cluster_store) const
{
    cluster_store->clear(); // reset the clusters
    
    auto dit = digit_store->begin();
    while (dit != digit_store->end())
    {
        auto cit = cluster_store->begin();
        
        // While-else block to either add a digit to 
        // an existing cluster or create a new cluster


        while (cit != cluster_store->end())
        {
            if (should_add(*cit, *dit)) {
                cit->add_digit(*dit);
                break;
            }
            cit++;
        }
        if (cit == cluster_store->end())
        {
            Cluster cluster;
            cluster.add_digit(*dit);
            cluster_store->add(cluster);
        }

        dit++;
    }
    
    // Find all of the cluster positions now
    for (auto cit = cluster_store->begin(); cit != cluster_store->end(); cit++)
    {
        cit->init();
    }
}

bool ClusterFinder::should_add(const Cluster &cluster, const Digit &digit) const
{
    auto dit = cluster.digits_begin();
    while (dit != cluster.digits_end())
    {
        if (Detector::strip_adjacent(dit->tdc(), dit->strip(),
                                     digit.tdc(), digit.strip()))
        {
            if (TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time())
              - TDC::combined_time_ns(digit.bcid_tdc(), digit.fine_time())
              < 10) // in ns
            {
                return true;
            }
        }
        dit++;
    }
    return false;
}