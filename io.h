#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "types.h"

void num_accounts_clients(FILE* fp, accounts *accs, clients *clients);
int read_input_file(char *filename, accounts *accs, clients *clients);
void free_accounts_clients(accounts *accs, clients *cli);
#endif
