#include "common/common.h"
#include "common/cliparser.h"

#define MAXSTR 50

#define PARSER_DELIM "=\n"
#define PARSER_INITIALIZE(str) strtok(str, PARSER_DELIM)
#define PARSER_CONTINUE strtok(NULL, PARSER_DELIM)

static ArgsOpt* getOptbyShort( ArgsOpt *p_opt, char p_arg )
{
    ArgsOpt *opt = p_opt;

    for( int i = 0; opt->args != 0; i++, opt = &p_opt[i] )
    {
        if( opt->args == p_arg )
        {
            return opt;
        }
    }
    return NULL;
}

static ArgsOpt* getOptbyLong( ArgsOpt *p_opt, char *p_arg )
{
    ArgsOpt *opt = p_opt;

    for( int i = 0; opt->args != 0; i++, opt = &p_opt[i] )
    {
        if( !strcmp(opt->argl, p_arg) )
        {
            return opt;
        }
    }
    return NULL;
}

int ParserCLI( ArgsCore *p_core, int p_argc, char ** p_argv, void *p_data)
{
    char key;
    ArgState state;
    ArgsOpt *opt;

    state.input = p_data;

    for( int i = 0; i < p_argc; i++ )
    {
        char *value = p_argv[i];
        char *arg = NULL;

        if(value[0] == '-')
        {
            if(value[1] == '-')
            {
                char *fsp = &value[2];

                char *keyLong = PARSER_INITIALIZE(fsp);
                opt = getOptbyLong(p_core->opt, keyLong);

                if( NULL == opt )
                {
                    ELOG(ERROR_CLI, "Invalid input");
                }
                key = opt->args;

                if( opt->input )
                {
                    arg = PARSER_CONTINUE;

                    if( NULL == arg)
                    {
                        ELOG(ERROR_CLI, "Invalid input");
                    }
                }
            }
            else
            {
                char keyCand = value[1];
                opt = getOptbyShort(p_core->opt, keyCand);
                
                if( NULL == opt )
                {
                    ELOG(ERROR_CLI, "Invalid input");
                }
                key = opt->args;

                if( opt->input )
                {
                    if( i+1 == p_argc )
                    {
                        ELOG(ERROR_CLI, "Invalid input");
                    }
                    
                    i++;
                    arg = p_argv[i];

                    if( arg[0] == '-')
                    {
                        ELOG(ERROR_CLI, "Invalid input");
                    }
                    
                }
            }
        }
        else
        {
            key = ARG_CMD;
            arg = value;
        }

        p_core->handler(key, arg, &state);
    }

    return SUCCESS;
}
