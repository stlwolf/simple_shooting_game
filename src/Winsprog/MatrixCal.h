#if !defined(___MATRIXCAL___H)
#define ___MATRIXCAL___H

/********************************************************
	インクルードファイル
********************************************************/

#include <math.h>

/**********************************************************
	構造体宣言
**********************************************************/

struct VECTOR3 {

	float m_x, m_y, m_z;																			// ３要素（ｘｙｚ）変数
	VECTOR3() { m_x = 0.0f; m_y = 0.0f; m_z = 0.0f; };												// コンストラクタ関数

	// メンバ変数 m_x m_y, m_z に値を代入
	void Set_XYZ( float x, float y, float z ) { m_x = x; m_y = y; m_z = z; }
	
	float Get_Xelement() { return m_x; }	// メンバ変数 m_x を返す
	float Get_Yelement() { return m_y; }	// メンバ変数 m_y を返す
	float Get_Zelement() { return m_z; } 	// メンバ変数 m_z を返す

};

struct VECTOR4 {
	
	float m_x, m_y, m_z, m_w;																		// ４要素（ｘｙｚｗ）変数
	VECTOR4() { m_x = 0.0f; m_y = 0.0f; m_z = 0.0f; m_w = 0.0f; };									// コンストラクタ関数

	// メンバ変数 m_x m_y, m_z, m_w に値を代入
	void Set_XYZW( float x, float y, float z, float w ) { m_x = x; m_y = y; m_z = z; m_w = w; } 

	float Get_Xelement() { return m_x; }	// メンバ変数 m_x を返す
	float Get_Yelement() { return m_y; }	// メンバ変数 m_y を返す
	float Get_Zelement() { return m_z; }	// メンバ変数 m_z を返す
	float Get_Welement() { return m_w; }	// メンバ変数 m_w を返す

};

struct MATRIX4x4 {

	float matrix4x4[4][4];																			// ４Ｘ４行列変数

	// コンストラクタ関数（単位行列を作成する）
	MATRIX4x4() {																					
		for ( int i = 0; i < 4; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				matrix4x4[i][j] = ( i == j ) ? 1.0f : 0.0f;
			}
		}
	};

};

struct OBJECT_BASE {

	VECTOR4 m_vPos;																			// 現在座標
	
	float m_Speed;																			// スピード（スカラー値）

	VECTOR4 m_vDir;																			// 方向ベクトル

	float m_AngleX, m_AngleY, m_AngleZ;														// 回転角度

	VECTOR4 m_vAxisX, m_vAxisY, m_vAxisZ;													// 回転軸（ノーマルＸ・Ｙ・Ｚ軸）

	MATRIX4x4 m_matRotate, m_matNow_Rotate;													// オブジェクト回転行列

};

/********************************************************
	マクロ定義
********************************************************/

#define T_PI		(3.1415926535f)
#define RAD(x)		(float)( x * ( T_PI / 180.0f ) )
#define DEGREES(x)	(float)( x * ( 180.0f / T_PI ) )

inline float ToDegrees( float rad )
{
	return ( rad * ( 180.0f / T_PI ) );
}

inline float ToRadian( float degrees )
{
	return ( degrees * ( T_PI / 180.0f ) );
}

/********************************************************
	グローバル変数宣言
********************************************************/

/********************************************************
	関数プロトタイプ宣言
********************************************************/

extern void Mat4x4_Identity( MATRIX4x4 );
extern void Mat4x4_Multiplay( MATRIX4x4 *, const MATRIX4x4 *, const MATRIX4x4 * );

extern float Vec3_Length( const VECTOR3 );

extern void Vec3_Normalize( VECTOR3 *, const VECTOR3 * );

extern float Vec3_Dotproduct( const VECTOR3 *, const VECTOR3 * );
extern void Vec3_Crossproduct( VECTOR3 *, const VECTOR3 *, const VECTOR3 * );

extern void Mat4x4_Vec4_Multiplay( VECTOR4 *, MATRIX4x4 *, VECTOR4 * ); 

#endif	// #if !defined(___MATRIXCAL___H)
