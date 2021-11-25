#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "agenda.h"

int main ()
{
    setlocale(LC_ALL, "Portuguese");
    FILE *f;
    mallocFile(f);
    cleanFile(&f);
    int op, counter=0, *ptr_counter;
    mallocInteger(ptr_counter);
    ptr_counter=&counter;

    do
    {
        cabecalho();
        op = menu();
        switch(op)
        {
        case 1:
            commandCriar(f, ptr_counter);
            break;
        case 2:
            commandSequencial(f, ptr_counter);
            break;
        case 3:
            commandListar(f, ptr_counter);
            break;
        case 4:
            commandBusca(f, ptr_counter);
            break;
        case 5:
            commandApagar(f, ptr_counter);
            break;
        case 6:
            commandSobre();
            break;
        case 0:
            printf("\t\t\t\t\tClosing...\n");
            exit(1);
            break;
        }
        system("CLS");
    }
    while(op!=0);

    free(f);
    free(ptr_counter);
}
