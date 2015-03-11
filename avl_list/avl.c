/**MOD+***********************************************************************/
/* Module:    ntlavl.c                                                       */
/*                                                                           */
/* Purpose:   AVL tree functions                                             */
/*                                                                           */
/* (C) COPYRIGHT DATA CONNECTION LIMITED 1993 - 2004                         */
/*                                                                           */
/* $Revision::   1.19                  $$Modtime::   Feb 12 2004 13:13:52   $*/
/*                                                                           */
/**MOD-***********************************************************************/

#include "avl.h"

NBB_VOID ntl_avl_balance_tree(AVL_TREE *, AVL_NODE *  );
NBB_VOID ntl_avl_rebalance(AVL_NODE **  );
NBB_VOID ntl_avl_rotate_right(AVL_NODE **  );
NBB_VOID ntl_avl_rotate_left(AVL_NODE **  );
NBB_VOID ntl_avl_swap_right_most(AVL_TREE *,
                                 AVL_NODE *,
                                 AVL_NODE * );
NBB_VOID ntl_avl_swap_left_most(AVL_TREE *, AVL_NODE *, AVL_NODE * );

/**PROC+**********************************************************************/
/* Name:      ntl_avl_insert_or_find                                         */
/*                                                                           */
/* Purpose:   Insert the supplied node into the specified AVL tree if key    */
/*            does not already exist, otherwise returning the existing node  */
/*                                                                           */
/* Returns:   NBB_VOID *               - Pointer to existing entry if found. */
/*                                       NULL if no such entry (implies node */
/*                                       successfully inserted)              */
/*                                                                           */
/* Params:    IN     tree              - a pointer to the AVL tree           */
/*            IN     node              - a pointer to the node to insert     */
/*                                                                           */
/* Operation: Scan down the tree looking for the insert point, going left    */
/*            if the insert key is less than the key in the tree and         */
/*            right if it is greater. When the insert point is found insert  */
/*            the new node and rebalance the tree if necessary. Return the   */
/*            existing entry instead, if found                               */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID *ntl_avl_insert_or_find(AVL_TREE *tree,
                                 AVL_NODE *node
                                  NBB_CXT)
{
  /***************************************************************************/
  /* insert specified node into tree                                         */
  /***************************************************************************/
  AVL_NODE *parent_node;
  NBB_INT result;
  NBB_VOID *existing_entry = NULL;

  NBB_TRC_ENTRY("ntl_avl_insert_or_find");

  NBB_ASSERT(!AVL_IN_TREE(*node));

  node->r_height = 0;
  node->l_height = 0;

  if (tree->root == NULL)
  {
    /*************************************************************************/
    /* tree is empty, so insert at root                                      */
    /*************************************************************************/
    NBB_TRC_FLOW(( "tree is empty, so insert at root" ));
    tree->root = node;
    tree->first = node;
    tree->last = node;
    goto EXIT;
  }

  /***************************************************************************/
  /* scan down the tree looking for the appropriate insert point             */
  /***************************************************************************/
  NBB_TRC_FLOW(( "scan for insert point" ));
  parent_node = tree->root;
  while (parent_node != NULL)
  {
    /*************************************************************************/
    /* go left or right, depending on comparison                             */
    /*************************************************************************/
    NBB_TRC_FLOW(( "Got parent %p", parent_node));
    result = tree->compare(node->key, parent_node->key  NBB_CCXT);

    if (result > 0)
    {
      /***********************************************************************/
      /* new key is greater than this node's key, so move down right subtree */
      /***********************************************************************/
      NBB_TRC_FLOW(( "move down right subtree" ));
      if (parent_node->right == NULL)
      {
        /*********************************************************************/
        /* right subtree is empty, so insert here                            */
        /*********************************************************************/
        NBB_TRC_FLOW(( "right subtree empty, insert here" ));
        node->parent = parent_node;
        parent_node->right = node;
        parent_node->r_height = 1;
        if (parent_node == tree->last)
        {
          /*******************************************************************/
          /* parent was the right-most node in the tree, so new node is now  */
          /* right-most                                                      */
          /*******************************************************************/
          NBB_TRC_FLOW(( "new last node" ));
          tree->last = node;
        }
        break;
      }
      else
      {
        /*********************************************************************/
        /* right subtree is not empty                                        */
        /*********************************************************************/
        NBB_TRC_FLOW(( "right subtree not empty" ));
        parent_node = parent_node->right;
      }
    }
    else if (result < 0)
    {
      /***********************************************************************/
      /* new key is less than this node's key, so move down left subtree     */
      /***********************************************************************/
      NBB_TRC_FLOW(( "move down left subtree" ));
      if (parent_node->left == NULL)
      {
        /*********************************************************************/
        /* left subtree is empty, so insert here                             */
        /*********************************************************************/
        NBB_TRC_FLOW(( "left subtree empty, insert here" ));
        node->parent = parent_node;
        parent_node->left = node;
        parent_node->l_height = 1;
        if (parent_node == tree->first)
        {
          /*******************************************************************/
          /* parent was the left-most node in the tree, so new node is now   */
          /* left-most                                                       */
          /*******************************************************************/
          NBB_TRC_FLOW(( "new first node" ));
          tree->first = node;
        }
        break;
      }
      else
      {
        /*********************************************************************/
        /* left subtree is not empty                                         */
        /*********************************************************************/
        NBB_TRC_FLOW(( "left subtree not empty" ));
        parent_node = parent_node->left;
      }
    }
    else
    {
      /***********************************************************************/
      /* found a matching key, so get out now and return entry found         */
      /***********************************************************************/
      NBB_TRC_FLOW(( "found matching key" ));
      existing_entry = parent_node->self;
      node->r_height = -1;
      node->l_height = -1;
      goto EXIT;
    }
  }

  /***************************************************************************/
  /* now rebalance the tree if necessary                                     */
  /***************************************************************************/
  NBB_TRC_FLOW(( "rebalance the tree" ));
  ntl_avl_balance_tree(tree, parent_node  NBB_CCXT);

EXIT:

  NBB_TRC_EXIT();

  return(existing_entry);

} /* ntl_avl_insert_or_find */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_delete                                                 */
/*                                                                           */
/* Purpose:   Delete the specified node from the specified AVL tree          */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     tree              - a pointer to the AVL tree           */
/*            IN     node              - a pointer to the node to delete     */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID ntl_avl_delete(AVL_TREE *tree, AVL_NODE *node   NBB_CXT)
{
  /***************************************************************************/
  /* delete specified node from tree                                         */
  /***************************************************************************/
  AVL_NODE *replace_node;
  AVL_NODE *parent_node;
  NBB_USHORT new_height;

  NBB_TRC_ENTRY("ntl_avl_delete");

  NBB_ASSERT(AVL_IN_TREE(*node));

  if ((node->left == NULL) &&
      (node->right == NULL))
  {
    /*************************************************************************/
    /* barren node (no children), so just delete it                          */
    /*************************************************************************/
    NBB_TRC_FLOW(( "delete barren node" ));
    replace_node = NULL;

    if (tree->first == node)
    {
      /***********************************************************************/
      /* node was first in tree, so replace it                               */
      /***********************************************************************/
      NBB_TRC_FLOW(( "replace first node in tree" ));
      tree->first = node->parent;
    }

    if (tree->last == node)
    {
      /***********************************************************************/
      /* node was last in tree, so replace it                                */
      /***********************************************************************/
      NBB_TRC_FLOW(( "replace last node in tree" ));
      tree->last = node->parent;
    }
  }
  else if (node->left == NULL)
  {
    /*************************************************************************/
    /* node has no left son, so replace with right son                       */
    /*************************************************************************/
    NBB_TRC_FLOW(( "node has no left son, replace with right son" ));
    replace_node = node->right;

    if (tree->first == node)
    {
      /***********************************************************************/
      /* node was first in tree, so replace it                               */
      /***********************************************************************/
      NBB_TRC_FLOW(( "replace first node in tree" ));
      tree->first = replace_node;
    }
  }
  else if (node->right == NULL)
  {
    /*************************************************************************/
    /* node has no right son, so replace with left son                       */
    /*************************************************************************/
    NBB_TRC_FLOW(( "node has no right son, replace with left son" ));
    replace_node = node->left;

    if (tree->last == node)
    {
      /***********************************************************************/
      /* node was last in tree, so replace it                                */
      /***********************************************************************/
      NBB_TRC_FLOW(( "replace last node in tree" ));
      tree->last = replace_node;
    }
  }
  else
  {
    /*************************************************************************/
    /* node has both left and right-sons                                     */
    /*************************************************************************/
    NBB_TRC_FLOW(( "node has two sons" ));
    if (node->r_height > node->l_height)
    {
      /***********************************************************************/
      /* right subtree is higher than left subtree                           */
      /***********************************************************************/
      NBB_TRC_FLOW(( "right subtree is higher" ));
      if (node->right->left == NULL)
      {
        /*********************************************************************/
        /* can replace node with right-son (since it has no left-son)        */
        /*********************************************************************/
        NBB_TRC_FLOW(( "replace node with right son" ));
        replace_node = node->right;
        replace_node->left = node->left;
        replace_node->left->parent = replace_node;
        replace_node->l_height = node->l_height;
      }
      else
      {
        /*********************************************************************/
        /* swap with left-most descendent of right subtree                   */
        /*********************************************************************/
        NBB_TRC_FLOW(( "swap with left-most right descendent" ));
        ntl_avl_swap_left_most(tree, node->right, node  NBB_CCXT);
        replace_node = node->right;
      }
    }
    else
    {
      /***********************************************************************/
      /* left subtree is higher (or subtrees are of same height)             */
      /***********************************************************************/
      NBB_TRC_FLOW(( "left subtree is higher" ));
      NBB_TRC_FLOW(( "(or both subtrees are of equal height)" ));
      if (node->left->right == NULL)
      {
        /*********************************************************************/
        /* can replace node with left-son (since it has no right-son)        */
        /*********************************************************************/
        NBB_TRC_FLOW(( "replace node with left son" ));
        replace_node = node->left;
        replace_node->right = node->right;
        replace_node->right->parent = replace_node;
        replace_node->r_height = node->r_height;
      }
      else
      {
        /*********************************************************************/
        /* swap with right-most descendent of left subtree                   */
        /*********************************************************************/
        NBB_TRC_FLOW(( "swap with right-most left descendent" ));
        ntl_avl_swap_right_most(tree, node->left, node  NBB_CCXT);
        replace_node = node->left;
      }
    }
  }

  /***************************************************************************/
  /* save parent node of deleted node                                        */
  /***************************************************************************/
  parent_node = node->parent;

  /***************************************************************************/
  /* reset deleted node                                                      */
  /***************************************************************************/
  NBB_TRC_FLOW(( "reset deleted node" ));
  node->parent = NULL;
  node->right = NULL;
  node->left = NULL;
  node->r_height = -1;
  node->l_height = -1;

  if (replace_node != NULL)
  {
    /*************************************************************************/
    /* fix-up parent pointer of replacement node, and calculate new height   */
    /* of subtree                                                            */
    /*************************************************************************/
    NBB_TRC_FLOW(( "fixup parent pointer of replacement node" ));
    replace_node->parent = parent_node;
    new_height = (NBB_USHORT)
                 (1 + NBB_MAX(replace_node->l_height, replace_node->r_height));
  }
  else
  {
    /*************************************************************************/
    /* no replacement, so new height of subtree is zero                      */
    /*************************************************************************/
    NBB_TRC_FLOW(( "no replacement"));
    new_height = 0;
  }
  NBB_TRC_FLOW(( "new height of parent is %d", new_height ));

  if (parent_node != NULL)
  {
    /*************************************************************************/
    /* fixup parent node                                                     */
    /*************************************************************************/
    NBB_TRC_FLOW(( "fix-up parent node" ));
    if (parent_node->right == node)
    {
      /***********************************************************************/
      /* node is right son of parent                                         */
      /***********************************************************************/
      NBB_TRC_FLOW(( "replacement node is right son" ));
      parent_node->right = replace_node;
      parent_node->r_height = new_height;
    }
    else
    {
      /***********************************************************************/
      /* node is left son of parent                                          */
      /***********************************************************************/
      NBB_TRC_FLOW(( "replacement node is left son" ));
      parent_node->left = replace_node;
      parent_node->l_height = new_height;
    }

    /*************************************************************************/
    /* now rebalance the tree (if necessary)                                 */
    /*************************************************************************/
    NBB_TRC_FLOW(( "rebalance the tree" ));
    ntl_avl_balance_tree(tree, parent_node  NBB_CCXT);
  }
  else
  {
    /*************************************************************************/
    /* replacement node is now root of tree                                  */
    /*************************************************************************/
    NBB_TRC_FLOW(( "replacement node is now root of tree" ));
    tree->root = replace_node;
  }

  NBB_TRC_EXIT();

  return;

} /* ntl_avl_delete */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_find                                                   */
/*                                                                           */
/* Purpose:   Find the node in the AVL tree with the supplied key            */
/*                                                                           */
/* Returns:   A pointer to the node                                          */
/*            NULL if no node is found with the specified key                */
/*                                                                           */
/* Params:    IN     tree              - a pointer to the AVL tree           */
/*            IN     key               - a pointer to the key                */
/*                                                                           */
/* Operation: Search down the tree going left if the search key is less than */
/*            the node in the tree and right if the search key is greater.   */
/*            When we run out of tree to search through either we've found   */
/*            it or the node is not in the tree.                             */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID *ntl_avl_find(AVL_TREE *tree, NBB_VOID *key   NBB_CXT)
{
  /***************************************************************************/
  /* find node with specified key                                            */
  /***************************************************************************/
  AVL_NODE *node;
  NBB_INT result;

  NBB_TRC_ENTRY("ntl_avl_find");

  node = tree->root;

  while (node != NULL)
  {
    /*************************************************************************/
    /* compare key of current node with supplied key                         */
    /*************************************************************************/
    NBB_TRC_FLOW(( "checking node %p", node));
    result = tree->compare(key, node->key  NBB_CCXT);

    if (result > 0)
    {
      /***********************************************************************/
      /* specified key is greater than key of this node, so look in right    */
      /* subtree                                                             */
      /***********************************************************************/
      NBB_TRC_FLOW(( "move down right subtree" ));
      node = node->right;
    }
    else if (result < 0)
    {
      /***********************************************************************/
      /* specified key is less than key of this node, so look in left        */
      /* subtree                                                             */
      /***********************************************************************/
      NBB_TRC_FLOW(( "move down left subtree" ));
      node = node->left;
    }
    else
    {
      /***********************************************************************/
      /* found the requested node                                            */
      /***********************************************************************/
      NBB_TRC_FLOW(( "found requested node" ));
      break;
    }
  }

  NBB_TRC_EXIT();

  return((node != NULL) ? node->self : NULL);

} /* ntl_avl_find */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_next                                                   */
/*                                                                           */
/* Purpose:   Find next node in the AVL tree                                 */
/*                                                                           */
/* Returns:   A pointer to the next node in the tree                         */
/*                                                                           */
/* Params:    IN     node              - a pointer to the current node in    */
/*                                       the tree                            */
/*                                                                           */
/* Operation: If the specified node has a right-son then return the left-    */
/*            most son of this. Otherwise search back up until we find a     */
/*            node of which we are in the left sub-tree and return that.     */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID *ntl_avl_next(AVL_NODE *node   NBB_CXT)
{
  /***************************************************************************/
  /* find next node in tree                                                  */
  /***************************************************************************/
  NBB_TRC_ENTRY("ntl_avl_next");

  NBB_ASSERT(AVL_IN_TREE(*node));

  if (node->right != NULL)
  {
    /*************************************************************************/
    /* next node is left-most node in right subtree                          */
    /*************************************************************************/
    NBB_TRC_FLOW(( "next node is left-most right descendent" ));
    node = node->right;
    while (node->left != NULL)
    {
      /***********************************************************************/
      /* FLOW TRACING NOT REQUIRED    Reason: Performance in AVL lookup.     */
      /***********************************************************************/
      node = node->left;
    }
  }
  else
  {
    /*************************************************************************/
    /* no right-son, so find a node of which we are in the left subtree      */
    /*************************************************************************/
    NBB_TRC_FLOW(( "find node which this is in left subtree of" ));

    while (node != NULL)
    {
      /***********************************************************************/
      /* FLOW TRACING NOT REQUIRED    Reason: Performance in AVL lookup.     */
      /***********************************************************************/
      if ((node->parent == NULL) ||
          (node->parent->left == node))
      {
        /*********************************************************************/
        /* FLOW TRACING NOT REQUIRED    Reason: Performance in AVL lookup.   */
        /*********************************************************************/
        node = node->parent;
        break;
      }
      node = node->parent;
    }
  }

  NBB_TRC_EXIT();

  return((node != NULL) ? node->self : NULL);

} /* ntl_avl_next */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_prev                                                   */
/*                                                                           */
/* Purpose:   Find previous node in the AVL tree                             */
/*                                                                           */
/* Returns:   A pointer to the previous node in the tree                     */
/*                                                                           */
/* Params:    IN     node              - a pointer to the current node in    */
/*                                       the tree                            */
/*                                                                           */
/* Operation: If we have a left-son then the previous node is the right-most */
/*            son of this. Otherwise, look for a node of whom we are in the  */
/*            left subtree and return that.                                  */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID *ntl_avl_prev(AVL_NODE *node   NBB_CXT)
{
  /***************************************************************************/
  /* find previous node in tree                                              */
  /***************************************************************************/
  NBB_TRC_ENTRY("ntl_avl_prev");

  NBB_ASSERT(AVL_IN_TREE(*node));

  if (node->left != NULL)
  {
    /*************************************************************************/
    /* previous node is right-most node in left subtree                      */
    /*************************************************************************/
    NBB_TRC_FLOW(( "previous node is right-most left descendent" ));
    node = node->left;
    while (node->right != NULL)
    {
      /***********************************************************************/
      /* FLOW TRACING NOT REQUIRED    Reason: Performance in AVL lookup.     */
      /***********************************************************************/
      node = node->right;
    }
  }
  else
  {
    /*************************************************************************/
    /* no left-son, so find a node of which we are in the right subtree      */
    /*************************************************************************/
    NBB_TRC_FLOW(( "find node which this is in right subtree of"));
    while (node != NULL)
    {
      /***********************************************************************/
      /* FLOW TRACING NOT REQUIRED    Reason: Performance in AVL lookup.     */
      /***********************************************************************/
      if ((node->parent == NULL) ||
          (node->parent->right == node))
      {
        /*********************************************************************/
        /* FLOW TRACING NOT REQUIRED    Reason: Performance in AVL lookup.   */
        /*********************************************************************/
        node = node->parent;
        break;
      }
      node = node->parent;
    }
  }

  NBB_TRC_EXIT();

  return((node != NULL) ? node->self : NULL);

} /* ntl_avl_prev */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_balance_tree                                           */
/*                                                                           */
/* Purpose:   Reblance the tree starting at the supplied node and ending at  */
/*            the root of the tree                                           */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     tree              - a pointer to the AVL tree           */
/*            IN     node              - a pointer to the node to start      */
/*                                       balancing from                      */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID ntl_avl_balance_tree(AVL_TREE *tree,
                              AVL_NODE *node
                               NBB_CXT)
{
  /***************************************************************************/
  /* balance the tree starting at the supplied node, and ending at the root  */
  /* of the tree                                                             */
  /***************************************************************************/
  NBB_TRC_ENTRY("ntl_avl_balance_tree");

  while (node->parent != NULL)
  {
    /*************************************************************************/
    /* node has uneven balance, so may need to rebalance it                  */
    /*************************************************************************/
    NBB_TRC_FLOW(( "check node balance" ));
    NBB_TRC_DETAIL(( "  r_height = %d", node->r_height ));
    NBB_TRC_DETAIL(( "  l_height = %d", node->l_height ));

    if (node->parent->right == node)
    {
      /***********************************************************************/
      /* node is right-son of its parent                                     */
      /***********************************************************************/
      NBB_TRC_FLOW(( "node is right-son" ));
      node = node->parent;
      ntl_avl_rebalance(&node->right  NBB_CCXT);

      /***********************************************************************/
      /* now update the right height of the parent                           */
      /***********************************************************************/
      node->r_height = (NBB_USHORT)
                   (1 + NBB_MAX(node->right->r_height, node->right->l_height));
      NBB_TRC_DETAIL(( "new r_height = %d", node->r_height ));
    }
    else
    {
      /***********************************************************************/
      /* node is left-son of its parent                                      */
      /***********************************************************************/
      NBB_TRC_FLOW(( "node is left-son" ));
      node = node->parent;
      ntl_avl_rebalance(&node->left  NBB_CCXT);

      /***********************************************************************/
      /* now update the left height of the parent                            */
      /***********************************************************************/
      node->l_height = (NBB_USHORT)
                     (1 + NBB_MAX(node->left->r_height, node->left->l_height));
      NBB_TRC_DETAIL(( "new l_height = %d", node->l_height ));
    }
  }

  if (node->l_height != node->r_height)
  {
    /*************************************************************************/
    /* rebalance root node                                                   */
    /*************************************************************************/
    NBB_TRC_FLOW(( "rebalance root node" ));
    ntl_avl_rebalance(&tree->root  NBB_CCXT);
  }

  NBB_TRC_EXIT();

  return;

} /* ntl_avl_balance_tree */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_rebalance                                              */
/*                                                                           */
/* Purpose:   Reblance a subtree of the AVL tree (if necessary)              */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN/OUT subtree           - a pointer to the subtree to         */
/*                                       rebalance                           */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID ntl_avl_rebalance(AVL_NODE **subtree   NBB_CXT)
{
  /***************************************************************************/
  /* Local data                                                              */
  /***************************************************************************/
  NBB_INT moment;

  /***************************************************************************/
  /* rebalance a subtree of the AVL tree                                     */
  /***************************************************************************/
  NBB_TRC_ENTRY("ntl_avl_rebalance");

  NBB_TRC_FLOW(( "rebalance subtree" ));
  NBB_TRC_DETAIL(( "  r_height = %d", (*subtree)->r_height ));
  NBB_TRC_DETAIL(( "  l_height = %d", (*subtree)->l_height ));

  /***************************************************************************/
  /* How unbalanced - don't want to recalculate                              */
  /***************************************************************************/
  moment = (*subtree)->r_height - (*subtree)->l_height;

  if (moment > 1)
  {
    /*************************************************************************/
    /* subtree is heavy on the right side                                    */
    /*************************************************************************/
    NBB_TRC_FLOW(( "subtree is heavy on right side" ));
    NBB_TRC_DETAIL(( "right subtree" ));
    NBB_TRC_DETAIL(( "  r_height = %d",
                                                 (*subtree)->right->r_height));
    NBB_TRC_DETAIL(( "  l_height = %d",
                                                 (*subtree)->right->l_height));
    if ((*subtree)->right->l_height > (*subtree)->right->r_height)
    {
      /***********************************************************************/
      /* right subtree is heavier on left side, so must perform right        */
      /* rotation on this subtree to make it heavier on the right side       */
      /***********************************************************************/
      NBB_TRC_FLOW(( "right subtree is heavier on left side ..." ));
      NBB_TRC_FLOW(( "... so rotate it right" ));
      ntl_avl_rotate_right(&(*subtree)->right  NBB_CCXT);
      NBB_TRC_DETAIL(( "right subtree" ));
      NBB_TRC_DETAIL(( "  r_height = %d",
                                                 (*subtree)->right->r_height));
      NBB_TRC_DETAIL(( "  l_height = %d",
                                                 (*subtree)->right->l_height));
    }

    /*************************************************************************/
    /* now rotate the subtree left                                           */
    /*************************************************************************/
    NBB_TRC_FLOW(( "rotate subtree left" ));
    ntl_avl_rotate_left(subtree  NBB_CCXT);
  }
  else if (moment < -1)
  {
    /*************************************************************************/
    /* subtree is heavy on the left side                                     */
    /*************************************************************************/
    NBB_TRC_FLOW(( "subtree is heavy on left side" ));
    NBB_TRC_DETAIL(( "left subtree" ));
    NBB_TRC_DETAIL(( "  r_height = %d", (*subtree)->left->r_height));
    NBB_TRC_DETAIL(( "  l_height = %d", (*subtree)->left->l_height));
    if ((*subtree)->left->r_height > (*subtree)->left->l_height)
    {
      /***********************************************************************/
      /* left subtree is heavier on right side, so must perform left         */
      /* rotation on this subtree to make it heavier on the left side        */
      /***********************************************************************/
      NBB_TRC_FLOW(( "left subtree is heavier on right side ..." ));
      NBB_TRC_FLOW(( "... so rotate it left" ));
      ntl_avl_rotate_left(&(*subtree)->left  NBB_CCXT);
      NBB_TRC_DETAIL(( "left subtree" ));
      NBB_TRC_DETAIL(( "  r_height = %d",
                                                  (*subtree)->left->r_height));
      NBB_TRC_DETAIL(( "  l_height = %d",
                                                  (*subtree)->left->l_height));
    }

    /*************************************************************************/
    /* now rotate the subtree right                                          */
    /*************************************************************************/
    NBB_TRC_FLOW(( "rotate subtree right" ));
    ntl_avl_rotate_right(subtree  NBB_CCXT);
  }

  NBB_TRC_DETAIL(( "balanced subtree" ));
  NBB_TRC_DETAIL(( "  r_height = %d", (*subtree)->r_height ));
  NBB_TRC_DETAIL(( "  l_height = %d", (*subtree)->l_height ));

  NBB_TRC_EXIT();

  return;

} /* ntl_avl_rebalance */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_rotate_right                                           */
/*                                                                           */
/* Purpose:   Rotate a subtree of the AVL tree right                         */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN/OUT subtree           - a pointer to the subtree to rotate  */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID ntl_avl_rotate_right(AVL_NODE **subtree   NBB_CXT)
{
  /***************************************************************************/
  /* rotate subtree of AVL tree right                                        */
  /***************************************************************************/
  AVL_NODE *left_son;

  NBB_TRC_ENTRY("ntl_avl_rotate_right");

  left_son = (*subtree)->left;

  (*subtree)->left = left_son->right;
  if ((*subtree)->left != NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    (*subtree)->left->parent = (*subtree);
  }
  (*subtree)->l_height = left_son->r_height;

  left_son->parent = (*subtree)->parent;

  left_son->right = *subtree;
  left_son->right->parent = left_son;
  left_son->r_height = (NBB_USHORT)
                     (1 + NBB_MAX((*subtree)->r_height, (*subtree)->l_height));

  *subtree = left_son;

  NBB_TRC_EXIT();

  return;

} /* ntl_avl_rotate_right */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_rotate_left                                            */
/*                                                                           */
/* Purpose:   Rotate a subtree of the AVL tree left                          */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN/OUT subtree           - a pointer to the subtree to rotate  */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID ntl_avl_rotate_left(AVL_NODE **subtree   NBB_CXT)
{
  /***************************************************************************/
  /* rotate a subtree of the AVL tree left                                   */
  /***************************************************************************/
  AVL_NODE *right_son;

  NBB_TRC_ENTRY("ntl_avl_rotate_left");

  right_son = (*subtree)->right;

  (*subtree)->right = right_son->left;
  if ((*subtree)->right != NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    (*subtree)->right->parent = (*subtree);
  }
  (*subtree)->r_height = right_son->l_height;

  right_son->parent = (*subtree)->parent;

  right_son->left = *subtree;
  right_son->left->parent = right_son;
  right_son->l_height = (NBB_USHORT)
                     (1 + NBB_MAX((*subtree)->r_height, (*subtree)->l_height));

  *subtree = right_son;

  NBB_TRC_EXIT();

  return;

} /* ntl_avl_rotate_left */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_swap_right_most                                        */
/*                                                                           */
/* Purpose:   Swap node with right-most descendent of subtree                */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     tree              - a pointer to the tree               */
/*            IN     subtree           - a pointer to the subtree            */
/*            IN     node              - a pointer to the node to swap       */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID ntl_avl_swap_right_most(AVL_TREE *tree,
                                 AVL_NODE *subtree,
                                 AVL_NODE *node
                                  NBB_CXT)
{
  /***************************************************************************/
  /* swap node with right-most descendent of specified subtree               */
  /***************************************************************************/
  AVL_NODE *swap_node;
  AVL_NODE *swap_parent;
  AVL_NODE *swap_left;

  NBB_TRC_ENTRY("ntl_avl_swap_right_most");

  NBB_ASSERT(node->right != NULL);
  NBB_ASSERT(node->left != NULL);

  /***************************************************************************/
  /* find right-most descendent of subtree                                   */
  /***************************************************************************/
  swap_node = subtree;
  while (swap_node->right != NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    swap_node = swap_node->right;
  }

  NBB_ASSERT(swap_node->r_height == 0);
  NBB_ASSERT(swap_node->l_height <= 1);

  /***************************************************************************/
  /* save parent and left-son of right-most descendent                       */
  /***************************************************************************/
  swap_parent = swap_node->parent;
  swap_left = swap_node->left;

  /***************************************************************************/
  /* move swap node to its new position                                      */
  /***************************************************************************/
  swap_node->parent = node->parent;
  swap_node->right = node->right;
  swap_node->left = node->left;
  swap_node->r_height = node->r_height;
  swap_node->l_height = node->l_height;
  swap_node->right->parent = swap_node;
  swap_node->left->parent = swap_node;
  if (node->parent == NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*                                                                       */
    /* node is at root of tree                                               */
    /*************************************************************************/
    tree->root = swap_node;
  }
  else if (node->parent->right == node)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*                                                                       */
    /* node is right-son of parent                                           */
    /*************************************************************************/
    swap_node->parent->right = swap_node;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*                                                                       */
    /* node is left-son of parent                                            */
    /*************************************************************************/
    swap_node->parent->left = swap_node;
  }

  /***************************************************************************/
  /* move node to its new position                                           */
  /***************************************************************************/
  node->parent = swap_parent;
  node->right = NULL;
  node->left = swap_left;
  if (node->left != NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    node->left->parent = node;
    node->l_height = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    node->l_height = 0;
  }
  node->r_height = 0;
  node->parent->right = node;

  NBB_TRC_EXIT();

  return;

} /* ntl_avl_swap_right_most */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_swap_left_most                                         */
/*                                                                           */
/* Purpose:   Swap node with left-most descendent of subtree                 */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     tree              - a pointer to the tree               */
/*            IN     subtree           - a pointer to the subtree            */
/*            IN     node              - a pointer to the node to swap       */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID ntl_avl_swap_left_most(AVL_TREE *tree,
                                AVL_NODE *subtree,
                                AVL_NODE *node
                                 NBB_CXT)
{
  /***************************************************************************/
  /* swap node with left-most descendent of specified subtree                */
  /***************************************************************************/
  AVL_NODE *swap_node;
  AVL_NODE *swap_parent;
  AVL_NODE *swap_right;

  NBB_TRC_ENTRY("ntl_avl_swap_left_most");

  NBB_ASSERT(node->right != NULL);
  NBB_ASSERT(node->left != NULL);

  /***************************************************************************/
  /* find left-most descendent of subtree                                    */
  /***************************************************************************/
  swap_node = subtree;
  while (swap_node->left != NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    swap_node = swap_node->left;
  }

  NBB_ASSERT(swap_node->l_height == 0);
  NBB_ASSERT(swap_node->r_height <= 1);

  /***************************************************************************/
  /* save parent and right-son of left-most descendent                       */
  /***************************************************************************/
  swap_parent = swap_node->parent;
  swap_right = swap_node->right;

  /***************************************************************************/
  /* move swap node to its new position                                      */
  /***************************************************************************/
  swap_node->parent = node->parent;
  swap_node->right = node->right;
  swap_node->left = node->left;
  swap_node->r_height = node->r_height;
  swap_node->l_height = node->l_height;
  swap_node->right->parent = swap_node;
  swap_node->left->parent = swap_node;
  if (node->parent == NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*                                                                       */
    /* node is at root of tree                                               */
    /*************************************************************************/
    tree->root = swap_node;
  }
  else if (node->parent->right == node)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*                                                                       */
    /* node is right-son of parent                                           */
    /*************************************************************************/
    swap_node->parent->right = swap_node;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*                                                                       */
    /* node is left-son of parent                                            */
    /*************************************************************************/
    swap_node->parent->left = swap_node;
  }

  /***************************************************************************/
  /* move node to its new position                                           */
  /***************************************************************************/
  node->parent = swap_parent;
  node->right = swap_right;
  node->left = NULL;
  if (node->right != NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    node->right->parent = node;
    node->r_height = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    node->r_height = 0;
  }
  node->l_height = 0;
  node->parent->left = node;

  NBB_TRC_EXIT();

  return;

} /* ntl_avl_swap_left_most */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_find_or_find_next                                      */
/*                                                                           */
/* Purpose:   Find the successor node to the supplied key in the AVL tree    */
/*                                                                           */
/* Returns:   A pointer to the node                                          */
/*            NULL if no successor node to the supplied key is found         */
/*                                                                           */
/* Params:    IN     tree         - a pointer to the AVL tree                */
/*            IN     key          - a pointer to the key                     */
/*            IN     not_equal    - TRUE return a node strictly > key        */
/*                                  FALSE return a node >= key               */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID *ntl_avl_find_or_find_next(AVL_TREE *tree,
                                    NBB_VOID *key,
                                    NBB_BOOL not_equal
                                     NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  AVL_NODE *node;
  NBB_VOID *found_node = NULL;
  NBB_INT result;

  NBB_TRC_ENTRY("ntl_avl_find_or_find_next");

  node = tree->root;

  if (node != NULL)
  {
    /*************************************************************************/
    /* There is something in the tree                                        */
    /*************************************************************************/
    NBB_TRC_FLOW(( "Got root %p", node));
    for (;;)
    {
      /***********************************************************************/
      /* compare key of current node with supplied key                       */
      /***********************************************************************/
      NBB_TRC_FLOW(( "compare node %p", node));
      result = tree->compare(key, node->key  NBB_CCXT);

      if (result > 0)
      {
        /*********************************************************************/
        /* specified key is greater than key of this node, so look in right  */
        /* subtree                                                           */
        /*********************************************************************/
        NBB_TRC_FLOW(( "move down right subtree" ));
        if (node->right == NULL)
        {
          /*******************************************************************/
          /* We've found the previous node - so we now need to find the      */
          /* successor to this one.                                          */
          /*******************************************************************/
          NBB_TRC_FLOW(( "found previous node"));
          found_node = ntl_avl_next(node  NBB_CCXT);
          break;
        }
        node = node->right;
      }
      else if (result < 0)
      {
        /*********************************************************************/
        /* specified key is less than key of this node, so look in left      */
        /* subtree                                                           */
        /*********************************************************************/
        NBB_TRC_FLOW(( "move down left subtree" ));

        if (node->left == NULL)
        {
           /******************************************************************/
           /* We've found the next node so store and drop out                */
           /******************************************************************/
           NBB_TRC_FLOW(( "found next node"));
           found_node = node->self;
           break;
        }
        node = node->left;
      }
      else
      {
        /*********************************************************************/
        /* found the requested node                                          */
        /*********************************************************************/
        NBB_TRC_FLOW(( "found node for suplied key" ));

        if (not_equal)
        {
          /*******************************************************************/
          /* need to find the successor node to this node                    */
          /*******************************************************************/
          NBB_TRC_FLOW(( "next node required"));
          found_node = ntl_avl_next(node  NBB_CCXT);
        }
        else
        {
          NBB_TRC_FLOW(( "return the exact match"));
          found_node = node->self;
        }
        break;
      }
    }
  }

  NBB_TRC_EXIT();

  return(found_node);

} /* ntl_avl_find_or_find_next */

/*****************************************************************************/
/* Standard compare functions                                                */
/*****************************************************************************/

/**PROC+**********************************************************************/
/* Name:      compare_byte                                                   */
/*                                                                           */
/* Purpose:   Standard function for comparing NBB_BYTEs                      */
/*                                                                           */
/* Returns:   -1 if aa < bb                                                  */
/*             0 if aa = bb                                                  */
/*             1 if aa > bb                                                  */
/*                                                                           */
/* Params:    IN  aa                                                         */
/*            IN  bb                                                         */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_INT compare_byte(NBB_VOID *aa, NBB_VOID *bb   NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_INT ret_val;

  NBB_TRC_ENTRY("compare_byte");

  if (*(NBB_BYTE *)aa < *(NBB_BYTE *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = -1;
  }
  else if (*(NBB_BYTE *)aa > *(NBB_BYTE *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 0;
  }

  NBB_TRC_DETAIL(( "Returning %d", ret_val ));

  NBB_TRC_EXIT();

  return(ret_val);

} /* compare_byte */

/**PROC+**********************************************************************/
/* Name:      compare_short                                                  */
/*                                                                           */
/* Purpose:   Standard function for comparing NBB_SHORTs                     */
/*                                                                           */
/* Returns:   -1 if aa < bb                                                  */
/*             0 if aa = bb                                                  */
/*             1 if aa > bb                                                  */
/*                                                                           */
/* Params:    IN  aa                                                         */
/*            IN  bb                                                         */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_INT compare_short(NBB_VOID *aa, NBB_VOID *bb   NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_INT ret_val;

  NBB_TRC_ENTRY("compare_short");

  if (*(NBB_SHORT *)aa < *(NBB_SHORT *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = -1;
  }
  else if (*(NBB_SHORT *)aa > *(NBB_SHORT *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 0;
  }

  NBB_TRC_DETAIL(( "Returning %d", ret_val ));

  NBB_TRC_EXIT();

  return(ret_val);

} /* compare_short */

/**PROC+**********************************************************************/
/* Name:      compare_ushort                                                 */
/*                                                                           */
/* Purpose:   Standard function for comparing NBB_USHORTs                    */
/*                                                                           */
/* Returns:   -1 if aa < bb                                                  */
/*             0 if aa = bb                                                  */
/*             1 if aa > bb                                                  */
/*                                                                           */
/* Params:    IN  aa                                                         */
/*            IN  bb                                                         */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_INT compare_ushort(NBB_VOID *aa, NBB_VOID *bb   NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_INT ret_val;

  NBB_TRC_ENTRY("compare_ushort");

  if (*(NBB_USHORT *)aa < *(NBB_USHORT *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = -1;
  }
  else if (*(NBB_USHORT *)aa > *(NBB_USHORT *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 0;
  }

  NBB_TRC_DETAIL(( "Returning %d", ret_val ));

  NBB_TRC_EXIT();

  return(ret_val);

} /* compare_ushort */

/**PROC+**********************************************************************/
/* Name:      compare_long                                                   */
/*                                                                           */
/* Purpose:   Standard function for comparing NBB_LONGs                      */
/*                                                                           */
/* Returns:   -1 if aa < bb                                                  */
/*             0 if aa = bb                                                  */
/*             1 if aa > bb                                                  */
/*                                                                           */
/* Params:    IN  aa                                                         */
/*            IN  bb                                                         */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_INT compare_long(NBB_VOID *aa, NBB_VOID *bb   NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_INT ret_val;

  NBB_TRC_ENTRY("compare_long");

  if (*(NBB_LONG *)aa < *(NBB_LONG *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = -1;
  }
  else if (*(NBB_LONG *)aa > *(NBB_LONG *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 0;
  }

  NBB_TRC_DETAIL(( "Returning %d", ret_val ));

  NBB_TRC_EXIT();

  return(ret_val);

} /* compare_long */

/**PROC+**********************************************************************/
/* Name:      compare_ulong                                                  */
/*                                                                           */
/* Purpose:   Standard function for comparing NBB_ULONGs                     */
/*                                                                           */
/* Returns:   -1 if aa < bb                                                  */
/*             0 if aa = bb                                                  */
/*             1 if aa > bb                                                  */
/*                                                                           */
/* Params:    IN  aa                                                         */
/*            IN  bb                                                         */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_INT compare_ulong(NBB_VOID *aa, NBB_VOID *bb   NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_INT ret_val;

  NBB_TRC_ENTRY("compare_ulong");

  if (*(NBB_ULONG *)aa < *(NBB_ULONG *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = -1;
  }
  else if (*(NBB_ULONG *)aa > *(NBB_ULONG *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 0;
  }

  NBB_TRC_DETAIL(( "Returning %d", ret_val ));

  NBB_TRC_EXIT();

  return(ret_val);

} /* compare_ulong */

/**PROC+**********************************************************************/
/* Name:      compare_int                                                    */
/*                                                                           */
/* Purpose:   Standard function for comparing NBB_INTs                       */
/*                                                                           */
/* Returns:   -1 if aa < bb                                                  */
/*             0 if aa = bb                                                  */
/*             1 if aa > bb                                                  */
/*                                                                           */
/* Params:    IN  aa                                                         */
/*            IN  bb                                                         */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_INT compare_int(NBB_VOID *aa, NBB_VOID *bb   NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_INT ret_val;

  NBB_TRC_ENTRY("compare_int");

  if (*(NBB_INT *)aa < *(NBB_INT *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = -1;
  }
  else if (*(NBB_INT *)aa > *(NBB_INT *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 0;
  }

  NBB_TRC_DETAIL(( "Returning %d", ret_val ));

  NBB_TRC_EXIT();

  return(ret_val);

} /* compare_int */

/**PROC+**********************************************************************/
/* Name:      compare_uint                                                   */
/*                                                                           */
/* Purpose:   Standard function for comparing NBB_UINTs                      */
/*                                                                           */
/* Returns:   -1 if aa < bb                                                  */
/*             0 if aa = bb                                                  */
/*             1 if aa > bb                                                  */
/*                                                                           */
/* Params:    IN  aa                                                         */
/*            IN  bb                                                         */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_INT compare_uint(NBB_VOID *aa, NBB_VOID *bb   NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_INT ret_val;

  NBB_TRC_ENTRY("compare_uint");

  if (*(NBB_UINT *)aa < *(NBB_UINT *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = -1;
  }
  else if (*(NBB_UINT *)aa > *(NBB_UINT *)bb)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 1;
  }
  else
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED    Reason: Performance.                     */
    /*************************************************************************/
    ret_val = 0;
  }

  NBB_TRC_DETAIL(( "Returning %d", ret_val ));

  NBB_TRC_EXIT();

  return(ret_val);

} /* compare_uint */

/**PROC+**********************************************************************/
/* Name:      ntl_avl_verify_tree                                            */
/*                                                                           */
/* Purpose:   To verify that an AVL tree is correctly sorted.                */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     tree         - a pointer to the AVL tree                */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID ntl_avl_verify_tree(AVL_TREE *tree  NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  AVL_NODE *node = NULL;
  AVL_NODE *tmpnode = NULL;
  NBB_INT result;

  NBB_TRC_ENTRY("ntl_avl_verify_tree");

  /***************************************************************************/
  /* Start at the nominal first entry in the tree.  We check later that      */
  /* this really is the first.                                               */
  /***************************************************************************/
  node = tree->first;

  /***************************************************************************/
  /* Step up through the first node's family tree asserting that each older  */
  /* generation does not have a left pointer, or that left pointer is us.    */
  /***************************************************************************/
  while (node != NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED Reason: Performance                         */
    /*************************************************************************/
    NBB_ASSERT((node->left == NULL) || (node->left == tmpnode));

    /*************************************************************************/
    /* Ensure the tree root is planted in Eden (it has no parents).          */
    /*************************************************************************/
    if (node == tree->root)
    {
      /***********************************************************************/
      /* FLOW TRACING NOT REQUIRED Reason: Performance                       */
      /***********************************************************************/
      NBB_ASSERT(node->parent == NULL);
    }
    tmpnode = node;
    node = node->parent;
  }

  /***************************************************************************/
  /* Now we've ascertained that the first and root are correct, check the    */
  /* relative ordering of everything else in the tree.                       */
  /***************************************************************************/
  node = tree->first;

  while (node != NULL)
  {
    /*************************************************************************/
    /* FLOW TRACING NOT REQUIRED Reason: Performance                         */
    /*************************************************************************/
    tmpnode = node;

    /*************************************************************************/
    /* The following code duplicates AVL_NEXT, BUT without the final         */
    /* dereferencing of the self pointer.  tmpnode must be set to the        */
    /* AVL_NODE structure itself.                                            */
    /*************************************************************************/
    if (tmpnode->right != NULL)
    {
      /***********************************************************************/
      /* FLOW TRACING NOT REQUIRED Reason: Performance in AVL walk.          */
      /***********************************************************************/
      /***********************************************************************/
      /* next node is left-most node in right subtree                        */
      /***********************************************************************/
      tmpnode = tmpnode->right;
      while (tmpnode->left != NULL)
      {
        /*********************************************************************/
        /* FLOW TRACING NOT REQUIRED Reason: Performance in AVL walk.        */
        /*********************************************************************/
        tmpnode = tmpnode->left;
      }
    }
    else
    {
      /***********************************************************************/
      /* no right-son, so find a node of which we are in the left subtree    */
      /***********************************************************************/
      NBB_TRC_FLOW(( "find node which this is in left subtree of"));

      while (tmpnode != NULL)
      {
        /*********************************************************************/
        /* FLOW TRACING NOT REQUIRED Reason: Performance in AVL walk.        */
        /*********************************************************************/
        if ((tmpnode->parent == NULL) ||
            (tmpnode->parent->left == tmpnode))
        {
          /*******************************************************************/
          /* FLOW TRACING NOT REQUIRED Reason: Performance in AVL walk.      */
          /*******************************************************************/
          tmpnode = tmpnode->parent;
          break;
        }
        tmpnode = tmpnode->parent;
      }
    }
    /*************************************************************************/
    /* END duplication of AVL_NEXT code.                                     */
    /*************************************************************************/

    /*************************************************************************/
    /* Get out if we have reach the end of the line.                         */
    /*************************************************************************/
    if (tmpnode == NULL)
    {
      /***********************************************************************/
      /* FLOW TRACING NOT REQUIRED Reason: Performance                       */
      /***********************************************************************/
      break;
    }

    /*************************************************************************/
    /* Check that node and its next node are ordered correctly.              */
    /*************************************************************************/
    result = tree->compare(node->key, tmpnode->key NBB_CCXT);
    NBB_ASSERT(result < 0);

    /*************************************************************************/
    /* Also check the parentage.                                             */
    /*************************************************************************/
    NBB_ASSERT((node->parent != NULL) || (node == tree->root));

    node = tmpnode;
  }

  /***************************************************************************/
  /* Finally, now we have completed the walk of the (potentially empty)      */
  /* tree, check that the last pointer is set correctly.                     */
  /***************************************************************************/
  NBB_ASSERT(node == tree->last);

  /***************************************************************************/
  /* There is no need to walk through last's parentage to check them since   */
  /* we have already checked the correct ordering of all members of the tree */
  /* while we were walking.  This step is necessary for the first since it   */
  /* it the foundation upon which we base our tree walk to check all the     */
  /* relative orderings.                                                     */
  /***************************************************************************/

  NBB_TRC_EXIT();

  return;

} /* ntl_avl_verify_tree */
