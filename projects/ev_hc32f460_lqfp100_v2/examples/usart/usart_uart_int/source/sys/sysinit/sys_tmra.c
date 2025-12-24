#include <main.h > 
#include "sys_tmra.h" 

#define TMRA6_UNIT                  (CM_TMRA_6)
#define TMRA6_PERIPH_CLK            (FCG2_PERIPH_TMRA_6)
#define TMRA6_PWM_CH                (TMRA_CH7)				//  PB5 (LCD TIMA_6_PWM7) 
#define TMRA6_PWM_PORT              (GPIO_PORT_B)
#define TMRA6_PWM_PIN               (GPIO_PIN_05)
#define TMRA6_PWM_PIN_FUNC          (GPIO_FUNC_5)

#define TMRA5_UNIT                  (CM_TMRA_5)
#define TMRA5_PERIPH_CLK            (FCG2_PERIPH_TMRA_5)
#define TMRA5_PWM_CH                (TMRA_CH8)				// PB10 (Buzzer TIMA_5_PWM8)
#define TMRA5_PWM_PORT              (GPIO_PORT_B)
#define TMRA5_PWM_PIN               (GPIO_PIN_10)
#define TMRA5_PWM_PIN_FUNC          (GPIO_FUNC_5)

/* 1kHz PWM, 50% duty, system clock 100MHz, PCLK1 = 100MHz */
/* Sawtooth mode: Period = (100 000KHz / 1kHz) - 1 = 100000 */
//#define TMRA_PERIOD_VAL             (50000U - 1U)
//#define TMRA_CMP_VAL                (25000U - 1U)

#define TMRA_PERIOD_VAL   (25000U - 1U)  
#define TMRA_CMP_VAL      (12500U - 1U) 

#define TMRA6_PERIOD_VAL   (25000U - 1U)  
#define TMRA6_CMP_VAL      (2500U - 1U) 

void TmrA_Config(void)
{
	
	  /* Peripheral unprotect */
    //LL_PERIPH_WE(LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU);

    stc_tmra_init_t stcTmraInit;
    stc_tmra_pwm_init_t stcPwmInit;

    /* TimerA6 for PB5 */
    FCG_Fcg2PeriphClockCmd(TMRA6_PERIPH_CLK, ENABLE);			// ??TMRA6 clock
    (void)TMRA_StructInit(&stcTmraInit);
		stcTmraInit.sw_count.u8ClockDiv = TMRA_CLK_DIV2;
    stcTmraInit.sw_count.u8CountMode = TMRA_MD_SAWTOOTH;		// ???
    stcTmraInit.sw_count.u8CountDir  = TMRA_DIR_UP;
    stcTmraInit.u32PeriodValue = TMRA6_PERIOD_VAL;
    (void)TMRA_Init(TMRA6_UNIT, &stcTmraInit);

    (void)TMRA_PWM_StructInit(&stcPwmInit);
    stcPwmInit.u32CompareValue = TMRA6_CMP_VAL;
    GPIO_SetFunc(TMRA6_PWM_PORT, TMRA6_PWM_PIN, TMRA6_PWM_PIN_FUNC);
    (void)TMRA_PWM_Init(TMRA6_UNIT, TMRA6_PWM_CH, &stcPwmInit);
    TMRA_PWM_OutputCmd(TMRA6_UNIT, TMRA6_PWM_CH, ENABLE);

    /* TimerA5 for PB10 */
    FCG_Fcg2PeriphClockCmd(TMRA5_PERIPH_CLK, ENABLE);
    (void)TMRA_StructInit(&stcTmraInit);
		stcTmraInit.sw_count.u8ClockDiv = TMRA_CLK_DIV2;
    stcTmraInit.sw_count.u8CountMode = TMRA_MD_SAWTOOTH;
    stcTmraInit.sw_count.u8CountDir  = TMRA_DIR_UP;
    stcTmraInit.u32PeriodValue = TMRA_PERIOD_VAL;
    (void)TMRA_Init(TMRA5_UNIT, &stcTmraInit);

    (void)TMRA_PWM_StructInit(&stcPwmInit);
    stcPwmInit.u32CompareValue = TMRA_CMP_VAL;
    GPIO_SetFunc(TMRA5_PWM_PORT, TMRA5_PWM_PIN, TMRA5_PWM_PIN_FUNC);
    (void)TMRA_PWM_Init(TMRA5_UNIT, TMRA5_PWM_CH, &stcPwmInit);
    TMRA_PWM_OutputCmd(TMRA5_UNIT, TMRA5_PWM_CH, ENABLE);
		
		// LL_PERIPH_WP(LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU);

    TMRA_Start(TMRA6_UNIT);
    //TMRA_Start(TMRA5_UNIT);
		TMRA_Stop(TMRA5_UNIT);

}

void Buzzer_on(void){
	 TMRA_Start(TMRA5_UNIT);
}

void Buzzer_off(void){
	 	TMRA_Stop(TMRA5_UNIT); 
}


void Buzzrt_on_ms(uint16_t duration)
{
		Buzzer_on();
		SysTick_Delay(duration);
		Buzzer_off();
}
