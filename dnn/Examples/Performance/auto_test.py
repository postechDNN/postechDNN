import sys, os

testfiles = open("testfiles.txt","r")

while True:
    line = testfiles.readline()
    if not line : break
    input_file, query_file, out_file = line.split()
    os.system("pddnn_performance\pddnn_performance.exe"+" "+input_file+" "+query_file+" "+out_file)

testfiles.close()
