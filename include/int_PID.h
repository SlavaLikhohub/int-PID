#ifndef _INT_PID_H_
#define _INT_PID_H_

#include <stdint.h>

typedef int32_t int_type;

/** Struct for storing data about regulator */
struct int_pid
{
    /** Proportional coefficient */
    int_type P;

    /** Integral coefficient */
    int_type I;

    /** Diferential coefficient */
    int_type D;

    /** Coefficient on which will be divided all coefficients. Should be greater than 0 */
    int_type div;

    /** Pointer to desired value (System input) */
    int_type *desired;

    /** Poiner to the input variable (Feedback loop) */
    int_type *in;

    /** Poiner to the output variable (Control signal) */
    int_type *out;

    /** 
     * Poiner to a variable that store a current time
     * If set to NULL regulator will assume that :c:func:`pid_update` is called with a constant interval
     * If this variable has overflowed regulator uses interval between previous 2 calls.
     */
    int_type *time;

    /* Value of a time on previous update */
    int_type _prev_time;

    /* Interval between previous 2 calls */
    int_type _prev_interval;

    /* Value of error on previous update */
    int_type _prev_err;

    /* Integral from error */
    int_type _i_err;
};

/** Error codes */
enum pid_errors 
{
    /** Ok */
    PID_OK = 0,
    /** Wrong paramenets */
    PID_WRONG_PARAMS,
    /** Warning of flooring the variable to 0 after divition */
    PID_FOOR_WARN,
    /** Null pointer was found where it is not allowed */
    PIN_NULL_PTR_ERROR
};

/** Debug information */
struct pid_dbg_info
{
    int_type P;
    int_type I;
    int_type D;
    int_type interval;
};

enum pid_errors pid_init(struct int_pid *pid);

struct pid_dbg_info pid_update(struct int_pid *pid);

#endif // _INT_PID_H_