#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	float curr_page_rank;
	float next_page_rank;
	long num_adj_nodes;
	void *next;
}vertex_t;

typedef struct ad_vert {
	long vertex_num;
	struct ad_vert *next;
}adj_vert_t;

float epsilon;
float rand_hop = 0.15;

#define RAND_HOP_LIKELIHOOD(r_hop_prob, nvert) ((r_hop_prob) / (nvert))
#define TRAV_LIKELIHOOD(r_hop_prob, g, index) ((1 - (r_hop_prob)) * (g)[index].curr_page_rank / (g)[index].num_adj_nodes)

void initialize_vertices(vertex_t *g, long num_vertices)
{
	long i;
	for(i = 0;i < num_vertices;i++)
	{
		g[i].curr_page_rank = 1 / (float)num_vertices;
		g[i].next_page_rank = RAND_HOP_LIKELIHOOD(rand_hop, num_vertices);
		g[i].num_adj_nodes = 2;
		g[i].next = NULL;
	}
}

adj_vert_t *adj_node_init(long *n)
{
	long j;
	adj_vert_t *ptr, *b_ptr = (adj_vert_t *)malloc(sizeof(adj_vert_t));
	ptr = b_ptr;
	ptr->vertex_num = n[0];
	for(j =1;j < 2;j++)
	{
		ptr->next = (adj_vert_t *)malloc(sizeof(adj_vert_t));
		ptr = ptr->next;
		ptr->vertex_num = n[j];
	}
	ptr->next = NULL;
	return b_ptr;
}
void print_converged_pr_vals(vertex_t *g, long num_vertices)
{
	long i;
	float sum = 0;
	for(i = 0;i < num_vertices;i++)
	{
		printf("Converged page rank for node %d : %f\n",i,g[i].curr_page_rank);
		sum += g[i].curr_page_rank;
	}
	printf("========== Sum : %f ==============\n",sum);
}

int main(void)
{
	long i;
	adj_vert_t *ptr = NULL;
	float value = 0;
	float pr_diff;
	long num_vertices = 5;
	epsilon = 0.00000001 * num_vertices;

	///////////
	long arr[5][2] = {{1,2},{2,3},{3,4},{1,2},{0,3}};
	///////////
	vertex_t *graph = (vertex_t *)malloc(num_vertices * sizeof(vertex_t));	
	initialize_vertices(graph, num_vertices);
	//////////////////////////////////////////////////
	for(i = 0;i < num_vertices;i++)
	{
		graph[i].next = adj_node_init(arr[i]);
	}
	//////////////////////////////////////////////////
	do 
	{	
		pr_diff = 0;
		//sleep(1);
		for(i = 0;i < num_vertices;i++)
		{
			value = TRAV_LIKELIHOOD(rand_hop, graph, i);
			for(ptr = (adj_vert_t *)graph[i].next;ptr != NULL;ptr = ptr->next)
				graph[ptr->vertex_num].next_page_rank += value;
		}
		for(i = 0;i < num_vertices;i++)
		{
			pr_diff += fabsf(graph[i].next_page_rank - graph[i].curr_page_rank);
			graph[i].curr_page_rank = graph[i].next_page_rank;
			graph[i].next_page_rank = RAND_HOP_LIKELIHOOD(rand_hop,num_vertices);
		}
		//printf("Diff : %f\n",pr_diff);
	}while(pr_diff > epsilon);
	print_converged_pr_vals(graph,num_vertices);
	return 0;
}
