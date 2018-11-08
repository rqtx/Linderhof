#include "common/common.h"

void defaultHandler();

typedef struct {
    void (*func)(int); 
}Error;

Error err = {
    .func = defaultHandler
};

static void printerror( char *p_msg )
{
    if( errno != 0 )
    {
        perror(p_msg);
    }
    else
    {
        puts( p_msg );
    }
}

void defaultHandler( int p_errorCode )
{
    memoryclean();
    exit(p_errorCode);
}

void Efatal( int p_errorCode, char * p_error )
{
    printerror(p_error);
    err.func(p_errorCode);
}

int Elog(int p_code, char *p_msg)
{
    if( NULL == p_msg)
    {
        return p_code;
    }

    printerror( p_msg );

    return p_code;
}

void ESetErroAction( void p_func( int ) )
{
    err.func = p_func;
}
