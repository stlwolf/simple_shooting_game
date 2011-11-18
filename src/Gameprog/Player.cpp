
/**********************************************************
	インクルードファイル
**********************************************************/

#include "Player.h"

/**********************************************************
	グローバル変数
**********************************************************/

PLAYER g_Player( -0.2f );

/**********************************************************
	プレイヤー初期化関数

	引数：なし　戻り値：なし

	プレイヤー情報の初期化を行う。
	プレイヤーの回転軸の設定、カメラベクトル・モードの設定
	プレイヤー弾の生存フラグの初期化を行う。
**********************************************************/

void Player_Init( void )
{

	MATRIX4x4 mat;

	// プレイヤー回転軸初期化設定
	// ローカルＸ軸
	g_Player.Obj.m_vAxisX.m_x = 1.0f;
	g_Player.Obj.m_vAxisX.m_y = 0.0f;
	g_Player.Obj.m_vAxisX.m_z = 0.0f;
	g_Player.Obj.m_vAxisX.m_w = 1.0f;

	// ローカルＹ軸
	g_Player.Obj.m_vAxisY.m_x = 0.0f;
	g_Player.Obj.m_vAxisY.m_y = 1.0f;
	g_Player.Obj.m_vAxisY.m_z = 0.0f;
	g_Player.Obj.m_vAxisY.m_w = 1.0f;

	// ローカルＺ軸
	g_Player.Obj.m_vAxisZ.m_x = 0.0f;
	g_Player.Obj.m_vAxisZ.m_y = 0.0f;
	g_Player.Obj.m_vAxisZ.m_z = 1.0f;
	g_Player.Obj.m_vAxisZ.m_w = 1.0f;

	// プレイヤー位置情報初期化
	g_Player.Obj.m_vPos.m_x = 0.0f;
	g_Player.Obj.m_vPos.m_y = 0.0f;
	g_Player.Obj.m_vPos.m_z = 0.0f;
	
	// プレイヤー回転量の初期化
	g_Player.Obj.m_matRotate     = mat;
	g_Player.Obj.m_matNow_Rotate = mat;

	// カメラモード№０のベクトル初期化
	// 視点ベクトル初期化
	g_Player.m_vEye[0].m_x = 0.0f;
	g_Player.m_vEye[0].m_y = 1.2f;
	g_Player.m_vEye[0].m_z = 5.5f;
	g_Player.m_vEye[0].m_w = 1.0f;

	// 注視点の初期化
	g_Player.m_vUp[0].m_x = 0.0f;
	g_Player.m_vUp[0].m_y = 1.0f;
	g_Player.m_vUp[0].m_z = 0.0f;
	g_Player.m_vUp[0].m_w = 1.0f;


	// カメラモード№１のベクトル初期化
	// 視点ベクトル初期化
	g_Player.m_vEye[1].m_x = 0.0f;
	g_Player.m_vEye[1].m_y = 0.2f;
	g_Player.m_vEye[1].m_z = 0.0f;
	g_Player.m_vEye[1].m_w = 1.0f;

	// 注視点の初期化
	g_Player.m_vLook[1].m_x =  0.0f;
	g_Player.m_vLook[1].m_y =  0.2f;
	g_Player.m_vLook[1].m_z = -0.5f;
	g_Player.m_vLook[1].m_w =  1.0f;

	// Upベクトルの初期化
	g_Player.m_vUp[1].m_x = 0.0f;
	g_Player.m_vUp[1].m_y = 1.0f;
	g_Player.m_vUp[1].m_z = 0.0f;
	g_Player.m_vUp[1].m_w = 1.0f;

	// 初期カメラモードを０に設定
	g_Player.m_nCameraNo = 0;

	// プレイヤー弾のフラグ初期化
	for ( int count = 0; count < MAX_ATTACK; count++ ) {
		g_Player.PAttack.m_AttackFlag[count] = 0;
	}

}

/**********************************************************
	プレイヤー移動処理関数

	　引数：なし
	戻り値：なし

	プレイヤーの移動処理等を行う関数。入力されたキーに
	応じて、移動・回転・攻撃・カメラモードの変更を行う。
	また移動の際に範囲外に出ないように補正を行う。
**********************************************************/

void Player_Move( void )
{

	if ( g_nKeyData & KEY_UP ) {							// [8]キー
		g_Player.Obj.m_AngleX = -2.0f;						// プレイヤーのＸ軸回転
	}
	else if ( g_nKeyData & KEY_DOWN ) {						// [2]キー
		g_Player.Obj.m_AngleX = 2.0f;						// プレイヤーのＸ軸回転
	}

	if ( g_nKeyData & KEY_LEFT ) {							// [4]キー
		g_Player.Obj.m_AngleY = 1.0f;						// プレイヤーのＹ軸回転
	}
	else if ( g_nKeyData & KEY_RIGHT ) {					// [6]キー
		g_Player.Obj.m_AngleY = -1.0f;						// プレイヤーのＹ軸回転
	}

	if ( g_nKeyData & KEY_ROTL ) {							// [1]キー
		g_Player.Obj.m_AngleZ =  1.0f;						// プレイヤーのＺ軸回転		
	}
	else if ( g_nKeyData & KEY_ROTR ) {						// [3]キー
		g_Player.Obj.m_AngleZ = -1.0f;						// プレイヤーのＺ軸回転
	}

	if ( g_nKeyEdgeData & KEY_BUTTON2 ) {					// [X]キー

		// 現在のカメラモードを取得
		int key = g_Player.m_nCameraNo;

		// カメラモード№が0なら実行
		if ( key < 1 ) {
			// カメラモードを1に変更
			g_Player.m_nCameraNo++;
		}
		// カメラモードNoが1なら実行
		else if ( key > 0 ) {
			// カメラモードを0に変更
			g_Player.m_nCameraNo--; 
		}

	}

	if ( g_nKeyEdgeData & KEY_BUTTON1 ) {					// [Z]キー

		// プレイヤー弾生存フラグが0の場合実行
		if ( g_Player.PAttack.m_AttackFlag[g_Player.PAttack.m_FlagCounter] == 0 ) {

			// プレイヤー弾初期化関数を実行する
			PAttack_Init( g_Player.PAttack.m_FlagCounter );
				
			// 生存フラグカウントが（最大発射可能数-1）より小さければカウンタをプラス
			if ( g_Player.PAttack.m_FlagCounter < ( MAX_ATTACK - 1 ) ) {
				g_Player.PAttack.m_FlagCounter++;
			}
			else {
				g_Player.PAttack.m_FlagCounter = 0;			// カウンタを0に戻す
			}

		}

	}

	// １フレーム前の回転量から新しい回転軸を作成する
	Mat4x4_Vec4_Multiplay( &g_Player.Obj.m_vAxisX, &g_Player.Obj.m_matNow_Rotate, &g_Player.Obj.m_vAxisX );
	Mat4x4_Vec4_Multiplay( &g_Player.Obj.m_vAxisY, &g_Player.Obj.m_matNow_Rotate, &g_Player.Obj.m_vAxisY );  
	Mat4x4_Vec4_Multiplay( &g_Player.Obj.m_vAxisZ, &g_Player.Obj.m_matNow_Rotate, &g_Player.Obj.m_vAxisZ );

	VECTOR4 mov1, mov2;

	memset( &mov1, 0x00, sizeof(VECTOR4) );

	// 移動スピードを取得
	mov1.m_z = g_Player.Obj.m_Speed;

	// 移動ベクトルに回転行列を掛けて新しい移動ベクトルを作成する
	Mat4x4_Vec4_Multiplay( &mov2, &g_Player.Obj.m_matRotate, &mov1 );

	// プレイヤーの位置に移動ベクトルの値を足して更新
	g_Player.Obj.m_vPos.m_x += mov2.m_x;
	g_Player.Obj.m_vPos.m_y += mov2.m_y;
	g_Player.Obj.m_vPos.m_z += mov2.m_z;

	// プレイヤー位置座標がエリア範囲外に出たときに実行（位置を補正）
	if ( g_Player.Obj.m_vPos.m_x - 0.8 <= -g_Ereamap.m_x
	  || g_Player.Obj.m_vPos.m_x + 0.8 >=  g_Ereamap.m_x ) {

		// 移動ベクトル分位置を戻す
		g_Player.Obj.m_vPos.m_x -= mov2.m_x;
		g_Player.Obj.m_vPos.m_y -= mov2.m_y;
		g_Player.Obj.m_vPos.m_z -= mov2.m_z;

	}

	// プレイヤー位置座標がエリア範囲外に出たときに実行（位置を補正）
	if ( g_Player.Obj.m_vPos.m_y - 0.8 <= -g_Ereamap.m_y
	  || g_Player.Obj.m_vPos.m_y + 0.8 >=  g_Ereamap.m_y ) {

		// 移動ベクトル分位置を戻す
		g_Player.Obj.m_vPos.m_x -= mov2.m_x;
		g_Player.Obj.m_vPos.m_y -= mov2.m_y;
		g_Player.Obj.m_vPos.m_z -= mov2.m_z;

	}
	
	// プレイヤー位置座標がエリア範囲外に出たときに実行（位置を補正）
	if ( g_Player.Obj.m_vPos.m_z - 0.8 <= -g_Ereamap.m_z
	  || g_Player.Obj.m_vPos.m_z + 0.8 >=  g_Ereamap.m_z ) {

		// 移動ベクトル分位置を戻す
		g_Player.Obj.m_vPos.m_x -= mov2.m_x;
		g_Player.Obj.m_vPos.m_y -= mov2.m_y;
		g_Player.Obj.m_vPos.m_z -= mov2.m_z;

	}
	
}

/**********************************************************
	プレイヤー描画処理関数

	引数：なし　戻り値：なし

	キー入力で変更された回転量分、プレイヤーの回転軸を
	変更している。また、視点の更新、移動量に応じた位置の
	更新、プレイヤーオブジェクトの表示を行う。
**********************************************************/

void Player_Display( void )
{

	// モデルビュー行列に設定
	glMatrixMode( GL_MODELVIEW );

	// 行列の初期化
	glLoadIdentity();

	glPushMatrix();

	// プレイヤー回転軸を元に回転を行う
	glRotatef( g_Player.Obj.m_AngleX, g_Player.Obj.m_vAxisX.m_x, g_Player.Obj.m_vAxisX.m_y, g_Player.Obj.m_vAxisX.m_z );
	glRotatef( g_Player.Obj.m_AngleY, g_Player.Obj.m_vAxisY.m_x, g_Player.Obj.m_vAxisY.m_y, g_Player.Obj.m_vAxisY.m_z );
	glRotatef( g_Player.Obj.m_AngleZ, g_Player.Obj.m_vAxisZ.m_x, g_Player.Obj.m_vAxisZ.m_y, g_Player.Obj.m_vAxisZ.m_z );

	// 回転量の初期化
	g_Player.Obj.m_AngleX = 0.0f;
	g_Player.Obj.m_AngleY = 0.0f;
	g_Player.Obj.m_AngleZ = 0.0f;

	// 今回のフレームの回転量を取り出す
	glGetFloatv( GL_MODELVIEW_MATRIX, (float *)&g_Player.Obj.m_matNow_Rotate );

	// 総回転量行列をモデルビュー行列に掛ける
	glMultMatrixf( (float *)&g_Player.Obj.m_matRotate );

	// 総回転量を取り出す
	glGetFloatv( GL_MODELVIEW_MATRIX, (float *)&g_Player.Obj.m_matRotate );

	glPopMatrix();

	// カメラモードに応じた視点を設定
	Player_Camera( g_Player.m_nCameraNo );

	glPushMatrix();

	// プレイヤー位置座標の更新（平行移動を行う）
	glTranslatef( g_Player.Obj.m_vPos.m_x, g_Player.Obj.m_vPos.m_y, g_Player.Obj.m_vPos.m_z );

	glMultMatrixf( (float *)&g_Player.Obj.m_matRotate );

	// ポリゴン表示処理
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );

	glDrawArrays( GL_TRIANGLES, 0, g_pObjBuf2->m_nPolcnt * 3 );

	glPopMatrix();

}

/**********************************************************
	プレイヤーカメラ作成関数

	　引数：なし
	戻り値：カメラモード№の値

	プレイヤーの回転量分に合わせて、視点・注視点・Upベクト
	ルを回転させる。かつプレイヤーが移動した分だけ座標を更
	新する。カメラ座標を更新した後の設置もここで行う。
**********************************************************/

void Player_Camera( int camerano )
{

	VECTOR4 eye, look, up;

	// カメラの各座標を回転させる（行列を掛け合わせる）
	Mat4x4_Vec4_Multiplay( &eye, &g_Player.Obj.m_matRotate, &g_Player.m_vEye[camerano] );
	Mat4x4_Vec4_Multiplay( &up, &g_Player.Obj.m_matRotate, &g_Player.m_vUp[camerano] );
	Mat4x4_Vec4_Multiplay( &look, &g_Player.Obj.m_matRotate, &g_Player.m_vLook[camerano] );

	// カメラの場所を計算
	eye.m_x += g_Player.Obj.m_vPos.m_x;
	eye.m_y += g_Player.Obj.m_vPos.m_y;
	eye.m_z += g_Player.Obj.m_vPos.m_z;

	// カメラの視点を計算
	look.m_x += g_Player.Obj.m_vPos.m_x;
	look.m_y += g_Player.Obj.m_vPos.m_y;
	look.m_z += g_Player.Obj.m_vPos.m_z;

	// カメラ行列の設定
	gluLookAt(  eye.m_x,  eye.m_y,  eye.m_z,
			   look.m_x, look.m_y, look.m_z,
			     up.m_x,   up.m_y,   up.m_z );

}
