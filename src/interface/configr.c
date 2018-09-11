#include <argp.h>
#include <argz.h> 

#include "venus.h"
#include "commander/linderhof.h"
#include "interface/configr.h"

#define FILENAME "linderhof.config"
#define FILEPATH "../../%s"
#define SERVERIP "192.168.0.141"
#define PORT 0
#define INITIALTHROUGHPUT 1.0
#define INCREMENTTHROUGHPUT 1
#define TIMEFREQUENCY 1
#define TIMER 120
#define DELIM "."

const char *argp_program_bug_address = "rqtx@protonmail.com";
const char *argp_program_version = "version 0.1"; 
ConfigOpt *conf = NULL;

struct argp_option options[] =
{
    { "target", 't', "target_ipv4", 0, "Attack target IPV4"},
    { "amplifier", 'a', "amp_ipv4", 0, "Memcached amplifier IPV4"},
    { 0 }
};


static int parse_opt (int key, char *arg, struct argp_state *state)
{ 
  LhfDraft * draft = state->input;
  
  switch (key)
  {
    case ARGP_KEY_ARG:
      if( !strcmp(arg, "test") || !strcmp(arg, "Test") || !strcmp(arg, "TEST") ){
        draft->type = TEST;
      }
      else if( !strcmp(arg, "memcached") || !strcmp(arg, "Memcached") || !strcmp(arg, "MEMCACHED") ){
        draft->type = MEMCACHED;
      }
      else if( !strcmp(arg, "ssdp") || !strcmp(arg, "SSDP") ){
        draft->type = SSDP;
      }
      else{
        argp_failure (state, 1, 0, "Unknown KEY_ARG"); 
      }
      break;

    case 't':
      if( !is_valid_ipv4(arg) )
        argp_failure (state, 1, 0, "Invalid IPv4");

      memcpy(draft->target_ip, arg, strlen(arg));   
      break;

    case 'a':
      if( !is_valid_ipv4(arg) )
        argp_failure (state, 1, 0, "Invalid IPv4");
     
      memcpy(draft->amp_ip, arg, strlen(arg));
      break;
  }
  return 0;
}

static void createConfigFile( ) 
{
  char * defaultConfig = "serverIP %s\nport %d\ninitialThroughput %.1f\nincrementThroughput %.1f\ntimeFrequency %d\ntimer %d";
  char fpPath[500];
  FILE *fp;

  sprintf(fpPath, FILEPATH, FILENAME);
  fp = fopen( fpPath, "w");

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

LhfDraft * CreateDraft(int argc, char ** argv)
{
    struct argp argp = { options, parse_opt, "ATK" };
    LhfDraft *draft = NULL;
  
    if( NULL == conf)
    {
        conf = getConfig();
    }
    
    memalloc(&draft, sizeof(LhfDraft));
    argp_parse(&argp, argc, argv, 0, 0, draft);
    draft->amp_port = conf->port;
    draft->target_port = conf->port;
    draft->initialThroughput = conf->initialThp;
    draft->incrementThroughput = conf->incThp;
    draft->timeFrequency = conf->timeFreq;
    draft->timer = conf->timer;

    return draft;
} 


