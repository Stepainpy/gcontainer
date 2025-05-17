#ifndef GENERIC_ADAPTOR_H
#define GENERIC_ADAPTOR_H

#include "gda.h"
#include "gheap.h"

/* Stack */

#define gstk_push gda_push
#define gstk_pop  gda_pop
#define gstk_top  gda_last

/* Priority queue */

#define gprq_push ghp_push
#define gprq_pop  ghp_pop
#define gprq_peek gda_front

#endif /* GENERIC_ADAPTOR_H */