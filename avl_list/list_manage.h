#ifndef _LIST_M_INCLUDE_
#define _LIST_M_INCLUDE_

#include "list_struct.h"

/*****************************************************************************/
/* Queue Management                                                          */
/*****************************************************************************/
#define NBB_INIT_Q_ROOT(R)                                                 \
      (R).first = NULL;                                                    \
      (R).last = NULL

#define NBB_ADD_TO_Q_HEAD(QR, QE)                                          \
      ((NBB_QE *)(QE))->next = (QR).first;                                 \
      (QR).first = (NBB_QE *)(QE);                                         \
      if ((QR).last == NULL)                                               \
      {                                                                    \
        (QR).last = (NBB_QE *)(QE);                                        \
      }

#define NBB_ADD_TO_Q_TAIL(QR, QE)                                          \
      ((NBB_QE *)(QE))->next = NULL;                                       \
      if ((QR).last != NULL)                                               \
      {                                                                    \
        (QR).last->next = (NBB_QE *)(QE);                                  \
      }                                                                    \
      else                                                                 \
      {                                                                    \
        (QR).first = (NBB_QE *)(QE);                                       \
      }                                                                    \
      (QR).last = (NBB_QE *)(QE)

#define NBB_REMOVE_NEXT_FROM_Q(QR, QE)                                     \
      NBB_ASSERT(((NBB_QE *)(QE))->next != NULL);                          \
      if ((QR).last == ((NBB_QE *)(QE))->next)                             \
      {                                                                    \
        (QR).last = (NBB_QE *)(QE);                                        \
      }                                                                    \
      ((NBB_QE *)(QE))->next = ((NBB_QE *)(QE))->next->next

#define NBB_GET_Q_HEAD(QR)      nbb_get_q_head(&QR  NBB_CCXT)
#define NBB_Q_HEAD(QR)          ((NBB_VOID *)(QR).first)
#define NBB_Q_NEXT(QE)          ((NBB_VOID *)((NBB_QE *)(QE))->next)

/*****************************************************************************/
/* List Management                                                           */
/*****************************************************************************/
#define NBB_INIT_ROOT(R)        (R).self = NULL;               \
                                (R).next = &(R);               \
                                (R).prev = &(R)

#define NBB_INIT_LQE(E, S)      (E).self = (S);                \
                                (E).next = NULL;               \
                                (E).prev = NULL

#define NBB_LIST_ROOT(R)        ((R).self == NULL)
#define NBB_EMPTY_LIST(R)       ((R).next == &(R))
#define NBB_IN_LIST(E)          ((E).next != NULL)
#define NBB_NEXT_IN_LIST(E)     (NBB_VOID *)((E).next->self)
#define NBB_PREV_IN_LIST(E)     (NBB_VOID *)((E).prev->self)

#define NBB_INSERT_AFTER(P, E)  NBB_ASSERT((E).next == NULL);  \
                                NBB_ASSERT((E).prev == NULL);  \
                                NBB_ASSERT((P).prev != NULL);  \
                                NBB_ASSERT((P).next != NULL);  \
                                (E).next = (P).next;           \
                                (E).prev = &(P);               \
                                (E).next->prev = &(E);         \
                                (E).prev->next = &(E)

#define NBB_INSERT_BEFORE(N, E) NBB_ASSERT((E).next == NULL);  \
                                NBB_ASSERT((E).prev == NULL);  \
                                NBB_ASSERT((N).prev != NULL);  \
                                NBB_ASSERT((N).next != NULL);  \
                                (E).prev = (N).prev;           \
                                (E).next = &(N);               \
                                (E).next->prev = &(E);         \
                                (E).prev->next = &(E)

#define NBB_INSERT_LIST_AFTER(P, R)                             \
                                NBB_ASSERT(NBB_LIST_ROOT(R));   \
                                if (!NBB_EMPTY_LIST(R))         \
                                {                               \
                                  NBB_ASSERT((P).prev != NULL); \
                                  NBB_ASSERT((P).next != NULL); \
                                  (P).next->prev = (R).prev;    \
                                  (R).prev->next = (P).next;    \
                                  (P).next = (R).next;          \
                                  (R).next->prev = &(P);        \
                                }                               \
                                NBB_INIT_ROOT(R)

#define NBB_INSERT_LIST_BEFORE(N, R)                            \
                                NBB_ASSERT(NBB_LIST_ROOT(R));   \
                                if (!NBB_EMPTY_LIST(R))         \
                                {                               \
                                  NBB_ASSERT((N).prev != NULL); \
                                  NBB_ASSERT((N).next != NULL); \
                                  (N).prev->next = (R).next;    \
                                  (R).next->prev = (N).prev;    \
                                  (N).prev = (R).prev;          \
                                  (R).prev->next = &(N);        \
                                }                               \
                                NBB_INIT_ROOT(R)

#define NBB_REMOVE_FROM_LIST(E) NBB_ASSERT((E).prev != NULL);  \
                                NBB_ASSERT((E).next != NULL);  \
                                (E).next->prev = (E).prev;     \
                                (E).prev->next = (E).next;     \
                                (E).next = NULL;               \
                                (E).prev = NULL

/*****************************************************************************/
/* L3QE List Management.                                                     */
/*                                                                           */
/* L3QE lists provide equivalent functionality to LLQE lists, with a lower   */
/* occupancy for the list root, but require the caller to supply an          */
/* additional parameter to some of the macros.                               */
/*                                                                           */
/* LLQE structures and macros are deprecated, but are retained for           */
/* back-compatibility.                                                       */
/*****************************************************************************/
#ifdef NBB_DEBUG
#define NBB_L3QE_INIT_ROOT(R, L3QE_OFFSET)                                    \
              (R).l3qe.next = &((R).l3qe);                                    \
              (R).l3qe.prev = &((R).l3qe);                                    \
              NBB_TRC_DETAIL(("Init L3QE root %p with offset %lu", \
                              (&(R)),                                         \
                              (L3QE_OFFSET)))
#else
#define NBB_L3QE_INIT_ROOT(R, L3QE_OFFSET)                                    \
              (R).l3qe.next = (NBB_L3QE *)&(R);                               \
              (R).l3qe.prev = (NBB_L3QE *)&(R)
#endif

#define NBB_L3QE_INIT_LQE(E)     (E).next = NULL;                             \
                                 (E).prev = NULL

#define NBB_L3QE_EMPTY_LIST(R)   (((NBB_L3QE *)&(R))->next == (NBB_L3QE *)&(R))

#define NBB_L3QE_IN_LIST(E)      (((NBB_L3QE *)&(E))->next != NULL)

/*****************************************************************************/
/* Note that if the next / prev element is the root, this macro returns      */
/* NULL.  Otherwise it returns a pointer to the next / prev NBB_L3QE         */
/* element.                                                                  */
/*****************************************************************************/
#define NBB_L3QE_NEXT_IN_LIST(R, E, L3QE_OFFSET)                              \
       nbb_l3qe_next_in_list(&(R), (NBB_L3QE *)(&(E)), (L3QE_OFFSET)  NBB_CCXT)

#define NBB_L3QE_PREV_IN_LIST(R, E, L3QE_OFFSET)                              \
       nbb_l3qe_prev_in_list(&(R), (NBB_L3QE *)(&(E)), (L3QE_OFFSET)  NBB_CCXT)

/*****************************************************************************/
/* These macros have to cope with the fact that P or N may be the root       */
/* element, in which case they must be cast to an NBB_L3QE element to access */
/* the next and prev fields.                                                 */
/*****************************************************************************/
#define NBB_L3QE_INSERT_AFTER(P, E)                                           \
                              NBB_ASSERT((E).next == NULL);                   \
                              NBB_ASSERT((E).prev == NULL);                   \
                              NBB_ASSERT(((NBB_L3QE *)(&(P)))->prev != NULL); \
                              NBB_ASSERT(((NBB_L3QE *)(&(P)))->next != NULL); \
                              (E).next = ((NBB_L3QE *)(&(P)))->next;          \
                              (E).prev = (NBB_L3QE *)(&(P));                  \
                              (E).next->prev = &(E);                          \
                              (E).prev->next = &(E)

#define NBB_L3QE_INSERT_BEFORE(N, E)                                          \
                              NBB_ASSERT((E).next == NULL);                   \
                              NBB_ASSERT((E).prev == NULL);                   \
                              NBB_ASSERT(((NBB_L3QE *)(&(N)))->prev != NULL); \
                              NBB_ASSERT(((NBB_L3QE *)(&(N)))->next != NULL); \
                              (E).prev = ((NBB_L3QE *)(&(N)))->prev;          \
                              (E).next = (NBB_L3QE *)(&(N));                  \
                              (E).next->prev = &(E);                          \
                              (E).prev->next = &(E)

#define NBB_L3QE_INSERT_LIST_AFTER(P, R)                                      \
                          if (!NBB_L3QE_EMPTY_LIST(R))                        \
                          {                                                   \
                            NBB_ASSERT(((NBB_L3QE *)(&(P)))->next != NULL);   \
                            NBB_ASSERT(((NBB_L3QE *)(&(P)))->prev != NULL);   \
                            ((NBB_L3QE *)(&(P)))->next->prev = (R).l3qe.prev; \
                            (R).l3qe.prev->next = ((NBB_L3QE *)(&(P)))->next; \
                            ((NBB_L3QE *)(&(P)))->next = (R).l3qe.next;       \
                            (R).l3qe.next->prev = ((NBB_L3QE *)(&(P)));       \
                            (R).l3qe.next = &((R).l3qe);                      \
                            (R).l3qe.prev = &((R).l3qe);                      \
                          }

#define NBB_L3QE_INSERT_LIST_BEFORE(N, R)                                     \
                          if (!NBB_L3QE_EMPTY_LIST(R))                        \
                          {                                                   \
                            NBB_ASSERT(((NBB_L3QE *)(&(N)))->next != NULL);   \
                            NBB_ASSERT(((NBB_L3QE *)(&(N)))->prev != NULL);   \
                            ((NBB_L3QE *)(&(N)))->prev->next = (R).l3qe.next; \
                            (R).l3qe.next->prev = ((NBB_L3QE *)(&(N)))->prev; \
                            ((NBB_L3QE *)(&(N)))->prev = (R).l3qe.prev;       \
                            (R).l3qe.prev->next = ((NBB_L3QE *)(&(N)));       \
                            (R).l3qe.next = &((R).l3qe);                      \
                            (R).l3qe.prev = &((R).l3qe);                      \
                          }

#define NBB_L3QE_REMOVE_FROM_LIST(E)  NBB_REMOVE_FROM_LIST((E))

/*****************************************************************************/
/* Lite List Management (deprecated).                                        */
/*****************************************************************************/
#define NBB_L_INIT_ROOT(R, O)      NBB_L3QE_INIT_ROOT((R).llqe, (O));         \
                                   (R).llqe_offset = O

#define NBB_L_INIT_LQE(E)          NBB_L3QE_INIT_LQE((E))

#define NBB_L_EMPTY_LIST(R)        NBB_L3QE_EMPTY_LIST((R).llqe)

#define NBB_L_IN_LIST(E)           NBB_L3QE_IN_LIST((E))

#define NBB_L_NEXT_IN_LIST(R, E)   NBB_L3QE_NEXT_IN_LIST((R).llqe,            \
                                                         (E),                 \
                                                         (R).llqe_offset)

#define NBB_L_PREV_IN_LIST(R, E)   NBB_L3QE_PREV_IN_LIST((R).llqe,            \
                                                         (E),                 \
                                                         (R).llqe_offset)

#define NBB_L_INSERT_AFTER(P, E)   NBB_L3QE_INSERT_AFTER((P), (E))

#define NBB_L_INSERT_BEFORE(N, E)  NBB_L3QE_INSERT_BEFORE((N), (E))

#define NBB_L_INSERT_LIST_AFTER(P, R)                                         \
                                   NBB_L3QE_INSERT_LIST_AFTER((P), (R).llqe)

#define NBB_L_INSERT_LIST_BEFORE(N, R)                                        \
                                   NBB_L3QE_INSERT_LIST_BEFORE((N), (R).llqe)

#define NBB_L_REMOVE_FROM_LIST(E)  NBB_L3QE_REMOVE_FROM_LIST((E))

#endif