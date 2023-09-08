#pragma once

//----ͼƬ------------------------------------
//����ͼ
#define BACKGROUND_WIDTH    600
#define BACKGROUND_HEIGHT   800

//��ҷɻ�
#define PLAYER_WIDTH    60
#define PLAYER_HEIGHT   60

#define GUNNER_WIDTH    6
#define GUNNER_HEIGHT   20

#define FOEBIG_WIDTH    150
#define FOEBIG_HEIGHT   100

#define FOEMID_WIDTH    80
#define FOEMID_HEIGHT   60


#define FOESMA_WIDTH    60
#define FOESMA_HEIGHT   40


//----------------------------------------

//������ز���
#define WINDOW_X   400
#define WINDOW_Y   100
#define WINDOW_WIDTH    (BACKGROUND_WIDTH+16)
#define WINDOW_HEIGHT   (BACKGROUND_HEIGHT+39)
#define WINDOW_TITLE     L"�ɻ���ս" 



//-------��ʱ��-----------------------------------------

#define TIMER_BACK_MOVE_ID         1
#define TIMER_BACK_MOVE_INTERVAL   100
#define TIMER_BACK_MOVE_STEP       3

//��ʱ��ⷽ����Ƿ���
#define TIMER_CHECK_DIRECTION_ID         2
#define TIMER_CHECK_DIRECTION_INTERVAL   30
#define TIMER_PLAYER_MOVE_STEP           4

//��ʱ�����ڵ�
#define TIMER_SEND_GUNNER_ID         3
#define TIMER_SEND_GUNNER_INTERVAL   800

//�ڵ��ƶ�
#define TIMER_GUNNER_MOVE_ID         4
#define TIMER_GUNNER_MOVE_INTERVAL   80
#define TIMER_GUNNER_MOVE_STEP       12

//�������ֵ��˷ɻ��Ķ�ʱ��
#define TIMER_CREATE_FOEBIG_ID         5
#define TIMER_CREATE_FOEBIG_INTERVAL   12000

#define TIMER_CREATE_FOEMID_ID         6
#define TIMER_CREATE_FOEMID_INTERVAL   9000

#define TIMER_CREATE_FOESMA_ID         7
#define TIMER_CREATE_FOESMA_INTERVAL   6000


#define TIMER_FOE_MOVE_ID            8
#define TIMER_FOE_MOVE_INTERVAL      50
#define TIMER_FOEBIG_MOVE_STEP       2
#define TIMER_FOEMID_MOVE_STEP       4
#define TIMER_FOESMA_MOVE_STEP       7


#define TIMER_CHECK_HIT_ID            9
#define TIMER_CHECK_HIT_INTERVAL      5

//��ʱ�л���ըͼƬ
#define TIMER_CHANGE_PIC_ID            10
#define TIMER_CHANGE_PIC_INTERVAL      300




#define DEF_GUNNER_HURT    1
#define DEF_FOEBIG_BLOOD   5
#define DEF_FOEMID_BLOOD   3
#define DEF_FOESMA_BLOOD   1