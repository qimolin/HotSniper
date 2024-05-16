#ifndef __DVFS_SOTA_H
#define __DVFS_SOTA_H

#include "dvfspolicy.h"
#include "performance_counters.h"
#include <vector>

class DVFSSOTA : public DVFSPolicy {
public:
  DVFSSOTA(const PerformanceCounters *performanceCounters, int coreRows,
               int coreColumns, int minFrequency, int maxFrequency,
               int frequencyStepSize, float upThreshold, float downThreshold,
               float dtmCriticalTemperature, float dtmRecoveredTemperature);

  virtual std::vector<int>
  getFrequencies(const std::vector<int> &oldFrequencies,
                 const std::vector<bool> &activeCores);

private:
  const PerformanceCounters *performanceCounters;
  unsigned int coreRows;
  unsigned int coreColumns;
  int minFrequency;
  int maxFrequency;
  int frequencyStepSize;
  float upThreshold;
  float downThreshold;
  float dtmCriticalTemperature;
  float dtmRecoveredTemperature;
  float maxUtil = 0.1;
  std::vector<float> last10utils;
  };
#endif