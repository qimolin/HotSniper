#include "dvfsAsim.h"
#include <iomanip>
#include <iostream>

using namespace std;

DVFSAsim::DVFSAsim(const PerformanceCounters *performanceCounters, int coreRows, int coreColumns, bool masterhigh)
	: performanceCounters(performanceCounters), coreRows(coreRows), coreColumns(coreColumns), masterhigh(masterhigh) {
	
}

std::vector<int> DVFSAsim::getFrequencies(const std::vector<int> &oldFrequencies, const std::vector<bool> &activeCores) {
	std::vector<int> frequencies(coreRows * coreColumns);

	for (unsigned int coreCounter = 0; coreCounter < coreRows * coreColumns; coreCounter++) {
		if (activeCores.at(coreCounter)) {
			float power = performanceCounters->getPowerOfCore(coreCounter);
			float temperature = performanceCounters->getTemperatureOfCore(coreCounter);
			int frequency = oldFrequencies.at(coreCounter);
			float utilization = performanceCounters->getUtilizationOfCore(coreCounter);

			cout << "[Scheduler][DVFS_Asyn_FREQ]: Core " << setw(2) << coreCounter << ":";
			cout << " MasterHigh=" << masterhigh << " highFrq=" << highFrequency << " lowFrq=" << lowFrequency;
			cout << " P=" << fixed << setprecision(3) << power << " W";
			cout << " f=" << frequency << " MHz";
			cout << " T=" << fixed << setprecision(1) << temperature << " °C";
			cout << " utilization=" << fixed << setprecision(3) << utilization << endl;

			
			if (coreCounter == 0){
				if (masterhigh){
					frequencies.at(coreCounter) = highFrequency;
					cout << "master assined highFrq"<< endl;			
				}
				else{
					frequencies.at(coreCounter) = lowFrequency;
					cout << "master assined lowFrq"<< endl;
				}
				
			}

			if (coreCounter != 0){
				if (masterhigh){
					frequencies.at(coreCounter) = lowFrequency;
					cout << "slave assined lowFrq"<< endl;			
				}
				else{
					frequencies.at(coreCounter) = highFrequency;
					cout << "slave assined highFrq"<< endl;
				}
				
		
			}
			cout << frequencies.at(coreCounter) << endl;
		}
	}

	
	return frequencies;
}