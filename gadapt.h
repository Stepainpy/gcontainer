#ifndef GENERIC_ADAPTOR_H
#define GENERIC_ADAPTOR_H

/* Stack */
#ifdef GAD_INCLUDE_STACK
#include "gda.h"

#define gstk_push gda_push
#define gstk_pop  gda_pop
#define gstk_top  gda_last

#endif /* GAD_INCLUDE_STACK */

/* Priority queue */
#ifdef GAD_INCLUDE_PR_QUEUE
#include "gda.h"
#include "gheap.h"

#define gprq_push ghp_push
#define gprq_pop  ghp_pop
#define gprq_peek gda_front

#endif /* GAD_INCLUDE_PR_QUEUE */

#endif /* GENERIC_ADAPTOR_H */