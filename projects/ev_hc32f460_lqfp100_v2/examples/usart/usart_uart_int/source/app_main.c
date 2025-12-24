#include "main.h"
#include "app_main.h"
#include "qs_log.h"
#include "sys_uart.h"
#include "sys_tmra.h"
#include "GC9a01_drv.h"

#define SENDTIME			  			500  	 //	2S
#define BUZZER_TIME			 		 	5000  	 //	5S
#define BUZZER_TOGGLE_TIME			  50  	 //	50ms
#define BUZZER_INTERVAL_TIME			950  	 //	50ms



static uint32_t commTimerOut;
// static uint16_t backlighttimerout = 0;
uint32_t comm_len_;


void comm_cmd_handle(uint8_t *pbuff,uint16_t len);
void comm_send(void);


void app_timer_1ms(void)
{
	g_system.runtimer++;

	if((commTimerOut > 0) &&  (comm_len != 0))
	{
		commTimerOut--;
		
		if(commTimerOut == 0)
		{
			comm_len = 0;
			memset(revbuff,0,sizeof(revbuff));		// 50ms timeout clear revbuff 
		}
	}

}


void comm_rev(uint8_t data)
{
	uint16_t crc = 0,crc1 = 0;

	if (comm_len < RX_BUFFER_SIZE) {
    	revbuff[comm_len++] = data;
	} else {
		comm_len = 0;
		memset(revbuff, 0, sizeof(revbuff));
		return;
	}

	// revbuff[comm_len++] = data;
	commTimerOut = 50;

	if(comm_len>3)
	{
		
		if(revbuff[0] != 0x5a)
		{
			comm_len = 0;
			memset(revbuff,0,sizeof(revbuff));
			return;
		}

		if((revbuff[2]+5) <= comm_len)
		{
			// QS_LOG_INFO("recv frame: ");for (int i = 0; i < comm_len; i++) {QS_LOG_INFO("%02X ", revbuff[i]);}

			// Frame: 0x5A + CMD + LEN + DATA + CRC_L + CRC_H
			crc = Modbus_Crc_Compute(revbuff,comm_len-2);	
			crc1 = (revbuff[comm_len-2])|(revbuff[comm_len-1]<<8);
			
			// QS_LOG_INFO("crc = %x  rx_crc = %x",&crc,&crc1);
			if(crc1 == crc)
			{
				// QS_LOG_INFO("crc ok ");
				comm_cmd_handle(revbuff,comm_len);
				comm_len = 0;
				memset(revbuff,0,sizeof(revbuff));
				// comm_send();				
			}
			else
			{
				comm_len = 0;
				memset(revbuff,0,sizeof(revbuff));
				return;
			}
		}
	}
}

void comm_cmd_handle(uint8_t *pbuff,uint16_t len)
{
	uint8_t cmd = pbuff[1];
	// QS_LOG_INFO(">> recv data: ");for(int i = 0 ; i < len;i++){QS_LOG_INFO("%d ",pbuff[i]);}QS_LOG_INFO("\n ");
	if(cmd == 0x03)
	{
		if(pbuff[2]<=9)
		{
			g_system.sw_version = pbuff[3];
			g_system.hw_version = pbuff[4];
			g_system.wifi_connect_status = pbuff[5];	
			g_system.heart_rate = pbuff[6];
			g_system.breath_rate = pbuff[7];
			g_system.diastolic_bp = pbuff[8];
			g_system.systolic_bp = pbuff[9];
			g_system.sleep_score = pbuff[10];
			g_system.off_bed = pbuff[11];
			
			QS_LOG_INFO("sw=%d, hw=%d, wifi=%d, hr=%d, br=%d, dbp=%d, sbp=%d, score=%d, off_bed=%d\n",
							g_system.sw_version,
							g_system.hw_version,
							g_system.wifi_connect_status,
							g_system.heart_rate,
							g_system.breath_rate,
							g_system.diastolic_bp,
							g_system.systolic_bp,
							g_system.sleep_score,
							g_system.off_bed);
		}
	}
}


void uart_send(uint8_t *data,uint8_t len)
{
	
	if (len == 0 || data == NULL) return;
    if (isSending) return;
    
    memcpy((void*)txbuff, data, len);
    tx_len = len;
    tx_idx = 0;
    isSending = 1;
    
    USART_FuncCmd(USART_UNIT, USART_TX | USART_INT_TX_EMPTY, ENABLE);
}

/*
void comm_send(void)
{
	static uint32_t timer = 0;
	static uint8_t buff[20];
	static uint16_t crc_t = 0;

	if(timer == 0)
	{
		timer = g_system.runtimer;
	}
	
	if( (timer!=0) &&(g_system.runtimer-timer > SENDTIME) ) 	// 200ms一次查询命令
	{

		int i = 0;
		
		memset(txbuff,0,sizeof(txbuff));
		buff[i++] = 0x5a;
		buff[i++] = 0x00;
		buff[i++] = 0x00;
		buff[i++] = 0x01;	// data lenth
		buff[i++] = 0x00;	
		crc_t = Modbus_Crc_Compute(buff,i);
		buff[i++] = crc_t&0xff; 
		buff[i++]	= (crc_t>>8)&0xff;
		uart_send(buff,i);
		timer = 0;
		// QS_LOG_INFO(">> send frame: "); // for(int a = 0 ; a < i;a++){QS_LOG_INFO("%x ",txbuff[a]);}QS_LOG_INFO("\n ");
	}

}
*/

void build_frame(uint8_t cmd, uint8_t subcmd, uint8_t data, uint8_t *out_buf, uint8_t *out_len)
{
    uint16_t crc;
    uint8_t i = 0;
    out_buf[i++] = 0x5A;       // 帧头
    out_buf[i++] = cmd;        // 命令字节
    out_buf[i++] = subcmd;     // 保留命令
    out_buf[i++] = 0x01;       // 数据长度
    out_buf[i++] = data;       // 数据内容
    crc = Modbus_Crc_Compute(out_buf, i);
    out_buf[i++] = crc & 0xFF;
    out_buf[i++] = (crc >> 8) & 0xFF;
    *out_len = i;
}

void comm_send(void)
{
    static uint32_t timer = 0;
    static uint8_t buff[20];
    uint8_t len = 0;

    if (timer == 0) {
        timer = g_system.runtimer;
    }

    if ((timer != 0) && (g_system.runtimer - timer > SENDTIME)) {

        // === 发送设置分数 ===
		if (g_send.need_set_score) {
			build_frame(0x01,0x00,g_send.sleep_score, buff, &len);
			g_send.need_set_score = 0;
		}else{
			// === 固定查询命令 ===
			build_frame(0x00, 0x00, 0x00, buff, &len);
		}
		uart_send(buff, len);
        timer = 0;
    }
}


// 定义背景类型
#define BG_BLACK      	0	// 无背景
#define BG_RED      	1	// 红色背景
#define BG_BLUE	  		2	// 蓝色背景
#define BG_GREEN	  	3	// 绿色背景
#define BG_YELLOW	  	4	// 黄色背景
#define BG_DEFAULT	  	5	// 默认背景

// 上一次刷的背景
static uint8_t last_bg_type = 0xFF;
void update_background(uint8_t bg_type)	// // 封装背景更新函数
{
    if (last_bg_type == bg_type) return;

    switch (bg_type)
    {
        case BG_RED:
            Picture_display(0, 0, 240, 240, gImage_red);
            break;
        case BG_BLUE:
            Picture_display(0, 0, 240, 240, gImage_blue);
            break;
        case BG_GREEN:
            Picture_display(0, 0, 240, 240, gImage_green);
            break;
        case BG_YELLOW:
            Picture_display(0, 0, 240, 240, gImage_yellow);
            break;
				case BG_BLACK:
						TFT_full(BLACK);
						break;
        default:
            TFT_full(BLACK);
            break;
    }

    last_bg_type = bg_type;
}

/*
* 蜂鸣器开启
*/
void Buzzer_StartTicking(void)
{
    if (g_buzzer.triggered) return;  // 已触发就不再重复

    g_buzzer.active = true;
    g_buzzer.buzzer_on = true;
    g_buzzer.start_time = g_system.runtimer;
    g_buzzer.toggle_time = g_system.runtimer;
    g_buzzer.triggered = true;
    Buzzer_on();
}


/*
*	蜂鸣器定时检查任务
*/
void Buzzer_Task(void)
{
		
    if (!g_buzzer.active) return;

    // 超过 5 秒总时长，关闭蜂鸣器
    if (g_system.runtimer - g_buzzer.start_time >= BUZZER_TIME) {
        Buzzer_off();
        g_buzzer.active = false;
        g_buzzer.buzzer_on = false;
		g_buzzer.triggered = false;		// 允许下次触发
        return;
    }

    // 间隔切换逻辑
    if (g_buzzer.buzzer_on) {
        // 当前为响铃状态，是否到达关闭时间
        if (g_system.runtimer - g_buzzer.toggle_time >= BUZZER_TOGGLE_TIME) {
            Buzzer_off();
			//  QS_LOG_INFO("Buzzer off\n");
            g_buzzer.buzzer_on = false;
            g_buzzer.toggle_time = g_system.runtimer;
        }
    } else {
        // 当前为关闭状态，是否到达开启时间
        if (g_system.runtimer - g_buzzer.toggle_time >= BUZZER_INTERVAL_TIME) {
            Buzzer_on();
			//  QS_LOG_INFO("Buzzer on\n");
            g_buzzer.buzzer_on = true;
            g_buzzer.toggle_time = g_system.runtimer;
        }
    }
}


void show_picture(void)
{
	
		static uint8_t abnormal_cnt;  // 异常计数
    // 1. 配网状态界面
    if (g_system.wifi_connect_status == 2) {
				update_background(BG_RED);
        LCD_ShowChinese(40, 100, "配网异常", WHITE, BLACK, 40, 0);
				abnormal_cnt = 0;
        return;
    }

    // 以下逻辑都在 wifi_connect_status == 1 的前提下

    // 2. 睡眠分数界面（优先级最高）
    if (g_system.sleep_score > 0) {
        uint8_t score = g_system.sleep_score;
        if (score < 10) {
			update_background(BG_RED);
            LCD_ShowIntNum(100, 84, score, 1, WHITE, BLACK, 40);
            LCD_ShowChinese(120, 92, "分", WHITE, BLACK, 32, 0);
            LCD_ShowChinese(72, 132, "睡眠较差", WHITE, BLACK, 24, 0);
        } else if (score < 70) {
			update_background(BG_RED);
            LCD_ShowIntNum(88, 88, score, 2, WHITE, BLACK, 40);
            LCD_ShowChinese(128, 92, "分", WHITE, BLACK, 32, 0);
            LCD_ShowChinese(72, 132, "睡眠较差", WHITE, BLACK, 24, 0);
        } else if (score < 85) {
			update_background(BG_YELLOW);
            LCD_ShowIntNum(88, 88, score, 2, WHITE, BLACK, 40);
            LCD_ShowChinese(128, 92, "分", WHITE, BLACK, 32, 0);
            LCD_ShowChinese(72, 132, "睡眠良好", WHITE, BLACK, 24, 0);
        } else if (score < 100) {
			update_background(BG_GREEN);
            LCD_ShowIntNum(88, 88, score, 2, WHITE, BLACK, 40);
            LCD_ShowChinese(128, 92, "分", WHITE, BLACK, 32, 0);
            LCD_ShowChinese(72, 132, "睡眠优秀", WHITE, BLACK, 24, 0);
        } else if(g_system.sleep_score == 100 ){
			update_background(BG_GREEN);
						LCD_ShowIntNum(76, 88, 100, 3, WHITE, BLACK, 40);
						LCD_ShowChinese(136, 92, "分", WHITE, BLACK, 32, 0);
						LCD_ShowChinese(72, 132, "睡眠优秀", WHITE, BLACK, 24, 0);
				}
        return;
    }

    // 3. 体征数据界面或异常报警
    uint8_t hr = g_system.heart_rate;
    uint8_t br = g_system.breath_rate;

    if (hr > 0 && br > 0) {
        bool hr_normal = (hr > 50 && hr < 100);
        bool br_normal = (br > 12 && br < 25);
				// QS_LOG_INFO("hr_normal = %d,br_normal = %d \n",hr_normal,br_normal);
        if (hr_normal && br_normal)
				{
					abnormal_cnt = 0;  // 清空异常报警次数
					update_background(BG_BLACK);
					// 正常体征显示
					LCD_ShowChinese(32, 76, "心率", WHITE, BLACK, 32, 0);LCD_ShowString(96, 76, ":", WHITE, BLACK, 32, 0);	LCD_ShowIntNum(120, 68, hr, 2, WHITE, BLACK, 40);
					LCD_ShowChinese(164, 86, "次", WHITE, BLACK, 16, 0);LCD_ShowString(180, 86, "/", WHITE, BLACK, 16, 0);LCD_ShowChinese(188, 86, "分", WHITE, BLACK, 16, 0);
				
					LCD_ShowChinese(32, 128, "呼吸", WHITE, BLACK, 32, 0);LCD_ShowString(96, 128, ":", WHITE, BLACK, 32, 0);	LCD_ShowIntNum(120, 124, br, 2, WHITE, BLACK, 40);
					LCD_ShowChinese(164, 140, "次", WHITE, BLACK, 16, 0);LCD_ShowString(180, 140, "/", WHITE, BLACK, 16, 0);LCD_ShowChinese(188, 140, "分", WHITE, BLACK, 16, 0);

        } else {
            // 异常报警
					if (abnormal_cnt < 30) {
						abnormal_cnt++;	// 连续异常30s才作为异常
						QS_LOG_INFO("abnormal_cnt = %d >>>>> hr_normal = %d,br_normal = %d \n",abnormal_cnt,hr_normal,br_normal);
					}
					if (abnormal_cnt >= 30) {			
						Buzzer_StartTicking();
						update_background(BG_RED);
						if (!hr_normal && br_normal) {
								LCD_ShowChinese(40, 84, "心率异常", WHITE, BLACK, 40, 0);
						} else if (hr_normal && !br_normal) {
								LCD_ShowChinese(40, 84, "呼吸异常", WHITE, BLACK, 40, 0);
						} else {
								LCD_ShowChinese(40, 84, "心率异常", WHITE, BLACK, 40, 0);
						}
							LCD_ShowChinese(60, 132, "请及时就医", WHITE, BLACK, 24, 0);
					} else {
							update_background(BG_BLACK);
							LCD_ShowChinese(32, 76, "心率", WHITE, BLACK, 32, 0);LCD_ShowString(96, 76, ":", WHITE, BLACK, 32, 0);	LCD_ShowIntNum(120, 68, hr, 2, WHITE, BLACK, 40);
							LCD_ShowChinese(164, 86, "次", WHITE, BLACK, 16, 0);LCD_ShowString(180, 86, "/", WHITE, BLACK, 16, 0);LCD_ShowChinese(188, 86, "分", WHITE, BLACK, 16, 0);
						
							LCD_ShowChinese(32, 128, "呼吸", WHITE, BLACK, 32, 0);LCD_ShowString(96, 128, ":", WHITE, BLACK, 32, 0);	LCD_ShowIntNum(120, 124, br, 2, WHITE, BLACK, 40);
							LCD_ShowChinese(164, 140, "次", WHITE, BLACK, 16, 0);LCD_ShowString(180, 140, "/", WHITE, BLACK, 16, 0);LCD_ShowChinese(188, 140, "分", WHITE, BLACK, 16, 0);
					}
				}
        return;
    }

    // 4. 默认提示
		if(g_system.off_bed == 1){
					update_background(BG_YELLOW);
					LCD_ShowChinese(40, 100, "当前离床", WHITE, BLACK, 40, 0);
					abnormal_cnt = 0;
					// Buzzer_StartTicking();
		}else{
					update_background(BG_BLUE);
					LCD_ShowChinese(40, 100, "正常运行", WHITE, BLACK, 40, 0);
					abnormal_cnt = 0;
		}

}

void appRun(void)
{
	static uint32_t timer = 0;	
	
	comm_send();
	
	if(timer == 0)
	{
		timer = g_system.runtimer;
	}
	
	if((timer!=0) &&(g_system.runtimer-timer > 1000) )		// 100ms刷新一次图片
	{
		show_picture();
		timer = 0;
	}
	

	Buzzer_Task();	// 蜂鸣器检查任务

}




