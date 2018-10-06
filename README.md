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
Diagrama na pasta docs.

## Oryx
UI engine.

Existem duas possíveis interfaces:

 - OryxCli
 - OryxNet

OryxCli deve ser utilizada caso queria acessar o linderhof na maquina local (API padrão). 

OryxNet deve ser utilizada para acessar o Lindehof em uma máquina hospedeira. Para compilar o Linderhof com o OryxNet deve-se descomentar a linha 17 do **CMakeLists.txt**. Cliente OryxNet é o binário snowman.

### Comandos API
Sintaxe: CMD cmd [args]

#### Ataque
- CMD 
atk
- cmd
OBS :Obrigatório. Esse parâmetro corresponde ao tipo de espelho utilizado. 
Espelhos disponíveis:
	- test 	
	- memcached
- args
<br/>

  `-a, --amplifier=amp_ipv4   Amplificador IPV4 (OBRIGATÓRIO)` 
  
   `-t, --target=target_ipv4   Alvo IPV4 (OBRIGATÓRIO)` 
   
  `-f, --full=thp Executa um ataque full (Se omitido é executado o ataque default)`
  
  `-g, --targport=targ_port   Target port (Se omitido porta 80)`
  
  `-p, --amport=am_port       Amplifier port (Se omitido porta padrão ataque)`
  
  `-r, --timer=timer          Tempo de execução em minutos (Se omitido tempo padrão infinito)`
  
<br/>
O ataque INCREMENT é o ataque **default**. Ele começa com um throughput de 0 Mb/s e vai incrementando o ataque até o tempo *timer* em 1 Mb/s a cada 5 minutos. O FULL é um ataque com um throughput constante *thp*, é executado até o tempo *timer*.

#### Encerrar conexão OryxNet
- CMD
	Exit

## Linderhof
Core engine.

Biblioteca dos ataques. Aqui é onde montamos um plano de ataque e executamos o injetor.

## Netuno
Injector engine.

Injetor de pacotes.
O netuno é o gerenciador dos injetores de ataque. Com cada injetor podendo no máximo ter um throughput de 5 Mb/s.
É possível requisitar um ataque de até 50Mb/s. Entretanto o Netuno não foi capaz de entregar tudo isso até o momento, ele consegue até no máximo 20 Mb/s. Em um momento da injeção (por volta dos 15 Mb/s) o fator de crescimento da injeção começa a diminuir, acredito que por causa de um início de saturação na escrita do buffer do socket.

# Monitoramento
O Netuno sempre terá o log do monitoramento da última injeção no arquivo atklogger.txt. 
Para fazer o log da vítima é necessário compilar o Linderhof com a OryxNet. O target do ataque deve ser o IP do cliente Linderhof. Esse log será salvo no arquivo monitorlogger.txt.

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

# Bugs

O argp está crashando quando mais que 3 opção são chamadas.
