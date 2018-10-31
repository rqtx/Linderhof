#!/bin/bash

sudo true
sudo bin/lhf -a 192.168.0.100 -t 192.168.0.1 --thp=1 --timer=5 -m memcached_stat &
wait $!
sudo bin/lhf -a 192.168.0.100 -t 192.168.0.1 --thp=10 --timer=5 -m memcached_stat &
wait $!
sudo bin/lhf -a 192.168.0.100 -t 192.168.0.1 --thp=50 --timer=5 -m memcached_stat &
wait $!
sudo bin/lhf -a 192.168.0.100 -t 192.168.0.1 --thp=0 --timer=5 -m memcached_stat &
wait $!
