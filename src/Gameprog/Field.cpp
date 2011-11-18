
/**********************************************************
	インクルードファイル
**********************************************************/

#include "Field.h"

/**********************************************************
	グローバル変数定義
**********************************************************/

EREA g_Ereamap;

/**********************************************************
	マップ初期化関数

	引数：なし　戻り値：なし

	マップの表示に必要な変数等を初期化する関数。
	マップの表示範囲と出現ターゲット数を初期化している。
**********************************************************/

void Map_Init( void )
{

	// エリアの表示範囲の初期化設定
	g_Ereamap.m_x = 80;
	g_Ereamap.m_y = 60;
	g_Ereamap.m_z = 80;

	// エリアマップ変数に登場ターゲット数を設定
	g_Ereamap.m_TargetNum = MAX_TARGET;

	g_Ereamap.m_EnemyTargetNum = 99;

}

/**********************************************************
	ワイヤー壁作成関数

	第１引数：ワイヤーを作成するＸ座標値
	第２引数：ワイヤーを作成するＹ座標値
	第３引数：ワイヤーを作成するＺ座標値
	第４引数：ワイヤー分割数
	　戻り値：なし

	０を中心として、引数として渡されたＸ・Ｙ・Ｚ座標の値
	の範囲を囲むような箱を表示する。
**********************************************************/

void WireWall_Disp( float x, float y, float z, float div )
{

    int loop;
    float st_x, st_y, st_z;
    float p_x, p_y, p_z;

    st_x = x / 2.0f;
    st_y = y / 2.0f;
    st_z = z / 2.0f;

    glEnable(GL_BLEND);

    // ワイヤーフレームのジャギ消し用
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    for( loop = 0; loop <= div; loop++ ) {

        p_x = x / div * loop;
        p_y = y / div * loop;
        p_z = z / div * loop;

        //ポリゴンの箱表示
        glBegin( GL_LINES  );
            glColor3f( 1.0f, 0.0f, 0.0f );  glVertex3f( st_x,  st_y,  st_z-p_z); glVertex3f(-st_x,  st_y,  st_z-p_z);
            glColor3f( 0.0f, 1.0f, 0.0f );  glVertex3f(-st_x,  st_y,  st_z-p_z); glVertex3f(-st_x, -st_y,  st_z-p_z);
            glColor3f( 0.0f, 0.0f, 1.0f );  glVertex3f(-st_x, -st_y,  st_z-p_z); glVertex3f( st_x, -st_y,  st_z-p_z);
            glColor3f( 1.0f, 1.0f, 0.0f );  glVertex3f( st_x, -st_y,  st_z-p_z); glVertex3f( st_x,  st_y,  st_z-p_z);
        glEnd();

        glBegin( GL_LINES );
            glColor3f( 1.0f, 1.0f, 0.0f );  glVertex3f( st_x,  st_y-p_y,  st_z); glVertex3f( st_x,  st_y-p_y, -st_z);
            glColor3f( 0.0f, 1.0f, 1.0f );  glVertex3f( st_x,  st_y-p_y, -st_z); glVertex3f(-st_x,  st_y-p_y, -st_z);
            glColor3f( 0.0f, 1.0f, 0.0f );  glVertex3f(-st_x,  st_y-p_y, -st_z); glVertex3f(-st_x,  st_y-p_y,  st_z);
            glColor3f( 1.0f, 0.0f, 1.0f );  glVertex3f(-st_x,  st_y-p_y,  st_z); glVertex3f( st_x,  st_y-p_y,  st_z);
        glEnd();

        glBegin( GL_LINES  );
            glColor3f( 1.0f, 0.0f, 1.0f );  glVertex3f( st_x-p_x,  st_y,  st_z); glVertex3f( st_x-p_x, -st_y,  st_z);
            glColor3f( 0.0f, 0.0f, 1.0f );  glVertex3f( st_x-p_x, -st_y,  st_z); glVertex3f( st_x-p_x, -st_y, -st_z);
            glColor3f( 0.0f, 1.0f, 1.0f );  glVertex3f( st_x-p_x, -st_y, -st_z); glVertex3f( st_x-p_x,  st_y, -st_z);
            glColor3f( 1.0f, 0.0f, 0.0f );  glVertex3f( st_x-p_x,  st_y, -st_z); glVertex3f( st_x-p_x,  st_y,  st_z);
        glEnd();

    }

    glDisable(GL_BLEND);

}
