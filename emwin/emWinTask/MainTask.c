///*
//*********************************************************************************************************
//*	                                  
//*	ģ������ : ����ʾ�������������
//*	�ļ����� : MainTask.c
//*	��    �� : V1.0
//*	˵    �� : ����ʾ�������������
//*	�޸ļ�¼ :
//*		�汾��    ����          ����           ˵��
//*		V1.0    2018-01-06     Eric2013        �׷� 
//*                                           
//*	Copyright (C), 2018-2028, ���������� www.armfly.com
//*
//*********************************************************************************************************
//*/
//#include "includes.h"
//#include "MainTask.h"



///*
//*********************************************************************************************************
//���������˥��������ADC��DAC�ⲿ�ο���3.3V������û�н���У׼��
//(1) ÿ�������Ӧ����50���㣬��ʾ1V ��
//(2) 3.3V ��ӦADC��ֵ4095  4095/3.3 = 1240.9;
//    ��Ӧ��ϵ���� 50������ == 1V == ADC������1240.9
//    �ɴ˿ɵ�1V��˥��������4095/3.3/50 = 24.818���������ϵ���Ƶ���������顣
//*********************************************************************************************************
//*/
//const uint16_t g_AttTable[ATT_COUNT][2] =
//{	
///*  {����*�Ŵ�100��, ÿ����ѹ��λmv}  ������Ҳ�ø�����Ӧ������зŴ� ���������������� */
//    {24818,	10000}, /* ��248.18 * 100 ==  10000mv ��������Ƶ��Ƕ�Ӧ�� */
//	{12409,	5000},  /* ��124.09 * 100 ==  5000mv  */
//	{4964,	2000},
//	{2482,	1000},
//	{1241,	500},
//	{496,	200},
//	{248,	100},
//	{124,	50},
//	{50,	20},
//	{25,	10},
//};

///* 10�ַ��ȵ�λ**************************************************************************************/
//const char *g_AttText[] =
//{
//    "10.0V",
//	"5.00V",
//	"2.00V",
//	"1.00V",
//	"500mV",
//	"200mV",
//	"100mV",
//	"50.0mV",
//	"20.0mV",
//	"10.0mV",
//};

///* ÿ��ɼ����������������ڲɼ������ź�TFT��ʾʹ��**************************************************/
///* ������ʾ���ӳ�ʱ������ */
//const uint16_t g_RefreshDelay[] =
//{
//	50, 
//	50,
//	50,
//	50,
//	50,
//	
//	50,
//	50,
//	50,
//	50,
//	50,
//	
//	50,
//	50,
//	50,
//	50,
//	50,
//	
//	50,
//	50,
//	50,
//	50,
//	50,
//	50,
//};

///* ��ͨ�������ʣ�2.8Msps��1sps****************************************************************************/
//const uint32_t g_SampleFreqTable[][2] =
//{
//	{2800000,  1},     //2.8Msps
//	{2000000,  1},     //2Msps
//	{1000000,  1},     //1Msps
//	{500000,   1},     //500Ksps
//	{200000,   1},     //200Ksps
//	
//	{100000,   1},     //100Ksps
//	{50000,    1},     //50Ksps
//	{20000,    1},     //20Ksps
//	{10000,    1},     //10Ksps
//	{5000,     1},     //5Ksps
//	
//	{16800,      5},    //2Ksps
//	{16800,     10},    //1Ksps
//	{16800,     20},    //500sps
//	{16800,     50},    //200sps
//	{16800,     100},   //100sps
//	
//	{16800,     200},   //50sps
//	{16800,     500},   //20sps
//	{16800,     1000},  //10sps
//	{16800,     2000},  //5sps
//	{16800,     5000},  //2sps
//	{16800,     10000}, //1sps
//};


///* ���������α������ʾ****************************************************************************/
///* ����5.1f��ʽ��ʾ ������222.2us �������� */
//const uint16_t g_CursorUintTable[][2] =
//{  
//	/********5.1F********************************/
//	//����*1000   ������ʾʱ�ĸ�ʽ
//	{357,   1},  // 600 * 0.36us = 216.0us -- 0.36us.
//	{500,   1},  // 600 * 0.5us  = 300.0us -- 0.5us.
//	{1000,  1},  // 600 * 1us    = 600.0us -- 1.0us.
//	
//	/* ���µĳ�����û�зŴ󣬵����Ľ����Ҫ����1000 */
//	/********5.3F********************************/
//	{2,  2},  // 600 * 2us    = 1.200ms -- 0.002ms. 
//	{5,  2},  // 600 * 5us    = 3.000ms -- 0.005ms.
//	{10, 2},  // 600 * 10us   = 6.000ms -- 0.010ms.
//	
//	/********5.2F********************************/
//	{20,  3},  // 600 * 20us   = 12.00ms -- 0.02ms.
//	{50,  3},  // 600 * 50us   = 30.00ms -- 0.05ms.
//	{100, 3},  // 600 * 100us  = 60.00ms -- 0.10ms.
//	
//	/********5.1F********************************/
//	{200,  4},  // 600 * 200us  = 120.0ms -- 0.2ms.
//	{500,  4},  // 600 * 500us  = 300.0ms -- 0.5ms.
//	{1000, 4},  // 600 * 1000us = 600.0ms -- 1.0ms.
//	
//	/********5.3F********************************/
//	{2,  5},  // 600 * 2ms  = 1.200s -- 0.002s.
//	{5,  5},  // 600 * 5ms  = 3.000s -- 0.005s.
//	{10, 5},  // 600 * 10ms = 6.000ms -- 0.010ms.
//	
//	/********5.2F********************************/
//	{20,  6},  // 600 * 20ms   = 12.00s -- 0.02s.
//	{50,  6},  // 600 * 50ms   = 30.00s -- 0.05s.
//	{100, 6},  // 600 * 100ms  = 60.00s -- 0.10s.
//	
//	/********5.1F********************************/
//	{200,  7},  // 600 * 200us  = 120.0s -- 0.2s.
//	{500,  7},  // 600 * 500us  = 300.0s -- 0.5s.
//	{1000, 7},  // 600 * 1000us = 600.0s -- 1.0s.
//};

///* �����ʺͶ�Ӧ������**********************************************************************************/
///* �����ʣ���2Msps��1Ksps */
//const char *g_TimeTable[] =
//{
//	"2.8Msps 357ns",
//	"2Msps   500ns",
//	"1Msps   1us",
//	"500Ksps 2us",
//	"200Ksps 5us",
//	"100Ksps 10us",
//	
//	"50Ksps  20us",
//	"20Ksps  50us",
//	"10Ksps  100us",
//	"5Ksps   200us",
//	"2Ksps   500us",
//	
//	"1Ksps   1ms",
//	"500sps  2ms",
//	"200sps  5ms",
//	"100sps  10ms",
//	"50sps   20ms",

//	"20sps   50ms",
//	"10sps   100ms",
//	"5sps    200ms",
//	"2sps    500ms",
//	"1sps    1s",
//};

///* ��ʾ��ǰʵ��֧�ֵĲ���ֵ******************************************************************************** */
//const char *g_MeasureTable[] =
//{
//	"Snapshot",
//	"Freq",
//	"Period",
//	"RiseTime",
//	"FallTime",
//	"Delay",
//	"Phase",
//	"PPW",
//	"NPW",
//	"PDC",
//	
//	"NDC",
//	"BurWidth",
//	"Pk-Pk",
//	"Amp",
//	"Max",
//	"Min",
//	"High",
//	"Low",
//	"PosOver",
//	"NegOver",
//	
//	"Mean",
//	"CMean",
//	"RMS",
//	"CRMS",
//	"PPC",
//	"NPC",
//	"REC",
//	"FEC",
//	"Area",
//	"Value",
//};

///*
//*********************************************************************************************************
//*	                                  ����
//*********************************************************************************************************
//*/
//WM_HWIN hMotion;           /* �������ھ�� */

//uint16_t TriggerFlag = 0;  /* 0�������Զ�������1,2��������ͨ���� */
//int8_t   TimeBaseId = 1;   /* ������ʱ�� */
//int8_t   Ch1AmpId = 3;     /* 3:��ʾĬ��ѡ��ÿ�������ʾ1V��ģʽ */


//FLAG_T       *g_Flag;      /* ���ܸ��ֱ�־�Ľṹ��ָ����� */
//TRIVOLTAGE_T *g_TrigVol;   /* ���ܸ��ִ���ֵ�Ľṹ��ָ����� */
//CURSORS_T    *g_Cursors;   /* ���ܸ��ֲ����α�Ľṹ��ָ����� */
//DSO_T 	     *g_DSO1;	   /* ����ʾ����ͨ��1�Ľṹ��ָ����� */
//DSO_T        *g_DSO2;      /* ����ʾ����ͨ��2�Ľṹ��ָ����� */

///* FFT������������ */
//float32_t *testInput_fft_2048;
//float32_t *testOutput_fft_2048;

///* ����һ�����η��� */
//GUI_RECT Rect ={DSOSCREEN_STARTX, DSOSCREEN_STARTY, DSOSCREEN_ENDX, DSOSCREEN_ENDY};

//WM_HWIN hTimer;
//GUI_MEMDEV_Handle   hMemDSO;

///*
//*********************************************************************************************************
//*	                                 ���ڻ��ƶ���ε�����
//*********************************************************************************************************
//*/

///* ������ʾ�������ϲ�����ʾ�Ĳο�λ�� */
//const GUI_POINT aPoints[5] = 
//{
//	{  0, -10},
//	{ 22, -10},
//	{ 32,   0},
//	{ 22,  10},
//	{ 0,   10}
//};

///* ������ʾ���Ҳ���ϴ���ֵ��ͷ */
//const GUI_POINT aPointsTrig[7] = 
//{
//	{  0, 0},
//	{ 6, -6},
//	{ 6, -1},
//	{ 12, -1},
//	{ 12, 1},
//	{ 6,  1},
//	{ 6,  6},
//};

///* 2k�����У�������ʾ����ʼλ�� */
//const GUI_POINT aPointsTrigBrowser[3] = 
//{   
//    { -6, 0},
//	{  6, 0},
//	{ 0, 6}
//};

///*
//*********************************************************************************************************
//*	                                 GUI_RECT��������
//*********************************************************************************************************
//*/
//GUI_RECT rClient    = {0,   0,  75, 38};     /* ������ʾʾ������logo */
//GUI_RECT rRunMode   = {76,  0, 140, 38};     /* ������ʾ����״̬�����к���ͣ */
//GUI_RECT rTrigMode  = {141, 0, 205, 38};     /* ������ʾ����ģʽ���Զ���������ͨ���� */
//GUI_RECT rTrigValue = {471, 0, 640, 38};     /* ������ʾ�Զ��������Ĵ������� */
//GUI_RECT rButState  = {621, 0, 799, 38};     /* ��ǰ������Ҫ���ڵ�״̬ */
//GUI_RECT rRefPos    = {0,  39, 39, 439};     /* ʾ��������ನ�βο���λ������ */
//GUI_RECT rTrigPos   = {210, 6, 470, 33};     /* 6KB���ݲ�ѯ���Զ���������ͨ���� */


///*
//*********************************************************************************************************
//*	�� �� ��: _cbBkWin
//*	����˵��: ���洰�ڵĻص����� 
//*	��    ��: pMsg
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//static void _cbBkWin(WM_MESSAGE * pMsg) 
//{
//	int NCode, Id;
//	
//	switch (pMsg->MsgId) 
//	{
//		
//		/* ���汳���ػ� */
//		case WM_PAINT:
//			DSO_Init(0);					
//			break;

//		/* ͨ�����ô�����־������һ�δ����α����ʾ */
//		case WM_TIMER:
//			g_Flag->ucWaveRefresh = 1;
//			//WM_RestartTimer(pMsg->Data.v, 1000);		
//			break;
//			
//		case WM_NOTIFY_PARENT:
//			Id = WM_GetId(pMsg->hWinSrc); 
//			NCode = pMsg->Data.v;        
//			switch (Id) 
//			{
//				 /* ��ť0����Measure�Ի��� */
//				 case GUI_ID_BUTTON0:
//                    switch(NCode)
//                    {
//                        case WM_NOTIFICATION_RELEASED:
//                            DSO_CreateMeasureDlg();
//                            break;
//                    }
//					break;
//					
//				 /* ��ť1����ADC�Ի��� */
//				 case GUI_ID_BUTTON1:
//                    switch(NCode)
//                    {
//                        case WM_NOTIFICATION_RELEASED:
//							DSO_CreateAdcDlg();
//                            break;
//                    }
//					break;
//				
//				/* ��ť2����DAC�Ի��� */
//				 case GUI_ID_BUTTON2:
//                    switch(NCode)
//                    {
//                        case WM_NOTIFICATION_RELEASED:
//							DSO_CreateDacDlg();
//                            break;
//                    }
//					break;
//				
//				/* ��ť3����Math�Ի��� */
//				 case GUI_ID_BUTTON3:
//                    switch(NCode)
//                    {
//                        case WM_NOTIFICATION_RELEASED:
//                            DSO_CreateMathDlg();
//                            break;
//                    }
//					break;
//					
//				 /* ��ť4����Settings�Ի��� */
//				 case GUI_ID_BUTTON4:
//                    switch(NCode)
//                    {
//                        case WM_NOTIFICATION_RELEASED:
//							DSO_CreateSettingsDlg();
//                            break;
//                    }
//					break;
//			}
//			break;
//			
//		default:
//			WM_DefaultProc(pMsg);
//	}
//}

///*
//*********************************************************************************************************
//*	�� �� ��: _DrawWave
//*	����˵��: ���Ʋ��κͲ��εı�����ʾ
//*	��    ��: ��         	
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//static void _DrawWave(void) 
//{
//	uint16_t i;
//	uint32_t temp;
//	GUI_RECT Rect = {40, 40, 640, 440};	

//	/* ����ʾ�������� */	
//	GUI_MULTIBUF_Begin();	
//	
//	/* ����ʾ�������� */		
//	GUI_SetClipRect(&Rect);
//	
//	/* ��1��������ʾ�������� ***********************************************************************/
//	GUI_MEMDEV_WriteAt(hMemDSO, 40, 40);
//	
//	/* ��2����ʵ�����ݵ��������Ա���ʾ��LCD��******************************************************/
//	for(i = 0; i < DSOSCREEN_LENGTH; i++)
//	{
//		temp = g_DSO1->usWaveBuf[i] * 100 / g_AttTable[Ch1AmpId][0];
//	    temp = temp + g_DSO1->usRefPos - 40;
//		g_DSO1->usWaveBuf[i] = 400 - temp + (g_DSO1->usRefPos-240) * 2;
//	}
//	
//	for(i = 0; i < DSOSCREEN_LENGTH; i++)
//	{
//		temp =  g_DSO2->usWaveBuf[i] * 100 / g_AttTable[Ch1AmpId][0];
//	    temp = temp + g_DSO2->usRefPos - 40;
//		g_DSO2->usWaveBuf[i] = 400 - temp + (g_DSO2->usRefPos-240) * 2;
//	}
//	
//	/* ��3������LCD����ʾ����**********************************************************************/
//	GUI_SetColor(GUI_YELLOW);
//	
//	/* ��ͨ�������ʴ�2.8Msps��2Ksps��������ķ�ʽ��ʾ */
//	if(TimeBaseId < 10)
//	{
//		GUI_DrawGraph((I16 * )&g_DSO1->usWaveBuf,   
//					  DSOSCREEN_LENGTH,     
//					  DSOSCREEN_STARTX, 	 
//					  DSOSCREEN_STARTY);  

//		/* ��ͨ����ģʽ��ʾͨ��2���Σ�������������ʾ */
//		if(TriggerFlag == 0)
//		{
//			GUI_SetColor(GUI_GREEN);
//			GUI_DrawGraph((I16 * )g_DSO2->usWaveBuf,   
//						  DSOSCREEN_LENGTH,     
//						  DSOSCREEN_STARTX, 	 
//						  DSOSCREEN_STARTY);  	
//		}
//	}
//	/* �����������ķ�ʽ��ʾ */
//	else
//	{
//		/* С��600��������ʱ��ʾ��ʽ��600������һ���ܹ���ʾ�����ݸ��� */
//		if(g_DSO1->ulSlowRefresh1 < 600)
//		{
//			GUI_DrawGraph((I16 * )&g_DSO1->usWaveBuf[0],   
//	              g_DSO1->ulSlowRefresh1 ,     
//				  DSOSCREEN_STARTX, 	 
//				  DSOSCREEN_STARTY); 
//		}
//		/* ���ڵ���600��������ʱ��ʾ��ʽ */
//		else
//		{
//			GUI_DrawGraph((I16 * )&g_DSO1->usWaveBuf[0],   
//								  600,     
//								  DSOSCREEN_STARTX, 	 
//								  DSOSCREEN_STARTY); 
//		}

//		/* ��ͨ����ģʽ��ʾͨ��2���Σ�������������ʾ */
//		if(TriggerFlag == 0)
//		{
//			/* С��600��������ʱ��ʾ��ʽ��600������һ���ܹ���ʾ�����ݸ��� */
//			GUI_SetColor(GUI_GREEN);
//			if(g_DSO2->ulSlowRefresh1 < 600)
//			{
//				GUI_DrawGraph((I16 * )&g_DSO2->usWaveBuf[0],   
//					  g_DSO2->ulSlowRefresh1 ,     
//					  DSOSCREEN_STARTX, 	 
//					  DSOSCREEN_STARTY); 
//			}
//			/* ���ڵ���600��������ʱ��ʾ��ʽ */
//			else
//			{
//				GUI_DrawGraph((I16 * )&g_DSO2->usWaveBuf[0],   
//									  600,     
//									  DSOSCREEN_STARTX, 	 
//									  DSOSCREEN_STARTY); 
//			}
//		}
//	}
//	
//	/* ��4������LCD����ʾFFT��Ƶ**********************************************************************/
//	/* ���Զ�����ģʽ��������Ϊ2.8Msps��5Kspsʱ����ʾFFT���� */
//	if((g_Flag->ucFFTDisp == 0)&&(TriggerFlag == 0)&&(TimeBaseId < 10))
//	{
//		/* ��FFT�任����ķ�ֵ */
//		for(i = 0; i < DSOSCREEN_LENGTH; i++)
//		{
//			testInput_fft_2048[i] = testInput_fft_2048[i]/1024; 
//		}
//		
//		/* ֱ��������С */
//		testInput_fft_2048[0] = testInput_fft_2048[0]/2;
//		
//		/* ��÷�Ƶ��һ��ת����������LCD����ʾ */
//		for(i = 0; i < DSOSCREEN_LENGTH-2; i++)
//		{
//			testInput_fft_2048[i] = testInput_fft_2048[i]/5.0f;
//			g_DSO1->usWaveBuf[i+2] = 390 - testInput_fft_2048[i] ;
//		}
//		
//		GUI_SetColor(GUI_RED);
//		GUI_DrawGraph((I16 * )&g_DSO1->usWaveBuf[2],   
//					  598,     
//					  DSOSCREEN_STARTX, 	 
//					  DSOSCREEN_STARTY); 
//	}
//	
//	/* ��6������ֱ���������ڲ������ε�ʱ��************************************************************************/
//	if(g_Flag->hWinCursors == 1)
//	{
//		DSO_DrawCursorH();
//	}
//	
//	/* ��7����ˮƽ���������ڲ�����ֵ��ֵ************************************************************************/
//	if(g_Flag->hWinCursors == 2)
//	{
//		DSO_DrawCursorV();
//	}
//	
//	/* ���������������в�������д�� */
//	GUI_SetClipRect(NULL);

//	GUI_MULTIBUF_End();

//	/* ��8������ʾ�����α��λ��*************************************************************************/
//	GUI_SetColor(GUI_YELLOW);
//	
//	/* ��ʾ�����α� */
//	GUI_FillPolygon(&aPointsTrig[0],          /* ָ��Ҫ��ʾ�����Ķ���� */
//	                GUI_COUNTOF(aPointsTrig), /* ���б���ָ���ĵ����� */
//	                627,                      /* ԭ���Xλ�� */
//	                g_TrigVol->usPos);        /* ԭ���Yλ�� */
//	
//	/* �������ô���ֵ����ʾ�α�ˮƽ��1sʱ�� */
//	if((GUI_GetTime() - g_TrigVol->ulTrigTimeLine) < 1000)
//	{
//		GUI_DrawHLine(g_TrigVol->usPos, 40, 626);
//	}
//}

///*
//*********************************************************************************************************
//*	�� �� ��: _cbTouchCalibration
//*	����˵��: ����У׼����
//*	��    ��: pMsg  WM_MESSAGE����ָ�����   
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//static void _cbTouchCalibration(WM_MESSAGE * pMsg) 
//{
//	switch (pMsg->MsgId) 
//	{
//		/* �����ػ� */
//		case WM_PAINT:
//			GUI_SetBkColor(GUI_BLUE);
//			GUI_Clear();
//			break;
//			
//		default:
//			WM_DefaultProc(pMsg);
//	}
//}

///*
//*********************************************************************************************************
//*	�� �� ��: DSO_Graph
//*	����˵��: ���Ʋ��κͲ��εı�����ʾ
//*	��    ��: ��        	
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//static void DSO_Graph(void) 
//{ 
//	uint32_t t0, t1;
//	uint8_t uKeyCode;
//	WM_HWIN hTouchWin;
//	
//	
//	/* �������洰������Ķ�ʱ�����ô�����ѹ�ο��ߵĶ�����ʾ */
//	hTimer = WM_CreateTimer(WM_HBKWIN,  /* ������Ϣ�Ĵ��ڵľ�� */
//				   0, 	        		/* �û������Id���������ͬһ����ʹ�ö����ʱ������ֵ��������Ϊ�㡣 */
//				   1050,        		/* ���ڣ������ڹ���ָ������Ӧ�յ���Ϣ*/
//				   0);	       			/* ��������ʹ�ã�ӦΪ0 */	
//	
//	while(1) 
//	{
//		/* ��¼ˢ��ǰ��ʱ�� */
//		t0 = GUI_GetTime();
//		
//		/* ���źŴ���������Ϣ�������εĴ����FFT */
//		/* ��������Ϣ�������������� */
//		g_Flag->ucDsoMsg = DspFFT2048Pro_15;
//		xEventGroupSetBits(xCreatedEventGroup, g_Flag->ucDsoMsg);
//		
//		/* 
//		   g_Flag->hWinRunStop = 0 ��ʾ����̬��= 1��ʾ��ͣ��
//		   g_ucWaveRefreshFlag ����0����ʾ��ִ�в��������ˢ�£�����1����ʾִ�в��������ˢ�¡�
//		*/
//		if((g_Flag->hWinRunStop == 0)||(g_Flag->ucWaveRefresh == 1))
//		{
//			_DrawWave();
//			g_Flag->ucWaveRefresh = 0;
//		}
//		
//		/* ���ֵ�����ݲ�ͬ��ִ��ʱ��ִ�в�ͬ���ӳ�ʱ�� */
//		t1 = GUI_GetTime() - t0;
//		if (t1 < g_RefreshDelay[TimeBaseId]) 
//		{  
//			GUI_Delay(g_RefreshDelay[TimeBaseId] - t1);
//		}
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		
//		/* ������Ϣ���� */
//		uKeyCode = bsp_GetKey();
//		
//		/* �ж��Ƿ��а������� */
//		if(uKeyCode != KEY_NONE)
//		{
//			switch (uKeyCode)
//			{
//			    /* ����K1 ����ʵ����Ļ��ͼ���� ********************************************************/	
//				case KEY_1_DOWN:
//					/************���ڰ��������ã�����ͼ����ȡ��***********/
//					#if 1
//						hTouchWin = WM_CreateWindowAsChild(0, 
//				                                           0, 
//				                                           800, 
//				                                           480, 
//				                                           WM_HBKWIN, 
//				                                           WM_CF_SHOW, 
//				                                           _cbTouchCalibration, 
//				                                           0);
//						WM_Exec();
//						WM_SelectWindow(hTouchWin);
//				
//						/* ִ�д���У׼ */
//						TOUCH_Calibration();
//				
//						WM_SelectWindow(0);
//				
//						WM_DeleteWindow(hTouchWin);
//						WM_Exec();
//				
//						/* �Զ�������ͣ״̬ */
//						if(g_Flag->hWinRunStop == 1)
//						{
//							g_Flag->ucWaveRefresh = 1;
//						}
//					
//						/* ��ͨ������ͣ״̬ */
//						if(TriggerFlag == 1)
//						{
//							TriggerFlag = 2;
//						}
//					#endif
//					
//					/* ���ڽ�ͼ */
//					#if 0
//						xTaskNotifyGive(xHandleTaskMsgPro); 
//					#endif
//					
//					/* ��ӡ�����ִ����� */						
//					#if 0
//						g_Flag->ucDsoMsg = DspTaskInfo_13;
//						xEventGroupSetBits(xCreatedEventGroup, g_Flag->ucDsoMsg);
//					#endif	
//						
//					break;
//				
//				/* ����K2 ����ʵ�ֲ��ε���ͣ������ ***************************************************/
//				case KEY_2_DOWN:
//					/* TriggerFlag = 0 ��ʾ��ǰ�õ����Զ�����ģʽ������ͣ�����в��� */
//					if(TriggerFlag == 0)
//					{
//						if(g_Flag->hWinRunStop == 0)
//						{
//							g_Flag->hWinRunStop = 1;
//							ADC_DMA_Close();
//						}
//						else
//						{
//							g_Flag->hWinRunStop = 0;						
//							ADC_DMA_Open();
//						}
//					}
//					/* TriggerFlag = 2��3��ʾ��ǰ�õ�����������ģʽ������ͣ�����в��� */
//					else
//					{
//						if(g_Flag->ucTrig == 0)
//						{
//							g_Flag->ucTrig = 1;
//							ADC_DMA_Close();
//						}
//						else
//						{
//							g_Flag->ucTrig = 0;						
//							ADC_DMA_Open();
//						}
//						
//						/* ���ø���һ�� */
//						TriggerFlag = 2;
//					}
//					g_Flag->ucWaveRefresh = 1;
//					WM_InvalidateArea(&rRunMode);
//					break;
//				
//				/* ����K3�����л��Զ���������ͨ���� **********************************************/
//                case KEY_3_DOWN:		
//					/* TriggerFlag = 0 ��ʾ��ǰ�õ����Զ�����ģʽ */
//					if(TriggerFlag == 0)
//					{
//						/* ���Զ������л�����ͨ���� */
//						TriggerFlag = 1;
//						g_Flag->ucTrig = g_Flag->hWinRunStop;
//						g_DSO1->sCurTriStep = 0;
//						
//						/* ���´���λ�� */
//						WM_InvalidateArea(&rTrigPos);
//						
//						/*  ���봥������������ADC����ֵΪ1V���л����������ò�����λ2Msps */
//						WM_SendMessageNoPara(hDlgScale, WM_TextUpDate);
//						
//						/* ���봥�������Ĭ�����ô�����1V��Ҳ����1241 */
//						ADC_AnalogWatchdogThresholdsConfig(ADC3, 1241, 0);
//					}
//					/* TriggerFlag = 2��3��ʾ��ǰ�õ�����������ģʽ */
//					else
//					{
//						/* TriggerFlag = 0 ��ʾ��ǰ�õ����Զ�����ģʽ */
//						TriggerFlag = 0;
//						
//						/* ����״̬ */
//						g_Flag->hWinRunStop = g_Flag->ucTrig;
//						
//						/* ���غ󣬲�����Ҫ���Ź����ô��� */
//						ADC_AnalogWatchdogThresholdsConfig(ADC3, 4095, 0);
//					}
//					
//					/* ���ִ���ģʽ�л��󣬶�����ֵ����ΪĬ�� */
//					g_TrigVol->usPos = 190;     
//					g_TrigVol->usTrigValue = 1241; 
//					WM_InvalidateArea(&rTrigValue);
//					WM_InvalidateArea(&rTrigMode);
//					
//					/* ���л�����ͨ����ģʽ��ʾһ�β��� */
//					if(TriggerFlag == 1)
//					{
//						TriggerFlag = 2;
//					}
//					break;
//             
//				/* ҡ���ϼ����� *************************************************************/
//                case JOY_DOWN_U:	
//					/* ���ڷ��ȷŴ� */
//					if(g_Flag->hWinButState == 1) /* ���ȵ��� */
//					{
//						Ch1AmpId++;
//						if(Ch1AmpId == ATT_COUNT)
//						{
//							Ch1AmpId = ATT_COUNT - 1;
//						}
//						WM_SendMessageNoPara(hDlgAmp, WM_TextUpDate);
//					}
//					/* ���ڲο���ѹ������ */
//					else if(g_Flag->hWinButState == 2) 
//					{
//						g_DSO1->usRefPos -= 10;
//						if(g_DSO1->usRefPos<= 40)
//						{
//							g_DSO1->usRefPos = 40;
//						}
//						WM_InvalidateArea(&rRefPos);
//					}
//					/* ����ˮƽ����A���� */
//					else if(g_Flag->hWinButState == 5)
//					{
//						g_Cursors->sCursorHA -= g_Cursors->usCursorStep;
//						if(g_Cursors->sCursorHA <= 40)
//						{
//							g_Cursors->sCursorHA = 40;
//						}
//					}
//					/* ����ˮƽ����B����  */
//					else if(g_Flag->hWinButState == 6) 
//					{
//						g_Cursors->sCursorHB -= g_Cursors->usCursorStep;
//						if(g_Cursors->sCursorHB <= g_Cursors->sCursorHA)
//						{
//							g_Cursors->sCursorHB = g_Cursors->sCursorHA;
//						}
//					}
//					/* �����Զ���������ͨ�����Ĵ���ֵ */
//					else if(g_Flag->hWinButState == 7) 
//					{
//						/* ����������ͷ��λ�ã�����鿴 */
//						g_TrigVol->usPos -= g_TrigVol->usTriStep;
//						if(g_TrigVol->usPos <= DSOSCREEN_STARTY)
//						{
//							g_TrigVol->usPos = DSOSCREEN_STARTY;
//						}
//						
//						/* ���ݵ��ڵĴ�����ͷλ�û�ô���ֵ */
//						g_TrigVol->ufTrigValue = 240 - g_TrigVol->usPos;
//						g_TrigVol->ufTrigValue = g_TrigVol->ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
//						g_TrigVol->ufTrigValue = g_TrigVol->ufTrigValue * 4095 / 3.3f;
//						g_TrigVol->usTrigValue = g_TrigVol->ufTrigValue;
//						
//						/* ���´���ֵ */
//						WM_InvalidateArea(&rTrigValue);
//						
//						/* ���º󣬸���һ�²�����ʾ��������ͨ������ʽ */
//						if(TriggerFlag != 0)
//						{
//							TriggerFlag = 2;
//							
//							/* �������ͨ����ģʽ������ADC3�Ŀ��Ź���ֵ */
//							ADC_AnalogWatchdogThresholdsConfig(ADC3, g_TrigVol->usTrigValue, 0);
//						}
//						
//						/* ���´���ֵ����ʾ����õ�ǰ��ʱ�䣬�Ӷ�ʵ�ֵ��ڴ���ֵ������һ��ˮƽ��ֱ�ߣ�ʱ��1s */
//						g_TrigVol->ulTrigTimeLine = GUI_GetTime();
//						WM_RestartTimer(hTimer, 1050);	
//					}
//					
//					/* ʹ��K2������������ͣ��ͨ������g_ucWaveRefreshFlag��1���в���ˢ�� */
//					if(g_Flag->hWinRunStop == 1)
//					{
//						g_Flag->ucWaveRefresh = 1;  
//					}
//					break;
//					
//				/* ҡ���¼����� *************************************************************/
//				case JOY_DOWN_D:	
//					/* ���ڷ���˥�� */
//					if(g_Flag->hWinButState == 1) 
//					{
//						Ch1AmpId--;
//						if(Ch1AmpId < 0)
//						{
//							Ch1AmpId = 0;
//						}
//						WM_SendMessageNoPara(hDlgAmp, WM_TextUpDate);
//					}
//					/* ���ڲο���ѹ������ */
//					else if(g_Flag->hWinButState == 2) 
//					{
//						g_DSO1->usRefPos += 10;
//						if(g_DSO1->usRefPos >= 440)
//						{
//							g_DSO1->usRefPos = 440;
//						}
//						WM_InvalidateArea(&rRefPos);
//					}
//					/* ����ˮƽ����A���� */
//					if(g_Flag->hWinButState == 5) 
//					{
//						g_Cursors->sCursorHA += g_Cursors->usCursorStep;
//						if(g_Cursors->sCursorHA >= g_Cursors->sCursorHB)
//						{
//							g_Cursors->sCursorHA = g_Cursors->sCursorHB;
//						}
//					}
//					 /* ����ˮƽ����B����  */
//					else if(g_Flag->hWinButState == 6)
//					{
//						g_Cursors->sCursorHB += g_Cursors->usCursorStep;
//						if(g_Cursors->sCursorHB >= 440)
//						{
//							g_Cursors->sCursorHB = 440;
//						}
//					}
//					/* �����Զ���������ͨ�����Ĵ���ֵ */
//					else if(g_Flag->hWinButState == 7) 
//					{
//						/* ����������ͷ��λ�ã�����鿴 */
//						g_TrigVol->usPos += g_TrigVol->usTriStep;
//						if(g_TrigVol->usPos >= DSOSCREEN_ENDY)
//						{
//							g_TrigVol->usPos = DSOSCREEN_ENDY;
//						}
//						
//						/* ���ݵ��ڵĴ�����ͷλ�û�ô���ֵ */
//						g_TrigVol->ufTrigValue = 240 - g_TrigVol->usPos;
//						g_TrigVol->ufTrigValue = g_TrigVol->ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
//						g_TrigVol->ufTrigValue = g_TrigVol->ufTrigValue * 4095 / 3.3f;
//						g_TrigVol->usTrigValue = g_TrigVol->ufTrigValue;
//						
//						WM_InvalidateArea(&rTrigValue);
//						
//						/* ���º󣬸���һ�²�����ʾ��������ͨ������ʽ */
//						if(TriggerFlag != 0)
//						{
//							TriggerFlag = 2;
//							/* �������ͨ����ģʽ������ADC3�Ŀ��Ź���ֵ */
//							ADC_AnalogWatchdogThresholdsConfig(ADC3, g_TrigVol->usTrigValue, 0);
//						}
//						
//						/* ���´���ֵ����ʾ����õ�ǰ��ʱ�䣬�Ӷ�ʵ�ֵ��ڴ���ֵ������һ��ˮƽ��ֱ�ߣ�ʱ��1s */
//						g_TrigVol->ulTrigTimeLine = GUI_GetTime();
//						WM_RestartTimer(hTimer, 1050);	
//					}
//					
//					/* ʹ��K2������������ͣ��ͨ������g_Flag->ucWaveRefresh��1���в���ˢ�� */
//					if(g_Flag->hWinRunStop == 1)
//					{
//						g_Flag->ucWaveRefresh = 1;  
//					}
//					break;
//					
//				/* ҡ��������� *************************************************************/
//				case JOY_DOWN_L:
//					/* ����ADC�Ĳ����� */
//					if(g_Flag->hWinButState == 0) 
//					{
//						TimeBaseId++;
//						if(TimeBaseId == TIME_COUNT)
//						{
//							TimeBaseId = TIME_COUNT - 1;
//						}
//						ADC_DMA_Close();
//						ADC_DMA_Open();
//						
//						/* ����ǵ�Ƶ��������ʾ��Ҫ��¼��ǰ��DMA������λ�� */
//						g_DSO1->ulSlowRefresh0 = 10240 - DMA2_Stream1->NDTR;
//						g_DSO2->ulSlowRefresh0 = 10240 - DMA2_Stream0->NDTR;
//						g_DSO1->ulSlowRefresh1 = 0;
//						g_DSO2->ulSlowRefresh1 = 0;						
//						
//						/* ���µ�ǰ����ʱ����ʾ */
//						WM_SendMessageNoPara(hDlgScale, WM_TextUpDate);
//						
//						/* 
//						   g_DSO1->ucSlowWaveFlag��g_DSO2->ucSlowWaveFlag = 0 ��ʾ����������ʾģʽ��
//						   Ҳ���ǹ�����ʾ�ڵ�Ƶ����ʱ�����������0����Ч������Ϊ�˷���ֱ������Ϊ0��
//						*/
//						g_DSO1->ucSlowWaveFlag = 0;
//						g_DSO2->ucSlowWaveFlag = 0;
//					}
//					/* ���ڴ�ֱ��A���� */
//					else if(g_Flag->hWinButState == 3) 
//					{
//						g_Cursors->sCursorVA -= g_Cursors->usCursorStep;
//						if(g_Cursors->sCursorVA <= 40)
//						{
//							g_Cursors->sCursorVA = 40;
//						}
//					}
//					/* ���ڴ�ֱ��B���� */
//					else if(g_Flag->hWinButState == 4) 
//					{
//						g_Cursors->sCursorVB -= g_Cursors->usCursorStep;
//						if(g_Cursors->sCursorVB <= g_Cursors->sCursorVA)
//						{
//							g_Cursors->sCursorVB = g_Cursors->sCursorVA;
//						}
//					}
//					/* �Զ�����ģʽ����ͨ����ģʽ2K���ݵ���ʾ��ʼλ���趨 */
//					else if(g_Flag->hWinButState == 7) 
//					{
//						/* �Զ�����ģʽ��2K���ݵ���ʾ��ʼλ���趨 */
//						if(TriggerFlag == 0)
//						{
//							/* ����100������ */
//							g_DSO1->sCurTriStep -= 100;	
//						}
//						/* ��ͨ����ģʽ��6K���ݵ���ʾ��ʼλ���趨 */
//						else
//						{
//							/* ����100������ */
//							g_DSO1->sCurTriStep -= 100;
//							/* 
//							   ��������ģʽ֧�ֵ�������ʾ��2K=2048��Ĭ����ʾ�������м�λ�ö�Ӧ���ݵ�
//							   ���м�λ�ã�����һ��������ʾ600�ֽڣ������������ƫ��-(1024-300) = -724�ֽڡ�
//							*/
//							if(g_DSO1->sCurTriStep <= -724)
//							{
//								g_DSO1->sCurTriStep = -724;
//							}
//							
//							/* ͨ������Ϊ2�����²�����ʾ */
//							TriggerFlag = 2;
//						}
//					
//						/* ���µ�ǰ������λ�õļ�ͷ */
//						WM_InvalidateArea(&rTrigPos);
//					}
//			
//					/* ʹ��K2������������ͣ��ͨ������g_Flag->ucWaveRefresh��1���в���ˢ�� */
//					if(g_Flag->hWinRunStop == 1)
//					{
//						g_Flag->ucWaveRefresh = 1;  
//					}
//					break;
//					
//				/* ҡ���Ҽ����� *************************************************************/
//				case JOY_DOWN_R:
//					/* ����ADC�Ĳ����� */
//					if(g_Flag->hWinButState == 0) 
//					{
//						TimeBaseId--;
//						if(TimeBaseId < 0)
//						{
//							TimeBaseId = 0;
//						}
//						
//						ADC_DMA_Close();
//						ADC_DMA_Open();
//						
//						/* ����ǵ�Ƶ��������ʾ��Ҫ��¼��ǰ��DMA������λ�� */
//						g_DSO1->ulSlowRefresh0 = 10240 - DMA2_Stream1->NDTR;
//						g_DSO2->ulSlowRefresh0 = 10240 - DMA2_Stream0->NDTR;
//						g_DSO1->ulSlowRefresh1 = 0;
//						g_DSO2->ulSlowRefresh1 = 0;
//						
//						/* ���µ�ǰ����ʱ����ʾ */
//						WM_SendMessageNoPara(hDlgScale, WM_TextUpDate);
//						
//						/* 
//						   g_DSO1->ucSlowWaveFlag��g_DSO2->ucSlowWaveFlag = 0 ��ʾ����������ʾģʽ��
//						   Ҳ���ǹ�����ʾ�ڵ�Ƶ����ʱ�����������0����Ч������Ϊ�˷���ֱ������Ϊ0��
//						*/
//						g_DSO1->ucSlowWaveFlag = 0;
//						g_DSO2->ucSlowWaveFlag = 0;
//					}
//					/* ���ڴ�ֱ��A���� */
//					else if(g_Flag->hWinButState == 3) 
//					{
//						g_Cursors->sCursorVA += g_Cursors->usCursorStep;
//						if(g_Cursors->sCursorVA >= g_Cursors->sCursorVB)
//						{
//							g_Cursors->sCursorVA = g_Cursors->sCursorVB;
//						}
//					}
//					/* ���ڴ�ֱ��B���� */
//					else if(g_Flag->hWinButState == 4)
//					{
//						g_Cursors->sCursorVB += g_Cursors->usCursorStep;
//						if(g_Cursors->sCursorVB >= 640)
//						{
//							g_Cursors->sCursorVB = 640;
//						}
//					}
//					/* �Զ�����ģʽ����ͨ����ģʽ6K���ݵ���ʾ��ʼλ���趨 */
//					else if(g_Flag->hWinButState == 7) 
//					{
//						/* �Զ�����ģʽ��6K���ݵ���ʾ��ʼλ���趨 */
//						if(TriggerFlag == 0)
//						{
//							/* ����100������ */
//							g_DSO1->sCurTriStep += 100;						
//						}
//						/* ��ͨ����ģʽ��2K���ݵ���ʾ��ʼλ���趨 */
//						else
//						{
//							/* ����100������ */
//							g_DSO1->sCurTriStep += 100;
//							
//							/* 
//							   ��������ģʽ֧�ֵ�������ʾ��2K=2048��Ĭ����ʾ�������м�λ�ö�Ӧ���ݵ�
//							   ���м�λ�ã�����һ��������ʾ600�ֽڣ������������ƫ��1024-300 =724�ֽڡ�
//							*/
//							if(g_DSO1->sCurTriStep >= 724) 
//							{
//								g_DSO1->sCurTriStep = 724;
//							}
//							
//							/* ͨ������Ϊ2�����²�����ʾ */
//							TriggerFlag = 2;
//						}
//						
//						/* ���µ�ǰ������λ�õļ�ͷ */
//						WM_InvalidateArea(&rTrigPos);
//					}
//					
//					/* ʹ��K2������������ͣ��ͨ������g_Flag->ucWaveRefresh��1���в���ˢ�� */
//					if(g_Flag->hWinRunStop == 1)
//					{
//						g_Flag->ucWaveRefresh = 1;  
//					}
//					break;

//				/* ҡ��OK������ *************************************************************/
//				case JOY_DOWN_OK:
//					/* OK������ʵ��8�ֹ��ܵ��л� */
//					g_Flag->hWinButState++;
//					if(g_Flag->hWinButState == 8)
//					{
//						g_Flag->hWinButState = 0;
//					}
//					WM_InvalidateArea(&rButState);
//					break;
//			}		
//		}
//	}
//}

///*
//*********************************************************************************************************
//*	�� �� ��: _cbMotion
//*	����˵��: Motion���ڵĻص���������Ҫ������ͼ��Ļ�������
//*	��    ��: pMsg  WM_MESSAGE����ָ�����   
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//static void _cbMotion(WM_MESSAGE * pMsg) 
//{
//	WM_MOTION_INFO * pInfo;
//	WM_HWIN          hWin = pMsg->hWin;
//	PARA           * pPara;

//	switch (pMsg->MsgId) 
//	{
//		case WM_MOTION:
//			WM_GetUserData(hWin, &pPara, sizeof(pPara));
//			pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
//			switch (pInfo->Cmd) 
//			{
//				/* Motion���ܳ�ʼ��������X�����Y����֧�ֻ��� */
//				case WM_MOTION_INIT:
//					pInfo->Flags =  WM_CF_MOTION_X | WM_CF_MOTION_Y | WM_MOTION_MANAGE_BY_WINDOW;
//					pInfo->SnapY = 1;
//					pInfo->SnapX = 1;
//					break;

//				case WM_MOTION_MOVE:
//					pPara->FinalMove = pInfo->FinalMove;
//					/* Y�᷽�����»��� */
//					if(g_Flag->ucMotionXY == 0)
//					{
//						if((pInfo->pState->Pressed == 1)&&(pInfo->pState->x <= 340))
//						{
//							pPara->iCH1Pos += pInfo->dy;
//							if(pPara->iCH1Pos >= 440)
//							{
//								pPara->iCH1Pos = 440;
//							}
//							if(pPara->iCH1Pos <= 40)
//							{
//								pPara->iCH1Pos = 40;
//							}

//							g_DSO1->usRefPos = pPara->iCH1Pos;
//						}
//						else if((pInfo->pState->Pressed == 1)&&(pInfo->pState->x > 340))
//						{						
//							pPara->iCH2Pos += pInfo->dy;
//							if(pPara->iCH2Pos >= 440)
//							{
//								pPara->iCH2Pos = 440;
//							}
//							if(pPara->iCH2Pos <= 40)
//							{
//								pPara->iCH2Pos = 40;
//							}

//							g_DSO2->usRefPos = pPara->iCH2Pos;
//						}

//						WM_InvalidateArea(&rRefPos);
//					}
//					/* X�᷽�����һ��� */
//					else
//					{
//						if(pInfo->pState->Pressed == 1)
//						{
//							g_DSO1->sCurTriStep -= pInfo->dx;
//						}
//						else if(pInfo->pState->Pressed == 1)
//						{	
//							g_DSO1->sCurTriStep -= pInfo->dx;						
//						}

//						if(TriggerFlag == 0)
//						{

//						}
//						else
//						{
//							if(g_DSO1->sCurTriStep <= -724)
//							{
//								g_DSO1->sCurTriStep = -724;
//							}

//							if(g_DSO1->sCurTriStep >= 724) 
//							{
//								g_DSO1->sCurTriStep = 724;
//							}

//							TriggerFlag = 2;
//						}

//						WM_InvalidateArea(&rTrigPos);
//					}

//					/* ���µ�ǰ������λ�õļ�ͷ */
//					g_Flag->ucWaveRefresh = 1;
//					break;

//				case WM_MOTION_GETPOS:
//					pInfo->yPos = pPara->iCH1Pos;
//					//pInfo->xPos = pPara->iCH1Pos;
//					break;
//			}
//			break;
//	}
//}

///*
//*********************************************************************************************************
//*	�� �� ��: MainTask
//*	����˵��: GUI������
//*	��    ��: ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void MainTask(void) 
//{
//	PARA Para;
//	PARA *pPara;
//	
//	/* ��1����������ʼ�� ***********************************************************************/
//	pPara = &Para;
//    pPara->iCH1Pos = 140;
//    pPara->iCH2Pos = 340;

//	/* ����Y��λ�� */
//	g_DSO1->usRefPos = 140;
//	g_DSO2->usRefPos = 340;	

//	/* �Զ���������ͨ����ֵλ�� */
//	g_TrigVol->usPos = 190;
//	g_TrigVol->usTriStep =1;
//	g_TrigVol->usTrigValue = 1241; /* ADC����ֵ1241��Ӧ1V */
//    g_TrigVol->ulTrigTimeLine = 1000;
//	g_DSO1->sCurTriStep = 0;
//	
//	/* FIR���� */
//	g_DSO1->ucFirFlter_Step100KHz = 0;
//	g_DSO1->ucFirFlter_Step10KHz = 0;
//	g_DSO1->ucFirFlter_Step1KHz = 0;

//	/* ��ֱ��ˮƽ�����α����� */
//	g_Cursors->usCursorStep = 5;
//	g_Cursors->sCursorVA = 190;   
//	g_Cursors->sCursorVB = 490;
//	g_Cursors->sCursorHA = 90;
//	g_Cursors->sCursorHB = 390;

//	g_Cursors->WaveCursorA = 0;
//	g_Cursors->WaveCursorB = 0;
//	
//	/* ������Ĭ����ʾ���Ƿ��ֵ�����ֵ����Сֵ��ƽ��ֵ��RMS��Freq */
//	g_DSO1->ucMeasureFlag[1] = 1;
//	g_DSO1->ucMeasureFlag[12] = 1;
//	g_DSO1->ucMeasureFlag[14] = 1;
//	g_DSO1->ucMeasureFlag[15] = 1;
//	g_DSO1->ucMeasureFlag[22] = 1;
//	g_DSO1->ucMeasureFlag[24] = 1;
//	
//	/* ˢ�����ٲ������ñ�־ */
//	g_DSO1->ucSlowWaveFlag = 0;
//	g_DSO2->ucSlowWaveFlag = 0;
//	
//	/* ȫ�ֱ�־��ʼ��*/
//	g_Flag->ucMotionXY = 0;      
//	g_Flag->ucFFTDisp = 0;     
//	g_Flag->hWinCursors = 0;    
//	g_Flag->hWinRunStop = 0;    
//	g_Flag->ucTrig = 0;
//	g_Flag->hWinButState = 0;  	
//	g_Flag->ucWaveRefresh = 0; 
//	
//	/* ��2����emWin��ʼ�� ***********************************************************************/
//	GUI_Init();

//	/*
//       �ȹرձ��⣬
//	   ע��LCDConf_Lin_Template.c�ļ��к���_LCD_DisplayOn�Ѿ�ע�͵�����򿪺�����
//	*/
//	LCD_SetBackLight(0);
//	
//	/* �������洰�ڵĻص����� */
//	WM_SetCallback(WM_HBKWIN, _cbBkWin);
//	
//	/*
//	 ���ڶ໺��ʹ����ڴ��豸������˵��
//	   1. ʹ�ܶ໺���ǵ��õ����º������û�Ҫ��LCDConf_Lin_Template.c�ļ��������˶໺�壬���ô˺�������Ч��
//		  WM_MULTIBUF_Enable(1);
//	   2. ����ʹ��ʹ���ڴ��豸�ǵ��ú�����WM_SetCreateFlags(WM_CF_MEMDEV);
//	   3. ���emWin�����ö໺��ʹ����ڴ��豸��֧�֣���ѡһ���ɣ����������ѡ��ʹ�ö໺�壬ʵ��ʹ��
//		  STM32F429BIT6 + 32λSDRAM + RGB565/RGB888ƽ̨���ԣ��໺�������Ч�Ľ��ʹ����ƶ����߻���ʱ��˺��
//		  �У�����Ч����������ԣ�ͨ��ʹ�ܴ���ʹ���ڴ��豸���������ġ�
//	   4. ����emWin����Ĭ���ǿ��������塣
//	*/
//	WM_MULTIBUF_Enable(1);
//	
//	/* ʹ�ܻ��� */ 
//	WM_MOTION_Enable(1);     
//	WM_MOTION_SetDefaultPeriod(100);
//	
//	/*
//       ����У׼����Ĭ����ע�͵��ģ���������ҪУ׼������������У׼������û���ҪУ׼�������Ļ���ִ��
//	   �˺������ɣ��Ὣ����У׼�������浽EEPROM���棬�Ժ�ϵͳ�ϵ���Զ���EEPROM������ء�
//	*/
//    //TOUCH_Calibration();

//	/* ��3�������Ƹ��ֱ������洢�豸 ***************************************************/
//	hMemDSO = GUI_MEMDEV_CreateFixed(0, 
//	                                 0, 
//	                                 600, 
//	                                 400, 
//									 GUI_MEMDEV_HASTRANS, 
//									 GUI_MEMDEV_APILIST_16, 
//									 GUICC_M565);
//	
//	/* ����ʾ�������ڱ��� */
//	GUI_MEMDEV_Select(hMemDSO);
//    DSO_DrawBakFrame(0,0,599,399);
//	GUI_MEMDEV_Select(0);
//	
//	/* ����ϵͳ��Ϣ���ڱ��� */
//	DrawWinSysBk();
//	
//	/* ���Ʒ�ֵ���ڱ��� */	
//	DrawWinAmpBk();
//	
//	/* ����״̬���ڱ��� */
//	DrawWinStatus1Bk();
//	DrawWinStatus2Bk();
//	
//	/* ����Ƶ�����ô��� */
//	DrawWinScaleBk();
//	
//	/* ��3������֤�������������ʾ ***************************************************/
//	WM_HideWin(WM_HBKWIN);
//	
//	/* ��ʼ�� DSO */
//	DSO_Init(1);
//	
//	WM_ShowWin(WM_HBKWIN);


//	/* ��4������һ��͸���Ļ������ڣ�ʵ��ˮƽ�ʹ�ֱ���� ******************************/
//	hMotion = WM_CreateWindowAsChild(40, 
//	                                 40, 
//									 600, 
//									 400, 
//					                 WM_HBKWIN,  
//								     WM_CF_MOTION_Y | WM_CF_SHOW | WM_CF_HASTRANS, 
//									 _cbMotion, 
//									 sizeof(pPara));
//									 
//    WM_SetUserData(hMotion, &pPara, sizeof(pPara));

//	/* ��ʾ�����еĴ��� */
//	GUI_Exec();
//	
//	/* �ٿ������� */
//	LCD_SetBackLight(255);	
//	
//	/* ��5��������ʾ�ʹ��� ************************************************************/
//	DSO_Graph();		
//}

///***************************** ���������� www.armfly.com (END OF FILE) *********************************/
