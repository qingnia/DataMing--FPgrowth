#ifndef	UTIL_H
#define UTIL_H
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define	FEW		3
#define PER_NODE	7

typedef struct node
{
	int id;
	char* label;
	int com;
}node;

typedef struct edge
{
	int source;
	int target;
}edge;


void getId(FILE* file, int* id)
{
 	char buf[20] = { 0 };
	fgets(buf, 20, file);
	sscanf(buf, "    id %d", id);
	fgets(buf, 20, file);
	fgets(buf, 20, file);
	fgets(buf, 20, file);
}

void getEdge(FILE* file, int* source, int* target)
{
	char buf[20] = { 0 };
	fgets(buf, 20, file);
	sscanf(buf, "    source %d", source);
	fgets(buf, 20, file);
	sscanf(buf, "    target %d", target);
	fgets(buf, 20, file);
}
/*
void writeData(FILE* file, Node* head, int id)
{
	fprintf(file, "%d:\n", id);
	int count = *(int*)getData(head) + 1;
	int i = 1;
	Node* current = head->next;
	for(; i < count; i++)
	{
		int data = *(int*)getData(current);
		fprintf(file, "%-8d", data);
		current = current->next;
		if(0 == i % 5)
			fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void write_no_edge(FILE* file, int id, int* count, int sp)
{
	(*count)++;
	fprintf(file, "%-8d", id);
	if(0 == *count % sp)
		fprintf(file, "\n");
}

void write_friend(FILE* file, int source, int target, int* count, int sp)
{
	(*count)++;
	fprintf(file, "%-8d, %-8d   ", source, target);
	if(0 == *count % sp)
		fprintf(file, "\n");
}

int main(int argc, char* argv[])
{
	int i = 0;
	Node* head_node = (Node*)calloc(465023, sizeof(Node));
	FILE* twitter = fopen("twitter", "r");
	char sentence[20] = { 0 };
	char buf[20] = { 0 };
	fgets(sentence, 20, twitter);
	i = 0;
	int j = 0;
	while(fgets(sentence, 20, twitter))
	{
		int* id = (int*)malloc(sizeof(int));
		sscanf(sentence, "  %s", buf);
		if(!strncmp(buf, "node", 4))
		{
			getId(twitter, id);
			create_List(&head_node[*id]);
		}
		else if(!strncmp(buf, "edge", 4))
		{
			int* target = (int*)malloc(sizeof(int));
			getEdge(twitter, id, target);
			insert_List(&head_node[*id], (void*)target);
		}
		else
			break;
	}
	FILE* out = fopen("twitter.out", "w+");
	FILE* out0 = fopen("twitter_0_edge", "w+");
	FILE* out1 = fopen("twitter_1_edge", "w+");
	FILE* out2 = fopen("twitter_2_edge", "w+");
	FILE* out3 = fopen("twitter_3_edge", "w+");
	FILE* out4 = fopen("twitter_many_edge", "w+");
	int* each_count = (int*)calloc(5, sizeof(int));
	for(i = 0; i < 465023; i++)
	{
		writeData(out, &head_node[i], i);
		int count = *(int*)getData(&head_node[i]);
		switch(count)
		{
			case 0:
				write_no_edge(out0, i, each_count, PER_NODE);
				break;
			case 1:	
				write_no_edge(out1, i, each_count + 1, PER_NODE);
				break;
			case 2:
				write_no_edge(out2, i, each_count + 2, PER_NODE);
				break;
			case 3:
				write_no_edge(out3, i, each_count + 3, PER_NODE);
				break;
			default:
				write_no_edge(out4, i, each_count + 4, PER_NODE);
				break;
		}

	}
//	printf("no_edge:%d, few_edge:%d, many_edge:%d\n", each_count[0], each_count[1], each_count[2]);
	printf("0_edge:%d, 1_edge:%d, 2_edge:%d, 3_edge:%d, many_edge:%d\n", each_count[0], each_count[1], each_count[2], each_count[3], each_count[4]);
	printf("in all:%d\n", each_count[0] + each_count[1] + each_count[2]);
	int* friend_count = (int*)malloc(sizeof(int));
	*friend_count = 0;
	FILE* friend = fopen("friend", "w+");
	for(i = 0; i < 465023; i++)
	{
		Node* cur = (head_node + i)->next;
		while(cur)
		{
			int target = *(int*)getData(cur);
//			if(i > 160000)
//				printf("id: %d, target: %d\n", i, target);
			if(!delete_List_value(&head_node[target], i))
*/

double	getFrequent(int A, int B, List* t_list, tnode* root)
{
	long m = random() % 10;
	return m / 11.4;
}

/*				printf("%d\n", target);
				write_friend(friend, i, target, friend_count, PER_NODE / 2);
			}
			cur = cur->next;
		}
	}
	printf("friend count: %d\n", *friend_count);
	free(head_node);
	free(each_count);
	free(friend_count);
	fclose(twitter);
	fclose(out1);
	fclose(out2);
	fclose(out3);
	fclose(out4);
	fclose(friend);
	return 0;
}
*/
#endif
