#include "int_PID.h"


/**
 * Initialize (reinitialize) the PID regulator. 
 * Should be called just before enabling main loop.
 *
 * :param pid: Pointer to the :c:type:`int_pid` with filled parameters.
 * :return: Error code according to :c:type:`pid_errors`.
 */
enum pid_errors pid_init(struct int_pid *pid)
{
    if (pid->div <= 0)
        return PID_WRONG_PARAMS;
    
    if (!pid->desired || !pid->in || !pid->out)
        return PIN_NULL_PTR_ERROR;

    pid->_prev_time = *pid->time;
    pid->_prev_interval = 0;
    pid->_prev_err = *pid->desired - *pid->in;
    pid->_i_err = 0;
    
    return PID_OK;
}


/**
 * Update the PID regulator.
 *
 * :param pid: Pointer to the :c:type:`int_pid` with filled parameters.
 */
struct pid_dbg_info pid_update(struct int_pid *pid)
{
    int_type interval;
    if (pid->time) {
        interval = *pid->time - pid->_prev_time;

        if (interval < 0) // Counter overflow
            interval = pid->_prev_interval;
        else 
            pid->_prev_interval = interval;

        pid->_prev_time = *pid->time;
    }
    else {
        interval = 1;
    }

    int_type p_err = *pid->desired - *pid->in;
    int_type d_err = (interval == 0) ? 0 : ((pid->_prev_err - p_err) / interval);

    pid->_i_err += pid->I * p_err * interval / pid->div;

    *pid->out = (pid->P * p_err + pid->D * d_err) / pid->div + pid->_i_err;
    

    pid->_prev_err = p_err;

    struct pid_dbg_info dbg = {
        .P = pid->P * p_err,
        .I = pid->I * pid->_i_err,
        .D = pid->D * d_err,
        .interval = interval
    };
    return dbg;
}
