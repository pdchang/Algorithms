#Philip Chang - CS 325 - Project 01 part 1
import sys
import re

def enumeration(array):
    if len(array) == 0:
        return 0
    else:
        max_sum = 0

    for i in range(len(array)):
        for j in range(i, len(array)):
            temp = 0
            for k in range(i, j+1):
                temp += array[k]
            if temp > max_sum:
                max_sum = temp
                sub = array[i:j+1]
    return [sub, max_sum]


def mss():

    inputfile = open('MSS_Problems.txt')
    outputfile = open('MSS_Results.txt', 'a')

    for line in inputfile:
        array = map(int, line.strip().split(' '))
        answer = enumeration(array)
        outputfile.write(str(array).replace('[','').replace(']', '') + '\n')
        outputfile.write(str(answer[0]).replace('[','').replace(']', '') + '\n')
        outputfile.write(str(answer[1]).replace('[','').replace(']', '') + '\n\n')

    inputfile.close()
    outputfile.close()


    inputfile.close()
    outputfile.close()

mss()
