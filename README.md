# LINDERHOF  
  
O projeto Linderhof tem o objetivo de ser uma biblioteca de ataques de negação de serviço refletidos/amplificados.  
  
# Build
Requisitos:
- CMake

Para compilar basta executar o script build.sh. Os binários estarão na pasta bin do projeto.
# Arquitetura

## Oryx
UI engine.
Existem duas possíveis interfaces:

 - OryxCli
 - OryxNet

OryxCli deve ser utilizada caso queria acessar o linderhof na maquina local (API padrão). 

OryxNet deve ser utilizada para acessar o Lindehof em uma máquina hospedeira. Para compilar o Linderhof com o OryxNet deve-se descomentar a linha 17 do **CMakeLists.txt**.

### Comandos API
A sintaxe:
CMD cmd [args]

#### Ataque
- CMD 
atk
- cmd
OBS :Obrigatório. Esse parâmetro corresponde ao tipo de espelho utilizado. 
Espelhos disponíveis:
	- test 	
	- memcached
- args
   -a, --amplifier=amp_ipv4   Amplificador IPV4 (OBRIGATÓRIO)
   -t, --target=target_ipv4   Alvo IPV4 (OBRIGATÓRIO)
  -f, --full=thp Executa um ataque full (Se omitido é executado o ataque default)
  -g, --targport=targ_port   Target port (Se omitido porta 80)
  -p, --amport=am_port       Amplifier port (Se omitido porta padrão ataque)
  -r, --timer=timer          Tempo de execução em minutos (Se omitido tempo padrão)

O ataque INCREMENT é o ataque **default**. Ele começa com um throughput de 1 Mb/s e vai incrementando o ataque até o tempo *timer* em 1 Mb/s a cada 5 minutos. O FULL é um ataque com um throughput constante *thp*, é executado até o tempo *timer*.

#### Encerrar conexão OryxNet
- CMD
	Exit

## Linderhof
Core engine.
Biblioteca dos ataques. Aqui é onde montamos um plano de ataque e executamos o injetor.

## Netuno
Injector engine.
Injetor de pacotes.

# Adicionando um Mirror
<!--stackedit_data:
eyJoaXN0b3J5IjpbNzMyMzU1OTUzLDY5MDQ5OTk3NSwxMDIxMz
I1MzYsLTE0NzU1OTU2NjcsLTQwOTI2MzY0NiwxNzA0NzExODE0
LDcxNjI2Mzk0OCwtMTcwNzM0NTUzNCw1MjIwMTM4MjgsLTk5Mz
IyNDU4Nl19
-->