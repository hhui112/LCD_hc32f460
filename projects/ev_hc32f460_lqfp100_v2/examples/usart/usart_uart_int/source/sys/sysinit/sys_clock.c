#include <main.h > 
#include "sys_clock.h"
#include "qs_log.h"
#include "app_main.h"

#define MCO_PORT            GPIO_PORT_A
#define MCO_PIN             GPIO_PIN_08
#define MCO_GPIO_FUNC       GPIO_FUNC_1

#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
#define EXAMPLE_PERIPH_WP               (LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM)

/* TMR0 unit and channel definition */
#define TMR0_UNIT                       (CM_TMR0_1)
#define TMR0_CLK                        (FCG2_PERIPH_TMR0_1)				// ????????
#define TMR0_CH                         (TMR0_CH_B)									// ??????????(A/B)
#define TMR0_TRIG_CH                    (AOS_TMR0)						
#define TMR0_CH_INT                     (TMR0_INT_CMP_B)						// 	????????
#define TMR0_CH_FLAG                    (TMR0_FLAG_CMP_B)			    	//  ???????
#define TMR0_INT_SRC                    (INT_SRC_TMR0_1_CMP_B)			// ?????,??????
#define TMR0_IRQn                       (INT006_IRQn)								// NVIC ???
/* Period = 1 / (Clock freq / div) * (Compare value + 1) = 1/(100Mhz)/1024 / (194 + 1) = 1ms */
#define TMR0_CMP_VALUE                  (100000000UL  / 1024U / 1000U - 0U)


static volatile uint32_t msTicks = 0;

static void MCOInit(void)
{
    /* Configure clock output pin */
    GPIO_SetFunc(MCO_PORT, MCO_PIN, MCO_GPIO_FUNC);
    /* Configure clock output system clock */
    CLK_MCOConfig(CLK_MCO1, CLK_MCO_SRC_HCLK, CLK_MCO_DIV8);
    /* MCO1 output enable */
    CLK_MCOCmd(CLK_MCO1, ENABLE);
}

/**
 * @brief  Xtal initialize
 * @param  None
 * @retval None
 */
static void XtalInit(void)
{
    stc_clock_xtal_init_t stcXtalInit;

    /* XTAL config */
    GPIO_AnalogCmd(BSP_XTAL_PORT, BSP_XTAL_PIN, ENABLE);
    (void)CLK_XtalStructInit(&stcXtalInit);
    /* Config Xtal and Enable Xtal */
    stcXtalInit.u8State = CLK_XTAL_ON;
    stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
    stcXtalInit.u8Drv = CLK_XTAL_DRV_ULOW;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);
}

/**
 * @brief  MPLL initialize
 * @param  None
 * @retval None
 */
static void MPLLInit(void)
{
    stc_clock_pll_init_t      stcMPLLInit;

    (void)CLK_PLLStructInit(&stcMPLLInit);
    /* MPLL config */
    /* 8MHz/M*N = 8/1*50/2 = 200MHz */
    stcMPLLInit.PLLCFGR = 0UL;
    stcMPLLInit.PLLCFGR_f.PLLM = (1UL - 1UL);
    stcMPLLInit.PLLCFGR_f.PLLN = (50UL - 1UL);
    stcMPLLInit.PLLCFGR_f.PLLP = (2UL - 1UL);
    stcMPLLInit.PLLCFGR_f.PLLQ = (2UL - 1UL);
    stcMPLLInit.PLLCFGR_f.PLLR = (2UL - 1UL);
    stcMPLLInit.u8PLLState = CLK_PLL_ON;
    stcMPLLInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;     /* Xtal = 8MHz */
    (void)CLK_PLLInit(&stcMPLLInit);
}

/**
 * @brief  ?????,????? 200MHz(MPLL),??? MCO1(PA08)
 */
void SYS_Init(void)
{
    /* Register write unprotected for some required peripherals. */
		//LL_PERIPH_WE(LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_GPIO | LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM);
    /* Set bus clock div. */
    CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV2 | CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | \
                                      CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2));

    /* sram init include read/write wait cycle setting */
    SRAM_SetWaitCycle(SRAM_SRAM_ALL, SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
    /* flash read wait cycle setting */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
    /* output system clock */
    MCOInit();
    /* Xtal initialize */
    XtalInit();
    /* Xtal32 initialize*/
    //BSP_XTAL32_Init();
    /* MPLL initialize */
    MPLLInit();
    /* enable LRC */
    (void)CLK_LrcCmd(ENABLE);
    /* enable HRC */
    (void)CLK_HrcCmd(ENABLE);
    /* Switch driver ability */
    PWC_HighSpeedToHighPerformance();
    /* Register write protected for some required peripherals. */
    // LL_PERIPH_WP(LL_PERIPH_EFM | LL_PERIPH_GPIO | LL_PERIPH_SRAM);
		CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);  // CLK_SYSCLK_SRC_XTAL

    (void)SysTick_Init(1000U);	// // 初始化为 1ms 中断一次
}



/**
 * @brief  TMR0 compare interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMR0_CompareIrqCallback(void)
{
    TMR0_ClearStatus(TMR0_UNIT, TMR0_CH_FLAG);  // ????????
    msTicks++;
		app_timer_1ms();
		// LED_R_TOGGLE();		// 498Khz   6???1s
	
}

uint32_t GetMsTicks(void)
{
    return msTicks;
}

void SysTick_Handler(void)
{
    SysTick_IncTick();	 // Tick 自增，支持 DDL_DelayMS()

    __DSB();  /* Arm Errata 838869 */
}


/**
 * @brief  Configure TMR0.
 * @note   In asynchronous clock, If you want to write a TMR0 register, you need to wait for
 *         at least 6 asynchronous clock cycles after the last write operation!
 * @param  None
 * @retval None
 */
void TMR0_Config(void)
{
		//LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
    stc_tmr0_init_t stcTmr0Init;
    stc_irq_signin_config_t stcIrqSignConfig;

    /* Enable timer0 and AOS clock */
    FCG_Fcg2PeriphClockCmd(TMR0_CLK, ENABLE);
    // FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_AOS, ENABLE);  // AOS ?????????(Asynchronous Event Control),??????????????? ?????????

    /* TIMER0 configuration */
    (void)TMR0_StructInit(&stcTmr0Init);
    stcTmr0Init.u32ClockSrc     = TMR0_CLK_SRC_INTERN_CLK;
    stcTmr0Init.u32ClockDiv     = TMR0_CLK_DIV1024;
    stcTmr0Init.u32Func         = TMR0_FUNC_CMP;
    stcTmr0Init.u16CompareValue = (uint16_t)TMR0_CMP_VALUE;
    (void)TMR0_Init(TMR0_UNIT, TMR0_CH, &stcTmr0Init);
    /* Asynchronous clock source, writing to TMR0 register requires waiting for 6 asynchronous clocks. */
    // DDL_DelayMS(1U);
    // TMR0_HWStopCondCmd(TMR0_UNIT, TMR0_CH, ENABLE);		// ?????????
    /* Asynchronous clock source, writing to TMR0 register requires waiting for 6 asynchronous clocks. */
    DDL_DelayMS(1U);
    TMR0_IntCmd(TMR0_UNIT, TMR0_CH_INT, ENABLE);
    /* Asynchronous clock source, writing to TMR0 register requires waiting for 6 asynchronous clocks. */
    DDL_DelayMS(1U);
    // AOS_SetTriggerEventSrc(TMR0_TRIG_CH, BSP_KEY_KEY10_EVT);	// ??????? ????

    /* Interrupt configuration */
    stcIrqSignConfig.enIntSrc    = TMR0_INT_SRC;
    stcIrqSignConfig.enIRQn      = TMR0_IRQn;
    stcIrqSignConfig.pfnCallback = &TMR0_CompareIrqCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
		
		TMR0_Start(TMR0_UNIT, TMR0_CH);
		DDL_DelayMS(1U);
		//LL_PERIPH_WP(EXAMPLE_PERIPH_WP);
		
}

