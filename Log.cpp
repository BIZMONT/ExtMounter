#include <stdio.h>
#include <stdarg.h>

static FILE *logfile = 0;
static char buff[4096];

int log_init()
{
    logfile = fopen("ExtMounter.log", "w+");
    if(!logfile)
        return -1;
    return 0;
}

int log(const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    vsnprintf(buff, 4096, msg, ap);
    va_end(ap);

    fprintf(logfile, "%s", buff);
    fflush(logfile);
    return 0;
}

void log_exit()
{
    fclose(logfile);
}
