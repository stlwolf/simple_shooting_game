
/**********************************************************
	インクルードファイル
**********************************************************/

#include "PlayerAttack.h"

/**********************************************************
	グローバル変数
**********************************************************/

static float s_fColorCode[][3] = {
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f }
};

static float s_fNumMulti[4] = { 0.0f, 3.0f, 4.0f, 5.0f };

/**********************************************************
	プレイヤー攻撃初期化関数

	　引数：なし
	戻り値：プレイヤー弾配列の要素番号

	プレイヤー弾の初期化を行う関数。プレイヤー弾の発射位置
	を設定、移動ベクトルを作成してプレイヤー弾にセット、
	半径・生存フラグＯＮ・反射回数・色の設定も行っている。
**********************************************************/

void PAttack_Init( int count )
{
	
	VECTOR4 pos;

	// プレイヤー弾の位置座標にプレイヤー位置をセット
	g_Player.PAttack.m_vApos[count].m_x = g_Player.Obj.m_vPos.m_x;
	g_Player.PAttack.m_vApos[count].m_y = g_Player.Obj.m_vPos.m_y;
	g_Player.PAttack.m_vApos[count].m_z = g_Player.Obj.m_vPos.m_z;

	VECTOR4 mov1;

	// 移動ベクトルの初期化
	memset( &mov1, 0x00, sizeof(VECTOR4) );

	// 移動量を設定
	mov1.m_z = 1.0f;

	// 移動ベクトルをプレイヤーの回転量分、回転させた移動ベクトルをセット
	Mat4x4_Vec4_Multiplay( &g_Player.PAttack.m_vAmov[count], &g_Player.Obj.m_matRotate, &mov1 );

	// プレイヤー弾の半径を設定
	g_Player.PAttack.m_radius[count] = 1.0f;

	// プレイヤー弾生存フラグを1に設定
	g_Player.PAttack.m_AttackFlag[count] = 1;

	// プレイヤー弾の最大（壁）反射数を設定
	g_Player.PAttack.m_LifeCounter[count] = 3;

	// プレイヤー弾の色を設定
	g_Player.PAttack.Rcolor[count] = s_fColorCode[0][0];
	g_Player.PAttack.Gcolor[count] = s_fColorCode[0][1];
	g_Player.PAttack.Bcolor[count] = s_fColorCode[0][2];
		
}

/**********************************************************
	プレイヤー攻撃移動関数

	引数：なし　戻り値：なし

	生存フラグが1のプレイヤー弾の移動処理等を行う関数。
	毎フレーム、弾の移動処理をし、更に当たり判定を行う。
	また、位置がエリア外にある場合補正（跳ね返り）を行って
	色の変化、弾半径を大きくしている。
**********************************************************/

void PAttack_Move( void )
{

	// プレイヤー弾最大発射可能数分ループ
	for ( int count = 0; count < MAX_ATTACK; count++ ) { 

		// 生存フラグが1になっているプレイヤー弾の移動処理のみを行う
		if ( g_Player.PAttack.m_AttackFlag[count] == 1 ) {

			// プレイヤー弾の位置座標に移動ベクトル分の数値を足す
			g_Player.PAttack.m_vApos[count].m_x -= g_Player.PAttack.m_vAmov[count].m_x;
			g_Player.PAttack.m_vApos[count].m_y -= g_Player.PAttack.m_vAmov[count].m_y;
			g_Player.PAttack.m_vApos[count].m_z -= g_Player.PAttack.m_vAmov[count].m_z;

			// 現在の跳ね返り数設定が3でない場合、当たり判定を行う（反射していない状態の弾の当たり判定はとっていない）
			if ( g_Player.PAttack.m_LifeCounter[count] != 3 ) {
				// ＜プレイヤー弾・ターゲット＞衝突判定関数
				Sphere_Bullet_Collision( count );
			}

			// プレイヤー弾位置座標が（エリアＸ範囲値＋弾半径）の場合、補正処理
			if ( g_Player.PAttack.m_vApos[count].m_x + g_Player.PAttack.m_radius[count] <= -g_Ereamap.m_x 
			  || g_Player.PAttack.m_vApos[count].m_x - g_Player.PAttack.m_radius[count] >=  g_Ereamap.m_x ) {

				// 弾の跳ね返り数設定が0になっていない場合
				if ( g_Player.PAttack.m_LifeCounter[count] != 0 ) { 

					// 跳ね返り回数に応じた色に変化（白→青→黄→赤）
					g_Player.PAttack.Rcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][0];
					g_Player.PAttack.Gcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][1];
					g_Player.PAttack.Bcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][2];

					g_Player.PAttack.m_vAmov[count].m_x *= -1;																// 移動ベクトルを反転
					g_Player.PAttack.m_radius[count] *= 2.5f;					// 弾半径を大きくしてセット
					g_Player.PAttack.m_LifeCounter[count]-= 1;																// 跳ね返り数をマイナス1

				}
				else {																										// 設定跳ね返り数分、跳ね返っていれば実行
					g_Player.PAttack.m_AttackFlag[count] = 0;																// 生存フラグを0に戻す
				}

			}

			// プレイヤー弾位置座標が（エリアＹ範囲値＋弾半径）の場合、補正処理
				if ( g_Player.PAttack.m_vApos[count].m_y + g_Player.PAttack.m_radius[count] <= -g_Ereamap.m_y 
				  || g_Player.PAttack.m_vApos[count].m_y - g_Player.PAttack.m_radius[count] >=  g_Ereamap.m_y ) {

				// 弾の跳ね返り数設定が0になっていない場合
				if ( g_Player.PAttack.m_LifeCounter[count] != 0 ) {

					// 跳ね返り回数に応じた色に変化（白→青→黄→赤）
					g_Player.PAttack.Rcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][0];
					g_Player.PAttack.Gcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][1];
					g_Player.PAttack.Bcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][2];

					g_Player.PAttack.m_vAmov[count].m_y *= -1;																// 移動ベクトルを反転
					g_Player.PAttack.m_radius[count] *= 2.5f;					// 弾半径を大きくしてセット
					g_Player.PAttack.m_LifeCounter[count]--;																// 跳ね返り数をマイナス1

				}
				else {																										// 設定跳ね返り数分、跳ね返っていれば実行
					g_Player.PAttack.m_AttackFlag[count] = 0;																// 生存フラグを0に戻す
				}

			}
			
			// プレイヤー弾位置座標が（エリアＺ範囲値＋弾半径）の場合、補正処理
				if ( g_Player.PAttack.m_vApos[count].m_z + g_Player.PAttack.m_radius[count] <= -g_Ereamap.m_z 
				  || g_Player.PAttack.m_vApos[count].m_z - g_Player.PAttack.m_radius[count] >=  g_Ereamap.m_z ) {
				
				// 弾の跳ね返り数設定が0になっていない場合
				if ( g_Player.PAttack.m_LifeCounter[count] != 0 ) {
					
					// 跳ね返り回数に応じた色に変化（白→青→黄→赤）
					g_Player.PAttack.Rcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][0];
					g_Player.PAttack.Gcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][1];
					g_Player.PAttack.Bcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][2];

					g_Player.PAttack.m_vAmov[count].m_z *= -1;																// 移動ベクトルを反転
					g_Player.PAttack.m_radius[count] *= 2.5f;					// 弾半径を大きくしてセット
					g_Player.PAttack.m_LifeCounter[count]--;																// 跳ね返り数をマイナス1
					
				}
				else {																										// 設定跳ね返り数分、跳ね返っていれば実行
					g_Player.PAttack.m_AttackFlag[count] = 0;																// 生存フラグを0に戻す
				}

			}
		
		}
	
	}

}

/**********************************************************
	プレイヤー攻撃描画関数

	引数：なし　戻り値：なし

	生存フラグが1になっている弾の描画処理を行う関数。
	弾を平行移動して、跳ね返りに応じた色を設定、その後
	描画処理を行っている。
**********************************************************/

void PAttack_Display( void )
{

	// 最大弾発射可能数分ループを行う
	for ( int count = 0; count < MAX_ATTACK; count++ ) {
		
		// 生存フラグが1になっていれば弾の処理を行う
		if ( g_Player.PAttack.m_AttackFlag[count] == 1 ) {
			
			glPushMatrix();

			// 弾の座標を更新
			glTranslatef( g_Player.PAttack.m_vApos[count].m_x, 
						  g_Player.PAttack.m_vApos[count].m_y, 
						  g_Player.PAttack.m_vApos[count].m_z );
    
			// 色の設定
			glColor3f( g_Player.PAttack.Rcolor[count], g_Player.PAttack.Gcolor[count], g_Player.PAttack.Bcolor[count] );
			
			// 弾の半径を取得
			float radius = g_Player.PAttack.m_radius[count];
			// 弾を表示
			gluSphere( globj, radius, 20, 20 );

			glPopMatrix();
	
		}

	}

}

