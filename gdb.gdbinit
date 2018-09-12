set directories src/
file bin/lhf

set args Memcached -t 10.0.0.10 -a 10.0.0.1

#General config
set logging on

#Breakpoints
b main.c:main
b oryx.c:144
b injector.c:CreateInjection

#Start
run

