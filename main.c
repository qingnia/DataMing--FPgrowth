#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define	Support_Degree	10
#define	Support_Frequent 0.3
#include "list.h"
#include "tree.h"
#include "util.h"
typedef struct MetaData
{
	int	count;
	int	index;
	List*	list;
}Mdata;

typedef struct Target
{
	int	target;
	int	degree;
}tar_deg;

int	getList(List* head, List** target, int index)
{
	List* cur = head->next;
	while(cur)
	{
		Mdata* data = (Mdata*)getData(cur);
		if(index == data->index)
		{
			*target = cur;
			return 1;
		}
		else
			cur = cur->next;
	}
	return 0;
}

void	scanData(List* s_list, List* t_list)
{
	int i = 0, j = 0;
	FILE* twitter = fopen("twitter", "r");
	char sentence[20] = { 0 };
	char buf[20] = { 0 };
	fgets(sentence, 20, twitter);
	List* s_cur = s_list;
	List* t_cur = t_list;
	while(fgets(sentence, 20, twitter))
	{
		i++;
		int id = 0, target = 0;
		sscanf(sentence, "  %s", buf);
		if(!strncmp(buf, "node", 4))
		{
			getId(twitter, &id);
			Mdata* s_m = (Mdata*)malloc(sizeof(Mdata));
			Mdata* t_m = (Mdata*)malloc(sizeof(Mdata));
			s_m->index = id;
			s_m->count = 0;
			s_m->list = (List*)malloc(sizeof(List));
			t_m->index = id;
			t_m->count = 0;
			t_m->list = (List*)malloc(sizeof(List));
			create_List(s_m->list);
			insert_List_WithLast(s_list, s_cur, (void*)s_m);
			s_cur = s_cur->next;
			create_List(t_m->list);
			insert_List_WithLast(t_list, t_cur, (void*)t_m);
			t_cur = t_cur->next;
		}
		else if(!strncmp(buf, "edge", 4))
		{
			getEdge(twitter, &id, &target);
			List* list = NULL;
			//sourceList用于建树时扫描
			getList(s_list, &list, id);
			Mdata* md = (Mdata*)getData(list);
			tar_deg* s_t_m = (tar_deg*)malloc(sizeof(tar_deg));	//target和支持度
			s_t_m->target = target;
			s_t_m->degree = 0;
			insert_List(md->list, (void*)s_t_m);
			(md->count)++;
			if(i > 600000)
			{
				break;
			}
			//targetList用于建树时链接
			getList(t_list, &list, id);
			md = (Mdata*)getData(list);
			(md->count)++;
		}
		else 
			break;
	}
}

void	screenData(List* list)
{
	List* cur = list;
	while(cur->next)
	{
		Mdata* t_m = (Mdata*)getData(cur->next);
		if(t_m->count < Support_Degree)
		{
			List* tmp = cur->next;
			cur->next = tmp->next;
			tmp->data = NULL;
			free(t_m);
			free(tmp);
			(*(int*)(list->data))--;	//由于外部强行删除节点。必须主动计数减1
		}
		else
			cur = cur->next;
	}
}

int	getIndex(List* node)
{
	Mdata* md = (Mdata*)(node->data);
	return md->index;
}

int	getCount(List* node)
{
	Mdata* md = (Mdata*)(node->data);
	return md->count;
}

int	getTarget(List* node)
{
	tar_deg* td = (tar_deg*)getData(node);
	return td->target;
}

void	tidyData(List* s_list, List* t_list)
{
	List* cur = s_list;
	while(cur->next)
	{
		Mdata* md = (Mdata*)getData(cur->next);
		List* tmp_front = md->list;
//		printf("%d\n", md->index);
		while(tmp_front->next)
		{
			tar_deg* td = (tar_deg*)getData(tmp_front->next);
			List* noUse = NULL;
			if(search_List_value(t_list, &noUse, td->target, getIndex))
			{
				delete_List_value(md->list, td->target, getTarget);
				(md->count)--;
			/*	
				List* tmp = tmp_front->next;
				tmp_front->next = tmp->next;
				free(td);
				tmp->data = NULL;
				free(tmp);
				(*(int*)getData(md->list))--;
			*/	
			}
			//如果next被删除的话此时next为NULL，
			//则tmp_front = NULL,
			//判断tmp_front->next会出错！！！
			else
				tmp_front = tmp_front->next;
		}
		cur = cur->next;
	}
}

int	getDegree(List* node)
{
	tar_deg* td = (tar_deg*)getData(node);
	return td->degree;
}

void	setDegree(List* list, List* t_list)
{
	List* cur = list->next;
	while(cur)
	{
		tar_deg* td = (tar_deg*)getData(cur);
		List* tmp = NULL;
		int ret = search_List_value(t_list, &tmp, td->target, getIndex);
		Mdata* md = (Mdata*)(tmp->next->data);
		td->degree = md->count;
		cur = cur->next;
	}
}

void	sortByDegree(List* s_list, List* t_list)
{
	List*	cur = s_list->next;
	while(cur)
	{
		Mdata* md = (Mdata*)getData(cur);
		List* list = md->list;
		setDegree(list, t_list);
		sort_List(list, getDegree);
		cur = cur->next;
	}
}

int	getValue_Tree(tnode* node)
{
	tar_deg* td = (tar_deg*)(node->data);
	return td->target;
}

void	FP_contribute(List* s_list, List* t_list, tnode* root)
{
	List* s_cur = s_list->next;
	tnode* tnode_cur = root;
	while(s_cur)
	{
		Mdata* md = (Mdata*)getData(s_cur);
		List* list = md->list->next;
		while(list)
		{
			tar_deg* s_td = (tar_deg*)getData(list);
			tnode* node = NULL;
			int ret = search_child(tnode_cur, &node, s_td->target, getValue_Tree);
			if(ret > 0)
			{
				tar_deg* tmp = (tar_deg*)(node->data);
				(tmp->degree)++;
			}
			else if(!ret)
			{
				tar_deg* td = (tar_deg*)malloc(sizeof(tar_deg));
				td->target = s_td->target;
				td->degree = 1;
				add_child(tnode_cur, (void*)td);
				List* t_cur = NULL;
				search_List_value(t_list, &t_cur, s_td->target, getIndex);
				Mdata* t_md = (Mdata*)(t_cur->next->data);
				search_child(tnode_cur, &node, s_td->target, getValue_Tree);
				insert_List(t_md->list, (void*)node);
			}
			else
			{
				printf("error!\n");
				continue;
			}
			list = list->next;
			tnode_cur = node;
		}
		tnode_cur = root;
		s_cur = s_cur->next;
	}
}

void	printList(List* s_list, char* name)
{
	FILE* file = fopen(name, "w+");
	List* cur = s_list->next;
	while(cur)
	{
		Mdata* md = (Mdata*)getData(cur);
		fprintf(file, "index: %d, count: %d\n", md->index, md->count);
		List* tmp = md->list->next;
		while(tmp)
		{
			tar_deg* td = (tar_deg*)getData(tmp);
			fprintf(file, "\ttarget: %d, degree:%d\n", td->target, td->degree);
			tmp = tmp->next;
		}
		cur = cur->next;
	}
}

void	printListValue(List* t_list)
{
	FILE* file = fopen("t_list", "w+");
	fprintf(file, "length: %d\n", *(int*)getData(t_list));
	int i = 0;
	List* cur = t_list->next;
	while(cur)
	{
		i++;
		Mdata* md = (Mdata*)getData(cur);
		fprintf(file, "%8d", md->index);
		if(i % 7 == 0)
			fprintf(file, "\n");
		cur = cur->next;
	}
}

void	tidyListByMdata(List* list)
{
	List* cur = list;
	int i = 0;
	while(cur && cur->next)
	{
		i++;
		Mdata* md = (Mdata*)getData(cur->next);
		if(0 == md->count)
		{
			List* tmp = cur->next;
			cur->next = tmp->next;
			free(md->list);
			md->list = NULL;
			free(tmp);
			(*(int*)getData(list))--;
		}
		else	
			//如果把下一个节点删除了，这里应该直接continue，要不然会跳过节点！！！！
			cur = cur->next;
	}
}

void	recurision(tnode* node, int index, float* min)
{
	if(!node)
		return;
	tar_deg* td = (tar_deg*)(node->data);
//	printf("index:%d, target: %d\n", index, td->target);
	tnode* tmp = node->sibling;
	while(tmp)
	{
		recurision(tmp, index, min);
		tmp = tmp->sibling;
	}
	if(index == td->target)
	{
		*min += td->degree;
		return;
	}
	else
		recurision(node->left_child, index, min);
}

float	getFrequentFromA2B(int A, int B, List* t_list)
{
	List*	tmp_A = NULL;
	List*	tmp_B = NULL;
	List*	cur = NULL;
	int	a, b;
	float	max = 0.0f;
	float	min = 0.0f;
	if(search_List_value(t_list, &tmp_A, A, getIndex) || search_List_value(t_list, &tmp_B, B, getIndex))
	{
		printf("error!\n");
		return;
	}
	Mdata* md_A = (Mdata*)getData(tmp_A->next);
	Mdata* md_B = (Mdata*)getData(tmp_B->next);
	if(md_A->count < md_B->count)
	{
		cur = md_B->list->next;
		b = A;
	}
	else
	{
		cur = md_A->list->next;
		b = B;
	}
	while(cur)
	{
		tnode* node = (tnode*)getData(cur);
		tar_deg* td = (tar_deg*)(node->data);
		max += td->degree;
		recurision(node->left_child, b, &min);
		cur = cur->next;
	}
	return min / max;
}

void	getSet(List* t_list)
{
	FILE* FP_set = fopen("FP_set", "w+");
	List* cur = t_list->next;
	while(cur)
	{
		Mdata* md_A = (Mdata*)getData(cur);
		int A = md_A->index;
		List* tmp = cur->next;
		while(tmp)
		{
			Mdata* md_B = (Mdata*)getData(tmp);
			int B = md_B->index;
			float ret = getFrequentFromA2B(A, B, t_list);
			if(ret > 0)
				fprintf(FP_set, "A: %d, B: %d, frequent A to B: %.2f\n", A, B, ret);
			tmp = tmp->next;
		}
		cur = cur->next;
	}
	fclose(FP_set);
}


void	theLast(List* t_list)
{
	while(1)
	{
		int A, B;
		List* noUse = NULL;
		printf("input A:");
		scanf("%d", &A);
		if(search_List_value(t_list, &noUse, A, getIndex))
		{
			printf("A is not in t_list\n");
			continue;
		}
		printf("input B:");
		scanf("%d", &B);
		if(search_List_value(t_list, &noUse, B, getIndex))
		{
			printf("B is not in t_list\n");
			continue;
		}
		float ret = getFrequentFromA2B(A, B, t_list);
		printf("%.2f\n", ret);
	}
}

void	printfTree(tnode* root)
{
	printf("root  \n");
	printf(" |    \n");
	char dest[128] = { 0 };
	char src[128] = { 0 };
	int i = 0, j = 0;
	tnode* node = root->left_child;
	tar_deg* td = (tar_deg*)(node->data);
	sprintf(dest, "%d(%d)", td->target, td->degree);
	node = node->sibling;
	while(node)
	{
		i++;
		td = (tar_deg*)(node->data);
		sprintf(src, "-------------------------->%5d(%3d)", td->target, td->degree);
		strcat(dest, src);
		memset(src, 0, 128);
		if(2 == i)
			break;
		node = node->sibling;
	}
	printf("%s\n", dest);
	memset(dest, 0, 128);
	printf("   |                                   |                                     |\n");
	node = root->left_child;
	i = 0;
	while(node)
	{
		i++;
		j = 0;
		tnode* tmp = node->left_child;
		if(!node)
		{
			sprintf(src, "      none  ");
			strcat(dest, src);
			memset(src, 0, 128);
			node = node->sibling;
			continue;
		}
		td = (tar_deg*)(tmp->data);
		if(1 == i)
			sprintf(dest, "%5d(%3d)", td->target, td->degree);
		else
		{
			sprintf(src, "  %5d(%3d)", td->target, td->degree);
			strcat(dest, src);
			memset(src, 0, 128);
		}
		tmp = tmp->sibling;
		while(tmp)
		{
			j++;
			td = (tar_deg*)(node->data);
			sprintf(src, "-->%5d(%3d)", td->target, td->degree);
			strcat(dest, src);
			memset(src, 0, 128);
			node = node->sibling;
			if(2 == j)
				break;
		}
		node = node->sibling;
		if(3 == i)
			break;
	}
	printf("%s\n", dest);
}

int	main(int argc, char* argv[])
{
	List* s_list = (List*)malloc(sizeof(List));
	List* t_list = (List*)malloc(sizeof(List));
	create_List(s_list);
	create_List(t_list);
	printf("scanData...\n");
	scanData(s_list, t_list);
	printf("screenData...!\n");
	screenData(t_list);
	printf("t_list count: %d\n", *(int*)getData(t_list));
	printf("sort target list...\n");
	sort_List(t_list, getCount);
	printList(t_list, "sort_t_list");
	printListValue(t_list);
	printf("tidy s_list...\n");
	tidyListByMdata(s_list);
	printf("tidyData...\n");
	printf("s_list count: %d\n", *(int*)getData(s_list));
	tidyData(s_list, t_list);
	tidyListByMdata(s_list);
	printList(s_list, "s_list");
	printf("s_list count: %d\n", *(int*)getData(s_list));
	Mdata* md = (Mdata*)getData(s_list->next);
	tar_deg* td = (tar_deg*)getData(md->list->next);
	printf("sort by degree...\n");
	sortByDegree(s_list, t_list);
	printList(s_list, "sort_s_list");
	tnode* root = (tnode*)malloc(sizeof(tnode));
	create_tree(root);
	printf("FP contribute...\n");
	FP_contribute(s_list, t_list, root);
	printfTree(root);
	printf("get Set...\n");
	getSet(t_list);
	theLast(t_list);
}
