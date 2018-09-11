#include "venus.h"
#include "interface/cli.h"
#include "commander/linderhof.h"

int main( int argc, char **argv)
{
  LinderhofMaster();
  memoryclean();
  return 0;
}
