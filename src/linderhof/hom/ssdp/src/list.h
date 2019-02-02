
/**
 * Arquivo responsavel por definir a estrutura de lista
 * que sera utilizada para armazenar os dispositivos da rede.
 * Autoria: Eduardo Schuabb Duarte
 *  Matricula: 110010876
 *  27/09/2018
*/

#include <stdio.h>
#include <string.h>	
#include <stdlib.h>	


static const int STATUS_DISP_ALVO = 0;
static const int STATUS_DISP_REFLETOR = 1;
static const int STATUS_INVALIDO = -1;


typedef struct dispositivoSSDP {
    char* ip;
    int quantMsgResposta;
    int quantBytesResp;
    int tipoDispositivo;            // 0 = Dispositivo de controle. -> Possiveis alvos de ataque.
} dispositivoSSDP;                  // 1 = Dispositivo a ser controlado. -> Possiveis ferramentas de ataque.
                                    // -1 = Invalido.

typedef struct listDispositivoSSDP {
    dispositivoSSDP* dispositivo;
    struct listDispositivoSSDP* next;
} listDispositivoSSDP;


void printDispositivo (dispositivoSSDP* disp);
void mostrarDispositivosObtidos(listDispositivoSSDP* head);
void mostrarRefletores(listDispositivoSSDP* head);
void mostrarAlvos(listDispositivoSSDP* head);
void insereDispositivo(listDispositivoSSDP * head, char* ip, int statusDisp, int tamanhoMensagem);
dispositivoSSDP* criaDispositivo (char* ip, int statusDisp, int tamanhoMensagem);
int verificaSePossuiIP(listDispositivoSSDP* listaDisp, char* novoIp);
listDispositivoSSDP* iniciaListDisp(void);
int contaRefletores(listDispositivoSSDP* head);
int contaAlvos(listDispositivoSSDP* head);
dispositivoSSDP* obtemDispPorPosicao(listDispositivoSSDP * head, int posicao);
dispositivoSSDP* obtemDispPorIP(listDispositivoSSDP * head, char* ip);
int contaBytesTodosRefletores(listDispositivoSSDP* listaDispositivos);


// depois fazer as funcoes para remover os elementos da lista.


