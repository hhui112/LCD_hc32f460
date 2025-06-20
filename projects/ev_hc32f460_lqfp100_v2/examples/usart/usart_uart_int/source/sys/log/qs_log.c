/*
 * @Author: wayne cao 
 * @Date: 2022-06-30 09:43:02 
 * @Last Modified by: wayne cao
 * @Last Modified time: 2022-06-30 10:51:44
 */
#include <stdarg.h>
#include "qs_config.h"

#include "qs_log.h"
#include "qs_log_backend_rtt.h"

static char m_log_buffer[QS_LOG_BUFFER_SIZE];

void qs_log_print(qs_log_level_t level, const char *fmt, ...)
{

#ifdef QS_LOG_HC
		int len = 0;
    qs_log_level_t const log_level_list_table[] = {LOG_LEVEL_DEBUG, LOG_LEVEL_WARN, LOG_LEVEL_NOTICE, LOG_LEVEL_INFO, LOG_LEVEL_ERROR};
    char const level_str_list_table[]={'D','W','N','I','E'};
		for(size_t i = 0;i < sizeof(level_str_list_table)/sizeof(char); i++) {
				if(log_level_list_table[i] == level) {
           len += snprintf(m_log_buffer + len, sizeof(m_log_buffer) - len, "[%c] ", level_str_list_table[i]);
				}
    }
    va_list args;
    va_start(args, fmt);
    len += vsnprintf(m_log_buffer + len, sizeof(m_log_buffer) - len, fmt, args);
    va_end(args);
		len += snprintf(m_log_buffer + len, sizeof(m_log_buffer) - len, "\r");
		nrf_log_backend_rtt_put(m_log_buffer, len);

#endif
    

}

static const char* str_get_in_str(const char* source, const char* dest, int8_t index)
{
    const char *str = source;
    int i, j;
    int lens = strlen(source);
    int dest_lens = strlen(dest);
    
    if (index > 0) {
        for (i = 0, j = 0; i < lens; i++) {
            if (!strncmp(source+i, dest, dest_lens)) {
                j++;
                if (j == index) {
                    str = source+i;
                    break;
                }
            }
        }
    } else if (index < 0) {
        for (i = lens - 1, j = 0; i >= 0; i--) {
            if (!strncmp(source+i, dest, dest_lens)) {
                j--;
                if (j == index) {
                    str = source+i;
                    break;
                }
            }
        }
    }
    
    return str;
}
const char * __filename(const uint8_t *p_file_name)
{
    return str_get_in_str((char*)p_file_name, "\\", -2) + 1;
}
void qs_log_error_handler(int32_t error_code, uint32_t line_num, const uint8_t *p_file_name, const uint8_t *p_func_name)
{
    qs_log_print(LOG_LEVEL_ERROR, "Error code: %d, Line: %d, File: %s, Function: %s\r\n", error_code, line_num, str_get_in_str((char*)p_file_name, "\\", -2) + 1, p_func_name);
}

qs_ret_code_t qs_log_init(void)
{

#ifdef QS_LOG_HC
    nrf_log_backend_rtt_init();
#endif
    return QS_SUCCESS;

}
