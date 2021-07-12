#define FILENAME_OUTPUT "output_file.txt"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "io.h"
#include "types.h"
#include "transaction.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s input.txt\n", argv[0]);
		exit(1);
	}
	accounts accs;
	clients clients;
	read_input_file(argv[1], &accs, &clients);

	int thread_err;
	pthread_t threads[clients.numClients];

	struct params parameters[clients.numClients];

	// Create a thread for each client
	for (int i = 0; i < clients.numClients; i++)
	{
		//printf("%s\n", clients.clientActions[i]);
		parameters[i].accs = &accs;
		parameters[i].actions = clients.clientActions[i];

		// Create each thread using the specific parameters
		thread_err = pthread_create(&threads[i], NULL, transactions, &parameters[i]);

		if (thread_err != 0)
			printf("\n Error creating thread %d", i);
	}

	// Join the threads
	for (int i = 0; i < clients.numClients; i++)
		pthread_join(threads[i], NULL);

	// Remove the file if it already exists
	if (access(FILENAME_OUTPUT, F_OK) != -1)
	{
		remove(FILENAME_OUTPUT);
	}
	
	// Open the file and write to it
	FILE *fp = fopen(FILENAME_OUTPUT, "w");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}

	for (int i = 0; i < accs.numAccounts; i++)
	{
		fprintf(fp, "a%d b %d\n", i + 1, accs.balance[i]); // Print to the file
		printf("a%d b %d\n", i + 1, accs.balance[i]);	   // Print to the screen
	}

	fclose(fp);
	free_accounts_clients(&accs, &clients);
	return 0;
}
