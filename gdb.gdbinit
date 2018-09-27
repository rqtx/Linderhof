set directories src/
file bin/lhf

#General config
set logging on

#Breakpoints
b main.c:main
b netuno.c:72

#Start
run

