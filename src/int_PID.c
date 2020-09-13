#include "int_PID.h"


/**
 * Initialize the PID regulator.
 *
 * :param pid: Pointer to the :c:type:`int_pid` with filled parameters.
 * :return: Error code according to :c:type:`pid_errors`.
 */
enum pid_errors pid_init(struct int_pid *pid)
{
    if (pid->div <= 0)
        return PID_WRONG_PARAMS;

    if (pid->P != 0 && pid->P / pid->div == 0)
        return PID_FOOR_WARN;

    if (pid->I != 0 && pid->I / pid->div == 0)
        return PID_FOOR_WARN;

    if (pid->D != 0 && pid->D / pid->div == 0)
        return PID_FOOR_WARN;

    if (!pid->desired || !pid->in || !pid->out)
        return PIN_NULL_PTR_ERROR;

    return PID_OK;
}


/**
 * Update the PID regulator.
 *
 * :param pid: Pointer to the :c:type:`int_pid` with filled parameters.
 */
void pid_update(struct int_pid *pid)
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

    pid->_i_err += p_err;

    *pid->out = pid->P / pid->div * p_err + 
                pid->D / pid->div * d_err +
                pid->I / pid->div * pid->_i_err;
    

    pid->_prev_err = p_err;
}