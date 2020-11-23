import sys, os

testfiles = open("Experiment2_testfiles.txt","r")
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
K = [10]
while True:
    line = testfiles.readline()
    if not line : break
    input_file, query_file = line.split()
    out_file = (input_file.split('\\'))[-1]
    #out_png = (out_file.split('.'))[0] +'_out.png'
    out_file = (out_file.split('.'))[0] +'_out.txt'
    #os.system("python draw_input.py"+" "+input_file +" "+out_png)
    for i in index:
        for k in K:
            os.system(exec_files[i]+" "+input_file+" "+query_file+" "+output_dir[i]+out_file + " "+str(k))
    print(out_file,"is completed!\n")

testfiles.close()
