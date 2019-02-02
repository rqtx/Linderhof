#!/bin/bash

tshark -z io,stat,1 -q -r config1/GetSet/amplifier.pcap > config1/GetSet/amplifier.txt && tshark -z io,stat,1 -q -r config1/GetSet/victim.pcap > config1/GetSet/victim.txt && tshark -z io,stat,1 -q -r config1/GetSet/attacker.pcap > config1/GetSet/attacker.txt
tshark -z io,stat,1 -q -r config1/Stat/amplifier.pcap   > config1/Stat/amplifier.txt   && tshark -z io,stat,1 -q -r config1/Stat/victim.pcap   > config1/Stat/victim.txt   && tshark -z io,stat,1 -q -r config1/Stat/attacker.pcap   > config1/Stat/attacker.txt
tshark -z io,stat,1 -q -r config1/GetSet/amplifier_input.pcap > config1/GetSet/amplifier_input.txt  && tshark -z io,stat,1 -q -r config1/GetSet/amplifier_output.pcap > config1/GetSet/amplifier_output.txt
tshark -z io,stat,1 -q -r config1/Stat/amplifier_input.pcap   > config1/Stat/amplifier_input.txt   && tshark -z io,stat,1 -q -r config1/Stat/amplifier_output.pcap   > config1/Stat/amplifier_output.txt

tshark -z io,stat,1 -q -r config2/GetSet/amplifier.pcap > config2/GetSet/amplifier.txt && tshark -z io,stat,1 -q -r config2/GetSet/victim.pcap > config2/GetSet/victim.txt && tshark -z io,stat,1 -q -r config2/GetSet/attacker.pcap > config2/GetSet/attacker.txt
tshark -z io,stat,1 -q -r config2/Stat/amplifier.pcap   > config2/Stat/amplifier.txt   && tshark -z io,stat,1 -q -r config2/Stat/victim.pcap   > config2/Stat/victim.txt   && tshark -z io,stat,1 -q -r config2/Stat/attacker.pcap   > config2/Stat/attacker.txt
tshark -z io,stat,1 -q -r config2/GetSet/amplifier_input.pcap > config2/GetSet/amplifier_input.txt && tshark -z io,stat,1 -q -r config2/GetSet/amplifier_output.pcap > config2/GetSet/amplifier_output.txt
tshark -z io,stat,1 -q -r config2/Stat/amplifier_input.pcap   > config2/Stat/amplifier_input.txt  && tshark -z io,stat,1 -q -r config2/Stat/amplifier_output.pcap   > config2/Stat/amplifier_output.txt

tshark -z io,stat,1 -q -r config3/GetSet/amplifier.pcap > config3/GetSet/amplifier.txt && tshark -z io,stat,1 -q -r config3/GetSet/victim.pcap > config3/GetSet/victim.txt && tshark -z io,stat,1 -q -r config3/GetSet/attacker.pcap > config3/GetSet/attacker.txt
tshark -z io,stat,1 -q -r config3/Stat/amplifier.pcap   > config3/Stat/amplifier.txt   && tshark -z io,stat,1 -q -r config3/Stat/victim.pcap   > config3/Stat/victim.txt   && tshark -z io,stat,1 -q -r config3/Stat/attacker.pcap   > config3/Stat/attacker.txt
tshark -z io,stat,1 -q -r config3/GetSet/amplifier_input.pcap > config3/GetSet/amplifier_input.txt && tshark -z io,stat,1 -q -r config3/GetSet/amplifier_output.pcap > config3/GetSet/amplifier_output.txt
tshark -z io,stat,1 -q -r config3/Stat/amplifier_input.pcap   > config3/Stat/amplifier_input.txt  && tshark -z io,stat,1 -q -r config3/Stat/amplifier_output.pcap   > config3/Stat/amplifier_output.txt

tshark -z io,stat,1 -q -r config4/GetSet/amplifier.pcap > config4/GetSet/amplifier.txt && tshark -z io,stat,1 -q -r config4/GetSet/victim.pcap > config4/GetSet/victim.txt && tshark -z io,stat,1 -q -r config4/GetSet/attacker.pcap > config4/GetSet/attacker.txt
tshark -z io,stat,1 -q -r config4/Stat/amplifier.pcap   > config4/Stat/amplifier.txt   && tshark -z io,stat,1 -q -r config4/Stat/victim.pcap   > config4/Stat/victim.txt   && tshark -z io,stat,1 -q -r config4/Stat/attacker.pcap   > config4/Stat/attacker.txt
tshark -z io,stat,1 -q -r config4/GetSet/amplifier_input.pcap > config4/GetSet/amplifier_input.txt && tshark -z io,stat,1 -q -r config4/GetSet/amplifier_output.pcap > config4/GetSet/amplifier_output.txt
tshark -z io,stat,1 -q -r config4/Stat/amplifier_input.pcap   > config4/Stat/amplifier_input.txt  && tshark -z io,stat,1 -q -r config4/Stat/amplifier_output.pcap   > config4/Stat/amplifier_output.txt

tshark -z io,stat,1 -q -r config5/GetSet/amplifier.pcap > config5/GetSet/amplifier.txt && tshark -z io,stat,1 -q -r config5/GetSet/victim.pcap > config5/GetSet/victim.txt && tshark -z io,stat,1 -q -r config5/GetSet/attacker.pcap > config5/GetSet/attacker.txt
tshark -z io,stat,1 -q -r config5/Stat/amplifier.pcap   > config5/Stat/amplifier.txt   && tshark -z io,stat,1 -q -r config5/Stat/victim.pcap   > config5/Stat/victim.txt   && tshark -z io,stat,1 -q -r config5/Stat/attacker.pcap   > config5/Stat/attacker.txt
tshark -z io,stat,1 -q -r config5/GetSet/amplifier_input.pcap > config5/GetSet/amplifier_input.txt && tshark -z io,stat,1 -q -r config5/GetSet/amplifier_output.pcap > config5/GetSet/amplifier_output.txt
tshark -z io,stat,1 -q -r config5/Stat/amplifier_input.pcap   > config5/Stat/amplifier_input.txt  && tshark -z io,stat,1 -q -r config5/Stat/amplifier_output.pcap   > config5/Stat/amplifier_output.txt

tshark -z io,stat,1 -q -r config6/GetSet/amplifier.pcap > config6/GetSet/amplifier.txt && tshark -z io,stat,1 -q -r config6/GetSet/victim.pcap > config6/GetSet/victim.txt && tshark -z io,stat,1 -q -r config6/GetSet/attacker.pcap > config6/GetSet/attacker.txt
tshark -z io,stat,1 -q -r config6/Stat/amplifier.pcap   > config6/Stat/amplifier.txt   && tshark -z io,stat,1 -q -r config6/Stat/victim.pcap   > config6/Stat/victim.txt   && tshark -z io,stat,1 -q -r config6/Stat/attacker.pcap   > config6/Stat/attacker.txt
tshark -z io,stat,1 -q -r config6/GetSet/amplifier_input.pcap > config6/GetSet/amplifier_input.txt && tshark -z io,stat,1 -q -r config6/GetSet/amplifier_output.pcap > config6/GetSet/amplifier_output.txt
tshark -z io,stat,1 -q -r config6/Stat/amplifier_input.pcap   > config6/Stat/amplifier_input.txt  && tshark -z io,stat,1 -q -r config6/Stat/amplifier_output.pcap   > config6/Stat/amplifier_output.txt
