#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

#define MAX_ESTUDANTES 100
#define MAX_NOME 50
#define MAX_NUMERO 10000
#define MIN_NOTA 0
#define MAX_NOTA 20
#define PESO_MINITESTES 0.2
#define PESO_TESTEESCRITO 0.3
#define PESO_TRABALHO 0.5
#define POSITIVA 10

typedef struct
{
    int dia, mes, ano;
} tData;

typedef struct
{
    float miniTestes, testeEscrito, trabalho;
} tNota;

typedef struct
{
    int numero;
    char nome[MAX_NOME];
    int nota;
    tData data;
    tNota notasParciais;
    int avaliado;  //0 - nao avaliado , 1 - avaliado
} tEstudante;

typedef struct
{
    int numero;
    tData data;
    int presencas[MAX_ESTUDANTES];
    int numeroPresencas;
} tAula;

void inserirEstudante(tEstudante estudantes[], int *contadorEstudantes);
void limparBufferTeclado(void);
int lerInteiro(char str[], int minimo, int maximo);
int pesquisaEstudantePorNumero(tEstudante estudantes[], int contadorEstudantes, int numeroAux);
float lerFloat(char str[], int minimo, int maximo);
void lerNotasAvaliacao(tEstudante estudantes[], int contadorEstudantes);
void mostrarDados(tEstudante estudantes[], int contadorEstudantes);
int calculaNotaFinal(tEstudante estudantes[], int contadorEstudantes);
void escreverEstudantesFicheiroBinario(tEstudante estudantes[], int contadorEstudantes);
void lerEstudantesFicheiroBinario(tEstudante estudantes[], int *contadorEstudantes);
int pesquisaAulaPorNumero(tAula * aulas, int contadorAulas, int numeroAux);

tAula* inserirAula(tAula * aulas, int * contadorAulas, tEstudante estudantes[],
                   int contadorEstudantes, int *contadorNumeros);
int main(void)
{
    int contadorEstudantes=0;
    tEstudante estudantes[MAX_ESTUDANTES];

    tAula * aulas = NULL;
    int contadorAulas = 0;
    int contadorNumeros = 1;

    inserirEstudante(estudantes, &contadorEstudantes);
    inserirEstudante(estudantes, &contadorEstudantes);
    inserirEstudante(estudantes, &contadorEstudantes);


    /*  lerNotasAvaliacao(estudantes, contadorEstudantes);
      lerNotasAvaliacao(estudantes, contadorEstudantes);

      calculaNotaFinal(estudantes, contadorEstudantes);

      mostrarDados(estudantes, contadorEstudantes);

      escreverEstudantesFicheiroBinario(estudantes, contadorEstudantes);

    lerEstudantesFicheiroBinario(estudantes, &contadorEstudantes);*/


    aulas = inserirAula(aulas, &contadorAulas, estudantes, contadorEstudantes, &contadorNumeros);
    aulas = inserirAula(aulas, &contadorAulas, estudantes, contadorEstudantes, &contadorNumeros);

    mostrarDados(estudantes, contadorEstudantes);

free(aulas);
    return 0;
}

int pesquisaAulaPorNumero(tAula * aulas, int contadorAulas, int numeroAux)
{
    int i, indice=-1;
    for (i=0 ; i<contadorAulas; i++)
    {
        if (aulas[i].numero == numeroAux)
        {
            indice = i;
        }
    }
    return indice;
}

tAula * eliminarAula(tAula * aulas, int * contadorAulas, int contadorNumeros)
{
    int numeroAulaAEliminar, indice, i;
    tAula * aux;
    aux = aulas;

    if (*contadorAulas == 0)
    {
        printf("Nao existem aulas\n");
    }
    else
    {
        numeroAulaAEliminar = lerInteiro("Numero da aula a eliminar: ", 1, contadorNumeros);
        indice = pesquisaAulaPorNumero(aulas, *contadorAulas, numeroAulaAEliminar);
        if (indice == -1)
        {
            printf("Essa aula nao existe\n");
        }
        else
        {
            for (i=indice; i< *contadorAulas-1 ; i++)
            {
                aulas[i] = aulas[i+1];
            }
            if (*contadorAulas == 1)
            {
                free(aulas);
                aulas = NULL;
            }
            else
            {
                aulas = realloc(aulas, (*contadorAulas-1)*sizeof(tAula));
                if (aulas == NULL)
                {
                    perror("Erro ao libertar memoria!");
                    aulas = aux;
                }
            }
            (*contadorAulas)--;
        }
    }


    return aulas;
}

tAula* inserirAula(tAula * aulas, int * contadorAulas, tEstudante estudantes[],
                   int contadorEstudantes, int *contadorNumeros)
{
    tAula * aux;
    aux = aulas;
    int i,j, numeroEstudanteAux, indice, repetido;

    if (contadorEstudantes == 0)
    {
        printf("Nao existem estudantes");
    }
    else
    {
        aulas = realloc(aulas, (*contadorAulas+1)*sizeof(tAula));
        if (aulas == NULL)
        {
            perror("Nao ha mais memoria!");
            aulas = aux;
        }
        else
        {
            aulas[*contadorAulas].numero = *contadorNumeros;
            (*contadorNumeros)++;
            //data
            aulas[*contadorAulas].numeroPresencas = lerInteiro("Quantos alunos presentes: ", 1, contadorEstudantes);
            for(i=0; i < aulas[*contadorAulas].numeroPresencas; i++)
            {
                do
                {
                    do
                    {
                        numeroEstudanteAux = lerInteiro("Numero do estudante presente:",1 ,MAX_NUMERO);
                        indice = pesquisaEstudantePorNumero(estudantes, contadorEstudantes, numeroEstudanteAux);
                        if (indice == -1)
                        {
                            printf("Esse estudante nao existe\n");
                        }
                    }
                    while(indice == -1);
                    repetido = 0;
                    for(j=0; j<i; j++)
                    {
                        if (numeroEstudanteAux == aulas[*contadorAulas].presencas[j])
                        {
                            printf("Esse aluno ja esta presente");
                            repetido = 1;
                        }
                    }
                }
                while (repetido == 1);
                aulas[*contadorAulas].presencas[i] = numeroEstudanteAux;
            }
        }
    }
    return aulas;
}

tAula* lerAulasFicheiroBinario(int *contadorAulas, int *contadorNumeros)
{
    tAula * aulas = NULL;
    FILE * fp;
    int aulasLidas;
    fp = fopen("aulas.dat", "rb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fread(contadorAulas, sizeof(int), 1, fp);
        fread(contadorNumeros, sizeof(int), 1, fp);
        aulas = realloc(aulas, *contadorAulas * sizeof(tAula));
        if (aulas == NULL)
        {
            perror("Erro ao reservar memoria\n");
            *contadorNumeros = 1;
            *contadorAulas = 0;
        }
        else
        {
            aulasLidas = fread(aulas, sizeof(tAula), *contadorAulas, fp);
            if (aulasLidas != *contadorAulas)
            {
                printf("Erro a ler as aulas");
                free(aulas);
                aulas = NULL;
                *contadorAulas = 0;
                *contadorNumeros = 1;
            }
        }
        fclose(fp);
    }
    return aulas;
}

void lerEstudantesFicheiroBinario(tEstudante estudantes[], int *contadorEstudantes)
{
    FILE * fp;
    int estudantesLidos;
    fp = fopen("estudantes.dat", "rb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fread(contadorEstudantes, sizeof(int), 1, fp);
        estudantesLidos = fread(estudantes, sizeof(tEstudante), *contadorEstudantes, fp);
        if (estudantesLidos < *contadorEstudantes)
        {
            *contadorEstudantes = estudantesLidos;
            printf("So foram lidos %d estudantes\n", estudantesLidos);
        }
        fclose(fp);
    }
}

void escreverAulasFicheiroBinario(tAula *aulas, int contadorAulas, int contadorNumeros)
{
    FILE * fp;

    fp = fopen("aulas.dat", "wb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fwrite(&contadorAulas, sizeof(int), 1, fp);
        fwrite(&contadorNumeros, sizeof(int), 1, fp);
        fwrite(aulas, sizeof(tAula), contadorAulas, fp);
        fclose(fp);
    }
}

void escreverEstudantesFicheiroBinario(tEstudante estudantes[], int contadorEstudantes)
{
    FILE * fp;

    fp = fopen("estudantes.dat", "wb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fwrite(&contadorEstudantes, sizeof(int), 1, fp);
        fwrite(estudantes, sizeof(tEstudante), contadorEstudantes, fp);
        fclose(fp);
    }
}

void limparBufferTeclado(void)
{
    while(getchar()!='\n');
}

float lerFloat(char str[], int minimo, int maximo)
{
    float numero;
    int controlo;
    do
    {
        printf("%s (%d a %d): ", str, minimo, maximo);
        controlo = scanf("%f", &numero);
        limparBufferTeclado();
        if (numero < minimo || numero > maximo || controlo == 0)
        {
            printf("Numero invalido!\n");
        }
    }
    while(numero < minimo || numero > maximo || controlo == 0);
    return numero;
}

int lerInteiro(char str[], int minimo, int maximo)
{
    int numero, controlo;
    do
    {
        printf("%s (%d a %d): ", str, minimo, maximo);
        controlo = scanf("%d", &numero);
        limparBufferTeclado();
        if (numero < minimo || numero > maximo || controlo == 0)
        {
            printf("Numero invalido!\n");
        }
    }
    while(numero < minimo || numero > maximo || controlo == 0);
    return numero;
}

int pesquisaEstudantePorNumero(tEstudante estudantes[], int contadorEstudantes, int numeroAux)
{
    int i, indice=-1;
    for (i=0 ; i<contadorEstudantes; i++)
    {
        if (estudantes[i].numero == numeroAux)
        {
            indice = i;
        }
    }
    return indice;
}

void lerString(char str[], char nome[], int tamanho)
{
    do
    {
        printf("%s", str);
        fgets(nome, tamanho, stdin);
        if (nome[strlen(nome)-1] == '\n')
        {
            nome[strlen(nome)-1] = '\0';
        }
    }
    while(strlen(nome) == 0);
}

void inserirEstudante(tEstudante estudantes[], int *contadorEstudantes)
{
    int numeroAux, indice;
    if (*contadorEstudantes < MAX_ESTUDANTES)
    {
        do
        {
            numeroAux = lerInteiro("Numero do Estudante: ", 1, MAX_NUMERO);
            indice = pesquisaEstudantePorNumero(estudantes, *contadorEstudantes, numeroAux);
            if (indice != -1)
            {
                printf("Esse numero ja existe. Escolha outro!\n");
            }
        }
        while(indice != -1);
        estudantes[*contadorEstudantes].numero = numeroAux;

        lerString("Nome do Estudante: ", estudantes[*contadorEstudantes].nome, MAX_NOME);
        estudantes[*contadorEstudantes].avaliado = 0;

        (*contadorEstudantes)++;
    }
    else
    {
        printf("Nao pode inserir mais estudantes. Vetor cheio!\n");
    }
}


void lerNotasAvaliacao(tEstudante estudantes[], int contadorEstudantes)
{
    int numeroAux, indice;
    if (contadorEstudantes == 0)
    {
        printf("Nao existem estudantes inseridos!\n");
    }
    else
    {
        numeroAux = lerInteiro("Numero do estudante a inserir as notas", 1, MAX_NUMERO);
        indice = pesquisaEstudantePorNumero(estudantes, contadorEstudantes, numeroAux);
        if (indice == -1)
        {
            printf("O estudante com numero: %d nao existe!\n", numeroAux);
        }
        else
        {
            if (estudantes[indice].avaliado == 1)
            {
                printf("Esse estudante ja esta avaliado!\n");
            }
            else
            {
                estudantes[indice].notasParciais.miniTestes = lerFloat("Nota dos minitestes", MIN_NOTA, MAX_NOTA);
                estudantes[indice].notasParciais.testeEscrito = lerFloat("Nota do teste escrito", MIN_NOTA, MAX_NOTA);
                estudantes[indice].notasParciais.trabalho = lerFloat("Nota do trabalho", MIN_NOTA, MAX_NOTA);
                estudantes[indice].avaliado = 1;
            }
        }
    }
}

int calculaNotaFinal(tEstudante estudantes[], int contadorEstudantes)
{
    int i, contadorPositivas = 0;

    for(i=0; i< contadorEstudantes ; i++)
    {
        if (estudantes[i].avaliado == 1)
        {
            estudantes[i].nota = round(estudantes[i].notasParciais.miniTestes * PESO_MINITESTES +
                                       estudantes[i].notasParciais.testeEscrito * PESO_TESTEESCRITO +
                                       estudantes[i].notasParciais.trabalho * PESO_TRABALHO);
            if (estudantes[i].nota >= POSITIVA)
            {
                contadorPositivas++;
            }
        }
    }
    return contadorPositivas;
}

void mostrarDados(tEstudante estudantes[], int contadorEstudantes)
{
    int i;
    for(i=0; i< contadorEstudantes ; i++)
    {
        printf("Nome: %s, Numero: %d", estudantes[i].nome, estudantes[i].numero);
        if (estudantes[i].avaliado == 1)
        {
            printf("Nota Final: %d, Parciais miniteste: %.2f, teste escrito: %.2f, Trabalho: %.2f\n",
                   estudantes[i].nota, estudantes[i].notasParciais.miniTestes,
                   estudantes[i].notasParciais.testeEscrito, estudantes[i].notasParciais.trabalho);
        }
        else
        {
            printf("Ainda nao avaliado\n");
        }
    }
}