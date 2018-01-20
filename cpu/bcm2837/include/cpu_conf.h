#ifndef CPU_CONF_H
#define CPU_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Kernel configuration
 * @{
 */
#define THREAD_EXTRA_STACKSIZE_PRINTF_FLOAT  (4096)
#define THREAD_EXTRA_STACKSIZE_PRINTF        (2048)

#ifndef THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_DEFAULT   (512)
#endif

#define THREAD_STACKSIZE_IDLE      (160)
/** @} */

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* CPU_CONF_H */
