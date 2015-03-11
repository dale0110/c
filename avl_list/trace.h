#ifndef _TRACE_H_
#define _TRACE_H_

#include "os.h"
#include <wtypes.h>

#define TRACE_MODULE

#ifdef TRACE_MODULE
#define NBB_TRC_ENTRY(FUNC_NAME)
/*#define NBB_TRC_FLOW(args) Trace args*/
#define NBB_TRC_FLOW(args) 
#define NBB_TRC_DETAIL(args)
#define NBB_TRC_EXIT()
#else
#define NBB_TRC_ENTRY(FUNC_NAME)	printf("func:%s	file:%s	line:%d\n",FUNC_NAME,__FILE__,__LINE__)
#define NBB_TRC_FLOW(args) fprintf("file:%s	line:%d ",__FILE__,__LINE__);\
							printf args
#define NBB_TRC_DETAIL(args) printf("file:%s	line:%d ",__FILE__,__LINE__);\
							printf args
#define NBB_TRC_EXIT()	printf("file:%s	line:%d\n",__FILE__,__LINE__)

#endif
#endif
