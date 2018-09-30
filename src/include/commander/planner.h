/**
 *  planner.h
 *  Linderhof attack planner
 *
 *  Aqui construimos o plano de ataque a partir do draft recebido.
 * */
#ifndef PLANNER_H
  #define PLANNER_H

#include "venus.h"
#include "hom.h"

/**
 *  @brief Plan constructor
 *
 *  @param p_draft[in] LhfDraft struct
 * */
LhfPlan * Planner( LhfDraft *p_draft );

#endif
