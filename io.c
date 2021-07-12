#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "io.h"

void num_accounts_clients(FILE *fp, accounts *accs, clients *clients)
{
	int numAccs = 0;
	int numClients = 0;
	int section = 0;
	char c;

	// loop through file until the end of the file is reached
	for (c = getc(fp); c != EOF; c = getc(fp))
	{
		if (c == 'c') // if the character is c then it has reached the client's and their transactions
			section = 1;
		if (c == '\n' && section == 0) // Increment account counter if in first section and end line reached
			numAccs++;
		else if (c == '\n' && section == 1)
		{ // Increment client counter if in second section and end line reached
			numClients++;
		}
	}

	// Set the number of account and clients
	accs->numAccounts = numAccs;
	clients->numClients = numClients;
	fseek(fp, 0, SEEK_SET);
}

int read_input_file(char *filename, accounts *accs, clients *clients)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror(filename);
		exit(1);
	}
	num_accounts_clients(fp, accs, clients); // Get the number of accounts and clients in the file

	int *balances = malloc(sizeof(int) * accs->numAccounts);			   // Allocate memory for each balance
	clients->clientActions = malloc(clients->numClients * sizeof(char *)); // Allocate memory for each string (for line of client actions)

	const char delim[2] = " ";
	char *buffer = NULL;
	size_t len = 0;
	size_t characters;
	char *tok;
	int accnum;
	int clientnum = 0;

	// Parse input file
	while ((characters = getline(&buffer, &len, fp)) != -1)
	{
		if (buffer[0] == 'c')
		{
			// Allocate memory for the actions of the clients and set the client actions
			clients->clientActions[clientnum] = (char *)malloc(characters + 1);
			strncpy(clients->clientActions[clientnum], buffer, characters);
			clientnum++;
			continue;
		}
		tok = strtok(buffer, delim);
		while (tok != NULL)
		{
			if (tok[0] == 'a') // Set the account balance
			{
				accnum = tok[1] - '0'; // convert to int
				tok = strtok(NULL, delim);
				if (tok[0] == 'b')
				{ // update balance at acc
					tok = strtok(NULL, delim);
					balances[accnum - 1] = atoi(tok);
				}
				else
				{
					tok = strtok(NULL, delim);
					continue;
				}

				//printf("%d: %d\n", accnum, balances[accnum - 1]);
			}

			tok = strtok(NULL, delim);
		}
	}
	accs->balance = balances;
	accs->locks = malloc(sizeof(pthread_mutex_t) * accs->numAccounts);

	// Initialize locks for the accounts
	for (int i = 0; i < accs->numAccounts; i++)
	{
		if (pthread_mutex_init(&(accs->locks[i]), NULL) != 0)
		{
			printf("\n mutex init failed\n");
			return 1;
		}
	}

	// for (int i = 0; i < clients->numClients; i++)
	// {
	// 	printf("%s\n", clients->clientActions[i]);
	// }

	fclose(fp);
	free(buffer);
}

void free_accounts_clients(accounts *accs, clients *cli)
{
	free(accs->balance);
	for (int i = 0; i < accs->numAccounts; i++)
	{
		pthread_mutex_destroy(&(accs->locks[i]));
	}
	free(accs->locks);

	free(cli->clientActions);
}