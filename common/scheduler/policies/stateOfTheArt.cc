#include "stateOfTheArt.h"
#include <iomanip>
#include <iostream>

using namespace std;

DVFSSOTA::DVFSSOTA(const PerformanceCounters *performanceCounters,
                           int coreRows, int coreColumns, int minFrequency,
                           int maxFrequency, int frequencyStepSize,
                           float upThreshold, float downThreshold,
                           float dtmCriticalTemperature,
                           float dtmRecoveredTemperature)
    : performanceCounters(performanceCounters), coreRows(coreRows),
      coreColumns(coreColumns), minFrequency(minFrequency),
      maxFrequency(maxFrequency), frequencyStepSize(frequencyStepSize),
      upThreshold(upThreshold), downThreshold(downThreshold),
      dtmCriticalTemperature(dtmCriticalTemperature),
      dtmRecoveredTemperature(dtmRecoveredTemperature) {}
std::vector<int>

DVFSSOTA::getFrequencies(const std::vector<int> &oldFrequencies,
                             const std::vector<bool> &activeCores) {
  
  std::vector<int> frequencies(coreRows * coreColumns);
  for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
      if (activeCores.at(coreCounter)) {
        //core attr
        float power = performanceCounters->getPowerOfCore(coreCounter);
        float temperature = performanceCounters->getTemperatureOfCore(coreCounter);
        int frequency = oldFrequencies.at(coreCounter);
        float utilization = performanceCounters->getUtilizationOfCore(coreCounter);
        cout << "[Scheduler][SOTA]: Core " << setw(2) << coreCounter << ":";
        cout << " P=" << fixed << setprecision(3) << power << " W";
        cout << " f=" << frequency << " MHz";
        cout << " T=" << fixed << setprecision(1) << temperature << " C";
        // avoid the little circle symbol, it is not ASCII
        cout << " utilization=" << fixed << setprecision(3) << utilization << endl;
        // use same period for upscaling and downscaling as described
        // in "The ondemand governor."

        if (utilization > maxUtil){
          maxUtil = utilization;
        }

        if (temperature > dtmCriticalTemperature){
          
          cout << "Temp too high on core " << coreCounter << " downclocking" << endl;
          
          frequency = frequency * 0.9;
          frequency = (frequency / frequencyStepSize) * frequencyStepSize; // round
          if (frequency < minFrequency) {
            frequency = minFrequency;
          }

        }

        // else if (utilization < 0.8) {
        //     cout << "util too low " << coreCounter << " upclocking" << endl;
            
        //     frequency = frequency * 1.25;
        //     frequency = (frequency / frequencyStepSize) * frequencyStepSize; // round
        //     if (frequency > maxFrequency) {
        //       frequency = maxFrequency;
        //     }
        // }

        else{

          cout << "adjusting frq on core " << coreCounter << " based on current util" << endl;

          frequency = 1.25 * frequency * (utilization/maxUtil);
          frequency = (frequency / frequencyStepSize) * frequencyStepSize; // round
          if (frequency < minFrequency) {
            frequency = minFrequency;
          }
          else if (frequency > maxFrequency) {
            frequency = maxFrequency;
          }
        }

        cout << "new freq=" << frequency << " MHz" << endl;

        frequencies.at(coreCounter) = frequency;
      }
        
      else {
        frequencies.at(coreCounter) = minFrequency;
      }
  }
      
    return frequencies;

}