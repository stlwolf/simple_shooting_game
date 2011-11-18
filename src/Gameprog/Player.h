#if !defined(___PLAYER___H)
#define ___PLAYER___H

/********************************************************
	インクルードファイル
********************************************************/

#include "../Winsprog/main.h"
#include "../Winsprog/MatrixCal.h"

#include "../Gameprog/PlayerAttack.h"

/********************************************************
	マクロ定義
********************************************************/

#define MAX_ATTACK (20)

/********************************************************
	構造体定義
********************************************************/

struct ATTACK {																// プレイヤー弾構造体

	float m_radius[MAX_ATTACK];												// プレイヤー弾半径

	// プレイヤー弾の半径をスカラー倍する（引数は倍率と計算を行う弾の配列番号）
	void Radius_Multiplay( float num, int no ) { m_radius[no] *= num; }

	VECTOR4 m_vApos[MAX_ATTACK], m_vAmov[MAX_ATTACK];						// プレイヤー弾の位置ベクトルと方向ベクトル

	int m_AttackFlag[MAX_ATTACK];											// プレイヤー弾の生存フラグ
	int m_FlagCounter;														// プレイヤー弾のフ生存フラグ数カウンター

	int m_LifeCounter[MAX_ATTACK];											// プレイヤー弾の跳ね返り回数カウンター

	float Rcolor[MAX_ATTACK], Gcolor[MAX_ATTACK], Bcolor[MAX_ATTACK];		// プレイヤー弾のＲＧＢ値情報
	
};

struct PLAYER {

	// オブジェクト共通要素構造体
	OBJECT_BASE Obj;
	
	// プレイヤー弾構造体変数
	ATTACK PAttack;

	int m_nCameraNo;														// カメラモード変数

	VECTOR4 m_vEye[3], m_vLook[3], m_vUp[3];								// 視点、注視点、Upベクトル

	// コンストラクタ関数
	// 引数として渡された値をスピード値にセットする
	PLAYER ( float speed ) {

		Obj.m_Speed = speed;
	
	}

};

/********************************************************
	グローバル変数宣言
********************************************************/

extern PLAYER g_Player;

/********************************************************
	関数プロトタイプ宣言
********************************************************/

extern void Player_Init( void );

extern void Player_Move( void );
extern void Player_Display( void );

extern void Player_Camera( int camerano );

#endif	// #if !defined(___PLAYER___H)
