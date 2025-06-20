#ifndef __G_H__
#define __G_H__

#include "stdint.h"
#include <string.h>
#include <stdbool.h>

#define VERSION    100
#define HANDWARE   100

#define WHITE            0xFFFF
#define BLACK            0x0000   
#define BLUE             0x001F  
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define GREEN            0x07E0
/*
typedef struct 
{
	uint32_t runtimer;
	
	uint8_t addr;
	uint8_t bg;
	uint8_t pwm;
	uint8_t ui_type;
	uint8_t running_status;
	uint8_t sleep_score;
	uint8_t alarm;
	uint8_t hb;
	uint8_t hx;
	
}g_system_st;

*/

typedef struct 
{
	uint32_t runtimer;
	
	uint8_t addr;
	uint8_t length;
	uint8_t sw_version;
	uint8_t hw_version;
	uint8_t wifi_connect_status;                // 配网状态
    uint8_t heart_rate;                         // 心率
    uint8_t breath_rate;                        // 呼吸率
    uint8_t diastolic_bp;                       // 舒张压
    uint8_t systolic_bp;                        // 收缩压
    uint8_t sleep_score;                        // 睡眠分数
	uint8_t off_bed;                        	// 离床状态

}g_system_st;

typedef struct {
    bool active;             // 当前是否正在响铃
    bool buzzer_on;          // 当前是否处于响状态
    uint32_t start_time;     // 起始时间
    uint32_t toggle_time;    // 上次切换时间
    bool triggered;          // 是否已经触发过一次
} buzzer_ctrl_t;

typedef struct {
    uint8_t need_set_score;
    uint8_t sleep_score;
} send_state_t;

extern send_state_t g_send;
extern buzzer_ctrl_t g_buzzer;
extern g_system_st g_system;
extern g_system_st g_last;
uint16_t Modbus_Crc_Compute(const uint8_t *buf, uint16_t bufLen);

#endif 
