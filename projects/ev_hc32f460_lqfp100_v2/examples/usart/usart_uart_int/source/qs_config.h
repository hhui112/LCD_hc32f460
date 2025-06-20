#ifndef QS_CONFIG_H_
#define QS_CONFIG_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include "main.h"
#include "hc32f4xx_conf.h"

#include "qs_error.h"

#define QS_SW_VER_MAJOR       1
#define QS_SW_VER_MINOR       0
#define QS_SW_VER_REVISION    1
#define QS_HW_VER_MAJOR       1
#define QS_HW_VER_REVISION    0
#define QS_SW_BUILD_DATE          __DATE__
#define QS_SW_BUILD_TIME          __TIME__


#define IS_POWER_OF_TWO(A) ( ((A) != 0) && ((((A) - 1) & (A)) == 0) )
#define swap_16bits(n) (((n >> 8)&0xFF)|((n << 8)&0xFF00))
#define swap_32bits(n) (((n >> 24)&0xFF)|((n >> 8)&0xFF00)|((n << 8)&0xFF0000)|((n << 24)&0xFF000000))
#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef enum
{
    LOG_LEVEL_NONE      = 0,
    LOG_LEVEL_ERROR     = 3,
    LOG_LEVEL_WARN      = 4,
    LOG_LEVEL_NOTICE    = 5,
    LOG_LEVEL_INFO      = 6,
    LOG_LEVEL_DEBUG     = 7,
} qs_log_level_t;
typedef struct {
    uint16_t analog;
    int16_t speed;
    uint16_t complete;
    int32_t hall;
    uint32_t unique_id;
    uint16_t flag;
}qs_motor_info_t;
#define QS_DEFAULT_LOG_LEVEL			LOG_LEVEL_INFO
#define QS_DEFAULT_DEVICE_INFO                  "bm_connect"
#define DEV_INFO                                "keeson"
#define M_PI											3.14159265358979323846
#define QS_BITS(X) 								(1U << X)


#define QS_LOG_HC																1
#define QS_LOG_BUFFER_SIZE          						256

#define NUM_DRIVES      4

/* unlock/lock peripheral */
#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
#define EXAMPLE_PERIPH_WP               (LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM)


//#define TARGET_AUTO_INCREMENT_PAGE_SIZE   0x1000
#endif


