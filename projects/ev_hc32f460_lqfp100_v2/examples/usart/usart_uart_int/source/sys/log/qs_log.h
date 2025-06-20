/*
 * @Author: wayne cao 
 * @Date: 2022-06-30 09:43:17 
 * @Last Modified by:   wayne cao 
 * @Last Modified time: 2022-06-30 09:43:17 
 */

#ifndef _QS_LOG_H_
#define _QS_LOG_H_

#include "qs_config.h"
typedef void (*qs_data_handler_t) (void *p_object, void *p_data, uint16_t data_len);
typedef void (*qs_log_handler)(qs_log_level_t level, const char *log);

qs_ret_code_t qs_log_init(void);
void qs_log_error_handler(int32_t error_code, uint32_t line_num, const uint8_t *p_file_name, const uint8_t *p_func_name);
void qs_log_print(qs_log_level_t level, const char *fmt, ...);
void qs_log_delay_print(qs_log_level_t level, const char *fmt, ...);
qs_ret_code_t qs_log_syslog_handler_set(qs_log_handler log_handler);
const char * __filename(const uint8_t *p_file_name);
#ifdef QS_LOG_STORAGE
uint8_t *qs_log_persistent_logs_read(int16_t *p_data_len);
#endif

#define QS_ERROR_CHECK(_ERR_CODE)                                                                   \
do                                                                                                  \
{                                                                                                   \
    if(_ERR_CODE != 0) {                                                                            \
        qs_log_error_handler((_ERR_CODE), __LINE__, (uint8_t *) __FILE__, (uint8_t *) __FUNCTION__);    \
    }                                                                                               \
} while (0)

#define QS_ERROR_CHECK_RETURN(_ERR_CODE, _ERR_CODE_RET)                                             \
do                                                                                                  \
{                                                                                                   \
    if (_ERR_CODE != 0) {                                                                           \
        qs_log_error_handler((_ERR_CODE), __LINE__, (uint8_t *) __FILE__, (uint8_t *) __FUNCTION__);    \
        return _ERR_CODE_RET;                                                                       \
    }                                                                                               \
} while (0)

#define QS_LOG_DEBUG(fmt, ...)  qs_log_print(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define QS_LOG_WARN(fmt, ...)   qs_log_print(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define QS_LOG_NOTICE(fmt, ...) qs_log_print(LOG_LEVEL_NOTICE, fmt, ##__VA_ARGS__)
#define QS_LOG_INFO(fmt, ...)   qs_log_print(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define QS_LOG_ERROR(fmt, ...)  qs_log_print(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define QS_LOG_FLUSH            NRF_LOG_FLUSH
#define QS_LOG_DELAY_INFO(fmt, ...)   qs_log_delay_print(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#endif
