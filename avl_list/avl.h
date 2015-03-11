#ifndef NTLAVL_INCLUDED
#define NTLAVL_INCLUDED

#include "os.h"

/**INC+***********************************************************************/
/* Header:    ntlavl.h                                                       */
/*                                                                           */
/* Purpose:   AVL tree functions                                             */
/*                                                                           */
/* (C) COPYRIGHT DATA CONNECTION LIMITED 1993 - 2004                         */
/*                                                                           */
/* $Revision::   1.22                  $$Modtime::   Feb 13 2004 12:20:30   $*/
/**INC-***********************************************************************/

/*****************************************************************************/
/* VxWorks 5.4.2 and above and VxWorks AE 1.0 typedef AVL_TREE and AVL_NODE  */
/* structures.  In order to avoid conflict with the OS equivalents the       */
/* N-BASE definitions are renamed.                                           */
/*****************************************************************************/
#define AVL_NODE NTL_AVL_NODE
#define AVL_TREE NTL_AVL_TREE

/**STRUCT+********************************************************************/
/* Structure: AVL_NODE                                                       */
/*                                                                           */
/* Description: Node in an AVL tree.                                         */
/*****************************************************************************/

typedef struct avl_node
{
  struct avl_node *parent;
  struct avl_node *left;
  struct avl_node *right;
  NBB_ULONG l_height;
  NBB_ULONG r_height;
  NBB_VOID *self;
  NBB_VOID *key;
} AVL_NODE;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/* compare function                                                          */
/*****************************************************************************/
typedef NBB_INT(AVL_COMPARE)(NBB_VOID *, NBB_VOID *  );

/**STRUCT+********************************************************************/
/* Structure: AVL_TREE                                                       */
/*                                                                           */
/* Description: AVL tree root.                                               */
/*****************************************************************************/

typedef struct avl_tree
{
  AVL_COMPARE *compare;
  AVL_NODE *root;
  AVL_NODE *first;
  AVL_NODE *last;
} AVL_TREE;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/* AVL functions                                                             */
/*****************************************************************************/
NBB_EXPORT NBB_VOID *ntl_avl_insert_or_find(AVL_TREE  *,
                                            AVL_NODE  *
                                            );
NBB_EXPORT NBB_VOID ntl_avl_delete(AVL_TREE *, AVL_NODE *  );
NBB_EXPORT NBB_VOID *ntl_avl_find(AVL_TREE *, NBB_VOID *  );
NBB_EXPORT NBB_VOID *ntl_avl_find_or_find_next(AVL_TREE *,
                                             NBB_VOID *, NBB_BOOL  );
NBB_EXPORT NBB_VOID *ntl_avl_next(AVL_NODE *  );
NBB_EXPORT NBB_VOID *ntl_avl_prev(AVL_NODE *  );
NBB_EXPORT NBB_VOID ntl_avl_verify_tree(AVL_TREE * );

/*****************************************************************************/
/* AVL access macros                                                         */
/*****************************************************************************/
#define AVL_INIT_TREE(TREE, COMPARE)   (TREE).compare = &(COMPARE);      \
                                       (TREE).first = NULL;              \
                                       (TREE).last = NULL;               \
                                       (TREE).root = NULL

#define AVL_INIT_NODE(NODE, SELF, KEY) (NODE).parent = NULL;             \
                                       (NODE).left = NULL;               \
                                       (NODE).right = NULL;              \
                                       (NODE).self = (SELF);             \
                                       (NODE).key = (KEY);               \
                                       (NODE).l_height = -1;             \
                                       (NODE).r_height = -1;

/*****************************************************************************/
/* Macro definitions                                                         */
/*****************************************************************************/
#define AVL_INSERT(TREE, NODE)                                                \
               (ntl_avl_insert_or_find(&(TREE), &(NODE)  NBB_CCXT) == NULL)
#define AVL_INSERT_OR_FIND(TREE, NODE)                                        \
               ntl_avl_insert_or_find(&(TREE), &(NODE)  NBB_CCXT)
#define AVL_DELETE(TREE, NODE)     ntl_avl_delete(&(TREE), &(NODE)  NBB_CCXT)
#define AVL_FIND(TREE, KEY)        ntl_avl_find(&(TREE), (KEY)  NBB_CCXT)
#define AVL_NEXT(NODE)             ntl_avl_next(&(NODE)  NBB_CCXT)
#define AVL_PREV(NODE)             ntl_avl_prev(&(NODE)  NBB_CCXT)
#define AVL_FIRST(TREE)                                                       \
                   (((&(TREE))->first != NULL) ? (&(TREE))->first->self : NULL)

#define AVL_LAST(TREE)                                                        \
                     (((&(TREE))->last != NULL) ? (&(TREE))->last->self : NULL)

#define AVL_IN_TREE(NODE)  (((NODE).l_height != -1) && ((NODE).r_height != -1))

#define AVL_FIND_NEXT(TREE, KEY)                                              \
                      ntl_avl_find_or_find_next(&(TREE), (KEY), TRUE  NBB_CCXT)

#define AVL_FIND_OR_FIND_NEXT(TREE, KEY)                                      \
                     ntl_avl_find_or_find_next(&(TREE), (KEY), FALSE  NBB_CCXT)

#ifdef NBB_DEBUG
#define AVL_VERIFY_TREE(TREE) ntl_avl_verify_tree(&(TREE) NBB_CCXT)
#else
#define AVL_VERIFY_TREE(TREE)
#endif

/*****************************************************************************/
/* Standard compare functions                                                */
/*****************************************************************************/
NBB_EXPORT NBB_INT compare_byte(NBB_VOID *, NBB_VOID *  );
NBB_EXPORT NBB_INT compare_short(NBB_VOID *, NBB_VOID *  );
NBB_EXPORT NBB_INT compare_ushort(NBB_VOID *, NBB_VOID *  );
NBB_EXPORT NBB_INT compare_long(NBB_VOID *, NBB_VOID *  );
NBB_EXPORT NBB_INT compare_ulong(NBB_VOID *, NBB_VOID *  );
NBB_EXPORT NBB_INT compare_int(NBB_VOID *, NBB_VOID *  );
NBB_EXPORT NBB_INT compare_uint(NBB_VOID *, NBB_VOID *  );

#endif
