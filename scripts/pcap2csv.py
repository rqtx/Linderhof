#!/bin/python

import getopt
import sys

class Pcap2Csv():

    __SHORTARGS = 'p:ho:t:'
    __LONGARGS = ['pcap=', 'help', 'output=', 'time=']
    __USAGE = ['Pcap file', 'Help', 'Output file', 'Start attack time']

    start_time = 0
    end_time = 0

    def __init__(self):
        self.__cliParser()
        self.__convert()

    def __cliParser(self):
        options, remainder = getopt.getopt(sys.argv[1:], self.__SHORTARGS, self.__LONGARGS)
        for opt, arg in options:
            if opt in ('-p', '--file'):
                self.input_file = arg
            elif opt in ('-o', '--output'):
                self.output_file = arg
            elif opt in ('-t', '--time'):
                start, end = arg.split(':')
                self.start_time = int(start)
                self.end_time = int(end)

            elif opt in ('-h', '--help'):
                for idx, item in enumerate(self.__LONGARGS):
                    print('-' + self.__LONGARGS[idx][0] + ', --' + self.__LONGARGS[idx] + '       ' + self.__USAGE[idx])
                quit()

    def __convert(self):
        pcap = open(self.input_file, 'r')
        csv = open(self.output_file, 'w')
        lineCtn = 1
        ptrLine = 1

        #csv.write("Interval;Frames;Bytes\n")
        #csv.write(str(0) + ';' + str(0) + ';' + str(0) + '\n')

        for line in pcap:
            if lineCtn > (12 + self.start_time):
                if line[0] != '=':

                    if ptrLine > self.end_time:
                        break

                    splited = line[1:].split('|')
                    csv.write( str(ptrLine) + ';' + splited[1].replace(" ", "") + ';' + splited[2].replace(" ", "") + '\n')
                    ptrLine += 1
 
            lineCtn += 1


def convert(prefix):
    txt = prefix + '.txt'
    csv = prefix + '.csv'
    fileTxt = open(txt, 'r')
    fileCsv = open(csv, 'w')
    lineCtn = 1
    ptrLine = 1

    #csv.write("Interval;Frames;Bytes\n")
    #csv.write(str(0) + ';' + str(0) + ';' + str(0) + '\n')

    for line in fileTxt:
        if lineCtn > 12:
            if line[0] != '=':

                if ptrLine > 50:
                    break

                splited = line[1:].split('|')
                fileCsv.write( str(ptrLine) + ';' + splited[1].replace(" ", "") + ';' + splited[2].replace(" ", "") + '\n')
                ptrLine += 1
 
        lineCtn += 1

convert('amplifier')
convert('attacker')
convert('victim')
convert('amplifier_input')
convert('amplifier_output')
