#include "avl.h"
#include "avll.h"
#include "list_manage.h"


#define NBB_TRC_FLOW(args) Trace args

typedef struct test_node{
	unsigned int num;
	AVLL_NODE avll_node;
}TEST_NODE;





typedef struct test_avll{
unsigned int node_num;
AVLL_TREE avll_tree;
}TEST_AVLL_TREE;

typedef struct test_avll2{
	unsigned int node_num;
	AVLL_TREE avll_tree;
}TEST_AVLL_TREE2;

typedef struct router_node{
	char id[8];
	AVLL_NODE avll_node;
}ROUTER_NODE;

typedef struct router_tree{
	unsigned int node_num;
	AVLL_TREE avll_tree;
}ROUTER_TREE;

int compare_id(void *aa, void *bb)
{
    /*************************************************************************/
    /* 局部变量                                                              */
    /*************************************************************************/
    int ret_val;
    unsigned char *id1 = (unsigned char *)aa;
    unsigned char *id2 = (unsigned char *)bb;
	
    
	
    assert((aa != NULL)||(bb != NULL));
    
    /*************************************************************************/
    /* aa与bb进行比较                                                        */
    /*************************************************************************/
    ret_val = memcmp(id1, id2, 8);
    NBB_TRC_DETAIL(("return %d", ret_val));
    
    
    return(ret_val);
	
}

#define UINT32_MAX 100
int main_test1()
{
	unsigned int ii=0;
	TEST_AVLL_TREE tree_root;
    TEST_AVLL_TREE2 tree_root2;
	TEST_NODE *new_node=NULL;
	TEST_NODE *current_node=NULL;
	TEST_NODE *next_node=NULL;
	TraceInit();
	
	NBB_TRC_ENTRY("main");
	NBB_TRC_FLOW(("insert begin:\n"));
	
	
	AVLL_INIT_TREE(tree_root.avll_tree,
		compare_long,
		NBB_OFFSETOF(TEST_NODE,num),
		NBB_OFFSETOF(TEST_NODE, avll_node));
	
	AVLL_INIT_TREE(tree_root2.avll_tree,
		compare_long,
		NBB_OFFSETOF(TEST_NODE,num),
		NBB_OFFSETOF(TEST_NODE, avll_node));
	
	if(AVLL_FIRST(tree_root2.avll_tree)!=NULL)
	{
		NBB_TRC_FLOW(("inner_link_tree is NOT empty"));
    }
	
	tree_root.node_num = 0;
	
	tree_root2.node_num = 0;
	srand( (unsigned)time( NULL ) );
	
	for(ii=0;ii<UINT32_MAX;ii++)
	{
		new_node=(TEST_NODE *)malloc(sizeof(TEST_NODE));
		assert(new_node!=NULL);
		AVLL_INIT_NODE(new_node->avll_node);
		new_node->num=rand();
		NBB_TRC_FLOW((" ii:%d 0x%x ",ii,new_node->num));
		AVLL_INSERT(tree_root.avll_tree,new_node->avll_node);
	}
	NBB_TRC_FLOW(("insert end\n"));
	
	ii=0;
	NBB_TRC_FLOW(("print tree start\n"));
	current_node=AVLL_FIRST(tree_root.avll_tree);
	while(current_node!=NULL)
	{
		NBB_TRC_FLOW((" ii:%d 0x%x ",ii,current_node->num));
		current_node=AVLL_NEXT(tree_root.avll_tree,current_node->avll_node);
		ii++;
	}
    NBB_TRC_FLOW(("print tree end\n"));
	
	
	for(ii=0;ii<UINT32_MAX;ii++)
	{
		new_node=(TEST_NODE *)malloc(sizeof(TEST_NODE));
		assert(new_node!=NULL);
		AVLL_INIT_NODE(new_node->avll_node);
		new_node->num=rand();
		NBB_TRC_FLOW((" ii:%d 0x%x ",ii,new_node->num));
		AVLL_INSERT(tree_root2.avll_tree,new_node->avll_node);
	}
	NBB_TRC_FLOW(("insert end\n"));
	
	ii=0;
	NBB_TRC_FLOW(("print tree start\n"));
	current_node=AVLL_FIRST(tree_root2.avll_tree);
	while(current_node!=NULL)
	{
		NBB_TRC_FLOW((" ii:%d 0x%x ",ii,current_node->num));
		current_node=AVLL_NEXT(tree_root2.avll_tree,current_node->avll_node);
		ii++;
	}
	NBB_TRC_FLOW(("print tree end\n"));
	
#if 0 
	AVL_INIT_TREE(slave_tree, compare_long);
	NBB_INIT_ROOT(ahl_data->master_join_list);
	
	AVL_INIT_NODE(NODE, SELF, KEY);
	
	AVL_INSERT(TREE, NODE);
	
	AVL_FIND(TREE, KEY);
	
	AVL_NEXT(NODE);
	
	AVL_PREV(NODE);
	
	AVL_FIRST(TREE);
	
	AVLL_FIND(TREE, KEY)
#endif
		NBB_TRC_EXIT();
}

int main()
{
	unsigned int ii=0;
	ROUTER_TREE tree_root;
	ROUTER_NODE *new_node=NULL;
	ROUTER_NODE *current_node=NULL;
	ROUTER_NODE *next_node=NULL;
	TraceInit();
	
	NBB_TRC_ENTRY("main");
	NBB_TRC_FLOW(("insert begin:\n"));
	
	
	AVLL_INIT_TREE(tree_root.avll_tree,
		compare_id,
		NBB_OFFSETOF(ROUTER_NODE,id),
		NBB_OFFSETOF(ROUTER_NODE, avll_node));
	
	
	if(AVLL_FIRST(tree_root.avll_tree)!=NULL)
	{
		NBB_TRC_FLOW(("inner_link_tree is NOT empty"));
    }
	
	tree_root.node_num = 0;
	
	srand( (unsigned)time( NULL ) );
	
	for(ii=0;ii<UINT32_MAX;ii++)
	{
		new_node=(ROUTER_NODE *)malloc(sizeof(ROUTER_NODE));
		assert(new_node!=NULL);
		AVLL_INIT_NODE(new_node->avll_node);
		sprintf(new_node->id,"%d",rand());
		NBB_TRC_FLOW((" ii:%d %08s ",ii,new_node->id));
		AVLL_INSERT(tree_root.avll_tree,new_node->avll_node);
	}
	NBB_TRC_FLOW(("insert end\n"));
	
	ii=0;
	NBB_TRC_FLOW(("print tree start\n"));
	current_node=AVLL_FIRST(tree_root.avll_tree);
	while(current_node!=NULL)
	{
		NBB_TRC_FLOW((" ii:%d %08s ",ii,current_node->id));
		current_node=AVLL_NEXT(tree_root.avll_tree,current_node->avll_node);
		ii++;
	}
    NBB_TRC_FLOW(("print tree end\n"));
	
		NBB_TRC_EXIT();	
}