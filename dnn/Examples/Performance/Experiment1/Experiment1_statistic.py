import sys, os

output_dir = ['.\\output\\pddnn_output\\', '.\\output\\eps_output\\' ]

testfiles = open("Experiment2_testfiles.txt","r")

out_file = open("output.txt","w")

while True:
    line = testfiles.readline()
    if not line : break
    eps_file, pddnn_file = line.split()
    
    eps_f = open(eps_file, "r")
    pddnn_f = open(pddnn_file,"r")
    
    #Distribution, File Number, K
    tmp = (eps_file.split('\\')[-1]).split('_')[:-1]
    tmp = "_".join(tmp)
    split_idx = tmp.rfind('n')
    Distribution = tmp[:split_idx+1]
    File_Number= tmp[split_idx+1:]

    n_query,K = pddnn_f.readline().split()
    n_query = int(n_query);K = int(K)
    
    pddnn_results = []
    for i in range(n_query):
        q_x,q_y,tick,d_sum = pddnn_f.readline().split()
        results = []
        dists = []
        for k in range(K):
            r_x,r_y,d = pddnn_f.readline().split()
            results.append((r_x,r_y))
            dists.append(float(d))
        pddnn_results.append((results,dists))
       
    for i in range(5):
        n_query, K = eps_f.readline().split()
        n_query = int(n_query); K = int(K)
        eps_d_sum = 0.0
        pddnn_d_sum = 0.0
        diff_sum = 0
        for j in range(n_query):
            q_x,q_y,tick,d_sum = eps_f.readline().split()
            eps_results = []
            for k in range(K):
                r_x,r_y,d = eps_f.readline().split()
                eps_results.append((r_x,r_y))
                eps_d_sum += float(d)
            #Analysis Part Distribution File_Number K Time Error(dist_sum) Error(set diff)
            pddnn_dists =pddnn_results[j][1][:K]
            pddnn_d_sum += sum(pddnn_dists)
            eps_set = set(eps_results)
            pddnn_set = set(pddnn_results[j][0][:K])
            diff = pddnn_set.difference(eps_set)
            diff_sum += len(diff)/K
        err_d_sum = abs(eps_d_sum - pddnn_d_sum) / pddnn_d_sum * 100
        out_file.write(Distribution+'\t'+File_Number+'\t'+str(K)+'\t'+tick+'\t'+str(err_d_sum)+'\t'+str(diff_sum/float(n_query) * 100)+'\n')

    print(Distribution, File_Number, 'is completed!')
out_file.close()
