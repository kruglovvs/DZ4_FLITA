#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>
#define MAX 10000000000
#define STEP 1
#define VERT 100

typedef struct list_struct {
	struct list_struct *next;
	uint64_t value;
} list;
typedef struct graph_struct {
	bool** inc_matrix;
	uint64_t* real_edges;	
	uint64_t edges;
	uint64_t vertices;
} graph;
void del_graph(graph *cur_graph){
	for (uint64_t i = 0; i < cur_graph->vertices; i++) 
		free(cur_graph->inc_matrix[i]);
	free(cur_graph->inc_matrix);
	free(cur_graph->real_edges);
	free(cur_graph);
}
void del_list(list *cur_list){
	if (cur_list == NULL) return;
	del_list(cur_list->next);
	free(cur_list);
}
list *init_list(uint64_t value) {
	list *new_list = (list *)malloc(sizeof(list));
	new_list->next = NULL;
	new_list->value = value;
	return new_list;
}
list *find_first_and_create_list(uint64_t max, graph *cur_graph) {
	for (uint64_t i = 0; i < cur_graph->vertices; i++){
		if (cur_graph->real_edges[i] == max){
			for (uint64_t j = 0; j < cur_graph->edges; j++){
				if (cur_graph->inc_matrix[i][j]){
					return init_list(j);
				}
			}
		}
	}
	return NULL;
}
void add_to_list(list *cur_list, uint64_t value) {
	//if cur_list-> != NULL - гарантируется//
	list *new_list = init_list(value);
	list *previous = NULL;
	while (cur_list != NULL) {
		if (cur_list->value < value) {
			previous = cur_list;
			cur_list = cur_list->next;
		} else if (cur_list->value > value) {
			if (previous != NULL)
				previous->next = new_list;
			new_list->next = cur_list;
			return;
		} else {
			return;
		}
	}
	previous->next = new_list;
}
void print_list(list *cur_list) {
	if (cur_list == NULL) return;
	printf("!%I64d!, ", cur_list->value);
	print_list(cur_list->next);
}
void print_graph(graph *cur_graph) {
	for (uint64_t i = 0; i < cur_graph->vertices; i++){
		for (uint64_t j = 0; j < cur_graph->edges; j++){
			printf("%d ", cur_graph->inc_matrix[i][j]);
		}
		printf("\n");
	}
}
graph *generate_graph(uint64_t edges, uint64_t vertices) {
	graph *new_graph = (graph*) malloc(sizeof(graph));
	new_graph->inc_matrix = (bool**) malloc(vertices * sizeof(bool*));
	new_graph->real_edges = (uint64_t *)calloc(vertices, sizeof(uint64_t));
	for (uint64_t i = 0; i < vertices; i++){
		new_graph->inc_matrix[i] = (bool *)calloc(edges, sizeof(bool));
	}
	new_graph->vertices = vertices;
	new_graph->edges = edges;
	
	for (uint64_t i = 0; i < edges; i++){
		new_graph->inc_matrix[rand() % vertices][i] = 1;
		new_graph->inc_matrix[rand() % vertices][i] = 1;
	}
	
	return new_graph;
}
list *find_edges(graph *cur_graph) {
	uint64_t max = 0; 
	for (uint64_t i = 0; i < cur_graph->vertices; i++){
		for (uint64_t j = 0; j < cur_graph->edges; j++){
			cur_graph->real_edges[i] += cur_graph->inc_matrix[i][j];
		}
		if (cur_graph->real_edges[i] > max) {
			max = cur_graph->real_edges[i];
		}
	}
	list *list_of_edges = find_first_and_create_list(max, cur_graph);
	if (list_of_edges != NULL){
		for (uint64_t i = 0; i < cur_graph->vertices; i++){
			if (cur_graph->real_edges[i] == max){
				for (uint64_t j = 0; j < cur_graph->edges; j++){
					if (cur_graph->inc_matrix[i][j]){
						add_to_list(list_of_edges, j);
					}
				}
			}
		}
	}
	printf("graph matrix\n");
	print_graph(cur_graph);
	printf("max = %I64d\n", max);
	printf("list\n");
	print_list(list_of_edges);
	printf("\n");
	return list_of_edges;
}
int main(int argc, char *argv[]) {
	srand(time(NULL)); time_t start, stop/*, start_gen, end_gen, start_del, end_del*/; list *edges;
	for (int i = STEP; i < MAX; i += STEP) {
		graph *cur_graph = generate_graph(i, i);
		
		start = time(NULL);
		edges = find_edges(cur_graph); 
		stop = time(NULL);
		
		del_graph(cur_graph);
		del_list(edges);
		
		printf("%lld\n", stop - start);
	}
	getchar();
	return 0;
}