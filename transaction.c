#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "transaction.h"

void *transactions(void *parameters)
{
    struct params *inputParams = parameters;
    accounts *accs = inputParams->accs;
    char *actions = inputParams->actions;

    char *tok;
    char *temp = actions;
    const char delim[2] = " ";
    int accountnum;
    int accountnumtwo;
    int amount;

    //printf("%s\n", actions);
    // iterate through the actions of the client
    tok = strtok_r(actions, delim, &temp);
    while (tok != NULL)
    {
        //fprintf(stderr,"%s  %s\n", tok, actions);
        if (tok[0] == 'c')
        {
            tok = strtok_r(NULL, delim, &temp);
            continue;
        }
        else if (tok[0] == 'd')
        {
            tok = strtok_r(NULL, delim, &temp); // get the account token
            accountnum = tok[1] - '0';
            tok = strtok_r(NULL, delim, &temp); // get the amount to deposit
            amount = atoi(tok);
            deposit(accountnum - 1, amount, accs);
        }
        else if (tok[0] == 'w')
        {
            tok = strtok_r(NULL, delim, &temp); // get the account token
            accountnum = tok[1] - '0';
            tok = strtok_r(NULL, delim, &temp); // get the amount to withdraw
            amount = atoi(tok);
            withdraw(accountnum - 1, amount, accs);
        }
        else if (tok[0] == 't')
        {
            tok = strtok_r(NULL, delim, &temp); // get the account token
            accountnum = tok[1] - '0';
            tok = strtok_r(NULL, delim, &temp); // get the other account token
            accountnumtwo = tok[1] - '0';
            tok = strtok_r(NULL, delim, &temp); // get the amount to transfer
            amount = atoi(tok);
            transfer(accountnum - 1, accountnumtwo - 1, amount, accs);
        }

        tok = strtok_r(NULL, delim, &temp);
    }
}

void withdraw(int accountnum, int amount, accounts *accs)
{
    //printf("Locking: %d\n", accountnum);
    pthread_mutex_lock(&(accs->locks[accountnum])); // Entry region

    //printf("Withdraw: %d from a%d\n", amount, accountnum + 1); // Critical starts
    if (accs->balance[accountnum] >= amount)
    {
        accs->balance[accountnum] = accs->balance[accountnum] - amount;
    }
    else
    {
        //printf("Request denied\n"); // Critical ends
    }
    //printf("Unlocking: %d\n", accountnum);
    pthread_mutex_unlock(&(accs->locks[accountnum])); // Exit region
}

void deposit(int accountnum, int amount, accounts *accs)
{
    //printf("Locking: %d\n", accountnum);
    pthread_mutex_lock(&(accs->locks[accountnum])); //Entry region

    //printf("Deposit: %d to a%d\n", amount, accountnum + 1); // Critical starts

    accs->balance[accountnum] = accs->balance[accountnum] + amount; // Critical ends

    //printf("Unlocking: %d\n", accountnum);
    pthread_mutex_unlock(&(accs->locks[accountnum])); // Exit region
}

void transfer(int accountone, int accounttwo, int amount, accounts *accs)
{
    //printf("Locking: %d\n", accountone);
    pthread_mutex_lock(&(accs->locks[accountone])); //Entry region
    //printf("Locking: %d\n", accounttwo);
    pthread_mutex_lock(&(accs->locks[accounttwo]));

    //printf("Transfer: %d from a%d to a%d\n", amount, accountone + 1, accounttwo + 1); // Critical starts
    if (accs->balance[accountone] >= amount)
    {
        accs->balance[accountone] = accs->balance[accountone] - amount;
        accs->balance[accounttwo] = accs->balance[accounttwo] + amount;
    }
    else
    {
        //printf("Request denied\n"); // Critical ends
    }

    //printf("Unlocking: %d\n", accountone);
    pthread_mutex_unlock(&(accs->locks[accountone])); // Exit region
    //printf("Unlocking: %d\n", accounttwo);
    pthread_mutex_unlock(&(accs->locks[accounttwo])); 
}