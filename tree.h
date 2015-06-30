#ifndef	TREE_H
#define	TREE_H
#include <stdio.h>
#include <stdlib.h>
typedef struct tree_node
{
	struct tree_node*	parent;
	struct tree_node*	left_child;
	struct tree_node*	sibling;
	void*	data;
}tnode;

typedef int (*getFunc_Tree)(tnode*);

int	create_tree(tnode* root)
{
	if(!root)
	{
		fprintf(stderr, "create_tree: Non pointer!\n");
		return -1;
	}
	root->parent = NULL;
	root->left_child = NULL;
	root->sibling = NULL;
	return 0;
}

//只查找当前节点的子节点，不进行递归遍历
int	search_child(tnode* parent, tnode** target, int value, getFunc_Tree getValue)
{
	if(!parent)
	{
		fprintf(stderr, "search_child: Non pointer!\n");
		return -1;
	}
	tnode* cur = parent->left_child;
	if(!cur)
		return 0;
	if(value == getValue(cur))
	{
		*target = cur;
		return 1;
	}
	cur = cur->sibling;
	while(cur)
	{
		if(value == getValue(cur))
		{
			*target = cur;
			return 2;
		}
		cur = cur->sibling;
	}
	return 0;
}

int	add_child(tnode* node, void* data)
{
	if(!node || !data)
	{
		fprintf(stderr, "add_child: Non pointer!\n");
		return -1;
	}
	tnode* child = (tnode*)malloc(sizeof(tnode));
	child->parent = node;
	child->left_child = NULL;
	child->sibling = NULL;
	child->data = data;
	if(!(node->left_child))
	{
		node->left_child = child;
		return 1;
	}
	tnode* cur = node->left_child;
	while(cur->sibling)
		cur = cur->sibling;
	cur->sibling = child;
	return 2;
}

int delete_child(tnode* node, void* data)
{
	if(!node)
	{
		fprintf(stderr, "delete_child: Non pointer!\n");
		return 1;
	}
	tnode* cur = node->left_child;
	if(data == cur->data)
	{
		node->left_child = cur->sibling;	//实际应该递归遍历
		return 1;
	}
	while(cur->sibling)
	{
		if(data == cur->sibling->data)
		{
			tnode* tmp = cur->sibling;
			cur->sibling = tmp->sibling;
			free(tmp);
			return 2;
		}
		cur = cur->sibling;
	}
	return -1;
}

#endif
