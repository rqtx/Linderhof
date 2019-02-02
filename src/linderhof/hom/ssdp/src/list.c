
/**
 * Arquivo responsavel por definir a estrutura de lista
 * que sera utilizada para armazenar os dispositivos da rede.
 * Autoria: Eduardo Schuabb Duarte
 *  Matricula: 110010876
 *  27/09/2018
*/

#include "list.h"


dispositivoSSDP* obtemDispPorIP(listDispositivoSSDP * head, char* ip){
    listDispositivoSSDP * current = head;

    while(strcmp(current->dispositivo->ip, ip) != 0){
        current = current->next;    
    }

    return current;
}

dispositivoSSDP* obtemDispPorPosicao(listDispositivoSSDP * head, int posicao){
    listDispositivoSSDP * current = head;
    int count = 0;

    while (current != NULL) {
        if(count == posicao)
            break;
        count++;    
        current = current->next;
    }
    return current->dispositivo;
}

int contaRefletores(listDispositivoSSDP * head){
    listDispositivoSSDP * current = head;
    int count = 0;

    while (current != NULL) {
        if(current->dispositivo->tipoDispositivo == STATUS_DISP_REFLETOR){
            count++;    
        }
        current = current->next;
    }
    return count;
}

int contaAlvos(listDispositivoSSDP * head){
    listDispositivoSSDP * current = head;
    int count = 0;

    while (current != NULL) {
        if(current->dispositivo->tipoDispositivo == STATUS_DISP_ALVO){
            count++;    
        }
        current = current->next;
    }
    return count;

}

void mostrarRefletores(listDispositivoSSDP * head){
    listDispositivoSSDP * current = head;
    int count = 0;

    while (current != NULL) {
        if(current->dispositivo->tipoDispositivo == STATUS_DISP_REFLETOR){
            printf("%d - ", count);
            printDispositivo(current->dispositivo);
        }
        current = current->next;
        count++;
    }
}

void mostrarAlvos(listDispositivoSSDP * head){
    listDispositivoSSDP* current = head;
    int count = 0;

    while (current != NULL) {
        if(current->dispositivo->tipoDispositivo == STATUS_DISP_ALVO){
            printf("%d - ", count);
            printDispositivo(current->dispositivo);
        }
        current = current->next;
        count++;
    }
}

void mostrarDispositivosObtidos(listDispositivoSSDP* head) {
    listDispositivoSSDP* current = head;
    int count = 0;

    while (current != NULL) {
        if(current->dispositivo->tipoDispositivo != -1){
            printf("%d - ", count);
            printDispositivo(current->dispositivo);
        }
        current = current->next;
        count++;
    }
}

void printDispositivo (dispositivoSSDP* disp){

    printf("IP: %s\n", disp->ip);
    printf("status: %d\n", disp->tipoDispositivo);
    printf("Numero de respostas: %d\n", disp->quantMsgResposta);
    printf("Quant. de bytes das respostas: %d\n", disp->quantBytesResp);
}

void insereDispositivo(listDispositivoSSDP * head, char* ip, int statusDisp, int tamanhoMensagem){
    
    listDispositivoSSDP* current = head;
    listDispositivoSSDP* novoDisp = malloc(sizeof(listDispositivoSSDP));

    novoDisp->dispositivo = criaDispositivo(ip, statusDisp, tamanhoMensagem);

    novoDisp->next = NULL;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = novoDisp;
}

dispositivoSSDP* criaDispositivo (char* ip, int statusDisp, int tamanhoMensagem){

    dispositivoSSDP* novoDisp;
    novoDisp = malloc(sizeof(dispositivoSSDP));
    novoDisp->ip = (char*)malloc(sizeof(char)*16);  

    strcpy(novoDisp->ip, ip);
    novoDisp->tipoDispositivo = statusDisp;

    novoDisp->quantMsgResposta = 1;
    novoDisp->quantBytesResp = tamanhoMensagem;

    return novoDisp;
}

int verificaSePossuiIP(listDispositivoSSDP* listaDisp, char* novoIp) {
    listDispositivoSSDP* current = listaDisp;

    while (current != NULL) {
        if(strcmp(current->dispositivo->ip, novoIp) == 0){ // entao ja existe esse ip.
            return 1;
        }
        current = current->next;
    }
    return 0;
}

listDispositivoSSDP* iniciaListDisp(){

    char* ipVazio = "000.000.000.000";

    listDispositivoSSDP* listaDispositivosSSDP = NULL;

	listaDispositivosSSDP = malloc(sizeof(listDispositivoSSDP));
	if (listaDispositivosSSDP == NULL) {
		printf("Erro ao criar lista do dispositivo HEAD.");
    	return NULL;
	}

    dispositivoSSDP* dispSSDP = NULL;

    dispSSDP = malloc(sizeof(dispositivoSSDP));
	if (dispSSDP == NULL) {
		printf("Erro ao criar lista do dispositivo HEAD.");
    	return NULL;
	}
    dispSSDP->ip = (char*)malloc(sizeof(char)*16);
    strcpy(dispSSDP->ip, ipVazio);
    dispSSDP->tipoDispositivo = -1;

    listaDispositivosSSDP->dispositivo = dispSSDP;
    listaDispositivosSSDP->next = NULL;

    return listaDispositivosSSDP;
}

int contaBytesTodosRefletores(listDispositivoSSDP* head){
    listDispositivoSSDP * current = head;
    int countBytes = 0;

    while (current != NULL) {
        if(current->dispositivo->tipoDispositivo == STATUS_DISP_REFLETOR){
            countBytes = current->dispositivo->quantBytesResp + countBytes;    
        }
        current = current->next;
    }
    return countBytes;
}