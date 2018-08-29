#include <argp.h>
#include <argz.h> 

#include "venus.h"
#include "commander/linderhof.h"
#include "interface/configr.h"

#define DELIM "."

const char *argp_program_bug_address = "rqtx@protonmail.com";
const char *argp_program_version = "version 0.1"; 

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
      if( !strcmp(arg, "memcached") || !strcmp(arg, "Memcached") || !strcmp(arg, "MEMCACHED") ){
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

      draft->target_ip = arg;   
      break;

    case 'a':
      if( !is_valid_ipv4(arg) )
        argp_failure (state, 1, 0, "Invalid IPv4");
      
      draft->amp_ip = arg;
      break;
  }
  return 0;
}

LhfDraft * cliParser(int argc, char ** argv)
{
  LhfDraft * draft;
  struct argp argp = { options, parse_opt, "ATK" };

  draft = (LhfDraft *) CreateEmptyDraft();
  GetConfig( draft );

  argp_parse(&argp, argc, argv, 0, 0, draft);
  
  return draft;
}
