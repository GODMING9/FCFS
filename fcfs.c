#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


// PCB
typedef struct process {
	int pid;
	int priority;
	int arrival_time;
	int burst_time;
	int waiting_time;
	int response_time;
	int turnaround_time;
}p;

// Check the average of waiting time
float checkWaiting(p process[], int num) {
	int sum = 0;
	for (int i = 0; i < num; i++) {
		sum = sum + process[i].waiting_time;
	}
	return (float)sum / num;
}

// Check the average of response time
float checkResponse(p process[], int num) {
	int sum = 0;
	for (int i = 0; i < num; i++) {
		sum = sum + process[i].response_time;
	}
	return (float)sum / num;
}

// Check the average of turnaround time
float checkTurnaround(p process[], int num) {
	int sum = 0;
	for (int i = 0; i < num; i++) {
		sum = sum + process[i].turnaround_time;
	}
	return (float)sum / num;
}

// Print time-flow and process execution
void printChart(p process[], int num) {
	int currentTime = 0;
	int nextArrivalIndex = 0;

	printf("\nScheduling : FCFS\n");
	printf("==========================================\n");

	while (currentTime < process[num - 1].arrival_time + process[num - 1].burst_time) {
		// Check if any new process arrives at this time
		while (nextArrivalIndex < num && currentTime == process[nextArrivalIndex].arrival_time) {
			printf("<time %d> [new arrival] process %d\n", currentTime, process[nextArrivalIndex].pid);
			nextArrivalIndex++;
		}

		// Find the current running process
		int runningProcess = -1;
		for (int i = 0; i < num; i++) {
			if (process[i].arrival_time <= currentTime && currentTime < process[i].arrival_time + process[i].burst_time) {
				runningProcess = process[i].pid;
				break;
			}
		}

		if (runningProcess == -1) {
			printf("<time %d> ---- system is idle ----\n", currentTime);
		}
		else {
			printf("<time %d> process %d is running\n", currentTime, runningProcess);
		}

		// Check if any process finishes at this time
		for (int i = 0; i < num; i++) {
			if (process[i].arrival_time + process[i].burst_time == currentTime + 1) {
				printf("<time %d> process %d is finished\n", currentTime + 1, process[i].pid);
				if (i < num - 1) {
					printf("------------------------------- (Context-Switch)\n");
				}
			}
		}

		currentTime++;
	}

	printf("<time %d> all processes finish\n", currentTime);
	printf("==========================================\n");
}


int main() {

	p process[10];
	int process_number = 0;
	int currentTime = 0;

	FILE* file;
	file = fopen("input.dat", "r");
	fopen_s(&file, "input.dat", "r");

	if (file == NULL) {
		printf("File Open Error!\n");
		return 1;
	}

	// Get the value
	while (fscanf(file, "%d %d %d %d", &process[process_number].pid, &process[process_number].priority, &process[process_number].arrival_time, &process[process_number].burst_time) != EOF) {
		process_number++;
	}

	fclose(file);

	// Arrange processes based on arrival time
	for (int i = 0; i < process_number - 1; i++) {
		for (int j = 0; j < process_number - (i + 1); j++) {
			if (process[j].arrival_time > process[j + 1].arrival_time) {
				p temp = process[j];
				process[j] = process[j + 1];
				process[j + 1] = temp;
			}
		}
	}

	// Calculate waiting time, response time, turnaround time
	for (int i = 0; i < process_number; i++) {
		if (currentTime < process[i].arrival_time) {
			currentTime = process[i].arrival_time;
		}
		process[i].waiting_time = currentTime - process[i].arrival_time;
		process[i].response_time = currentTime - process[i].arrival_time;
		process[i].turnaround_time = process[i].waiting_time + process[i].burst_time;
		currentTime += process[i].burst_time;
	}

	printChart(process, process_number);
	
	// Print statistical data 
	float cpuUsage = (float)currentTime / currentTime * 100;
	float avg_waiting_time = checkWaiting(process, process_number);
	float avg_response_time = checkResponse(process, process_number);
	float avg_turnaround_time = checkTurnaround(process, process_number);
	
	printf("Average CPU usage: %.2f %%\n", cpuUsage);
	printf("Average Waiting Time: %.2f\n", avg_waiting_time);
	printf("Average Response Time: %.2f\n", avg_response_time);
	printf("Average Turnaround Time : %.2f\n", avg_turnaround_time);

	return 0;

}
