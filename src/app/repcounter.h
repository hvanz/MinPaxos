#ifndef _REPCOUNTER_H_
#define _REPCOUNTER_H_

/******************************************************************************/
/* Replicated counter                                                         */
/******************************************************************************/

void repcounter_init (uint client_id);
int repcounter_read (void);
int repcounter_inc (uint x);
void repcounter_destroy (void);

#endif /* _REPCOUNTER_H_ */
