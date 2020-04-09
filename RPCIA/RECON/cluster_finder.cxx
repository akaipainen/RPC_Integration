#include "cluster_finder.h"

#include <iostream>
#include <deque>

#include <TMath.h>

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

    if (cluster_store->trigger_id() == 140) {
        std::cout;
    }


    for (int tdc = 0; tdc < 18; tdc++)
    {
        // Get all digits from this tdc
        std::deque<Digit*> digits_to_cluster;
        for (auto dit = digit_store->begin(); dit != digit_store->end(); dit++)
        {
            if (dit->tdc() == tdc) digits_to_cluster.push_back(dit.get());
        }

        // Form initial cluster
        // If there are no digits on this tdc, continue
        if (!digits_to_cluster.empty())
        {
            Cluster initial_cluster;
            initial_cluster.add_digit(*digits_to_cluster.front());
            digits_to_cluster.pop_front();
            cluster_store->add(initial_cluster);
        }
        else continue;

        // Loop over remaining digits and create clusters
        int not_added_counter = 0;
        while (!digits_to_cluster.empty())
        {
            Digit* current_digit = digits_to_cluster.front();

            // Check if current digit can be added to last cluster
            auto dit = cluster_store->back()->begin_digits();
            while (dit != cluster_store->back()->end_digits())
            {
                if (satisfy_constraints(*digits_to_cluster.front(), *dit))
                {
                    cluster_store->back()->add_digit(*digits_to_cluster.front());
                    digits_to_cluster.pop_front();
                    not_added_counter = 0;
                    break;
                }
                dit++;
            }
            // Current digit cannot be added to last cluster
            if (dit == cluster_store->back()->end_digits()) 
            {
                // If no digits can be added to last cluster,
                // create a new cluster
                if (++not_added_counter == digits_to_cluster.size())
                {
                    Cluster cluster;
                    cluster.add_digit(*current_digit);
                    digits_to_cluster.pop_front();
                    cluster_store->add(cluster);
                    not_added_counter = 0;
                }
                // Cycle to next digit
                else
                {
                    digits_to_cluster.push_back(current_digit);
                    digits_to_cluster.pop_front();
                }
            }
        }
    }
    
    // for (auto dit = digit_store->begin(); dit != digit_store->end(); dit++)
    // {
    //     // While-else block to either add a digit to 
    //     // an existing cluster or create a new cluster
    //     auto cit = cluster_store->begin();
    //     while (cit != cluster_store->end())
    //     {
    //         if (should_add(*cit, *dit)) {
    //             cit->add_digit(*dit);
    //             break;
    //         }
    //         cit++;
    //     }
    //     if (cit == cluster_store->end()) // Digit should create new cluster
    //     {
    //         Cluster cluster;
    //         cluster.add_digit(*dit);
    //         cluster_store->add(cluster);
    //     }
    // }
    
    // Initialize all of the cluster positions now
    for (auto cit = cluster_store->begin(); cit != cluster_store->end(); cit++)
    {
        cit->init();
    }
}

bool ClusterFinder::satisfy_constraints(const Digit &digit1, const Digit &digit2) const
{
    // Digits must be adjacent
    if (Detector::strip_adjacent(digit1.tdc(), digit1.strip(),
                                 digit2.tdc(), digit2.strip()))
    {
        // Digits must occur within 10ns of each other
        if (TDC::combined_time_ns(digit1.bcid_tdc(), digit1.fine_time()) - 
            TDC::combined_time_ns(digit2.bcid_tdc(), digit2.fine_time()) < 10) // in ns
        {
            return true;
        }
    }
    return false;
}

// bool ClusterFinder::should_add(const Cluster &cluster, const Digit &digit) const
// {
//     for (auto dit = cluster.begin_digits(); dit != cluster.end_digits(); dit++)
//     {
//         // Digit must be adjacent to an existing digit in cluster
//         if (Detector::strip_adjacent(dit->tdc(), dit->strip(),
//                                      digit.tdc(), digit.strip()))
//         {
//             // Digit must occur within 10ns of a digit in cluster
//             if (TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time())
//               - TDC::combined_time_ns(digit.bcid_tdc(), digit.fine_time())
//               < 10) // in ns
//             {
//                 return true;
//             }
//         }
//     }
//     return false;
// }