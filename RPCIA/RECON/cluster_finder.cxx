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
    
    for (auto dpit = digit_store->begin(); dpit != digit_store->end(); dpit++)
    {
        auto cpit = cluster_store->begin();
        
        // While-else block to either add a digit to 
        // an existing cluster or create a new cluster

        while (cpit != cluster_store->end())
        {
            if (should_add(**cpit, **dpit)) {
                (*cpit)->add_digit(*dpit);
                break;
            }
            cpit++;
        }
        if (cpit == cluster_store->end())
        {
            Cluster cluster;
            cluster.add_digit(*dpit);
            cluster_store->add(cluster);
        }
    }
    
    // Find all of the cluster positions now
    for (auto cpit = cluster_store->begin(); cpit != cluster_store->end(); cpit++)
    {
        (*cpit)->init();
    }
}

bool ClusterFinder::should_add(const Cluster &cluster, const Digit &digit) const
{
    for (auto dpit = cluster.digits_begin(); dpit != cluster.digits_end(); dpit++)
    {
        if (Detector::strip_adjacent((*dpit)->tdc(), (*dpit)->strip(),
                                     digit.tdc(), digit.strip()))
        {
            if (TDC::combined_time_ns((*dpit)->bcid_tdc(), (*dpit)->fine_time())
              - TDC::combined_time_ns(digit.bcid_tdc(), digit.fine_time())
              < 10) // in ns
            {
                return true;
            }
        }
    }
    return false;
}