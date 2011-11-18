
/**********************************************************
	インクルードファイル
**********************************************************/

#include "Target.h"

/**********************************************************
	グローバル変数
**********************************************************/

// ターゲットのＲＧＢ値変数
float R = 0.0f;
float G = 1.0f;
float B = 0.0f;

// ターゲット構造体
TARGET g_Target[MAX_TARGET];

/**********************************************************
	ターゲット初期化関数

	引数：なし　戻り値：なし

	ターゲットを生成するための初期化を行う関数。
	半径、位置座標（ランダム値）の初期化を行い、生存フラグ
	を1に設定する。
**********************************************************/

void Target_Init( void )
{

	// 最大ターゲット数分ループを行う
	for ( int count = 0; count < MAX_TARGET; count++ ) {

		// ターゲットの半径を設定初期化する
		g_Target[count].m_radius = 4.0f;

		float x, y, z;

		// ランダムのＸ・Ｙ・Ｚ値を生成
		x = ( rand() % 150 ) - 75;
		y = ( rand() % 110 ) - 55;
		z = ( rand() % 150 ) - 75;

		// ランダムに算出した値をターゲットの位置座標にセット
		g_Target[count].Obj.m_vPos.m_x = x;
		g_Target[count].Obj.m_vPos.m_y = y;
		g_Target[count].Obj.m_vPos.m_z = z;

		// ターゲット生存フラグを1でセット
		g_Target[count].m_nLifeFlag = 1;


	}

}

/**********************************************************
	ターゲット描画関数

	引数：なし　戻り値：なし

	生存フラグが1のターゲットの描画処理を行う関数。
	色の設定、それぞれのターゲット位置座標を元に描画を
	行っている。
**********************************************************/

void Target_Display( void )
{

	// 最大ターゲット数分ループを行う
	for ( int count = 0; count < MAX_TARGET; count++ ) { 

		// 生存フラグが1のターゲットならば処理を実行
		if ( g_Target[count].m_nLifeFlag == 1 ) {

			glPushMatrix();

			// ターゲットの色を設定
			glColor3f( R, G, B );

			// ターゲットの位置座標に平行移動処理
			glTranslatef( g_Target[count].Obj.m_vPos.m_x, 
						  g_Target[count].Obj.m_vPos.m_y,
						  g_Target[count].Obj.m_vPos.m_z );

			// ターゲットの表示処理
			gluSphere( globj, g_Target[count].m_radius, 20, 20 );

			glPopMatrix();

		}

	}
	
}