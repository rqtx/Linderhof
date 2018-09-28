#include "venus.h"
#include "commander/linderhof.h"

int main( int argc, char **argv)
{
#ifdef ORYXNET
    LinderhofNet();
#else
    LinderhofCli(argc,argv);
#endif
    memoryclean();
    return 0;
}
