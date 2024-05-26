import numpy as np
import gzip

def calculate_mean_from_gz_file(file_path):
    # Initialize an empty list to store all numerical values
    all_values = []
    
    headers = True

    # Open the compressed file and read line by line
    with gzip.open(file_path, 'rt') as file:
        for line in file:
            if headers == True:
                headers = False
            else:
                # Split the line into values based on spaces
                values = line.strip().split()
                # Convert each value from string to float and add to the list
                all_values.extend([float(value) for value in values])

    # Calculate the mean using numpy
    mean_value = np.mean(all_values)
    return mean_value

# Example usage:
mean = calculate_mean_from_gz_file('PeriodicThermal.log.gz')
print(f"The mean of all values is: {mean}")

