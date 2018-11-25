# LINDERHOF  
  
O projeto Linderhof tem o objetivo de ser uma biblioteca de ataques de negação de serviço refletidos/amplificados.  
  
# Build
Requisitos:
- CMake

Para compilar basta executar o script build.sh. Os binários estarão na pasta bin do projeto.
Caso não queira utilizar o script, execute após a compilação o comando:

    sudo setcap cap_net_raw+ep bin/lhf

Após executar esse comando não será necessário executar o lhf como superuser.

# Arquitetura
[Linderhof.pdf](https://github.com/rqtx/Linderhof/files/2562968/Linderhof.pdf)

## Oryx
UI engine.

Existem duas possíveis interfaces:

 - OryxCli

OryxCli deve ser utilizada caso queria acessar o linderhof na maquina local (API padrão). 

#### Comandos
[Opcoes_lhf.pdf](https://github.com/rqtx/Linderhof/files/2563008/Opcoes_lhf.pdf)

## Linderhof
Core engine.

Biblioteca dos ataques. Aqui é onde montamos um plano de ataque e executamos o injetor.

## Netuno
Injector engine.

Injetor de pacotes.
O netuno é o gerenciador dos injetores de ataque. Com cada injetor podendo no máximo ter um throughput de 100 Mb/s.

# Criando mirror

## Passo 1: Criar a função de chamada do mirror

A função de chamada do mirror deve seguir o protótipo a seguir:

    int ExecuteMirrorNameMirror( void *p_arg );

Sendo p_arg os argumentos necessários para executar o mirror. 
Após o linderhof fazer a chamada da função mirror é dever dela fazer toda a preparação do ataque, com tudo pronto deve fazer a chamada da engine de injeção Netuno.

Os arquivos do mirror devem ficar na pasta src/linderhof/hom/nome_do_mirror/.

A função de chamada será declarada em src/linderhof/hom/nome_do_mirror.h

## Passo 2: Adicionar mirror ao Linderhof planner

2.1 - Adicione seu mirror no enum MirrorType em src/include/venus.h

Os passo seguintes serão executados no arquivo src/linderhof/commander/planner.c.

2.2 - Inclua o header com a função de chamada do mirror.

2.3 Atualize o switch-case da função Planner para  construir o LhfPlan do seu mirror. O ponteiro atkData deve conter os argumentos que serão utilizados pelo mirror.

## Passo 3: Adicionar mirror a engine UI Oryx

No arquivo src/interface/interface.c:parserAttackOpt adicione a construção do draft do seru mirror.

## Observações

- Para forjar um pacote utilize o forjador de pacotes blacksmitsh. Sua API está disponível na biblioteca common do projeto, visando o objetivo de padronizar o forjador de pacotes do projeto.
- Caso seja necessário dentro do mirror fazer alguma chamada para as funcionalidades de alocação de memória, netio, linux capability e signals utilize a API common do projeto. Ela está disponível na biblioteca common e está incluída no header venus.h que deve ser incluído em todos os .c do projeto.

