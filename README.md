# Google_Page_Rank
Implmentation of Google Page Rank Algorithm

Serial code build:
gcc gp_rank.c -o gp_rank

CUDA code build:
nvcc -gencode arch=compute_20,code=sm_20 gp_rank_cuda.cu -o gp_rank_cuda

Usage:
./gp_rank <Num_of_nodes> <graph_file>
