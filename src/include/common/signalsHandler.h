/**
 *  signalsHandler.h
 *
 * */
#ifndef SIGNALSHANDLER_H
  #define SIGNALSHANDLER_H

/**
 *  @brief Set signal action
 *
 *  @param p_sig[in] Signal
 *  @param p_func[in] Signal function handeler 
 * 
 * */
int SetSigHdr( int p_sig, void p_func(int sig) );

#endif
