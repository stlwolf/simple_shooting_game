
/**********************************************************
	インクルードファイル
**********************************************************/

#include "CollisionJudge.h"

/**********************************************************
	＜プレイヤー弾・ターゲット＞衝突判定関数

	　引数：衝突処理を行うプレイヤー弾の番号
	戻り値：なし

	プレイヤー弾とターゲットとの距離を測り当たり判定を行う。
	弾とターゲットと距離ベクトルを作成してその長さが、
	弾＋ターゲットの半径より短ければ当たり処理を行う。
	ターゲットの存在フラグをオフにして、総ターゲット数を
	一つ減らす。
**********************************************************/

void Sphere_Bullet_Collision( int count )
{

	float l, r;

	VECTOR3 APvec, BPvec, BP1vec, Pvec;

	// ターゲット分の処理
	for ( int no = 0; no < MAX_TARGET; no++ ) {

		// 該当ターゲットの座標を取得（位置ベクトル）
		Pvec.m_x = g_Target[no].Obj.m_vPos.m_x;
		Pvec.m_y = g_Target[no].Obj.m_vPos.m_y;
		Pvec.m_z = g_Target[no].Obj.m_vPos.m_z;

		// ターゲットと弾との位置の差からベクトルをつくる（弾とターゲットの距離）
		BPvec.Set_XYZ( g_Player.PAttack.m_vApos[count].m_x - Pvec.m_x,
					   g_Player.PAttack.m_vApos[count].m_y - Pvec.m_y,
					   g_Player.PAttack.m_vApos[count].m_z - Pvec.m_z );


		// ベクトルの長さを求める
		l = Vec3_Length( BPvec );

		// ターゲットと弾の半径をそれぞれ求め、それを足して当たり半径を算出
		r = g_Target[no].m_radius + g_Player.PAttack.m_radius[count];

		// もしターゲット・弾ベクトルの長さが当たり半径より短ければ実行
		if ( l <= r ) {
			
			// 弾の存在フラグを0にリセット（描画を終了）
			g_Player.PAttack.m_AttackFlag[count] = 0;

			// ターゲットの存在フラグが1ならば実行
			if ( g_Target[no].m_nLifeFlag == 1 ) {
				// エリアマップ変数から残りターゲット数を取得して一つ減らしてセット
				g_Ereamap.m_TargetNum--;
			}

			//　ターゲットの存在フラグに0をセット
			g_Target[no].m_nLifeFlag = 0;

			return;

		}

	}

}

