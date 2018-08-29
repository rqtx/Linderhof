#include "venus.h"

#define FILENAME "linderhof.config"

#define PORT 0
#define INITIALTHROUGHPUT 1
#define INCREMENTTHROUGHPUT 1
#define TIMEFREQUENCY 1
#define TIMER 120


static void createConfigFile( ) 
{
  char * defaultConfig = "port %d\ninitialThroughput %f\nincrementThroughput %f\ntimeFrequency %d\ntimer %d";
  FILE *fp = fopen( FILENAME, "w");

  fprintf( fp, defaultConfig, PORT, INITIALTHROUGHPUT, INCREMENTTHROUGHPUT, TIMEFREQUENCY, TIMER);
  fclose( fp );
}

static void readConfigFile( FILE *p_file, LhfDraft *p_draft)
{
  char *arg, *value;
  
  memalloc( &arg, sizeof(char) );
  memalloc( &value, sizeof(char) );

  while( !feof(p_file) )
  {
    fscanf (p_file, "%s %s", arg, value);

    if( !strcmp("port", arg) )
    {  
      int i = atoi( value );
      p_draft->target_port = i;
      p_draft->amp_port = i;
    }
    else if( !strcmp("initialThroughput", arg) )
    {
      float f = atof( value );
      p_draft->initialThroughput = f; 
    }
    else if( !strcmp("incrementThroughput", arg) )
    {
      float f = atof( value );
      p_draft->incrementThroughput = f; 
    }
    else if( !strcmp("timeFrequency", arg) )
    {
      int i = atoi( value );
      p_draft->timeFrequency = i; 
    }
    else if( !strcmp("timer", arg) )
    {
      int i = atoi( value );
      p_draft->timer = i;
    }
    else{
      //error
    }
  }
}

static void useDefaultConfig( LhfDraft *p_draft )
{
  p_draft->target_port = PORT;
  p_draft->amp_port = PORT;
  p_draft->initialThroughput = INITIALTHROUGHPUT;
  p_draft->incrementThroughput = INCREMENTTHROUGHPUT;
  p_draft->timeFrequency = TIMEFREQUENCY;
  p_draft->timer = TIMER;
}

void GetConfig( LhfDraft *p_draft )
{
  FILE *fp;
  
  if( NULL == p_draft)
  {
    return;
  }

  fp = fopen( FILENAME, "r");

  if( NULL != fp )
  {
    createConfigFile();
    useDefaultConfig( p_draft );
    return;
  }

  readConfigFile( fp, p_draft );
  fclose(fp);
}
