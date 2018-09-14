#include <argp.h>
#include <argz.h> 

#include "venus.h"
#include "commander/linderhof.h"
#include "interface/configr.h"

#define FILENAME "../../linderhof.config"
#define SERVERIP "192.168.0.141"
#define PORT 0
#define INITIALTHROUGHPUT 1.0
#define INCREMENTTHROUGHPUT 1
#define TIMEFREQUENCY 1
#define TIMER 120
#define DELIM "."

ConfigOpt *conf = NULL;

static void createConfigFile( ) 
{
  char * defaultConfig = "serverIP %s\nport %d\ninitialThroughput %.1f\nincrementThroughput %.1f\ntimeFrequency %d\ntimer %d";
  FILE *fp;

  fp = fopen( FILENAME, "w");

  fprintf( fp, defaultConfig, SERVERIP, PORT, INITIALTHROUGHPUT, INCREMENTTHROUGHPUT, TIMEFREQUENCY, TIMER);
  fclose( fp );
}

static ConfigOpt * readConfigFile( FILE *p_file )
{
  char *arg, *value;
  ConfigOpt *config; 
  memalloc( &arg, sizeof(char *) );
  memalloc( &value, sizeof(char *) );
  memalloc(&config, sizeof(ConfigOpt));

  while( !feof(p_file) )
  {
    fscanf (p_file, "%s %s", arg, value);

    if( !strcmp("serverIP", arg))
    {
      memalloc(&config->serverIP, strlen(value));
      memcpy(config->serverIP, value, strlen(value));
    }
    else if( !strcmp("port", arg) )
    { 
      config->port = atoi( value );
    }
    else if( !strcmp("initialThroughput", arg) )
    {
      config->initialThp = atof( value );
    }
    else if( !strcmp("incrementThroughput", arg) )
    { 
      config->incThp = atof( value );; 
    }
    else if( !strcmp("timeFrequency", arg) )
    {
      config->timeFreq = atoi( value );
    }
    else if( !strcmp("timer", arg) )
    {
      config->timer = atoi( value );
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

char * GetServerIP()
{
    if( NULL == conf)
    {
        conf = getConfig();
    }
    return conf->serverIP;
}

int GetServerPort()
{
    return DEFAULT_COMPORT;
}

void SetDraftConfig( LhfDraft *p_draft )
{
    if( NULL == conf)
    {
        conf = getConfig();
    }
    
    p_draft->amp_port = conf->port;
    p_draft->target_port = conf->port;
    p_draft->initialThroughput = conf->initialThp;
    p_draft->incrementThroughput = conf->incThp;
    p_draft->timeFrequency = conf->timeFreq;
    p_draft->timer = conf->timer;
} 


