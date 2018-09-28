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

O ataque INCREMENT( Incremental ) é o ataque **default**. Ele começa com um throughput de 1 Mb/s e vai incrementando o ataque até o tempo *timer* em 1 Mb/s a cada 5 minutos. O FULL é um ataque com um throughput constante *thp*.
  





## Linderhof

## Netuno
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTIwMzM5NDU0OTAsLTE0NzU1OTU2NjcsLT
QwOTI2MzY0NiwxNzA0NzExODE0LDcxNjI2Mzk0OCwtMTcwNzM0
NTUzNCw1MjIwMTM4MjgsLTk5MzIyNDU4Nl19
-->