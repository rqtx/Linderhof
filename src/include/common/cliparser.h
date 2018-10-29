
/*
 * file:                                                              	    
 * purpose:                                               		    
 * Maintained by:  Igor Fernandes Miranda <igormiranda5@gmail.com>          
 */

#ifndef CLIPARSER_H
  #define CLIPARSER_H

#define ARG_CMD -1 

typedef struct{
    char args;            //Short argument
    char argl[10];          //Long argument
    bool input;             //Is input mandatory?
    bool bounden;           //Is it a mandatory arg?
    char help[100];         //Help message
}ArgsOpt;

typedef struct{
    void *input;
}ArgState;

typedef struct{
    ArgsOpt *opt;                                               //Command args
    int (*handler)(char p_key, char *p_arg, ArgState *state);    //Function handler
}ArgsCore;

int ParserCLI( ArgsCore *p_core, int p_argc, char ** p_argv, void *p_data);

#endif
