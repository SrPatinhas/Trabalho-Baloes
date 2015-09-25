#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

// ********* Declaracao de constantes
#define MAX_DESIGNACAO 50
#define MAX_BALOES 10
#define MAX_LOTACAO 25
#define MIN_NUMERO_BALAO 1
#define MAX_NUMERO_BALAO 10

// ********* Estruturas
typedef struct                                                                                              // Estrutura de datas
{
    int dia, mes, ano;
} tdata;

typedef struct {
    int qntPassageirosA, qntPassageirosB, qntPassageirosC;
} tPassageiros;

typedef struct                                                                                              // Estrutura para baloes
{
    int idBalao, lotacaoMaxima;
    char designacaoBalao[MAX_DESIGNACAO], pontoInicialBalao;
    tdata dataFabrico;
    //dados para estatisticas
    int totalViagem, totalPassageiros;
} tBaloes;

typedef struct                                                                                              // Estrutura para Viagens
{
    int idViagem, idBalao, qntTuristasTransportados;
    char inicioViagem, destinoViagem;
    int status;                                                                                             // estado da viagem (0==concluido)
    tPassageiros qntTransportados;
} tViagens;

typedef struct                                                                                              // Estrutura para Lista de Espera
{
    int idListaEspera, numeroPessoasEspera;
    char pontoInicioViagem, pontoFimViagem;
} tListaEspera;

// Funcao Menu
char menu(tBaloes vBaloes[], tListaEspera vListaEspera[], int contadorListaEspera, int contadorBaloes, int contadorViagens, float preco, float totalReceita);

// Funcoes base
void limparBufferTeclado(void);
void limparEcra(void);

// Refere-se apenas a valores
float lerFloat(char str[], int minimo, int maximo);
int lerInteiro(char str[], int minimo, int maximo);
void lerString(char str[], char nome[], int tamanho);

//funcoes base do programa em si
void editarPreco(float *preco);
char lerPontoInicial(char str[]);

// Funcoes de Data
int lerAno(void);
int lerMes(void);
int lerDia(int mes);

//Funcoes de Ficheiros Binarios;
void escreverBaseBinario(float preco, int backupBaloes, int backupViagens, int idNumBaloes, int listaEsperaID, float totalReceita);
void lerBaseBinario(float *preco, int *backupBaloes, int *backupViagens, int *idNumBaloes, int *listaEsperaID, float *totalReceita);
void escreverBaloesBinario(tBaloes vBaloes[], int contadorBaloes);
void lerBaloesBinario(tBaloes vBaloes[], int *contadorBaloes);
void escreverViagensBinario(tViagens vViagens[], int contadorViagens);
tViagens * lerViagensBinario(tViagens vViagens[], int *contadorViagens);
void escreverListaBinario(tListaEspera vListaEspera[], int contadorListaEspera);
tListaEspera *lerListaBinario(tListaEspera vListaEspera[], int *contadorListaEspera);
void escreverLog(tViagens vViagens[], int contadorViagens);

// Funcoes de Baloes
void adicionarDadosBaloes(tBaloes vBaloes[],  int *contadorBaloes, int *idNumBaloes);
void eliminarBaloes(tBaloes vBaloes[MAX_BALOES], int *contadorBaloes);
void editarDadosBaloes(tBaloes vBaloes[MAX_BALOES], int contadorBaloes);
int procuraBalao(tBaloes vBaloes[], int contadorBaloes, int numeroBalao);
void mostrarDadosBaloes(tBaloes vBaloes[MAX_BALOES], int contadorBaloes);
int lotacaoMaximaBaloes(tBaloes vBaloes[], int contadorBaloes);

// Funcoes Viagens
void mostrarViagens(tViagens vViagens[], int contadorBaloes, int contadorViagens);
char avancarPonto(char ponto);
int tirarPassageirosBalao(tViagens vViagens[], int *contadorViagens, char destino, int idBalao);
int procurarListaEspera(tListaEspera vListaEspera[], int idEliminarListaEspera[MAX_LOTACAO], int contadorListaEspera, int idBalao, char pontoInicialBalao, int lugaresDisponiveis, int *contadorIdEliminarListaEspera);
tViagens *colocarNaViagem(tListaEspera vListaEspera[], tViagens *vViagens, int *contadorViagens, int idBalao, int pontoBalao, int *viagensErro, int idEliminarListaEspera[MAX_LOTACAO], int contadorIdEliminarListaEspera);
tListaEspera *eliminarListaEspera(tListaEspera *vListaEspera, int *contadorListaEspera, int *listaEsperaErro, int idEliminarListaEspera[MAX_LOTACAO], int contadorIdEliminarListaEspera);

// Funcoes Bilhetes
tListaEspera *compraBilhete(tListaEspera vListaEspera[], tBaloes vBaloes[], float preco, int contadorBaloes, float *totalReceita, int *contadorListaEspera);
int contarBilhetesEspera(tListaEspera vListaEspera[], int contadorListaEspera, char ponto);
int contarBilhetesUsados(tViagens vViagens[], int contadorViagens, char ponto);
int consultarBilhetes(tViagens vViagens[], tListaEspera vListaEspera[], int contadorViagens, int contadorListaEspera);

// Funcoes Estatisticas
void mostrarEstatisticas(tBaloes vBaloes[], tViagens vViagens[], int contadorBaloes, int idNumBaloes, int contadorViagens);

int quantpassageirosEmBalao(tViagens vViagens[], int contadorViagens);
    //Menu
int quantPassageirosTransportados(tBaloes vBaloes[], int contadorBaloes);
int quantPassageirosEspera(tListaEspera vListaEspera[], int contadorListaEspera);
    // Baloes
int quantViagensBalao(tViagens vViagens[], int idNumBaloes, int contadorViagens);
float quantMediaTuristasBalao(tViagens vViagens[], int idNumBaloes, int contadorViagens);
int balaoComMaisViagens(tViagens vViagens[], int idNumBaloes, int contadorViagens, int nBalao);
void percentagemTuristasPorDestino(tViagens vViagens[], int contadorViagens);
    // Lista de Espera
int obterMaiorGrupoListaEspera(tListaEspera vListaEspera[], int contadorListaEspera, char ponto);
void estatisticaListaEspera(tListaEspera vListaEspera[], int contadorListaEspera);

//ordenar Vetor com ids de lista de espera para viagens
int ordenarIdEliminarListaEspera (const void *v1, const void *v2);

// **************************** Programa Principal ******************************
int main (void)
{
    //variaveis
    char opcao;                                                                             // menu
    int backupBaloes=0, backupViagens=0;                                                    // variaveis guardadas do ultimo programa
    int viagensErro=0, listaEsperaErro=0, qntPassageirosEspera=0, passageirosEmBalao=0;     // Verificacoes de erros
    int contadorBaloes=0, contadorListaEspera=0;                                            // contadores
    int idNumBaloes=0, listaEsperaID=0;                                                     // incrementador ID's reais
    float preco=0, totalReceita=0;
    int contadorViagens=0;

    //variaveis para transicao de lista de espera para viagens usada na opcao 7
    int passageirosSair=0, i=0, qntEntrarBalao=0, lugaresDisponiveis=0, contadorIdEliminarListaEspera=0;
    int idEliminarListaEspera[MAX_LOTACAO]={0};
    char pontoInicialBalao;
    
    //vectores
    tBaloes vBaloes[MAX_BALOES];
    tViagens *vViagens=NULL;
    tListaEspera *vListaEspera=NULL;

    //inicio de variaveis necessarias a 0
    lerBaseBinario(&preco, &backupBaloes, &backupViagens, &idNumBaloes, &listaEsperaID, &totalReceita);
    do
    {
        if(preco <= 0.0)
        {
            preco = lerFloat("Digite o valor do bilhete", 0, 250);
            escreverBaseBinario(preco, backupBaloes, backupViagens, idNumBaloes, listaEsperaID, totalReceita);
        }
        opcao = menu (vBaloes, vListaEspera, contadorListaEspera, contadorBaloes, contadorViagens, preco, totalReceita);
        backupBaloes = contadorBaloes;
        backupViagens = contadorViagens;
        switch (opcao)
        {
        //baloes
        case '1': //inserir balao
            adicionarDadosBaloes(vBaloes, &contadorBaloes, &idNumBaloes);
            limparEcra();
            break;

        case '2': //editar balao
            editarDadosBaloes(vBaloes, contadorBaloes);
            limparEcra();
            break;

        case '3': //remover balao
            eliminarBaloes(vBaloes, &contadorBaloes);
            limparEcra();
            break;

        case '4': //listar balao
            mostrarDadosBaloes(vBaloes, contadorBaloes);
            limparEcra();
            break;

        //bilhetes
        case '5': //vender/comprar bilhete
            vListaEspera = compraBilhete(vListaEspera, vBaloes, preco, contadorBaloes, &totalReceita, &contadorListaEspera);
            limparEcra();
            break;

        case '6': //consultar Bilhetes
            consultarBilhetes(vViagens, vListaEspera, contadorViagens, contadorListaEspera);
            limparEcra();
            break;

        //viagem balao
        case '7': //inserir viagem
            if(contadorBaloes == 0){
                printf("Nao existem baloes para fazer viagens ou nao esta ninguem para viajar.\n");
            }
            else
            {
                qntPassageirosEspera = quantPassageirosEspera(vListaEspera, contadorListaEspera);
                passageirosEmBalao = quantpassageirosEmBalao(vViagens, contadorViagens);
                
                if((qntPassageirosEspera == 0) && (passageirosEmBalao == 0))
                {
                    printf("Baloes em pausa. Nao existe ninguem em lista de espera.\n");
                }
                else
                {
                    printf("\n\n -- Registo de viagem -- \n\n");
                    for(i=0; i < contadorBaloes; i++)                                       // RETIRAR PASSAGEIROS E COLOCAR NOVOS PASSAGEIROS NO BALAO
                    {
                        pontoInicialBalao = vBaloes[i].pontoInicialBalao;
                                                                                            //ETAPA 1 - retirar passageiros que chegaram ao destino (status == 1)
                        passageirosSair = tirarPassageirosBalao(vViagens, &contadorViagens, pontoInicialBalao, vBaloes[i].idBalao);
                        lugaresDisponiveis = vBaloes[i].lotacaoMaxima - passageirosSair;
                                                                                            // ETAPA 2 - procurar na lista de espera os passageiros que vao entrar no balao
                        qntEntrarBalao = procurarListaEspera(vListaEspera, idEliminarListaEspera, contadorListaEspera, vBaloes[i].idBalao, pontoInicialBalao, lugaresDisponiveis, &contadorIdEliminarListaEspera);
                                                                                            // ETAPA 3 - colocar passageiros no balao
                        vViagens = colocarNaViagem(vListaEspera, vViagens, &contadorViagens, vBaloes[i].idBalao, pontoInicialBalao, &viagensErro, idEliminarListaEspera, contadorIdEliminarListaEspera);

                                                                                            // ETAPA 4 - atualizar total passageiros, total viagens do balao   
                        vBaloes[i].totalViagem++;
                        vBaloes[i].totalPassageiros += vViagens[contadorViagens-1].qntTuristasTransportados;
                        printf("\nO balao N.%d", vBaloes[i].idBalao);
                        printf(" comecou a sua viagem no ponto '%c' para o ponto '%c'.", vViagens[contadorViagens-1].inicioViagem, vViagens[contadorViagens-1].destinoViagem);
                        printf(" Sairam %d pessoas e entraram %d pessoas.\n", passageirosSair, qntEntrarBalao);
                                                                                            // ETAPA 5 - eliminar os passageiros que entraram no balao da lista de espera
                        if( viagensErro == 0)
                        {
                            qsort(idEliminarListaEspera, contadorIdEliminarListaEspera, sizeof(int), ordenarIdEliminarListaEspera);
                            vListaEspera = eliminarListaEspera(vListaEspera, &contadorListaEspera, &listaEsperaErro, idEliminarListaEspera, contadorIdEliminarListaEspera);
                        }
                                                                                        // BALAO ANDA PARA A PROXIMA PARAGEM
                        vBaloes[i].pontoInicialBalao = avancarPonto(vBaloes[i].pontoInicialBalao);
                    }
                }
            }
            
            escreverLog(vViagens, contadorViagens);
            limparEcra();
            break;

        case '8': //listar viagens
            mostrarViagens(vViagens, contadorBaloes, contadorViagens);
            limparEcra();
            break;
            
        case '9': // estatisticas
            mostrarEstatisticas(vBaloes, vViagens, contadorBaloes, idNumBaloes, contadorViagens);
            limparEcra();
            break;

        //dados
        case 'G': //Guardar dados
            escreverBaseBinario(preco, backupBaloes, backupViagens, idNumBaloes, listaEsperaID, totalReceita);
            escreverBaloesBinario(vBaloes, contadorBaloes);
            escreverViagensBinario(vViagens, contadorViagens);
            escreverListaBinario(vListaEspera, contadorListaEspera);
            limparEcra();
            break;

        case 'A': //Abrir dados
            lerBaloesBinario(vBaloes, &contadorBaloes);
            vViagens = lerViagensBinario(vViagens, &contadorViagens);
            vListaEspera = lerListaBinario(vListaEspera, &contadorListaEspera);
            limparEcra();
            break;

        case 'P': //Alterar Preco
            editarPreco(&preco);
            escreverBaseBinario(preco, backupBaloes, backupViagens, idNumBaloes, listaEsperaID, totalReceita);
            limparEcra();
            break;

        //opcao de erro ou opcao invalida
        default:
            limparEcra();
            break;
        }
    }
    while (opcao != '0');

    escreverBaseBinario(preco, backupBaloes, backupViagens, idNumBaloes, listaEsperaID, totalReceita);
    printf("Obrigado por utilizar o nosso programa!! :)\n\n");
    free(vViagens);
    free(vListaEspera);
    return 0;
}
// **************************** Funcao Menu ******************************

char menu(tBaloes vBaloes[], tListaEspera vListaEspera[], int contadorListaEspera, int contadorBaloes, int contadorViagens, float preco, float totalReceita)
{
    char op;
    printf("\n._______________________________________________________________________.");
    printf("\n| \t Menu - Viagem de baloes \t|  Preco Bilhete: %4.2f\t\t|", preco);
    printf("\n|-----------------------------------------------------------------------|");
    printf("\n| \t   ***** Balao *****\t\t|\t\t\t\t|");
    printf("\n| \t 1 - Inserir \t 2 - Editar \t|   Quantidade baloes: %d \t|", contadorBaloes);
    printf("\n| \t 3 - Remover \t 4 - Listar \t|   Quantidade viagens: %d \t|", contadorViagens);
    printf("\n|---------------------------------------|\t\t\t\t|");
    printf("\n| \t   ***** Bilhetes *****\t\t|\t\t\t\t|");
    printf("\n| \t 5 - Vender \t 6 - Consultar  |   P. Transportados: %d \t|", quantPassageirosTransportados(vBaloes, contadorBaloes));
    printf("\n|---------------------------------------|\t\t\t\t|");
    printf("\n| \t   ***** Viagens *****\t\t|\t\t\t\t|");
    printf("\n|  7 - Registar Viagens (avancar ponto) |   P. em Lista de Espera: %d \t|", quantPassageirosEspera(vListaEspera, contadorListaEspera));
    printf("\n|  8 - Consultar Viagens dos baloes     |   Receita de bilhetes: %4.2f\t|", totalReceita);
    printf("\n|-----------------------------------------------------------------------|");
    printf("\n|\t\t\t    9 - Estatisticas  \t\t\t\t|");
    printf("\n|-----------------------------------------------------------------------|");
    printf("\n| \t\t\t***** Dados *****\t\t\t\t|");
    printf("\n| \t\t\tp/P - Alterar Preco \t\t\t\t|");
    printf("\n| \t\t\tg/G - Guardar \ta/A - Abrir \t\t\t|");
    printf("\n|-----------------------------------------------------------------------|");
    printf("\n|\t\t\t    0 - Sair do programa  \t\t\t|");
    printf("\n|_______________________________________________________________________|");
    printf("\n\n Opcao: ");
    do
    {
        scanf(" %c", &op);
        limparBufferTeclado();
        op = toupper(op);
        if (op!='A' && op!='G' && op!='P' && (op<='0' && op>'9') )
            printf("Opcao Invalida \n");
    }
    while (op!='A' && op!='G' && op!='P' && (op<='0' && op>'9') );
    return op;
}


/************************************************************************
 *                            Funções Base                              *
 ************************************************************************/

void limparBufferTeclado(void)
{
    while(getchar()!='\n');
}

void limparEcra(void)
{
    int i;
    for(i=0; i<10; i++)
    {
        printf("\n");
    }
}
// Refere-se apenas a valores
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
            printf("Numero invalido!\n\n");
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
            printf("Numero invalido!\n\n");
        }
    }
    while(numero < minimo || numero > maximo || controlo == 0);
    return numero;
}
void lerString(char str[], char nome[], int tamanho)
{
    do
    {
        printf("%s ", str);
        fgets(nome, tamanho, stdin);
        if (nome[strlen(nome)-1] == '\n')
        {
            nome[strlen(nome)-1] = '\0';
        }
    }
    while(strlen(nome) == 0);
}
//funcoes base do programa em si
void editarPreco(float *preco)
{
    *preco = lerFloat("Digite o valor do bilhete", 0, 250);
}

char lerPontoInicial(char str[])
{
    char ponto;
    do
    {
        printf("%s", str);
        scanf(" %c", &ponto);
        limparBufferTeclado();
        ponto = toupper(ponto);
        if (ponto != 'A' && ponto != 'B' && ponto != 'C')
        {
            printf("Posicao incorreta\n\n");
        }
    }
    while(ponto != 'A' && ponto != 'B' && ponto != 'C');

    return ponto;
}
/************************************************************************
 *                          Funcoes de Data                             *
 ************************************************************************/
int lerAno(void)
{
    int ano, controlo, minimo=1900, maximo=2015;
    do
    {
        printf("Insira o ano (%d a %d): ", minimo, maximo);
        controlo = scanf("%d", &ano);
        limparBufferTeclado();
        if (ano < minimo || ano > maximo || controlo == 0)
        {
            printf("\nAno invalido!\n\n");
        }
    }
    while(ano < minimo || ano > maximo || controlo == 0);
    return ano;
}
int lerMes(void)
{
    int mes, controlo, minimo=1, maximo=12;
    do
    {
        printf("Insira o mes (%d a %d): ", minimo, maximo);
        controlo = scanf("%d", &mes);
        limparBufferTeclado();
        if (mes < minimo || mes > maximo || controlo == 0)
        {
            printf("\nMes invalido!\n\n");
        }
    }
    while(mes < minimo || mes > maximo || controlo == 0);
    return mes;
}
int lerDia(int mes)
{
    int dia, check, minimo=1, maximo;
    maximo=31;
    check = 0;
    do
    {
        switch(mes){
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                printf("Insira o dia (%d a %d): ", minimo, maximo);
                scanf("%d", &dia);
                limparBufferTeclado();
                if (dia < 1 || dia > 31)
                {
                    printf("\nDia do Mes invalido!\n\n");
                    check = 0;
                } else {
                    check = 1;
                }
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                maximo=30;
                printf("Insira o dia (%d a %d): ", minimo, maximo);
                scanf("%d", &dia);
                limparBufferTeclado();
                if (dia < 1 || dia > 30)
                {
                    printf("\nDia do Mes invalido!\n\n");
                    check = 0;
                } else {
                    check = 1;
                }
                break;
            case 2:
                maximo=28;
                printf("Insira o dia (%d a %d): ", minimo, maximo);
                scanf("%d", &dia);
                limparBufferTeclado();
                if (dia < 1 || dia > 28)
                {
                    printf("\nDia do Mes invalido!\n\n");
                    check = 0;
                } else {
                    check = 1;
                }
                break;
        }
    }
    while(check != 1);
    return dia;
}
/************************************************************************
 *                   Funcoes de Ficheiros Binarios                      *
 ************************************************************************/
                                                                                                            //Ficheiro Guardar base programa
void escreverBaseBinario(float preco, int backupBaloes, int backupViagens, int idNumBaloes, int listaEsperaID, float totalReceita)
{
    FILE * fp;
    
    fp = fopen("data/base.dat", "wb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro 'base'");
    }
    else
    {
        fwrite(&idNumBaloes, sizeof(int), 1, fp);
        fwrite(&preco, sizeof(float), 1, fp);
        fwrite(&backupBaloes, sizeof(int), 1, fp);
        fwrite(&backupViagens, sizeof(int), 1, fp);
        fwrite(&listaEsperaID, sizeof(int), 1, fp);
        fwrite(&totalReceita, sizeof(float), 1, fp);
        fclose(fp);
    }
}
                                                                                                            //Ficheiro Abrir base programa
void lerBaseBinario(float *preco, int *backupBaloes, int *backupViagens, int *idNumBaloes, int *listaEsperaID, float *totalReceita)
{
    FILE * fp;
    
    fp = fopen("data/base.dat", "rb");
    if (fp == NULL)
    {
        printf("Sem configuracoes iniciais!\n");
    }
    else
    {
        fread(idNumBaloes, sizeof(int), 1, fp);
        fread(preco, sizeof(float), 1, fp);
        fread(backupBaloes, sizeof(int), 1, fp);
        fread(backupViagens, sizeof(int), 1, fp);
        fread(listaEsperaID, sizeof(int), 1, fp);
        fread(totalReceita, sizeof(float), 1, fp);
        fclose(fp);
    }
}
                                                                                                            //Ficheiro Guardar baloes
void escreverBaloesBinario(tBaloes vBaloes[], int contadorBaloes)
{
    int numLog=0;
    FILE * fp;
    fp = fopen("data/baloes.dat", "wb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fwrite(&contadorBaloes, sizeof(int), 1, fp);
        numLog=fwrite(vBaloes, sizeof(tBaloes), contadorBaloes, fp);
        printf("Baloes guardados: %d \n", numLog);
        fclose(fp);
    }
}
                                                                                                            //Ficheiro Abrir baloes
void lerBaloesBinario(tBaloes vBaloes[], int *contadorBaloes)
{
    FILE * fp;
    int baloesLidos;
    fp = fopen("data/baloes.dat", "rb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fread(contadorBaloes, sizeof(int), 1, fp);
        baloesLidos = fread(vBaloes, sizeof(tBaloes), *contadorBaloes, fp);
        if (baloesLidos < *contadorBaloes)
        {
            *contadorBaloes = baloesLidos;
            printf("So foram lidos %d baloes\n", baloesLidos);
        }
        printf("Lidos %d baloes\n", baloesLidos);
        fclose(fp);
    }
}
                                                                                                            //Ficheiro Guardar viagens
void escreverViagensBinario(tViagens vViagens[], int contadorViagens)
{
    int numLog=0;
    FILE * fp;
    fp = fopen("data/viagens.dat", "wb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fwrite(&contadorViagens, sizeof(int), 1, fp);
        numLog = fwrite(vViagens, sizeof(tViagens), contadorViagens, fp);
        printf("Viagens guardadas: %d \n", numLog);
        fclose(fp);
    }
}
                                                                                                            //Ficheiro Abrir viagens
tViagens * lerViagensBinario(tViagens vViagens[], int *contadorViagens)
{
    tViagens * pViagens = NULL;
    FILE * fp;
    int viagensLidas;
    fp = fopen("data/viagens.dat", "rb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fread(contadorViagens, sizeof(int), 1, fp);
        vViagens = realloc(vViagens, *contadorViagens * sizeof(tViagens));
        if (vViagens == NULL)
        {
            perror("Erro ao reservar memoria\n");
            *contadorViagens = 0;
        }
        else
        {
            viagensLidas = fread(vViagens, sizeof(tViagens), *contadorViagens, fp);
            if (viagensLidas != *contadorViagens)
            {
                free(vViagens);
                pViagens = NULL;
                *contadorViagens = 0;
                printf("Erro a ler viagens\n");
            }
            printf("Lidas %d viagens\n", viagensLidas);
            fclose(fp);
        }
    }
    return pViagens;
}
                                                                                                            //Ficheiro Guardar Lista de Espera
void escreverListaBinario(tListaEspera vListaEspera[], int contadorListaEspera)
{
    int numLog=0;
    FILE * fp;
    fp = fopen("data/listaEspera.dat", "wb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fwrite(&contadorListaEspera, sizeof(int), 1, fp);
        numLog = fwrite(vListaEspera, sizeof(tListaEspera), contadorListaEspera, fp);
        printf("N. de grupos na Lista de espera guardada: %d \n", numLog);
        fclose(fp);
    }
}
                                                                                                            //Ficheiro Abrir Lista de Espera
tListaEspera *lerListaBinario(tListaEspera vListaEspera[], int *contadorListaEspera)
{
    tListaEspera * pLista = NULL;
    FILE * fp;
    int ListaEsperaLida;
    fp = fopen("data/listaEspera.dat", "rb");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fread(contadorListaEspera, sizeof(int), 1, fp);
        vListaEspera = realloc(vListaEspera, *contadorListaEspera * sizeof(tListaEspera));
        if (vListaEspera == NULL)
        {
            perror("Erro ao reservar memoria\n");
            *contadorListaEspera = 0;
        }
        else
        {
            ListaEsperaLida = fread(vListaEspera, sizeof(tListaEspera), *contadorListaEspera, fp);
            if (ListaEsperaLida != *contadorListaEspera)
            {
                printf("Erro a ler a Lista de Espera");
                free(vListaEspera);
                vListaEspera = NULL;
                *contadorListaEspera = 0;
            }
        }
        printf("Lidos %d grupos na lista de espera.\n", ListaEsperaLida);
        fclose(fp);
    }
    return vListaEspera;
}
                                                                                                            //Ficheiro log viagens
void escreverLog(tViagens vViagens[], int contadorViagens)
{
    FILE * fp;
    int viagensLidas,n,i;
    fp = fopen("data/viagens.log", "w");
    if (fp == NULL)
    {
        perror("Erro na abertura do ficheiro");
    }
    else
    {
        fprintf(fp,"Log\nN.Viagens: %d\n",contadorViagens);
        for (i=0; i<contadorViagens; i++){
            fprintf(fp,"\t-Id Balao: %d\n", vViagens[i].idBalao);
            fprintf(fp,"\t->Viagem N.: %d\n", vViagens[i].idViagem);
            fprintf(fp,"\t--->Ponto Inicial: %c\n", vViagens[i].inicioViagem);
            fprintf(fp,"\t--->Ponto Final: %c\n", vViagens[i].destinoViagem);
            fprintf(fp,"\tQuantidade Turistas: %d\n", vViagens[i].qntTuristasTransportados);
        }
        fclose(fp);
    }
}
/************************************************************************
 *                          Funcoes de Baloes                           *
 ************************************************************************/
                                                                                                            //Adiciona Baloes
void adicionarDadosBaloes(tBaloes vBaloes[],  int *contadorBaloes, int *idNumBaloes)
{
    int mes;
    printf("\n\n -- Inserir dados do Balao -- \n\n");
    if(*contadorBaloes < MAX_BALOES)
    {
        // Numero do Balao vai ser automatico pois vai buscar o ultimo criado
        (*idNumBaloes)++;
        vBaloes[*contadorBaloes].idBalao = *idNumBaloes;
        // Descricao do Balao
        lerString("Descricao do Balao: ", vBaloes[*contadorBaloes].designacaoBalao, MAX_DESIGNACAO);
        printf("->Data de Fabrico\n");
        vBaloes[*contadorBaloes].dataFabrico.ano = lerAno();
        vBaloes[*contadorBaloes].dataFabrico.mes = lerMes();
        vBaloes[*contadorBaloes].dataFabrico.dia = lerDia(vBaloes[*contadorBaloes].dataFabrico.mes);
        //Lotacao Maxima do Balao
        vBaloes[*contadorBaloes].lotacaoMaxima = lerInteiro("Lotacao Maxima do Balao: ", 1, MAX_LOTACAO);
        //Ponto Inicial do Balao
        vBaloes[*contadorBaloes].pontoInicialBalao = lerPontoInicial("Posicao inicial do balao (A, B, C) ");
        vBaloes[*contadorBaloes].totalViagem = 0;
        vBaloes[*contadorBaloes].totalPassageiros = 0;
        (*contadorBaloes)++;
    }
    else
    {
        printf("Nao pode inserir mais baloes!\n\n");
    }
}
                                                                                                            //Eliminar Baloes
void eliminarBaloes(tBaloes vBaloes[MAX_BALOES], int *contadorBaloes)
{
    int i, pos, numeroBalao;
    char confirm;
    if(contadorBaloes == 0){
        printf("Nao existem baloes para eliminar.");
    }
    else
    {
        do
        {
            printf("\nEscolha um dos seguintes balões para eliminar:\n");
            for(i=0;i<*contadorBaloes;i++)
            {
                printf("\n----Id: %d, Descricao: %s ", vBaloes[i].idBalao, vBaloes[i].designacaoBalao);
            }
            numeroBalao = lerInteiro("\n\nNumero do Balao", MIN_NUMERO_BALAO, MAX_NUMERO_BALAO);
            pos = procuraBalao(vBaloes, *contadorBaloes, numeroBalao);

            if (pos == -1)
            {
                printf ("\n\nBalao nao existe!\n\n");
            }
        }
        while(pos == -1);
        printf("\nTem a certeza que pretende eliminar este Balao? (S/N)");
        scanf("%s",&confirm);
        limparBufferTeclado();
        confirm = toupper(confirm);
        if(confirm == 'S')
        {
            for (i=pos; i < *contadorBaloes-1; i++)
            {
                vBaloes[i] = vBaloes[i+1];
            }
            (*contadorBaloes)--;
            printf("\nBalao eliminado com sucesso!\n");
        }
        else
        {
            printf ("\nElimininacao sem efeito!\n");
        }
    }
}
                                                                                                            //Editar Baloes
void editarDadosBaloes(tBaloes vBaloes[MAX_BALOES], int contadorBaloes)
{
    int i, pos, numeroBalao;
    char confirm;
    if(contadorBaloes == 0)
    {
        printf("Sem baloes inseridos");
    }
    else
    {
        numeroBalao = lerInteiro("Numero do Balao para editar", MIN_NUMERO_BALAO, MAX_NUMERO_BALAO);
        pos = procuraBalao(vBaloes, contadorBaloes, numeroBalao);
        if (pos == -1)
        {
            printf ("Balao não existe!");
        }
        else {
            printf("Pretende editar estes dados?(S/N)\n");
            printf(".____________________________________.\n");
            printf("|  Id Balao: %d  \t\t\t|\n", vBaloes[pos].idBalao);
            printf("|  Designacao: %s\t\t\t|\n",  vBaloes[pos].designacaoBalao);
            printf("|  Data de Fabrico: %d-%d-%d  \t|\n",vBaloes[pos].dataFabrico.ano,vBaloes[pos].dataFabrico.mes,vBaloes[pos].dataFabrico.dia);
            printf("|  Lotacao Maxima: %d \t\t\t|\n", vBaloes[pos].lotacaoMaxima);
            printf("|  Ponto Inicial de Partida: %c \t\t|\n", vBaloes[pos].pontoInicialBalao);
            printf("|_____________________________________|\n");
            printf ("->");
            scanf("%s",&confirm);
            limparBufferTeclado();
            confirm = toupper(confirm);
            if(confirm == 'S')
            {
                lerString("Descricao do Balao: ", vBaloes[pos].designacaoBalao, MAX_DESIGNACAO);
                printf("->Data de Fabrico\n");
                vBaloes[pos].dataFabrico.ano = lerAno();
                vBaloes[pos].dataFabrico.mes = lerMes();
                vBaloes[pos].dataFabrico.dia = lerDia(vBaloes[pos].dataFabrico.mes);
                //Lotacao Maxima do Balao
                vBaloes[pos].lotacaoMaxima = lerInteiro("Lotacao Maxima do Balao: ", 1, MAX_LOTACAO);
                //Ponto Inicial do Balao
                vBaloes[pos].pontoInicialBalao = lerPontoInicial("Posicao inicial do balao (A, B, C) ");
                printf ("Edicao completa!\n");
            }
            else
            {
                printf ("Edicao sem efeito!\n");
            }
        }
    }
}
                                                                                                            //Procurar Baloes
int procuraBalao(tBaloes vBaloes[], int contadorBaloes, int numeroBalao)
{
    int i, posicao=-1;

    for(i=0;i<contadorBaloes;i++)
    {
        if (vBaloes[i].idBalao == numeroBalao)          // Elemento encontrado
        {
            posicao=i;                                  // Armazena posicao no vetor
            i=contadorBaloes;                           // para nao continuar no for..
        }
    }
    return posicao;
}
                                                                                                            //Mostrar Baloes
void mostrarDadosBaloes(tBaloes vBaloes[MAX_BALOES], int contadorBaloes)
{
    int i;
    if(contadorBaloes == 0){
        printf("Sem baloes inseridos");
    }
    else
    {
        for(i=0; i < contadorBaloes; i++)
        {
            printf("Id : %d \n\t Designacao: %s\n", vBaloes[i].idBalao, vBaloes[i].designacaoBalao);
            printf("\t Data de Fabrico: %d-%d-%d  ", vBaloes[i].dataFabrico.ano,vBaloes[i].dataFabrico.mes,vBaloes[i].dataFabrico.dia);
            printf("\n \t Lotacao Maxima: %d ", vBaloes[i].lotacaoMaxima);
            printf("\n \t Ponto Inicial de Partida: %c ", vBaloes[i].pontoInicialBalao);
            printf("\n------------------------------------");
            printf("\n \t Total Passageiros: %d\n\n", vBaloes[i].totalPassageiros);
        }
    }
}
                                                                                                            //Lotacao Maxima Baloes
int lotacaoMaximaBaloes(tBaloes vBaloes[], int contadorBaloes)
{
    int i=0, aux=0;
    for(i=0;i<contadorBaloes;i++)
    {
        if(vBaloes[i].lotacaoMaxima > aux)
        {
            aux = vBaloes[i].lotacaoMaxima;
        }
    }
    return aux;
}
/************************************************************************
 *                          Funcoes Viagens                             *
 ************************************************************************/
                                                                                                            //Mostrar viagens
void mostrarViagens(tViagens vViagens[], int contadorBaloes, int contadorViagens)
{
    int i;
    if(contadorBaloes == 0){
        printf("Sem baloes inseridos");
    }
    else
    {
        if(contadorViagens == 0){
            printf("Sem Viagens registadas");
        }
        else
        {
            for(i=0; i < contadorViagens; i++)
            {
                printf("Id Viagem : %d \n\t Id Balao: %d\n", vViagens[i].idViagem, vViagens[i].idBalao);
                printf("\t Inicio Viagem: %c \n \t Final Viagem: %c \n \t Quantidade de Turistas: %d \n\n",vViagens[i].inicioViagem, vViagens[i].destinoViagem, vViagens[i].qntTuristasTransportados);

            }
        }
    }
}
                                                                                                            //Avancar Ponto do Balao
char avancarPonto(char ponto)
{
    char nextPoint;
    switch(ponto)
    {
        case 'A':
            nextPoint = 'B';
            break;
        case 'B':
            nextPoint = 'C';
            break;
        case 'C':
            nextPoint = 'A';
            break;
    }
    return nextPoint;
}
                                                                                                            //etapa 1 - retirar passageiros que chegaram ao destino (status == 1)
int tirarPassageirosBalao(tViagens vViagens[], int *contadorViagens, char destino, int idBalao)
{
    int passageirosSair=0, i;
    for(i=0; i < *contadorViagens; i++)
    {
        if ( (vViagens[i].status == 1) && ( vViagens[i].destinoViagem == destino) && ( vViagens[i].idBalao == idBalao))
        {
            passageirosSair = vViagens[i].qntTuristasTransportados;
            vViagens[i].status = 0;
        }
    }
    return passageirosSair;
}
                                                                                                            //etapa 2 - procurar na lista de espera os passageiros que vao entrar no balao, e guardar os IDs que irão ser apagados da lista de espera
int procurarListaEspera(tListaEspera vListaEspera[], int idEliminarListaEspera[MAX_LOTACAO], int contadorListaEspera, int idBalao, char pontoInicialBalao, int lugaresDisponiveis, int *contadorIdEliminarListaEspera)
{
    int i, passageirosParaEntrar=0, qntDisponivel, auxIDListaEspera;
    
    qntDisponivel = lugaresDisponiveis;
    (*contadorIdEliminarListaEspera)=0;
    
    for(i=0;(lugaresDisponiveis > passageirosParaEntrar) && (i < contadorListaEspera);i++)                  //faz ciclo enquanto existe lugares disponiveis
    {
        if(vListaEspera[i].pontoInicioViagem == pontoInicialBalao)                                          //verifica o ponto de entrada das pessoas
        {
            if(vListaEspera[i].numeroPessoasEspera <= qntDisponivel)                                         //verifica quantos lugares existem
            {
                passageirosParaEntrar += vListaEspera[i].numeroPessoasEspera;                               //Coloca passageiros em fila para entrar no balao (revisor à porta)
                qntDisponivel -= vListaEspera[i].numeroPessoasEspera;                                       //Reduz a quantidade de lugares disponiveis no balao
                idEliminarListaEspera[*contadorIdEliminarListaEspera] = i;                                  //Guardar ids da lista de espera para eliminar do vetor
                (*contadorIdEliminarListaEspera)++;
            }
        }
    }
    return passageirosParaEntrar;
}
                                                                                                            // etapa 3 - colocar passageiros da lista de espera no balao
tViagens* colocarNaViagem(tListaEspera vListaEspera[], tViagens *vViagens, int *contadorViagens, int idBalao, int pontoBalao, int *viagensErro, int idEliminarListaEspera[], int contadorIdEliminarListaEspera)
{
    tViagens * auxViagem=NULL;
    auxViagem = vViagens;
    *viagensErro = 0;
    
    int i=0, qntA=0, qntB=0, qntC=0, qntTotal=0, pontoListaEspera=0;
    char pontoInicial, pontoFinal;

    vViagens = realloc(vViagens, (*contadorViagens+1)*sizeof(tViagens));

    if (vViagens == NULL)
    {
        perror("Nao ha mais memoria!");
        vViagens = auxViagem;
        *viagensErro = 1;
    }
    else
    {
        for (i=0;i < contadorIdEliminarListaEspera;i++)                                                         //contem os ids para uma linha de viagens
        {
            pontoListaEspera = idEliminarListaEspera[i];                                                        //por cada vetor de lista de espera vai haver apenas 1 viagem
            pontoInicial = vListaEspera[pontoListaEspera].pontoInicioViagem;                                    //o ponto inicial vai ser sempre o mesmo pois e onde o balao se encontra
            pontoFinal = vListaEspera[pontoListaEspera].pontoFimViagem;                                         //o ponto final e apenas para saber mos no switch onde incrementar a quantidade de passageiros
            qntTotal += vListaEspera[pontoListaEspera].numeroPessoasEspera;                                     //quantidade total no balao
            switch(pontoFinal)                                                                                  //ira contar quantas pessoas irao sair para cada ponto seguinte onde o balao passar
            {
                case 'A':
                    qntA += vListaEspera[pontoListaEspera].numeroPessoasEspera;
                    break;
                case 'B':
                    qntB += vListaEspera[pontoListaEspera].numeroPessoasEspera;
                    break;
                case 'C':
                    qntC += vListaEspera[pontoListaEspera].numeroPessoasEspera;
                    break;
            }
        }
        if(qntTotal == 0)
        {
            pontoInicial = pontoBalao;
        }
        vViagens[*contadorViagens].idViagem = *contadorViagens + 1;
        vViagens[*contadorViagens].inicioViagem = pontoInicial;                                                 //onde o balao se encontra
        vViagens[*contadorViagens].destinoViagem = avancarPonto(pontoInicial);                                  //proximo ponto
        vViagens[*contadorViagens].qntTuristasTransportados = qntTotal;                                         //quantidade total que o balao tem
        vViagens[*contadorViagens].qntTransportados.qntPassageirosA = qntA;                                     //quantidade de pessoas a sair no ponto A
        vViagens[*contadorViagens].qntTransportados.qntPassageirosB = qntB;                                     //quantidade de pessoas a sair no ponto B
        vViagens[*contadorViagens].qntTransportados.qntPassageirosC = qntC;                                     //quantidade de pessoas a sair no ponto C
        vViagens[*contadorViagens].idBalao = idBalao;                                                           //id do balao a qual esta viagem pertence
        vViagens[*contadorViagens].status = 1;                                                                  //status = 1 esta em viagem /|/ status = 0 ja fez a viagem
        
        (*contadorViagens)++;
        
    }
    return vViagens;
}
                                                                                                            //etapa 4 - eliminar os passageiros que entraram no balao da lista de espera
tListaEspera * eliminarListaEspera(tListaEspera *vListaEspera, int *contadorListaEspera, int *listaEsperaErro, int idEliminarListaEspera[], int contadorIdEliminarListaEspera)
{
    int numeroListaEliminar, indice, i, ii;
    tListaEspera * backupListaEspera;
    backupListaEspera = vListaEspera;

    if (*contadorListaEspera == 0)
    {
        printf("A lista de Espera está vazia.\n");
    }
    else
    {
        for(ii=0; ii<contadorIdEliminarListaEspera; ii++)
        {
            indice = idEliminarListaEspera[ii];
            for (i=indice; i< *contadorListaEspera-1 ; i++)
            {
                vListaEspera[i] = vListaEspera[i+1];
            }
            if (*contadorListaEspera == 1)
            {
                free(vListaEspera);
                vListaEspera = NULL;
                *listaEsperaErro = 1;
            }
            else
            {
                vListaEspera = realloc(vListaEspera, (*contadorListaEspera-1)*sizeof(tListaEspera));
                if (vListaEspera == NULL)
                {
                    perror("Erro ao libertar memoria!");
                    vListaEspera = backupListaEspera;
                }
            }
            (*contadorListaEspera)--;
        }
    }
    return vListaEspera;
}
                                                                                                            //funcao para ordenar os ids a eliminar na ordem inversa para nao eliminar as linhas erradas
int ordenarIdEliminarListaEspera (const void *v1, const void *v2){
    int *valor1, *valor2;
    int comp;
    valor1 = (int *) v1;
    valor2 = (int *) v2;
    comp = valor1 - valor2;
    return comp;
}
/************************************************************************
 *                          Funcoes Bilhetes                             *
 ************************************************************************/
                                                                                                            //Comprar Bilhete
tListaEspera *compraBilhete(tListaEspera vListaEspera[], tBaloes vBaloes[], float preco, int contadorBaloes, float *totalReceita, int *contadorListaEspera)
{
    tListaEspera *pLista;
    pLista = vListaEspera;

    int maximoLotacaoDisponivel, contadorBilhetes;
    char pontoInicioViagem, pontoFimViagem;
    float precoBilhetesGrupo;

    vListaEspera = realloc(vListaEspera,(*contadorListaEspera+1)*sizeof(tListaEspera));
    if (vListaEspera == NULL) {
       printf ("Memória insuficiente");
       vListaEspera = pLista;
    }
    else
    {
        if(contadorBaloes == 0){
            printf("Neste momento não temos baloes a funcionar. Por favor volte mais tarde.");
        }
        else
        {
            printf("\n\n -- Compra de Bilhetes -- \n\n");
            maximoLotacaoDisponivel = lotacaoMaximaBaloes(vBaloes, contadorBaloes);
            contadorBilhetes = lerInteiro("Quantas pessoas irao viajar?", 1, maximoLotacaoDisponivel);
            vListaEspera[*contadorListaEspera].numeroPessoasEspera = contadorBilhetes;

            if (contadorBilhetes == 1)
            {
                printf("\n\tVocê deve pagar %.2f € pelo bilhete.", preco);
                *totalReceita += preco;
            }
            else
            {
                preco = preco * 0.9;
                precoBilhetesGrupo = preco * contadorBilhetes;
                *totalReceita += precoBilhetesGrupo;

                printf("\n\tBilhete de grupo contem desconto de 10%% por pessoa.");
                printf("\n\tVoce deve pagar %.2f Euros pelo bilhete de grupo.\n", precoBilhetesGrupo);

            }
            vListaEspera[*contadorListaEspera].pontoInicioViagem = lerPontoInicial("\nPosicao inicial da viagem (A, B, C): ");
            vListaEspera[*contadorListaEspera].pontoFimViagem = lerPontoInicial("Posicao final da viagem (A, B, C): ");
            (*contadorListaEspera)++;
        }
    }
    return vListaEspera;
}
                                                                                                            //Quantidade de Bilhetes em Lista de espera
int contarBilhetesEspera(tListaEspera vListaEspera[], int contadorListaEspera, char ponto)
{
    int i=0, totalBilhetes=0;
    for(i=0;i<contadorListaEspera;i++)
    {
        if(ponto == vListaEspera[i].pontoInicioViagem)
        {
            totalBilhetes += vListaEspera[i].numeroPessoasEspera;
        }
    }
    return totalBilhetes;
}
                                                                                                            //Quantidade de Bilhetes nas viagens
int contarBilhetesUsados(tViagens vViagens[], int contadorViagens, char ponto)
{
    int i=0, totalBilhetes=0;
    for(i=0;i<contadorViagens;i++)
    {
        if(ponto == vViagens[i].inicioViagem)
        {
            totalBilhetes += vViagens[i].qntTuristasTransportados;
        }
    }
    return totalBilhetes;
}
                                                                                                            //Consultar Bilhetes vendidos
int consultarBilhetes(tViagens vViagens[], tListaEspera vListaEspera[], int contadorViagens, int contadorListaEspera)
{
    int totalBilhetes=0, bilhetesUsados=0, bilhetesEspera=0;
    char var;
    if(contadorViagens + contadorListaEspera == 0)
    {
        printf("Nao existem bilhetes para consultar.");
    }
    else
    {
        totalBilhetes = contarBilhetesEspera(vListaEspera, contadorListaEspera, 'A') + contarBilhetesUsados(vViagens, contadorViagens, 'A');
        printf("\n\tForam vendidos %d bilhetes no ponto A", totalBilhetes);

        totalBilhetes = contarBilhetesEspera(vListaEspera, contadorListaEspera, 'B') + contarBilhetesUsados(vViagens, contadorViagens, 'B');
        printf("\n\tForam vendidos %d bilhetes no ponto B", totalBilhetes);

        totalBilhetes = contarBilhetesEspera(vListaEspera, contadorListaEspera, 'C') + contarBilhetesUsados(vViagens, contadorViagens, 'C');
        printf("\n\tForam vendidos %d bilhetes no ponto C", totalBilhetes);
    }
}

/************************************************************************
 *                      Funcoes Estatisticas                            *
 ************************************************************************/
void mostrarEstatisticas(tBaloes vBaloes[], tViagens vViagens[], int contadorBaloes, int idNumBaloes, int contadorViagens)
{
    int nViag=0, bCviagens=0, bCviagensTotal=0, i;
    float media=0.0;
    char x;
    
    if(contadorBaloes == 0){
        printf("\nSem baloes inseridos!");
    }
    else
    {
        if(contadorViagens == 0){
            printf("\nSem viagens registadas!");
        }
        else
        {
            bCviagensTotal = balaoComMaisViagens(vViagens, idNumBaloes, contadorViagens, 0);
            
            printf(".__________________________________________________.\n");
            printf("| IDs Baloes | Viagens | medias | Balao c/+Viagens |\n");
            printf("|--------------------------------------------------|\n");
            for(i=0;i<idNumBaloes;i++)
            {
                nViag = quantViagensBalao(vViagens, i+1, contadorViagens);
                media = quantMediaTuristasBalao(vViagens, i+1, contadorViagens);
                
                bCviagens = balaoComMaisViagens(vViagens, idNumBaloes, contadorViagens, i+1);
                if (bCviagensTotal >= bCviagens){
                    x='x';
                }
                else
                {
                    x=' ';
                }
                printf("|     %d      |    %d    |  %.02f   |       %c        |\n", i+1, nViag, media, x);
            }
            printf("|__________________________________________________|\n");
            percentagemTuristasPorDestino(vViagens, contadorViagens);
        }
    }
}
                                                                                                            //Quantidade passageiros transportados
int quantPassageirosTransportados(tBaloes vBaloes[], int contadorBaloes)
{
    int i, total=0;
    
    for(i=0; i < contadorBaloes; i++)
    {
        total += vBaloes[i].totalPassageiros;
    }
    return total;
}
                                                                                                            //Quantidade passageiros em lista de espera
int quantPassageirosEspera(tListaEspera vListaEspera[], int contadorListaEspera)
{
    int quant=0, i;
    
    for(i=0; i < contadorListaEspera; i++)
    {
        quant += vListaEspera[i].numeroPessoasEspera;
    }
    return quant;
}
                                                                                                            //Quantidade de viagens realizadas
int quantViagensBalao(tViagens vViagens[], int idNumBaloes, int contadorViagens)
{
    int i, quantViagens=0;
    
    for(i=0; i<contadorViagens; i++)
    {
        if(vViagens[i].idBalao == idNumBaloes)
        {
            quantViagens++;
        }
    }
    return quantViagens;
}

int quantpassageirosEmBalao(tViagens vViagens[], int contadorViagens)
{
    int i, quantViagens=0;
    for(i=0; i<contadorViagens; i++)
    {
        if(vViagens[i].status == 1)
        {
            quantViagens++;
        }
    }
    return quantViagens;
}

float quantMediaTuristasBalao(tViagens vViagens[], int idNumBaloes, int contadorViagens)
{
    int i,a, quant=0, aux=0;
    float media=0;
    
    for(i=0; i<contadorViagens; i++)
    {
        if(vViagens[i].idBalao == idNumBaloes)
        {
            quant += vViagens[i].qntTuristasTransportados;
            aux++;
        }
    }
    media = quant / aux;
    
    return media;
}

int balaoComMaisViagens(tViagens vViagens[], int idNumBaloes, int contadorViagens, int nBalao)
{
    int i, quantViagens=0;
    if(nBalao == 0)
    {
        for(i=0; i<contadorViagens; i++)
        {
            if(vViagens[i].idBalao == idNumBaloes)
            {
                quantViagens++;
            }
        }
    }
    else
    {
        for(i=0; i<contadorViagens; i++)
        {
            if(vViagens[i].idBalao == nBalao)
            {
                quantViagens++;
            }
        }
    }
    return quantViagens;
}

void percentagemTuristasPorDestino(tViagens vViagens[], int contadorViagens)
{
    int i, qntA=0, qntB=0, qntC=0, totalTransportados=0;
    float percentagemA=0, percentagemB=0, percentagemC=0;
    
    for(i=0; i<contadorViagens; i++)
    {
        switch(vViagens[i].destinoViagem)
        {
            case 'A':
                qntA += vViagens[i].qntTransportados.qntPassageirosA;
                break;
            case 'B':
                qntB += vViagens[i].qntTransportados.qntPassageirosB;
                break;
            case 'C':
                qntC += vViagens[i].qntTransportados.qntPassageirosC;
                break;
        }
        
    }
    totalTransportados = qntA + qntB + qntC;
    percentagemA = (qntA * 100 ) / totalTransportados;
    percentagemB = (qntB * 100 ) / totalTransportados;
    percentagemC = (qntC * 100 ) / totalTransportados;
    
    printf("\nO total passageiros para o ponto A foi %.2f%%", percentagemA);
    printf("\nO total passageiros para o ponto B foi %.2f%%", percentagemB);
    printf("\nO total passageiros para o ponto C foi %.2f%%", percentagemC);
}

int obterMaiorGrupoListaEspera(tListaEspera vListaEspera[], int contadorListaEspera, char ponto)
{
    int i, aux=0;
    
    for(i=0;i<contadorListaEspera;i++)
    {
        if(ponto == vListaEspera[i].pontoInicioViagem)
        {
            if(vListaEspera[i].numeroPessoasEspera > aux)
            {
                aux = vListaEspera[i].numeroPessoasEspera;
            }
        }
    }
}

void estatisticaListaEspera(tListaEspera vListaEspera[], int contadorListaEspera)
{
    int i=0, totalPassageirosEspera=0, maiorGrupoEspera=0;
    
    if(contadorListaEspera == 0)
    {
        printf("\nNao existem passageiros em espera.");
    }
    else
    {
        totalPassageirosEspera = contarBilhetesEspera(vListaEspera, contadorListaEspera, 'A');
        maiorGrupoEspera = obterMaiorGrupoListaEspera(vListaEspera, contadorListaEspera, 'A');
        printf("\n\t-> Estão %d passageiros em espera no ponto A. O maior grupo e de %d passageiros.", totalPassageirosEspera, maiorGrupoEspera);

        totalPassageirosEspera = contarBilhetesEspera(vListaEspera, contadorListaEspera, 'B');
        maiorGrupoEspera = obterMaiorGrupoListaEspera(vListaEspera, contadorListaEspera, 'B');
        printf("\n\t-> Estão %d passageiros em espera no ponto B. O maior grupo e de %d passageiros.", totalPassageirosEspera, maiorGrupoEspera);
        
        totalPassageirosEspera = contarBilhetesEspera(vListaEspera, contadorListaEspera, 'C');
        maiorGrupoEspera = obterMaiorGrupoListaEspera(vListaEspera, contadorListaEspera, 'C');
        printf("\n\t-> Estão %d passageiros em espera no ponto C. O maior grupo e de %d passageiros.", totalPassageirosEspera, maiorGrupoEspera);
    }
}
