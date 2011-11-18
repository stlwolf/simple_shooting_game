#if !defined(__GAME__MAIN)
#define	__GAME__MAIN

/********************************************************
	インクルードファイル
********************************************************/

#include "../winsprog/main.h"
#include "../GL_Utility/Expand.h"
#include "ObjData.h"
#include "ObjRead.h"
#include "Player.h"
#include "PlayerAttack.h"
#include "Target.h"
#include "Field.h"

// ネットワーク追加ヘッダー
#include "../Class/CNetClient.h"

/********************************************************
	構造体定義
********************************************************/

struct FLAG {

	int m_OpeningFlag;
	int m_EndingFlag;

};

/********************************************************
	マクロ定義
********************************************************/

/********************************************************
	グローバル変数宣言
********************************************************/

extern DWORD fpsChk1;								// 処理開始時刻
extern DWORD fpsChk2;								// 処理終了時刻
extern int	 fpsCnt;

extern float g_Stringx, g_Stringy;					// 文字表示位置

extern OBJ_VARIABLE	   *g_pObjData;

extern OBJ_VERTEX_BUF  *g_pObjBuf;
extern OBJ_VERTEX_BUF2 *g_pObjBuf2;

extern GLUquadricObj  *globj;

extern GLuint g_nVerbufNo[3];

extern FLAG g_Flag;

extern TTF_TEXT g_Ttftext[6];

/********************************************************
	関数プロトタイプ宣言
********************************************************/

extern int Init_OpenGL( void );
extern BOOL Init_Game( void );

extern BOOL Game_Main( HWND ); 
extern void Move_Main( void );

extern void Display_Main( void );

extern BOOL Opening_Display( void );
extern BOOL Ending_Display( void );

extern int WGL_Begin( void );
extern int WGL_End( void );

#endif

