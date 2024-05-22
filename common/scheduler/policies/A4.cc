#include "A4.h"
#include <iomanip>
#include <iostream>

using namespace std;

DVFSA4::DVFSA4(const PerformanceCounters *performanceCounters,
                           int coreRows, int coreColumns, int minFrequency,
                           int maxFrequency, int frequencyStepSize,
                           float upThreshold, float downThreshold,
                           float dtmCriticalTemperature,
                           float dtmRecoveredTemperature,float algoStepFactor,
                           float tempStepFactor, float degreesDown)
    : performanceCounters(performanceCounters), coreRows(coreRows),
      coreColumns(coreColumns), minFrequency(minFrequency),
      maxFrequency(maxFrequency), frequencyStepSize(frequencyStepSize),
      upThreshold(upThreshold), downThreshold(downThreshold),
      dtmCriticalTemperature(dtmCriticalTemperature),
      dtmRecoveredTemperature(dtmRecoveredTemperature), 
      algoStepFactor(algoStepFactor), tempStepFactor(tempStepFactor), degreesDown(degreesDown) {}
std::vector<int>

DVFSA4::getFrequencies(const std::vector<int> &oldFrequencies,
                             const std::vector<bool> &activeCores) {
  
  std::vector<int> frequencies(coreRows * coreColumns);
  for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
      if (activeCores.at(coreCounter)) {
        //core attr
        float power = performanceCounters->getPowerOfCore(coreCounter);
        float temperature = performanceCounters->getTemperatureOfCore(coreCounter);
        int frequency = oldFrequencies.at(coreCounter);
        float utilization = performanceCounters->getUtilizationOfCore(coreCounter);
        cout << "[Scheduler][A4]: Core " << setw(2) << coreCounter << ":";
        cout << " P=" << fixed << setprecision(3) << power << " W";
        cout << " f=" << frequency << " MHz";
        cout << " T=" << fixed << setprecision(1) << temperature << " C";
        // avoid the little circle symbol, it is not ASCII
        cout << " utilization=" << fixed << setprecision(3) << utilization << endl;
        cout << " last5utils= "; 
        for (size_t utilCounter = 0; utilCounter < last5utils[coreCounter].size(); ++utilCounter){
          cout << last5utils[coreCounter][utilCounter] << ",";
        }
        cout << endl;
        // use same period for upscaling and downscaling as described
        // in "The ondemand governor."

        if (utilization > maxUtil[coreCounter]){
          maxUtil[coreCounter] = utilization;
        }

        if (last5utils[coreCounter].size() < 5){
          last5utils[coreCounter].push_back(utilization);
        }

        else {
          last5utils[coreCounter].erase(last5utils[coreCounter].begin());
          last5utils[coreCounter].push_back(utilization);
        }

        float newMax = last5utils[coreCounter][0];
        for (size_t i = 0; i < last5utils[coreCounter].size(); ++i){
          if (last5utils[coreCounter][i] > newMax)
              newMax = last5utils[coreCounter][i];
        }

        if (newMax - 0.2 < maxUtil[coreCounter]){
          maxUtil[coreCounter] = newMax;
        }

        float tempMod = 0;
        float diff = dtmCriticalTemperature - degreesDown;
        diff = diff-temperature;
        if (diff < 0){
          diff = diff * -1;
        }

        if (diff <= 2){
          tempMod = 0.05;
        }

        if (diff <= 1){
          tempMod = 0.075;
        }

        if (temperature > dtmCriticalTemperature - degreesDown){
          
          cout << "Temp too high on core " << coreCounter << " downclocking" << endl;
          
          frequency = frequency * (tempStepFactor+tempMod);
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

          frequency = (algoStepFactor-tempMod) * frequency * (utilization/maxUtil[coreCounter]);
          frequency = (frequency / frequencyStepSize) * frequencyStepSize; // round
          if (frequency < minFrequency) {
            frequency = minFrequency;
          }
          else if (frequency > maxFrequency) {
            frequency = maxFrequency;
          }
          
          if (abs(frequency - oldFrequencies.at(coreCounter)) < frequencyStepSize * 2){
            frequency = oldFrequencies.at(coreCounter);
            cout << "change too small" << endl;
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