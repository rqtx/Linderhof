#!/bin/bash

tshark -r memcachedserver.pcap -Y "ip.src == 192.168.0.104" -T fields -E header=y -E separator=, -E quote=d -e ip.src -e ip.dst -e ip.len > memcachedserver.csv 
