#ifndef TYPES_H
#define TYPES_H



/* Status will be used in fn. return type */
typedef enum
{
    v_success,
    v_failure,
    e_success,
    e_failure
} Status;

typedef enum
{
    view,
    edit,
    unsupported
} OperationType;

#endif
