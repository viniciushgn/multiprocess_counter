/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#define MAX_NUM 100


//init variaveis
int *numero_elementos;
int *numero_primos;
long long unsigned int *lista;


//TESTE_PRIMO--------------------------WORKING----------------------
int isPrime(long long unsigned int number){
long long unsigned int i = 2;

if(number == 0 | number == 1){ //1 e 0 nao sao primos nem sao divisores validos
  return 0;
}

while ( i < number){ //testando todos os valores complexidade O(n)
  if(number%i == 0){
    return 0;
  }
  i++;
}
return 1;
}
//------------------------------------------------------------------------------------


//--LER TECLADO CRIAR VETOR UNIVERSAL TAMANHO E CONTADOR-----------------------------------------------
void readDeclareVector (void){

// Ler teclado
unsigned long long int triagem[MAX_NUM];
char lixo;
unsigned int i;
i = 0;
while(lixo != '\n'){
scanf("%llu", &triagem[i]);
lixo = getchar();
i++;
}
// Declarar Vetores Universais
// Definir flags de protecao e visibilidade de memoria

int protection = PROT_READ | PROT_WRITE;
int visibility = MAP_SHARED | MAP_ANON;

// Criar vetor compartilhado
numero_elementos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0,0);
if ((long int)numero_elementos == -1) {printf("Erro de alocacao!\n");}
(*numero_elementos)=i;

lista = (long long unsigned int*) mmap(NULL, sizeof(long long unsigned int)*i, protection, visibility, 0,0);
if ((long int)lista == -1) {printf("Erro de alocacao!\n");}

for(unsigned int k = 0; k < i; k++)
{
  lista[k] = triagem[k];
}


numero_primos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0,0);
if ((long int)numero_primos == -1) {printf("Erro de alocacao!\n");}
(*numero_primos)=0;
}
//-------------------------------------------------------------------------------


//-----PROCESSAMENTO------------------------------------------
void proc(int id){

//para separar a tarefa entre processos, podemos trabalhar
//com multiplos percorrendo o vetor de elementos

//ex:.  proc 1 -> [>0,1,2,3,>4,5,6,7,>8,9]

for(int g = 0; (id+g*4) < (*numero_elementos); g++){
if (  isPrime(lista[id + g*4])  ){
  (*numero_primos)++;
}
}

}
//-------------------------------------------------------------


int main() {

readDeclareVector();
//--------------------------------
int n_processos = 0;
if((*numero_elementos) >3){
  n_processos = 4;
}
else{
  n_processos = (*numero_elementos);
}
//---------------------------------
pid_t proc1, proc2, proc3, proc4;

if(n_processos>0){
if((proc1 = fork())== 0){
  proc(0);
  _exit(0);
}
}

if(n_processos>1){
if((proc2 = fork())== 0){
  proc(1);
  _exit(0);
}
}


if(n_processos>2){
if((proc3 = fork())== 0){
  proc(2);
  _exit(0);
}
}

if(n_processos>3){
if((proc4 = fork())== 0){
  proc(3);
  _exit(0);
}
}

//---------------------
if(n_processos>0){
  waitpid(proc1, NULL, 0);
}
if(n_processos>1){
  waitpid(proc2, NULL, 0);
}
if(n_processos>2){
  waitpid(proc3, NULL, 0);
}
if(n_processos>3){
  waitpid(proc4, NULL, 0);
}

printf("%d\n",(*numero_primos) );

  return 0;
}
