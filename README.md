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

    int func( void *p_arg );

Sendo p_arg os argumentos necessários para executar o mirror. 
Após o linderhof fazer a chamada da função mirror é dever dela fazer toda a preparação do ataque, com tudo pronto deve fazer a chamada da engine de injeção Netuno.

Os arquivos do mirror devem ficar na pasta src/linderhof/mirrors/NOME_DO_MIRROR.

## Passo 2: Adicionar mirror ao Linderhof planner


## Passo 3: Adicionar mirror a engine UI Oryx


## Observações

- Para forjar um pacote utilize o forjador de pacotes blacksmitsh. Sua API está disponível na biblioteca common do projeto, visando o objetivo de padronizar o forjador de pacotes do projeto.
- Caso seja necessário dentro do mirror fazer alguma chamada para as funcionalidades de alocação de memória, netio, linux capability e signals utilize a API common do projeto. Ela está disponível na biblioteca common e está incluída no header venus.h que deve ser incluído em todos os .c do projeto.

