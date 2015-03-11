#ifndef _OS_INCLUDE_H_
#define _OS_INCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>


/**INC+***********************************************************************/
/* Header:    nbbos.h                                                        */
/*                                                                           */
/* Purpose:   Macros for C runtimes.  These allow customers to easily port   */
/*            to private versions of the C runtimes.                         */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Common Types                                                              */
/*****************************************************************************/
/*****************************************************************************/
typedef unsigned char    NBB_BYTE;     /* 8 bits                             */
typedef          char    NBB_CHAR;     /*                                    */
typedef          short   NBB_SHORT;    /* 2 byte signed integer              */
typedef unsigned short   NBB_USHORT;   /* 2 byte unsigned integer            */
typedef          long    NBB_LONG;     /* 4 byte signed integer              */
typedef unsigned long    NBB_ULONG;    /* 4 byte unsigned integer            */
typedef          int     NBB_INT;      /* >= 2 byte signed integer (machine  */
                                       /* word)                              */
typedef unsigned int     NBB_UINT;     /* >= 2 byte unsigned integer         */
                                       /* machine word)                      */
typedef          int     NBB_BOOL;     /* boolean                            */
typedef          void    NBB_VOID;     /* void if supported, char otherwise  */

#define NBB_CONST const

/*****************************************************************************/
/* See OS specific header file for NBB_COUNTER                               */
/*****************************************************************************/

/*****************************************************************************/
/*****************************************************************************/
/* Derived types                                                             */
/*****************************************************************************/
/*****************************************************************************/
typedef NBB_ULONG       NBB_BUF_SIZE;           /* size of memory, buffers   */
                                                /* etc.                      */
typedef NBB_ULONG       NBB_QUEUE_ID;           /* queue identifier          */
typedef NBB_ULONG       NBB_HANDLE;
typedef NBB_HANDLE      NBB_PROC_ID;            /* process identifier        */
typedef NBB_VOID *      NBB_LOCK_ID;
typedef NBB_LOCK_ID     NBB_LOCK_DATA;
typedef NBB_LONG        NBB_PROC_TYPE;          /* process type              */
typedef NBB_BYTE        NBB_PROC_PRIORITY;      /* process priority          */



/*****************************************************************************/
/* FILE structure                                                            */
/*****************************************************************************/
#define OS_FILE      FILE

/*****************************************************************************/
/* File IO functions                                                         */
/*****************************************************************************/
#define OS_FREAD(DEST, SIZE, NUM, FILE)    fread((DEST), (SIZE), (NUM), (FILE))
#define OS_FEOF(FILE)                      feof(FILE)
#define OS_FCLOSE(FILE)                    fclose(FILE)
#define OS_STDIN                           stdin
#define OS_STDOUT                          stdout
#ifdef VXWORKS_NBASE
/*****************************************************************************/
/* VxWorks file access is routed through the VxWorks FTP file system, and so */
/* we intercept all open and remove calls.                                   */
/*                                                                           */
/* Note that VxWorks doesnt support file renaming over FTP                   */
/*****************************************************************************/
#define OS_FOPEN(FILE, MODE)                nbb_fopen((FILE), (MODE))
#define OS_REMOVE(FILE)                     nbb_remove(FILE)
#define OS_SETVBUF(STREAM, BUF, TYPE, SIZE)
#else
#define OS_FOPEN(FILE, MODE)                fopen((FILE), (MODE))
#define OS_REMOVE(FILE)                     remove(FILE)
#define OS_RENAME(OLDNAME, NEWNAME)         rename((OLDNAME), (NEWNAME))
#define OS_SETVBUF(STREAM, BUF, TYPE, SIZE)                                  \
                                       setvbuf((STREAM), (BUF), (TYPE), (SIZE))
#endif
#define OS_FGETS(DEST, LENGTH, FILE)        fgets((DEST), (LENGTH), (FILE))
#define OS_FFLUSH(FILE)                     fflush(FILE)
#define OS_FWRITE(DT, B, LEN, FILE)         fwrite(DT, B, LEN, FILE)
#define OS_FSEEK(FILE, NUM, QUAL)           fseek(FILE, NUM, QUAL)
#define OS_FTELL(FILE)                      ftell(FILE)

#define OS_PRINTF    printf
#define OS_SPRINTF   sprintf
#define OS_VSNPRINTF   vsprintf
#define OS_FPRINTF   fprintf
#define OS_SSCANF    sscanf

#define OS_GETCHAR   getchar
#define OS_GETS      gets

/*****************************************************************************/
/* String functions                                                          */
/*                                                                           */
/* As these are never used in product code, it is reasonably safe to put     */
/* casts in here.                                                            */
/*                                                                           */
/* Note:      On all platforms where it is available, we make use of the     */
/*            strtok_r() function to provide defence against non-N-BASE      */
/*            threads calling strtok() at the same time as the N-BASE.       */
/*****************************************************************************/
#define OS_STRLEN(STR)                 strlen((STR))
#define OS_STRCMP(STR1, STR2)          strcmp((STR1), (STR2))
#define OS_STRNCMP(STR1, STR2, LEN)    strncmp((STR1), (STR2), (size_t)(LEN))
#ifdef WIN32
#define OS_STRICMP(STR1, STR2)         stricmp((STR1), (STR2))
#else
#define OS_STRICMP(STR1, STR2)         strcasecmp((STR1), (STR2))
#endif
#define OS_STRCAT(STR1, STR2)          strcat((STR1), (STR2))
#define OS_STRNCAT(STR1, STR2, LEN)    strncat((STR1), (STR2), (size_t)(LEN))
#define OS_STRSTR(STR1, STR2)          strstr((STR1), (STR2))
#define OS_STRCPY(DEST, SRC)           strcpy((char *)(DEST), \
                                              (const char *)(SRC))
#define OS_STRNCPY(STR1, STR2, LEN)    strncpy((STR1), (STR2), (size_t)(LEN))
#define OS_STRCHR(STR, CHR)            strchr((STR), (CHR))
#define OS_STRRCHR(STR, CHR)           strrchr((STR), (CHR))

#if (defined(UNIX_NBASE) || defined(VXWORKS_NBASE) || defined(OSE_NBASE))
#define OS_STRTOK(STR, TOK, LAST_PTR)  (((LAST_PTR) == NULL) ?                \
                                                         strtok_r((STR),      \
                                                         (TOK),               \
                                                         (&(TGD->token_ptr))) \
                                                         :                    \
                                                         strtok_r((STR),      \
                                                         (TOK),               \
                                                         (LAST_PTR)))
#else
#define OS_STRTOK(STR, TOK, LAST_PTR)  strtok((STR), (TOK))
#endif

#define OS_STRCSPN(STR, CHR)           strcspn((STR), (CHR))

#define OS_ATOI(STR)                   atoi(STR)
#define OS_TOLOWER(CH)                 tolower(CH)
#define OS_TOUPPER(CH)                 toupper(CH)
#define OS_STRTOL(STR, ENDP, BASE)     strtol(STR, ENDP, BASE)
#define OS_STRTOUL(STR, ENDP, BASE)    strtoul(STR, ENDP, BASE)

#define OS_ISALPHA(CH)                 isalpha(CH)
#define OS_ISDIGIT(CH)                 isdigit(CH)
#define OS_ISPRINT(CH)                 isprint(CH)
#define OS_ISUPPER(CH)                 isupper(CH)
#define OS_ISSPACE(CH)                 isspace(CH)

/*****************************************************************************/
/* These are necessary in addition to the NBB_MEMSET                         */
/*****************************************************************************/
#define OS_MEMSET(MEM, VALUE, LENGTH) \
                               memset((char *)(MEM), (VALUE), (size_t)(LENGTH))
#define OS_MEMCMP(MEM1, MEM2, LENGTH) memcmp((MEM1), (MEM2), (size_t)(LENGTH))
#define OS_MEMCHR(MEM1, VALUE, LENGTH)memchr((MEM1), (VALUE), (size_t)(LENGTH))
#define OS_MEMMOVE(DEST, SRC, LENGTH) memmove((DEST), (SRC), (size_t)(LENGTH))
#define OS_MEMCPY(DEST, SRC, LENGTH)  \
                  memcpy((char *)(DEST), (const char *)(SRC), (size_t)(LENGTH))

/*****************************************************************************/
/* rand routines                                                             */
/*                                                                           */
/* Note:      On all platforms where it is available, we make use of the     */
/*            rand_r() function to provide defence against non-N-BASE        */
/*            threads calling rand() at the same time as the N-BASE.         */
/*****************************************************************************/
#if (defined(UNIX_NBASE) || defined(OSE_NBASE))
#define OS_RAND()          rand_r(&(TGD->seed))
#else
#define OS_RAND()          rand()
#endif
#define OS_SRAND(SEED)     srand(SEED)

/*****************************************************************************/
/* OS specific memory routines                                               */
/*****************************************************************************/
#define OS_INIT()

#define OS_ALLOC(MEMORY_SIZE, MEMORY_TYPE) malloc((size_t)(MEMORY_SIZE))
#define OS_FREE(MEMORY, MEMORY_SIZE)       free(MEMORY)


/*****************************************************************************/
/* Time routines                                                             */
/*                                                                           */
/* Note:      On all platforms where it is available, we make use of the     */
/*            localtime_r() function to provide defence against non-N-BASE   */
/*            threads calling localtime() at the same time as the N-BASE.    */
/*****************************************************************************/
#define OS_TIME(TIME_STR)            time(TIME_STR)
#define OS_DIFFTIME(TIME2, TIME1)    difftime(TIME2, TIME1)

#if (defined(UNIX_NBASE) || defined(VXWORKS_NBASE) || defined(OSE_NBASE))
#define OS_LOCALTIME(LCL_TIME_STR, LAST_PTR)   (((LAST_PTR) == NULL) ?        \
                                                  localtime_r((LCL_TIME_STR), \
                                                       (&(TGD->time_struct))) \
                                                  :                           \
                                                  localtime_r((LCL_TIME_STR), \
                                                                  (LAST_PTR)))
#else
#define OS_LOCALTIME(LCL_TIME_STR, LAST_PTR)   localtime((LCL_TIME_STR))
#endif

/*****************************************************************************/
/* General                                                                   */
/*****************************************************************************/
#if (defined(NBB_RELEASE_ASSERT) && defined(WIN32))
#ifndef _CRTIMP
#ifdef  _DLL
#define _CRTIMP __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif
extern _CRTIMP void __cdecl _assert(void *, void *, unsigned);
#define OS_ASSERT(X)        (void)((X) || (_assert(#X, __FILE__, __LINE__), 0))
#else
#define OS_ASSERT(X)                 assert(X)
#endif

#define OS_EXIT(X)                   exit(X)

/*****************************************************************************/
/* Variable argument list functions.                                         */
/*                                                                           */
/* Note that types smaller than int are promoted to int size when put on the */
/* stack by a function call.  The implementation of OS_VA_ARG below handles  */
/* the fact that some implementations of va_arg() do not work correctly      */
/* (they get the wrong bytes from the stack for types smaller than int).     */
/*****************************************************************************/
#define OS_VA_LIST                   va_list

#define OS_VA_START(ARGLIST, LAST_NAMED_PARM) \
                                     va_start((ARGLIST), (LAST_NAMED_PARM))

#define OS_VA_ARG(ARGLIST, TYPE)     (TYPE)((sizeof(TYPE) < sizeof(int)) ? \
                                     (TYPE)va_arg((ARGLIST), int):     \
                                     (TYPE)va_arg((ARGLIST), TYPE))

#define OS_VA_END(ARGLIST)           va_end(ARGLIST)

/*****************************************************************************/
/* Thread functions.  These are implemented in the SPARC MT N-BASE only.     */
/*****************************************************************************/
#if defined(SPARC_MT_NBASE) || defined(LINUX_MT_NBASE)
#define OS_GET_PROCESS_ID()          ((NBB_ULONG)(getpid()))
#define OS_GET_THREAD_ID()           ((NBB_ULONG)(pthread_self()))
#else
#define OS_GET_PROCESS_ID()          ((NBB_ULONG)0)
#define OS_GET_THREAD_ID()           ((NBB_ULONG)0)
#endif

/*****************************************************************************/
/* Macro definitions for OS specific functions.                              */
/*****************************************************************************/
#define OS_GET_DATE_TIME(DT)            nbb_os_get_date_time((DT))
#define OS_GET_OS_PROCESS_INFO(PROC_INFO)                                     \
                                        nbb_os_get_os_process_info((PROC_INFO))

#define NBB_ASSERT assert

/*****************************************************************************/
/* Find the offset of specified field within specified structure             */
/*****************************************************************************/
#define NBB_OFFSETOF(STRUCT, FIELD)                                           \
	(NBB_BUF_SIZE)((NBB_BYTE *)(&((STRUCT *)0)->FIELD) - (NBB_BYTE *)0)

#if 0
#define NBB_TRC_ENTRY(FUNC_NAME)	printf("func:%s	file:%s	line:%d\n",FUNC_NAME,__FILE__,__LINE__)
#define NBB_TRC_FLOW(args) printf("file:%s	line:%d ",__FILE__,__LINE__);\
							printf args
#define NBB_TRC_DETAIL(args) printf("file:%s	line:%d ",__FILE__,__LINE__);\
							printf args
#define NBB_TRC_EXIT()	printf("file:%s	line:%d\n",__FILE__,__LINE__)
#endif
#if 0
#define NBB_TRACE_LOC(LEVEL, FILE, LINE, PARAMS)                              \
  ((nbb_trace_level <= LEVEL) && (nbb_include_trace(LEVEL, FILE)) ?           \
   (                                                                          \
    NBB_SPRINTF PARAMS,                                                       \
    nbb_trace_out(FILE, LINE  NBB_CCXT), 0                                    \
   ) : 0)

#define NBB_TRACE(LEVEL, PARAMS)                                              \
  NBB_TRACE_LOC(LEVEL, __FILE__, __LINE__, PARAMS)


#define NBB_TRC_FLOW(PARAMS)          NBB_TRACE(NBB_FLOW_TRC, PARAMS)

#endif

#define NBB_MAX(a,b) (((a)>(b))?(a):(b))

#define NBB_EXPORT

#define NBB_CXT
#define NBB_CCXT
#define NBB_CCXT_T

#define err_proc printf
#include "trace.h"

#endif
