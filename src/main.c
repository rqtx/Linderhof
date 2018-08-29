#include "venus.h"
#include "interface/cli.h"
#include "commander/linderhof.h"

int main( int argc, char **argv)
{
  LhfDraft * draft = NULL;

  draft = cliParser(argc, argv);
  LhfStartController(draft);
  
  memoryclean();

  return 0;
}
