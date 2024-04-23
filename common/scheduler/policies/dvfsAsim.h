/**
 * This header implements the max. freq DVFS policy
 */

#ifndef __DVFS_ASYNC_H
#define __DVFS_ASYNC_H

#include <vector>
#include "dvfspolicy.h"

class DVFSAsim : public DVFSPolicy {
public:
    DVFSAsim(const PerformanceCounters *performanceCounters, int coreRows, int coreColumns, bool masterhigh);
    virtual std::vector<int> getFrequencies(const std::vector<int> &oldFrequencies, const std::vector<bool> &activeCores);

private:
    const PerformanceCounters *performanceCounters;
    unsigned int coreRows;
    unsigned int coreColumns;
    int highFrequency = 4000;
    int lowFrequency = 1000;
    bool masterhigh;
};

#endif