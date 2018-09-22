set directories src/
file bin/lhf

#General config
set logging on

#Breakpoints
b main.c:main
b netio.c:210

#Start
run

