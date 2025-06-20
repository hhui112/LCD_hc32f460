/*
 * @Author: wayne cao 
 * @Date: 2022-06-30 09:35:56 
 * @Last Modified by: wayne cao
 * @Last Modified time: 2022-06-30 10:52:31
 */
#include "qs_config.h"
#include <SEGGER_RTT_Conf.h>
#include <SEGGER_RTT.h>
#include "qs_log_backend_rtt.h"

#define NRF_LOG_BACKEND_RTT_TX_RETRY_CNT 3

static bool m_host_present;

void nrf_log_backend_rtt_init(void)
{
    SEGGER_RTT_Init();
}

static void serial_tx(char const * buffer, size_t len)
{
    if (len)
    {
        uint32_t idx    = 0;
        uint32_t processed;
        uint32_t watchdog_counter = NRF_LOG_BACKEND_RTT_TX_RETRY_CNT;
        do
        {
            processed = SEGGER_RTT_WriteNoLock(0, &buffer[idx], len);
            idx += processed;
            len -= processed;
            if (processed == 0)
            {
                /* There are two possible reasons for not writing any data to RTT:
                 * - The host is not connected and not reading the data.
                 * - The buffer got full and will be read by the host.
                 * These two situations are distinguished using the following algorithm.
                 * At the begining, the module assumes that the host is active,
                 * so when no data is read, it busy waits and retries.
                 * If, after retrying, the host reads the data, the module assumes that the host is active.
                 * If it fails, the module assumes that the host is inactive and stores that information. On next
                 * call, only one attempt takes place. The host is marked as active if the attempt is successful.
                 */
                if (!m_host_present)
                {
                    break;
                }
                else
                {
                    //nrf_delay_ms(NRF_LOG_BACKEND_RTT_TX_RETRY_DELAY_MS);
                    watchdog_counter--;
                    if (watchdog_counter == 0)
                    {
                        m_host_present = false;
                        break;
                    }
                }
            }
            m_host_present = true;
        } while (len);
    }
}
void nrf_log_backend_rtt_put(char const * buffer, size_t len)
{
    serial_tx(buffer, len);
}

