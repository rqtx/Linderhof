#/bin/bash

interval=1
filename="logcap.pcap"

while getopts "if:" opt; do
    case "$opt" in
        i)  
            interval=$OPTARG
            ;;

        f)  
            filename=$OPTARG
            ;;

        p)
            
    esac
done

COMMAND="tshark -z io,stat,$interval -w $filename -B 10 -i any"
eval $COMMAND
