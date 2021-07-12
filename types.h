#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>

typedef struct accounts
{
	int numAccounts;
	int *balance;			// array to store the balance of each account
	pthread_mutex_t *locks; // an array of locks for the accounts
} accounts;

typedef struct clients
{
	int numClients;		  // total number of clients
	char **clientActions; // string containing the actions of the client
} clients;
#endif