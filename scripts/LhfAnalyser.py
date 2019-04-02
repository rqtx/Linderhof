#!/bin/python

def ParseCsv(csvFile, interval):
    bytesList = []
    framesList = []
    ctn = 0
    intervalCtr = 0
    frameSun = 0
    byteSun = 0

    for i in range (0, 10*interval):
        id, frames, bytes = csvFile[i].split(';')
        frameSun += int(frames)
        byteSun += int(bytes)
        intervalCtr += 1

        if interval == intervalCtr:
            framesList.append(int(frameSun/interval))
            bytesList.append(int((byteSun/interval) - (frameSun/interval * 14)))
            frameSun = 0
            byteSun = 0
            intervalCtr = 0

    return [framesList, bytesList]

def LhfAnalyser():

    with open('attacker.csv') as fp1:
        attacker = ParseCsv(fp1.read().splitlines(), 5)

    with open('victim.csv') as fp3:
        victim = ParseCsv(fp3.read().splitlines(), 5)
    
    with open('amplifier_input.csv') as fp4:
        amplifierInput = ParseCsv(fp4.read().splitlines(), 5)

    with open('amplifier_output.csv') as fp5:
        amplifierOutput = ParseCsv(fp5.read().splitlines(), 5)

    bytesCsv = open("bytesLhf.csv", 'w')
    framesCsv = open("framesLhf.csv", 'w')
    amplifierBytes = open("ampBytes.csv", 'w')
    amplifierFrames = open("ampFrames.csv", 'w')
    bp = open("bytesframesLhf.csv", 'w')

    framesCsv.write("Level;Atk;AmpIn;AmpOut;Vic\n")
    #framesCsv.write("0;0;0;0;0\n")
    bytesCsv.write("Level;Atk;AmpIn;AmpOut;Vic\n")
    #bytesCsv.write("0;0;0;0;0\n")
    amplifierBytes.write("Level;Input;Output;Amplificação\n")
    #amplifierBytes.write("0;0;0\n")
    amplifierFrames.write("Level;Input;Output;Amplificação\n")
    #amplifierFrames.write("0;0;0\n")
    bp.write("Level;Input;Output\n")

    for i in range(0, 10):
        framesCsv.write(str(i+1) + ';' + str(attacker[0][i]) + ';' + str(amplifierInput[0][i]) + ';' + str(amplifierOutput[0][i]) + ';' + str(victim[0][i]) + '\n')
        bytesCsv.write(str(i+1) + ';' + str(attacker[1][i]) + ';' + str(amplifierInput[1][i]) + ';' + str(amplifierOutput[1][i]) + ';' + str(victim[1][i]) + '\n')
        amplifierFrames.write(str(i+1) + ';' + str(amplifierInput[0][i]) + ';' + str(amplifierOutput[0][i]) + ';' + str(round(amplifierOutput[0][i]/amplifierInput[0][i], 2)) + '\n')
        amplifierBytes.write(str(i+1) + ';' + str(amplifierInput[1][i]) + ';' + str(amplifierOutput[1][i]) + ';' + str(round(amplifierOutput[1][i]/amplifierInput[1][i], 2)) + '\n')
        bp.write(str(i+1) + ';' + str(round(amplifierInput[1][i]/amplifierInput[0][i], 2)) + ';' + str(round(amplifierOutput[1][i]/amplifierOutput[0][i], 2)) + '\n')

LhfAnalyser()
