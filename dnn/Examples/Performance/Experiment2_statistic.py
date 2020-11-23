import sys, os

output_dir = ['.\\output\\pddnn_output\\', '.\\output\\eps_output\\' ]

testfiles = open("Experiment2_outfiles.txt","r")

out_file = open("output.txt","w")

while True:
    line = testfiles.readline()
    if not line : break
    eps_file = (line.split())[0]
    
    eps_f = open(eps_file, "r")
    
    #Distribution, File Number, K
    tmp = (eps_file.split('\\')[-1]).split('_')[:-1]
    tmp = "_".join(tmp)
    split_idx = tmp.rfind('n')
    Distribution = tmp[:split_idx+1]
    File_Number= tmp[split_idx+1:]

    n_query, K = eps_f.readline().split()
    n_query = int(n_query); K = int(K)
    tick_sum = 0
    for j in range(n_query):
        q_x,q_y,tick,d_sum = eps_f.readline().split()
        eps_results = []
        for k in range(K):
            r_x,r_y,d = eps_f.readline().split()
        tick_sum += int(tick)
    out_file.write(Distribution+'\t'+File_Number+'\t'+str(K)+'\t'+str(tick_sum/n_query)+'\n')

    print(Distribution, File_Number, 'is completed!')
out_file.close()
