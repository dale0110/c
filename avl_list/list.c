#include "list_struct.h"
 
/**PROC+**********************************************************************/
/* Name:      nbb_get_q_head                                                 */
/*                                                                           */
/* Purpose:   get the queue element from the head of a queue                 */
/*                                                                           */
/* Returns:   pointer to queue element                                       */
/*            NULL if queue empty                                            */
/*                                                                           */
/* Params:    IN     root              - pointer to root of queue            */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID *nbb_get_q_head(NBB_Q_ROOT *root   )
{
  /***************************************************************************/
  /* get the queue elemnt from the head of a queue                           */
  /***************************************************************************/
  NBB_QE *element;

  NBB_TRC_ENTRY("nbb_get_q_head");

  element = root->first;
  if (element != NULL)
  {
    /*************************************************************************/
    /* queue is not empty                                                    */
    /*************************************************************************/
    NBB_TRC_FLOW(("Valid element."));

    root->first = element->next;

    if (root->first == NULL)
    {
      /***********************************************************************/
      /* queue is now empty                                                  */
      /***********************************************************************/
      NBB_TRC_FLOW(("Empty queue."));

      root->last = NULL;
    }
    element->next = NULL;
  }

  NBB_TRC_EXIT();

  return(element);

} /* nbb_get_q_head */

/**PROC+**********************************************************************/
/* Name:      nbb_l3qe_next_in_list                                          */
/*                                                                           */
/* Purpose:   Get the next entry in the list.                                */
/*                                                                           */
/* Returns:   Pointer to entry, or NULL if the next element is the root.     */
/*                                                                           */
/* Params:    IN     root         - Pointer to root of L3QE                  */
/*            IN     element      - Pointer to L3QE element, which might be  */
/*                                  the root element.                        */
/*            IN     offset       - Offset of L3QE element within structures */
/*                                  in this list.                            */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID *nbb_l3qe_next_in_list(NBB_L3QE_ROOT *root,
                                NBB_L3QE *element,
                                NBB_BUF_SIZE offset
                                 )
{
  /***************************************************************************/
  /* Get the next entry in the list.                                         */
  /***************************************************************************/
  NBB_VOID *next_entry;

  NBB_TRC_ENTRY("nbb_l3qe_next_in_list");

  NBB_TRC_DETAIL(("Offset is %lu", offset));

  NBB_TRC_DETAIL(("Root is %p; next element is %p",
                  &(root->l3qe),
                  element->next));

  if (&(root->l3qe) != element->next)
  {
    NBB_TRC_FLOW(("Next element is not root"));
    next_entry = (NBB_VOID *)(((NBB_BYTE *)element->next) - offset);
  }
  else
  {
    NBB_TRC_FLOW(("Next element is root"));
    next_entry = NULL;
  }

  NBB_TRC_DETAIL(("Returning %p", next_entry));
  NBB_TRC_EXIT();

  return(next_entry);

} /* nbb_l3qe_next_in_list */

/**PROC+**********************************************************************/
/* Name:      nbb_l3qe_prev_in_list                                          */
/*                                                                           */
/* Purpose:   Get the previous entry in the list.                            */
/*                                                                           */
/* Returns:   Pointer to entry, or NULL if the previous element is the root. */
/*                                                                           */
/* Params:    IN     root         - Pointer to root of L3QE                  */
/*            IN     element      - Pointer to L3QE element, which might be  */
/*                                  the root element.                        */
/*            IN     offset       - Offset of L3QE element within structures */
/*                                  in this list.                            */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID *nbb_l3qe_prev_in_list(NBB_L3QE_ROOT *root,
                                NBB_L3QE *element,
                                NBB_BUF_SIZE offset
                                 )
{
  /***************************************************************************/
  /* Get the previous entry in the list.                                     */
  /***************************************************************************/
  NBB_VOID *prev_entry;

  NBB_TRC_ENTRY("nbb_l3qe_prev_in_list");

  NBB_TRC_DETAIL(("Offset is %lu", offset));

  NBB_TRC_DETAIL(("Root is %p; previous element is %p",
                  &(root->l3qe),
                  element->prev));

  if (&(root->l3qe) != element->prev)
  {
    NBB_TRC_FLOW(("Previous element is not root"));
    prev_entry = (NBB_VOID *)(((NBB_BYTE *)element->prev) - offset);
  }
  else
  {
    NBB_TRC_FLOW(("Previous element is root"));
    prev_entry = NULL;
  }

  NBB_TRC_DETAIL(("Returning %p", prev_entry));
  NBB_TRC_EXIT();

  return(prev_entry);

} /* nbb_l3qe_prev_in_list */