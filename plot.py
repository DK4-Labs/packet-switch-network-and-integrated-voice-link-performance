import matplotlib.pyplot as plt 
import csv 


#Columns: RANDOM_SEED, arrival_rates, utilization, fractionServed, meanNumberInSystem, meanDelay, totalCustomers
def plot(file, xcolumn, ycolumn1, ycolumn2, xlabel, ylabel, ylimUpper=0, ylimLower=0):
    num_samples = 8
    average_data = {}
    average_voice = {}
    with open(file, 'r') as csvfile:
        plots = csv.reader(csvfile, delimiter = ',')

        for row in plots:
            x = float(row[xcolumn])
            y1 = float(row[ycolumn1])
            y2 = float(row[ycolumn2])
            if x in average_data:
                average_data[x] += y1
            else:
                average_data[x] = y1
            if x in average_voice:
                average_voice[x] += y2
            else:
                average_voice[x] = y2

    x_output = []
    y1_output = []
    y2_output = []

    for key in average_data:
        x_output.append(key)
        y1_output.append(average_data[key]/num_samples)
    
    for key in average_data:
        print("%d, %f, %f " %(key, average_data[key]/num_samples, average_voice[key]/num_samples))
        y2_output.append(average_voice[key]/num_samples)


    plt.plot(x_output, y1_output, color = 'g', linestyle = 'solid', marker = 'o')
    plt.plot(x_output, y2_output, color = 'b', linestyle = 'solid', marker = 'o')
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.legend(["Data Mean Delay", "Voice Mean Delay"])
    if ylimLower != 0 or ylimUpper != 0:
        plt.ylim(ylimUpper, ylimLower)
    plt.grid()
    plt.show()

def main():
    # plot mean delay versus arrival rate 
    plot("output/non_preemptive_packet_vs_mean_delay.txt", 0, 1, 2, "Arrival Rates", "Mean Delay")

if __name__ == '__main__':
    main()