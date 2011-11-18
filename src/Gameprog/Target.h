#if !defined(___TARGET___H)
#define ___TARGET___H

/**********************************************************
	インクルードファイル
***********************************************************/

#include "../Winsprog/MatrixCal.h"
#include "Gamemain.h"

/**********************************************************
	構造体定義
**********************************************************/

struct TARGET {

	// オブジェクト共通要素構造体
	OBJECT_BASE Obj;

	float m_radius;											// ターゲット半径値

	int m_nLifeFlag;										// ターゲット生存フラグ

};

/**********************************************************
	マクロ定義
**********************************************************/

#define MAX_TARGET (10)										// 最大ターゲット数

/**********************************************************
	グローバル変数宣言
**********************************************************/

extern float R, B, G;										// ターゲットＲＧＢ値

extern TARGET g_Target[MAX_TARGET];

/**********************************************************
	関数プロトタイプ宣言
**********************************************************/

extern void Target_Init( void );
extern void Target_Display( void );

#endif	// #if !defined(___??___?)

