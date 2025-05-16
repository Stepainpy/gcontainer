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

#define gprq_push(pq, value) do { \
    gda_push(pq, value); gh_push(pq); \
} while (0)

#define gprq_pop(pq, retp) do { \
    gh_pop(pq); *(retp) = gh_poped(pq); \
} while (0)

#define gprq_peek gda_front

#endif /* GAD_INCLUDE_PR_QUEUE */

#endif /* GENERIC_ADAPTOR_H */