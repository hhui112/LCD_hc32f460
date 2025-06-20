#ifndef SYS_CLOCK_H_
#define SYS_CLOCK_H_


void SYS_Init(void);
void TMR0_Config(void);
uint32_t GetMsTicks(void);		// 返回当前定时器时间

#endif
