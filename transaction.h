#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>
#include "types.h"

struct params
{
    accounts *accs;
    char *actions;
};

void *transactions(void *parameters);
void withdraw(int accountnum, int amount, accounts *accs);
void deposit(int accountnum, int amount, accounts *accs);
void transfer(int accountone, int accounttwo, int amount, accounts *accs);
#endif
