#include "os.h"
#include "list_manage.h"
#include <wtypes.h>


	const char *LOG_PATH = "trace.log";
	FILE *log_file = NULL;
	char TraceBuf[2048]={0};

int TraceInit()
{
	if((log_file= fopen(LOG_PATH,"a+")) == NULL)
	err_proc("open trace file error");
}

void Trace(const char *pcFormat, ...)          /*lint +e1916*/
{
    OS_VA_LIST         ArgList;
	int buf_len;
    
    (void)OS_VA_START(ArgList, pcFormat);

    buf_len=OS_VSNPRINTF(TraceBuf, pcFormat, ArgList);
    OS_VA_END(ArgList); 
	fprintf(log_file,"%s",TraceBuf);
	fflush(log_file);
    return;
} 
