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
OBS :Obrigatório. No ataque esee parâmetro corresponde ao tipo de espelho utilizado. 
Espelhos deisponíveis:
	- test 	
	- memcached
- args
OBS 




## Linderhof

## Netuno
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE4Njc2MTQzNTEsLTQwOTI2MzY0NiwxNz
A0NzExODE0LDcxNjI2Mzk0OCwtMTcwNzM0NTUzNCw1MjIwMTM4
MjgsLTk5MzIyNDU4Nl19
-->