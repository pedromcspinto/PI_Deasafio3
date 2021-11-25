#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define BACKSPACE 8
#define ENTER 13
#define PONTO 46
#define TRACO 45
#define maxNome 21
#define maxMorada 31
#define maxCidade 21
#define maxPais 21
#define maxTelefone 11
#define maxCodPostal 11

typedef struct contacto
{
    char Nome[maxNome], Telefone[maxTelefone], Morada[maxMorada], CodPostal[maxCodPostal], Cidade[maxCidade] ,Pais[maxPais];
    int deleted;

} CONTACTO;

/**
 *  COMANDO SOBRE
 */

void commandSobre()
{
    sobre();
}
/**
 *
 * f - ficheiro que guarda os contactos
 * counter - apontador para o contador de contactos
 *
 * Comando que cria um contacto e escreve no ficheiro
 */

void commandCriar(FILE *f, int *counter)
{
    system("CLS");
    cabecalho();
    CONTACTO*tmp;
    mallocContacto(tmp);
    CONTACTO c;
    tmp=&c;
    rotinaFicheiros(&f, 2);
    criaContacto(tmp, counter);
    fwrite(tmp, sizeof(CONTACTO), 1, f);
    rotinaFicheiros(&f, 5);
}
/**
 *
 * f - ficheiro que guarda os contactos
 * counter - apontador para o contador de contactos
 *
 * Comando que lista todos os contactos do ficheiro
 */
void commandListar(FILE *f, int *counter)
{
    system("CLS");
    cabecalho();
    CONTACTO*tmp;
    mallocContacto(tmp);
    CONTACTO c;
    tmp=&c;
    if(checkFileEmpty(counter))
    {
    rotinaFicheiros(&f, 1);
    readContactosFromFile(f, tmp);
    rotinaFicheiros(&f, 5);
    }
}
/**
 *
 * f - ficheiro que guarda os contactos
 * counter - apontador para o contador de contactos
 *
 * Comando que lista todos os contactos do ficheiro sequencialmente um por um
 * permitindo apagar ou editar cada um dos contactos.
 */
void commandSequencial(FILE *f, int *counter)
{
    system("CLS");
    cabecalho();
    checkFileEmpty(counter);
    CONTACTO*tmp;
    mallocContacto(tmp);
    CONTACTO c;
    tmp=&c;
    rotinaFicheiros(&f, 3);
    readContactosFromFileSequencial(f, tmp, counter);
    rotinaFicheiros(&f, 5);
}
/**
 *
 * f - ficheiro que guarda os contactos
 * counter - apontador para o contador de contactos
 *
 * Função que busca contactos no ficheiro consoante os atributos que o utilizador
 * pretende encontrar. Permite tambem editar e apagar contactos.
 */
void commandBusca(FILE *f, int *counter)
{
    system("CLS");
    int op;
    char atributte[maxMorada];
    CONTACTO*tmp;
    mallocContacto(tmp);
    CONTACTO c;
    tmp=&c;
    cabecalho();
    if(checkFileEmpty(counter))
    {
        op=menuAtributte();
        rotinaFicheiros(&f, 1);
        readContactosFromFileBusca(f,tmp,op, counter);
        rotinaFicheiros(&f, 5);
    }
}
/**
 *
 * f - ficheiro que guarda os contactos
 * counter - apontador para o contador de contactos
 *
 * Função que apaga todo o conteudo do ficheiro com dupla confirmação
 */
void commandApagar(FILE *f, int *counter)
{
    system("CLS");
    cabecalho();
    int op;
    if(checkFileEmpty(counter))
    {
        op=menuConfirm();
        deleteFile(f, op, counter);
        printf("\t\t\t\t\t");
        system("pause");
    }
}
/**
 *-------------------------------------------------------------------------OPERAÇÕES SOBRE FICHEIROS---------------------------------------------------------------------------------
 * ptr - apontador de contactos
 * counter - apontador para o contador de contactos
 *
 * Função que cria com um contacto com os parametros definidos pelo utilizador
 * Tem Portugal como país default e incrementa o contador de contactos.
 */

void criaContacto(CONTACTO*ptr, int *counter)
{
    printf("\n\t\t\t\t\tNome:          ");   rotinaDados(1, maxNome, ptr->Nome);
    printf("\n\t\t\t\t\tTelefone:      ");   rotinaDados(2, maxTelefone, ptr->Telefone);
    printf("\n\t\t\t\t\tMorada:        ");   rotinaDados(4, maxMorada, ptr->Morada);
    printf("\n\t\t\t\t\tCodigo postal: ");   rotinaDados(3, maxCodPostal, ptr->CodPostal);
    printf("\n\t\t\t\t\tCidade:        ");   rotinaDados(1, maxCidade, ptr->Cidade);
    printf("\n\t\t\t\t\tPaís:          ");   rotinaDados(1, maxPais, ptr->Pais);
    if(strlen(ptr->Pais)==0)
       strcpy(ptr->Pais,"Portugal");
    ptr->deleted=0;
    *counter+=1;
    putchar('\n');putchar('\n');
}
/**
 *
 * ptr - apontador de contactos
 *
 * Função apenas imprime um contacto na forma designada
 *
 */
void printContacto(CONTACTO *ptr)
{
    printf("\n\t\t\t\t\tNome:          %s", ptr->Nome);
    printf("\n\t\t\t\t\tTelefone:      %s", ptr->Telefone);
    printf("\n\t\t\t\t\tMorada:        %s", ptr->Morada);
    printf("\n\t\t\t\t\tCodigo postal: %s", ptr->CodPostal);
    printf("\n\t\t\t\t\tCidade:        %s", ptr->Cidade);
    printf("\n\t\t\t\t\tPaís:          %s", ptr->Pais);
    putchar('\n');putchar('\n');
}
/**
 *
 * f - ficheiro que guarda os contactos
 * ptr - apontador de contactos
 *
 * Função lê e imprime os contactos do ficheiro sem interrupção caso nao tenham sido apagados
 *
 */
void readContactosFromFile(FILE *f, CONTACTO *ptr)
{
    while(fread(ptr, sizeof(CONTACTO), 1, f))
    {
        if(ptr->deleted==0)
        {
           printContacto(ptr);
        }
    }
    printf("\t\t\t\t\t");
    system("pause");
}
/**
 *
 * f - ficheiro que guarda os contactos
 * ptr - apontador de contactos
 * op - opção que define que atributo o utilizador procura
 *
 * Função lê e imprime os contactos que igualam os atributos procurados pelo
 * utilizador caso nao tenham sido apagados
 *
 */
void readContactosFromFileBusca(FILE *f, CONTACTO *ptr, int op, int *counter)
{
    char atributte[maxMorada];
    printf("\t\t\t\t\tProcurar por: ");
    scanf("%s", atributte);
    system("CLS");
    cabecalho();
    while(fread(ptr, sizeof(CONTACTO), 1, f))
    {
        if(ptr->deleted==0 && rotinaBusca(op,ptr,atributte))
        {
            system("CLS");
            cabecalho();
            printContacto(ptr);
            modify(f, ptr, counter);
        }
    }
}
/**
 *
 * f - ficheiro que guarda os contactos
 * ptr - apontador de contactos
 * counter - apontador para o contador de contactos
 *
 * Função lê e imprime os contactos em sequencia um por um caso não tenham sido apagados
 * Os contactos podem ser editados ou apagados
 *
 */
void readContactosFromFileSequencial(FILE *f, CONTACTO *ptr, int *counter)
{
    while(fread(ptr, sizeof(CONTACTO), 1, f))
    {
        if(ptr->deleted==0)
        {
            system("CLS");
            cabecalho();
            printContacto(ptr);
            modify(f, ptr, counter);
        }
    }
}
/**
 *
 * f - ficheiro que guarda os contactos
 * temp - apontador de contactos
 *
 * Função apaga um contacto do ficheiro alterando a flag deleted presente em cada contacto
 * Reencrevendo novamente no ficheiro para que futuramente não seja mais lido
 *
 */
void deleteContactoFromFile(FILE *f, CONTACTO *temp)
{
    fseek(f, -sizeof(CONTACTO), SEEK_CUR);
    temp->deleted=1;
    fwrite(temp, sizeof(CONTACTO), 1, f);
    fseek(f, -sizeof(CONTACTO), SEEK_CUR);
}
/**
 *
 * f - ficheiro que guarda os contactos
 * temp - apontador de contactos
 *
 * Função edita um atributo de um contacto no ficheiro usando um menu de escolha
 * Reencrevendo novamente no ficheiro
 *
 */
void editContactoFromFile(FILE *f, CONTACTO *temp)
{
    int choice;
    choice=menuAtributte();
    fseek(f, -sizeof(CONTACTO), SEEK_CUR);
    rotinaEdicao(choice, temp);
    fwrite(temp, sizeof(CONTACTO), 1, f);
    fseek(f, -sizeof(CONTACTO), SEEK_CUR);

}

void goBackContactoFromFile(FILE *f)
{
    CONTACTO*ptr;
    fseek(f, -sizeof(CONTACTO), SEEK_CUR);
    do
    {
    fseek(f, -sizeof(CONTACTO), SEEK_CUR);
    fread(ptr, sizeof(CONTACTO), 1, f);
    fseek(f, -sizeof(CONTACTO), SEEK_CUR);
    }while(ptr->deleted==1);

}

/**
 *
 * counter - apontador para o contador de contactos
 *
 * Função que verifica de o ficheiro está vazio
 *
 */
int checkFileEmpty(int *counter)
{
    if(*counter==0)
    {
        printf("\n\t\t\t\t\tAgenda Vazia!\n\n");
        printf("\t\t\t\t\t");
        system("pause");
    }
}
/**
 *
 * f - ficheiro que guarda os contactos
 * op - escolha vinda do menu de dupla confirmação
 * counter - apontador para o contador de contactos
 *
 * Função que apaga o conteúdo total do ficheiro
 * altera o contador dos contactos para 0
 *
 */
void deleteFile(FILE*f, int op, int *counter)
{
    if(op==1)
    {
        system("CLS");
        cabecalho();
        rotinaFicheiros(&f, 4);
        *counter=0;
        printf("\n\t\t\t\t\tAgenda apagada com sucesso!\n\n");
    }
}
/**
 *
 * f - ficheiro que guarda os contactos
 * tmp - apontador de contactos
 * counter - apontador para o contador de contactos
 *
 * Função que consoante um menu de escolha permite editar ou apagar um contacto que se apresente
 * no ecrã. A opção apagar possui também um menu de dupla confirmação
 *
 */
void modify(FILE *f, CONTACTO*tmp, int*counter)
{
    int op, choice, hit;
    op=menuModify();
    if(op==1)
    {
        system("CLS");
        cabecalho();
        editContactoFromFile(f, tmp);
    }
    if(op==2)
    {
        system("CLS");
        cabecalho();
        choice=menuConfirm();
        if(choice==1)
        {
            deleteContactoFromFile(f, tmp);
            *counter-=1;
        }
    }
    if(op==3)
    {
        system("CLS");
        cabecalho();
        goBackContactoFromFile(f);
    }
}

/**
 *  -----------------------------------------------------------------------ROTINAS E AS SUAS FUNÇÕES------------------------------------------------------------------
 */


/**
    função que delimita os caracteres de letras minúsculas na tabela ASCII que se podem usar para adicionar à string
 */

int minuscula(char c){
    int found=0;

    if((c>=97 && c<=122)|| c==32)
    found=1;

    return found;
}

/**
    função que delimita os caracteres de letras maiúsculas na tabela ASCII que se podem usar para adicionar à string
 */

int maiuscula(char c){
    int found=0;

    if((c>=65 && c<=90)|| c==32)
    found=1;

    return found;
}

/**
    função que delimita os caracteres especiais na tabela ASCII que se podem usar para adicionar à string
 */


int especial(char c){
    int found=0;

    if((c>=33 && c<=47)|| (c>=58 && c<=64)
       || (c>=91 && c<=96) || (c>=123 && c<=126) || c==32)
    found=1;

    return found;
}

/**
    função que delimita os numeros inteiros na tabela ASCII que se podem usar para adicionar à string
 */

int inteiro(char c){
    int found=0;

    if(c>=48 && c<=57)
    found=1;

    return found;
}

/**
    função que delimita todos os caracteres da tabela ASCII que se podem usar para adicionar à string
 */

int tudo(char c){
    int found=0;

    if(c>=32 && c<=254)
    found=1;

    return found;
}

/**
    função para escolher a limitação de dados não numéricos a entrar
 */

int limitacao(char op, char c){
    int res;
    switch(op){
        case 1: res=(maiuscula(c) || minuscula(c)); break;
        case 2: res=inteiro(c); break;
        case 3: res=(inteiro(c) || c==TRACO); break;
        case 4: res=(maiuscula(c) || minuscula(c) || inteiro(c)); break;
    }
    return res;
}

/**
    função para detetar a existência de um ponto da string
 */

int existePonto(char* s){
    int found=1;
    if((strchr(s,'.')-s)<0)
        found=0;

    return found;
}

/**
    Adicionar um caracter a uma string, utilizando o apontador do indice da mesma
 */

void addCh(char* s, char c, int* ptr){
    int i=*ptr;
    s[i]=c;
    s[i+1]='\0';
    *ptr+=1;
}

/**
    Apagar um caracter a uma string, utilizando o apontador do indice da mesma
 */

void delCh(char* s, int* ptr){
    int i=*ptr;
    s[i-1]='\0';
    *ptr=*ptr-1;
}

/**
    limpeza de string
 */

void clearString(char* s){
    s[0]='\0';
}
/**
 *  Rotina de entrada de dados
 */
void rotinaDados(char opcao, int comp, char *ptr)
{
    int i;
    char c;
    int* i_ptr;
    i=0;
    i_ptr=&i;
    clearString(ptr);

    do
    {
        c=getche();
        if(limitacao(opcao,c))
        {
            addCh(ptr,c,i_ptr);
        }
        else if(c==BACKSPACE && *i_ptr!=0)
        {
            delCh(ptr,i_ptr);
            putchar(' ');
            printf("\b");

        }
        else if(c==ENTER)
        {
            break;
        }
        else if(*i_ptr==0 && c==BACKSPACE)
        {
            putchar(' ');

        }
        else
        {
            printf("\b");
            putchar(' ');
            printf("\b");
        }

    }
    while(*i_ptr<comp-1);
}
/**
 *  Rotina de busca de atributos
 */
int rotinaBusca(int op, CONTACTO*temp, char*atributte)
{
    int found=0, i;
    int tam =tamString(atributte);
    char*atalho = malloc(31*sizeof(char));
    atalho[0]='\0';



    switch(op)
    {
    case 1:
        for(i=0; i<tam; i++)
        {
            atalho[i] = temp->Nome[i];
            atalho[i+1]='\0';

        }
        if(strcasecmp(atalho, atributte)==0)
            found=1;
        break;
    case 2:
        for(i=0; i<tam; i++)
        {
            atalho[i] = temp->Telefone[i];
            atalho[i+1]='\0';

        }
        if(strcasecmp(atalho, atributte)==0)
            found=1;
        break;
    case 3:
        for(i=0; i<tam; i++)
        {
            atalho[i] = temp->Morada[i];
            atalho[i+1]='\0';

        }
        if(strcasecmp(atalho, atributte)==0)
            found=1;
        break;
    case 4:
        for(i=0; i<tam; i++)
        {
            atalho[i] = temp->CodPostal[i];
            atalho[i+1]='\0';

        }
        if(strcasecmp(atalho, atributte)==0)
            found=1;
        break;
    case 5:
        for(i=0; i<tam; i++)
        {
            atalho[i] = temp->Cidade[i];
            atalho[i+1]='\0';

        }
        if(strcasecmp(atalho, atributte)==0)
            found=1;
        break;
    case 6:
        for(i=0; i<tam; i++)
        {
            atalho[i] = temp->Pais[i];
            atalho[i+1]='\0';

        }
        if(strcasecmp(atalho, atributte)==0)
            found=1;
        break;
    default:
        system("CLS");
        cabecalho();
        printf("\n\t\t\t\t\tNao existe essa opcao!");
        printf("\n\n\t\t\t\t\t");
    }
    return found;
}

int tamString(char*atributte)
{
    char ch;
    int i=0;

    do
    {
        ch = atributte[i];
        i+=1;
    }while(ch!='\0');

    return i-1;
}

/**
 *  Rotina de abertura e fecho de ficheiros
 */
void rotinaFicheiros(FILE **f, int type)
{
     switch(type)
    {
        case 1:
            openFileR(f);
            break;
        case 2:
            openFileA(f);
            break;
        case 3:
            openFileRW(f);
            break;
        case 4:
            cleanFile(f);
            break;
        case 5:
            closeFile(f);
            break;
    }
}
void cleanFile(FILE **f)
{
    *f= fopen ("c1.txt", "w");
    fclose(*f);
}

void openFileR(FILE **f)
{
    *f=fopen("c1.txt", "r");
    if (*f == NULL)
    {
        fprintf(stderr, "\nErro a abrir o ficheiro\n");
        exit (1);
    }
}

void openFileRW(FILE **f)
{
    *f=fopen("c1.txt", "r+b");
    if (*f == NULL)
    {
        fprintf(stderr, "\nErro a abrir o ficheiro\n");
        exit (1);
    }
}

void openFileA(FILE **f)
{
    *f=fopen("c1.txt", "a+");
    if (*f == NULL)
    {
        fprintf(stderr, "\nErro a abrir o ficheiro\n");
        exit (1);
    }
}

void closeFile(FILE **f)
{
    fclose(*f);
}

/*void rotinaAlocacao()
{
     switch(op)
    {
        case 1:
            mallocContacto();
            break;
        case 2:
            mallocFile();
            break;
        case 3:
            mallocInteger();
            break;
    }
}*/

void mallocContacto(CONTACTO*tmp)
{
    tmp=(CONTACTO*)malloc(sizeof(CONTACTO));
}

void mallocFile(FILE*tmp)
{
    tmp=(FILE*)malloc(sizeof(FILE));
}

void mallocInteger(int*tmp)
{
    tmp=(int*)malloc(sizeof(int));
}

/**
 *  Rotina de edição de atributos
 */
void rotinaEdicao(int op, CONTACTO*temp)
{
        switch(op)
        {
        case 1:
            mudarNome(temp);
            break;
        case 2:
            mudarTelefone(temp);
            break;
        case 3:
            mudarMorada(temp);
            break;
        case 4:
            mudarCodPostal(temp);
            break;
        case 5:
            mudarCidade(temp);
            break;
        case 6:
            mudarPais(temp);
            break;
        default:
            system("CLS");
            cabecalho();
            printf("\n\t\t\t\t\tNao existe essa opcao!");
            printf("\n\n\t\t\t\t\t");
            system("pause");
        }
}

void mudarNome(CONTACTO *temp)
{
    printf("\t\t\t\t\tMudar nome para: ");
    rotinaDados(1, maxNome, temp->Nome);
    strcpy(temp->Telefone,temp->Telefone);
    strcpy(temp->Morada,temp->Morada);
    strcpy(temp->CodPostal,temp->CodPostal);
    strcpy(temp->Cidade,temp->Cidade);
    strcpy(temp->Pais,temp->Pais);
    putchar('\n');putchar('\n');
}

void mudarTelefone(CONTACTO *temp)
{
    printf("\t\t\t\t\tMudar telefone para: ");
    strcpy(temp->Nome,temp->Nome);
    rotinaDados(2, maxTelefone, temp->Telefone);
    strcpy(temp->Morada,temp->Morada);
    strcpy(temp->CodPostal,temp->CodPostal);
    strcpy(temp->Cidade,temp->Cidade);
    strcpy(temp->Pais,temp->Pais);
    putchar('\n');putchar('\n');
}

void mudarMorada(CONTACTO *temp)
{
    printf("\t\t\t\t\tMudar morada para: ");
    strcpy(temp->Nome,temp->Nome);
    strcpy(temp->Telefone,temp->Telefone);
    rotinaDados(1, maxMorada, temp->Morada);
    strcpy(temp->CodPostal,temp->CodPostal);
    strcpy(temp->Cidade,temp->Cidade);
    strcpy(temp->Pais,temp->Pais);
    putchar('\n');putchar('\n');
}

void mudarCodPostal(CONTACTO *temp)
{
    printf("\t\t\t\t\tMudar codigo postal para: ");
    strcpy(temp->Nome,temp->Nome);
    strcpy(temp->Telefone,temp->Telefone);
    strcpy(temp->Morada,temp->Morada);
    rotinaDados(3, maxCodPostal, temp->CodPostal);
    strcpy(temp->Cidade,temp->Cidade);
    strcpy(temp->Pais,temp->Pais);
    putchar('\n');putchar('\n');
}

void mudarCidade(CONTACTO *temp)
{
    printf("\t\t\t\t\tMudar cidade para: ");
    strcpy(temp->Nome,temp->Nome);
    strcpy(temp->Telefone,temp->Telefone);
    strcpy(temp->Morada,temp->Morada);
    strcpy(temp->CodPostal,temp->CodPostal);
    rotinaDados(1, maxCidade, temp->Cidade);
    strcpy(temp->Pais,temp->Pais);
    putchar('\n');putchar('\n');
}

void mudarPais(CONTACTO *temp)
{
    printf("\t\t\t\t\tMudar pais para: ");
    strcpy(temp->Nome,temp->Nome);
    strcpy(temp->Telefone,temp->Telefone);
    strcpy(temp->Morada,temp->Morada);
    strcpy(temp->CodPostal,temp->CodPostal);
    strcpy(temp->Cidade,temp->Cidade);
    rotinaDados(1, maxPais, temp->Pais);
    putchar('\n');putchar('\n');
}
/**
 *   ---------------------------------------------------------------------------- MENUS--------------------------------------------------------------------------
 */


void cabecalho()
{
    printf("\t\t\t\t\t#####################################\n");
    printf("\t\t\t\t\t              * AGENDA *             \n");
    printf("\t\t\t\t\t                * DE *               \n");
    printf("\t\t\t\t\t             * CONTACTOS *           \n");
    printf("\t\t\t\t\t-------------------------------------\n");
}

int menu()
{
    int op;
    printf("\t\t\t\t\t                                     \n");
    printf("\t\t\t\t\t        1 - Criar contacto           \n");
    printf("\t\t\t\t\t        2 - Consulta sequencial      \n");
    printf("\t\t\t\t\t        3 - Listar contactos         \n");
    printf("\t\t\t\t\t        4 - Consulta por atributo    \n");
    printf("\t\t\t\t\t        5 - Apagar todos contactos   \n");
    printf("\t\t\t\t\t        6 - Sobre                    \n");
    printf("\t\t\t\t\t        0 - Exit                     \n");
    printf("\t\t\t\t\t                                     \n");
    printf("\t\t\t\t\t-------------------------------------\n\n");
    printf("\t\t\t\t\tEscolha uma opcao: ");
    scanf("%d", &op);
    return op;
}

int menuAtributte()
{
    int op;
    printf("\t\t\t\t\t                                     \n");
    printf("\t\t\t\t\t        1 - Nome                     \n");
    printf("\t\t\t\t\t        2 - Telefone                 \n");
    printf("\t\t\t\t\t        3 - Morada                   \n");
    printf("\t\t\t\t\t        4 - Codigo Postal            \n");
    printf("\t\t\t\t\t        5 - Cidade                   \n");
    printf("\t\t\t\t\t        6 - Pais                     \n");
    printf("\t\t\t\t\t                                     \n");
    printf("\t\t\t\t\t-------------------------------------\n\n");
    printf("\t\t\t\t\tEscolha uma opcao: ");
    scanf("%d", &op);
    return op;
}

int menuModify()
{
    int op;
    printf("\n\t\t\t\t\t[1]- EDITAR");
    printf("           [2]- APAGAR\n");
    printf("\n\t\t\t\t\t[3]- ANTERIOR");
    printf("         [4]- CONTINUAR\n");
    printf("\n\t\t\t\t\tEscolha a opcao: ");
    scanf("%d", &op);
    return op;
}

int menuConfirm()
{
    int op;
    printf("\t\t\t\t\t\t  > APAGAR CONTACTOS <             \n");
    printf("\n\t\t\t\t\tTem a certeza que quer apagar?    \n");
    printf("\n\t\t\t\t\t\t     1 - Sim                     \n");
    printf("\t\t\t\t\t\t     2 - Nao                       \n");
    printf("\n\t\t\t\t\tEscolha uma opcao: ");
    scanf("%d", &op);
    return op;
}

void sobre()
{
    system("CLS");
    cabecalho();
    printf("\n\t\t\t\t\tDeveloper:    Pedro Pinto\n");
    printf("\t\t\t\t\tDisciplina:   Programação Imperativa\n");
    printf("\t\t\t\t\tVersão:       1.0v\n\n");
    printf("\t\t\t\t\t");
    system("pause");
}
