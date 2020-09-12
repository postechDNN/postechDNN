import sys, os

testfiles = open("testfiles.txt","r")
exec_files= ['DNN_pddnn.exe','DNN_eps.exe']
output_dir = ['.\\output\\pddnn_output\\', '.\\output\\eps_output\\' ]
DNN_dir = ['.\\DNN_pddnn\\','.\\DNN_eps\\']
current_dir = os.getcwd()

#arg[0] : index of DNN module to execute
#It's useful to execute specific DNN module

index = range(len(exec_files))
if len(sys.argv) > 1:
    idx = int(sys.argv[1])
    if idx >= len(exec_files):
        print("ERROR : the index of executable file is wrong!")
        exit(0)
    index = [idx]

for i in index:
    os.chdir(DNN_dir[i])
    os.system('make')
    os.chdir(current_dir)

while True:
    line = testfiles.readline()
    if not line : break
    input_file, query_file = line.split()
    out_file = (input_file.split('\\'))[-1]
    out_file = (out_file.split('.'))[0] +'_out.txt'
    for i in index:
        os.system(exec_files[i]+" "+input_file+" "+query_file+" "+output_dir[i]+out_file)
    print(out_file,"is completed!\n")
#for i in index:
#    os.chdir(DNN_dir[i])
#    os.system('make clean')

testfiles.close()
