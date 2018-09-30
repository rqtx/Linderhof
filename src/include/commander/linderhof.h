
#ifndef LINDERHOF_H
  #define LINDERHOF_H

#include "venus.h"

/**
 * @brief Initialize linderhof with network interface (OryxNet)
 * */
void LinderhofNet();

/**
 * @brief Initialize linderhof with cli interface (OryxCli)
 * */
void LinderhofCli( int p_argc, char **p_argv );

/**
 * @brief Attack request
 *
 * @param p_draft[in] LhfDraft struct
 * */
int StartMirrorAttack( LhfDraft p_draft );

#endif
