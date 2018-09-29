# LINDERHOF  
  
O projeto Linderhof tem o objetivo de ser uma biblioteca de ataques de negação de serviço refletidos/amplificados.  
  
# Build
Requisitos:
- CMake

Para compilar basta executar o script build.sh. Os binários estarão na pasta bin do projeto.
Caso não queira utilizar o script execute após a compilaç
# Arquitetura

## Oryx
UI engine.
Existem duas possíveis interfaces:

 - OryxCli
 - OryxNet

OryxCli deve ser utilizada caso queria acessar o linderhof na maquina local (API padrão). 

OryxNet deve ser utilizada para acessar o Lindehof em uma máquina hospedeira. Para compilar o Linderhof com o OryxNet deve-se descomentar a linha 17 do **CMakeLists.txt**.

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
O netuno é o gerenciador dos injetores de ataque. Com cada injetor podendo no máximo ter um throughput de 5 Mb/s.
É possível requisitar um ataque de até 50Mb/s. Entretanto o netuno não foi capaz de entregar tudo isso até o momento, ele consegue até no máximo 20 Mb/s. Em um momento da injeção (por volta dos 15 Mb/s) o fator de crescimento da injeção começa a diminuir, acredito que por causa de um início de saturação na escrita do buffer do socket.

# Adicionando um Mirror
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTc2NTc4NjEyMiw3MzIzNTU5NTMsNjkwND
k5OTc1LDEwMjEzMjUzNiwtMTQ3NTU5NTY2NywtNDA5MjYzNjQ2
LDE3MDQ3MTE4MTQsNzE2MjYzOTQ4LC0xNzA3MzQ1NTM0LDUyMj
AxMzgyOCwtOTkzMjI0NTg2XX0=
-->