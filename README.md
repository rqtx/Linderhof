# LINDERHOF  
  
O projeto Linderhof tem o objetivo de ser uma biblioteca de ataques de negação de serviço refletido/amplificado.  
  
  
# Arquitetura

## Oryx
UI engine.
Existem duas possíveis interfaces:

 - OryxCli
 - OryxNet

OryxCli deve ser utilizada caso queria acessar o linderhof na maquina local, é a API acesso padrão. 

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
  -f, --full=thp Executa um ataque full
  -g, --targport=targ_port   Target port
  -p, --amport=am_port       Amplifier port
  -r, --timer=timer          Attack timer

Existe dois tipos de ataque. O ataque INCREMENT( Incremental ) é o ataque default. Ele começa em 1 Mb/s e vai incrementando o ataque até o tempo timer
  





## Linderhof

## Netuno
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTU5MjU0MDIxNCwtMTQ3NTU5NTY2NywtND
A5MjYzNjQ2LDE3MDQ3MTE4MTQsNzE2MjYzOTQ4LC0xNzA3MzQ1
NTM0LDUyMjAxMzgyOCwtOTkzMjI0NTg2XX0=
-->