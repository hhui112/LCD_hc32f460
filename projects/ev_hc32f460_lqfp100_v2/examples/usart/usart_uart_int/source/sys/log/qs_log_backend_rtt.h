/*
 * @Author: wayne cao 
 * @Date: 2022-06-30 09:48:49 
 * @Last Modified by: wayne cao
 * @Last Modified time: 2022-06-30 10:52:40
 */

#pragma once

void nrf_log_backend_rtt_init(void);
void nrf_log_backend_rtt_put(char const * buffer, size_t len);

