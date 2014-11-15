//#pragma once
#ifdef __APPLE__
	#define WINDOW_WIDTH 1200
	#define WINDOW_HEIGHT 720
#else 
	#define WINDOW_WIDTH 2500
	#define WINDOW_HEIGHT 1500
	#define M_PI (float)3.1415926
	//#define INF 3.36e38
	#define INF 100000
#endif


/************************/
#define STAGE PLANE
#define VFAR 5000
#define KEY_SHOOT 's'
#define KEY_BOMB  'x'
#define KEY_CHANGE_WEAPON 'c'
#define KEY_CHANGE_VIEWPOINT 'a'
#define KEY_CHANGE_STAGE GLUT_KEY_F4
#define KEY_RESET GLUT_KEY_F5
//#define INF 1000
#define NUM_OF_DAMY 9
#define GAMEMODE 0//GAMEMODE_SINGLE|GAMEMODE_LIFE
/************************/

//物理情報
#define GRAVITY (float)9.8		//重力
#define DT (float)0.05			//ステップ
#define MEW (float)0.2	//動摩擦係数



//キーの状態
#define KEYSTAT_IDLE	0x00000001
#define KEYSTAT_GO		0x00000002
#define KEYSTAT_BACK	0x00000004
#define KEYSTAT_LEFT	0x00000008
#define KEYSTAT_RIGHT	0x00000010
#define KEYSTAT_BOOST	0x00000020
#define KEYSTAT_BOMB 	0x00000040
#define KEYSTAT_CVIEW	0x00000080
#define KEYSTAT_CWEAP   0x00000100
#define KEYSTAT_FLAG1	0x00000200	//視点切り替え
#define KEYSTAT_FLAG2	0x00000400	//武器切り替え
#define KEYSTAT_FLAG3	0x00000800	//ジャンプ用のフラグ
#define KEYSTAT_SHOOT	0x00001000


