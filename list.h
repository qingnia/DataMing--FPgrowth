#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include<stdlib.h>

typedef struct Linked_List
{
	struct Linked_List* next;
	void* data;
}List;

typedef int (*getFunc_List)(List*);

/*	由于void*可能是结构体。为了能让结构体的每个值都能成为索引值，
 *	FP应用中要能通过index增删节点，通过count对节点进行排序。
 *	为了链表的通用性，定义一个函数指针，在search和delete时传入,
 *	由上层代码根据结构体的构成自由指定取值的规则
 */

int create_List(List* head)
{
	if(NULL == head)
		return 1;
	head->next = NULL;
	int* count = (int*)malloc(sizeof(int));
	*count = 0;
	head->data = (void*)count;
	return 0;
}

int search_List_index(List* head, List** cur, int index)
{
	if(NULL == head)
		return 1;
	int* count = (int*)(head->next);
	if(index < 0 || index > *count)
		return 2;
	*cur = head;
	int i = 0;
	for(; i < index; i++)
		*cur = (*cur)->next;
	return 0;
}

int search_List_value(List* head, List** cur, int value, getFunc_List getValue)
{
	if(NULL == head)
		return 1;
	*cur = head;
	//应该判断指针的值是否非空！！！
	while((*cur)->next)
	{
//		int* val = (int*)((*cur)->next->data);
		int val = getValue((*cur)->next);
		if(value == val)
		{
			return 0;
		}
		else
			*cur = (*cur)->next;
	}
	return 2;
}

void* getData(List* node)
{
	return node->data;
}

int insert_List(List* head, void* data)
{
	if(NULL == head || NULL == data)
		return 1;
	int* count = (int*)(head->data);
	(*count)++;
	List* current = head;
	while(current->next)
		current = current->next;
	List* new = (List*)malloc(sizeof(List));
	new->next = NULL;
	new->data = data;
	current->next = new;
	return 0;
}

int insert_List_WithLast(List* head, List* last, void* data)
{
	int* count = (int*)(head->data);
	(*count)++;
	List* node = (List*)malloc(sizeof(List));
	node->next = NULL;
	node->data = data;
	last->next = node;
	return 0;
}

int delete_List_value(List* head, int value, getFunc_List getValue)
{
	List* front = NULL;
	int* count = (int*)(head->data);
	int ret = search_List_value(head, &front, value, getValue);
	if(0 != ret)
		return 1;
	List* current = front->next;
	if(NULL == current->next)
		front->next = NULL;
	else
		front->next = current->next;
	(*count)--;
	free(current);
	return 0;
}

int delete_List_index(List* head, int index)
{
	List* front = NULL;
	int ret = search_List_index(head, &front, index);
	if(1 == ret)
		return 1;
	int* count = (int*)(head->data);
	if(2 == ret || index == *count)
		return 1;
	List* current = front->next;
	if(NULL == current->next)
		front->next = NULL;
	else
		front->next = current->next;
	printf("free data\n");
	free(current->data);
	printf("free node\n");
	free(current);
	(*count)--;
	return 0;
}

void	sort_List(List* list, getFunc_List getValue)
{
	int	i = 0, j;
	int	state = 0;
	List*	log = list;
	List*	head = list;
	while(log && log->next)
	{
		i++;
		List* tmp_front = log;	//要交换的值的前一个节点位置
		List* cur = log->next;	//从当前节点开始遍历找最大的
		if(log->next->next)
		{
			while(cur && cur->next)
			{
				if(getValue(tmp_front->next) > getValue(cur->next))
					tmp_front = cur;
				cur = cur->next;
			}
		}
		List* tmp = tmp_front->next;
		tmp_front->next = tmp->next;
		tmp->next = head->next;
		head->next = tmp;
		log = tmp;
		for(j = 0; j < i - 1; j++)
			log = log->next;
	}
}
/*
int main(int argc, char* argv[])
{
	List* head = (List*)malloc(sizeof(List));
	create_List(head);
	int* m = (int*)malloc(sizeof(int));
	*m = 2;
	int* n = (int*)malloc(sizeof(int));
	insert_List(head, (void*)m);
	insert_List(head, (void*)n);
	printf("count: %d\n", *(int*)(head->data));
	printf("count: %d, 1st: %d\n", *(int*)getData(head), *(int*)(head->next->data));
	printf("count: %d, 1st: %d, 2nd: %d\n", *(int*)getData(head), *(int*)getData(head->next), *(int*)getData(head->next->next));
	delete_List_value(head, 2);	
	printf("count: %d, 1st: %d\n", *(int*)getData(head), *(int*)getData(head->next));
	return 0;
}
*/
#endif
