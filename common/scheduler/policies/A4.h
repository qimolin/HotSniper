#ifndef __DVFS_A4_H
#define __DVFS_A4_H

#include "dvfspolicy.h"
#include "performance_counters.h"
#include <vector>

class DVFSA4 : public DVFSPolicy {
public:
  DVFSA4(const PerformanceCounters *performanceCounters, int coreRows,
               int coreColumns, int minFrequency, int maxFrequency,
               int frequencyStepSize, float upThreshold, float downThreshold,
               float dtmCriticalTemperature, float dtmRecoveredTemperature, float algoStepFactor,
               float tempStepFactor, float degreesDown);

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
  float maxUtil[4] = {0.1,0.1,0.1,0.1};
  float algoStepFactor;
  float tempStepFactor;
  float   degreesDown;

  std::vector<float> last5utils[4];
  };
#endif