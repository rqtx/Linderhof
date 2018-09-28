#include <argp.h>
#include <argz.h> 

#include "venus.h"
#include "commander/linderhof.h"
#include "interface/configr.h"

#define MAXIPBUFFER 500
#define FILENAME "linderhof.config"
#define SERVERIP "127.0.0.1"
#define DELIM "."

ConfigOpt *conf = NULL;
char * serverIP = NULL;

static void createConfigFile( ) 
{
  char * defaultConfig = "serverIP %s\nport %d";
  FILE *fp;

  fp = fopen( FILENAME, "w");

  fprintf( fp, defaultConfig, SERVERIP, DEFAULT_COMPORT );
  fclose( fp );
}

static ConfigOpt * readConfigFile( FILE *p_file )
{
    FILE *fp = p_file;
    char *arg, *value;
    ConfigOpt *config; 
    memalloc( &arg, sizeof(char *) );
    memalloc( &value, sizeof(char *) );
    memalloc(&config, sizeof(ConfigOpt));

    while( !feof(fp) )
    {
        fscanf (fp, "%s %s", arg, value);

        if( !strcmp("serverIP", arg))
        {
            memalloc(&config->serverIP, strlen(value));
            memcpy(config->serverIP, value, strlen(value));
        }
        else if( !strcmp("port", arg) )
        { 
            config->port = atoi( value );
        } 
        else{
            char error[200];
            sprintf(error, "Error in config file, unknow argument %s", arg);
            Efatal(ERROR_FILE, error);
        }
    }

    return config;
}

static ConfigOpt * getConfig()
{
    FILE *fp;
    ConfigOpt *rtn = NULL;

    memalloc(&rtn, sizeof(ConfigOpt));
    fp = fopen( FILENAME, "r");

    if( NULL == fp )
    {
        char msg[100];
        createConfigFile();
        sprintf(msg, "File %s does not exist. We created one for you, please configure it!\n", FILENAME);
        Efatal(ERROR_FILE, msg);
    }

    rtn = readConfigFile( fp ); 

    fclose(fp);
    return rtn;
}

void SetServerIP( char * p_ip )
{
    serverIP = p_ip;
}

char * GetServerIP()
{
   return serverIP; 
}

int GetServerPort()
{
    return DEFAULT_COMPORT;
}

char * AskServerIP( )
{
    char *buffer = NULL;
    memalloc(&buffer, sizeof(MAXIPBUFFER));
    fputs("\nServer IP: ", stdout);
    fgets(buffer, MAXIPBUFFER, stdin);
    return buffer;
}
