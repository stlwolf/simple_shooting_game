
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "MatrixCal.h"

/**********************************************************
	単位行列作成関数

	　引数：単位行列にするMATRIX4x4型変数のアドレス
	戻り値：なし

	引数として渡された行列を単位行列にする関数
**********************************************************/

void Mat4x4_Identity( MATRIX4x4 *mat )
{

	mat->matrix4x4[0][1] = mat->matrix4x4[0][2] = mat->matrix4x4[0][3] = \
	mat->matrix4x4[1][0] = mat->matrix4x4[1][2] = mat->matrix4x4[1][3] = \
	mat->matrix4x4[2][0] = mat->matrix4x4[2][1] = mat->matrix4x4[2][3] = \
	mat->matrix4x4[3][0] = mat->matrix4x4[3][1] = mat->matrix4x4[3][2] = 0.0f;

	mat->matrix4x4[0][0] = mat->matrix4x4[1][1] = mat->matrix4x4[2][2] = mat->matrix4x4[3][3] = 1.0f;

}

/**********************************************************
	行列の乗算関数

	第１引数：乗算した結果を格納するMATRIX4x4型変数のアドレス
	第２引数：乗算するMATRIX4x4型変数のアドレス
	第３引数：第２引数に乗算するMATRIX4x4型の変数のアドレス
	　戻り値：なし

	引数として渡されたMATRIX4x4型変数同士を乗算する関数。
	結果を第３引数として渡されたアドレスにセットする。
**********************************************************/

void Mat4x4_Multiplay( MATRIX4x4 *mat, const MATRIX4x4 *m1, const MATRIX4x4 *m2 )
{

	MATRIX4x4 mcopy;

	for ( int loop1 = 0; loop1 < 4; loop1++ ) {
		for ( int loop2 = 0; loop2 < 4; loop2++ ) {
			for ( int loop3 = 0; loop3 < 4; loop3++ ) {
				mcopy.matrix4x4[loop1][loop2] += m1->matrix4x4[loop1][loop3] * m2->matrix4x4[loop3][loop2];
			}
		}
	}
	
	for ( int loop1 = 0; loop1 < 4; loop1++ ) {
		for ( int loop2 = 0; loop2 < 4; loop2++ ) {
			mat->matrix4x4[loop1][loop2] = mcopy.matrix4x4[loop1][loop2];
		}
	}

}

/**********************************************************
	ベクトルの長さを求める関数

	　引数：VECTOR3型のベクトル
	戻り値：求めたベクトルの長さ

	与えられたベクトルの長さを求めて返す関数。
**********************************************************/

float Vec3_Length( const VECTOR3 dir )
{

	return sqrt( dir.m_x * dir.m_x + dir.m_y * dir.m_y + dir.m_z * dir.m_z );

}

/**********************************************************
	ベクトル正規化関数

	第１引数：正規化したベクトルの値を格納する変数のアドレス
	第２引数：正規化したいベクトル変数のアドレス
	　戻り値：なし

	第２引数として与えられたベクトルの正規化を行う関数。
**********************************************************/

void Vec3_Normalize( VECTOR3 *vec, const VECTOR3 *dir )
{

	VECTOR3 vcopy;

	float length = sqrt( dir->m_x * dir->m_x + dir->m_y * dir->m_y + dir->m_z * dir->m_z );

	vcopy.m_x = dir->m_x / length;
	vcopy.m_y = dir->m_y / length;
	vcopy.m_z = dir->m_z / length;

	vec->m_x = vcopy.m_x;
	vec->m_y = vcopy.m_y;
	vec->m_z = vcopy.m_z;

}

/**********************************************************
	ベクトル内積関数

	第１引数：内積を行うVECTOR3型の変数のアドレス
	第２引数：内積を行うVECTOR3型の変数のアドレス
	　戻り値：内積の計算結果

	引数として与えられたべクトルの内積を行い結果を返す。
**********************************************************/

float Vec3_Dotproduct( const VECTOR3 *dir1, const VECTOR3 *dir2 )
{

	 return ( dir1->m_x * dir2->m_x + dir1->m_y * dir2->m_y + dir1->m_z * dir2->m_z );

}

/**********************************************************
	ベクトル外積関数


	第１引数：外積を行うVECTOR3型の変数のアドレス
	第２引数：外積を行うVECTOR3型の変数のアドレス
	第３引数：外積の計算結果を格納するVECTOR3型の変数のアドレス
	　戻り値：なし

	引数として与えられたべクトルの外積を行い結果を返す。
**********************************************************/

void Vec3_Crossproduct( VECTOR3 *vec, const VECTOR3 *dir1, const VECTOR3 *dir2 )
{

	VECTOR3 vcopy;

	vcopy.m_x = ( dir1->m_y * dir2->m_z ) - ( dir1->m_z * dir2->m_y );
	vcopy.m_y = ( dir1->m_z * dir2->m_x ) - ( dir1->m_x * dir2->m_z );
	vcopy.m_z = ( dir1->m_x * dir2->m_y ) - ( dir1->m_y * dir2->m_x );

	vec->m_x = vcopy.m_x;
	vec->m_y = vcopy.m_y;
	vec->m_z = vcopy.m_z;

}

/**********************************************************
	行列Ｘベクトル関数（OpenGL仕様）

	第１引数：MATRIX4x4型変数のアドレス
	第２引数：VECTOR4型変数のアドレス
	第３引数：結果を格納するVECTOR4型変数のアドレス

	第２引数として渡された４ｘ４行列と第３引数として
	渡されたベクトルの乗算を行い、その結果を返す。
**********************************************************/

void Mat4x4_Vec4_Multiplay( VECTOR4 *vec, MATRIX4x4 *mat, VECTOR4 *dir )
{

    VECTOR4 vcopy;

	vcopy.m_x = mat->matrix4x4[0][0] * dir->m_x + mat->matrix4x4[1][0] * dir->m_y + mat->matrix4x4[2][0] * dir->m_z + mat->matrix4x4[3][0] * dir->m_w;
	vcopy.m_y = mat->matrix4x4[0][1] * dir->m_x + mat->matrix4x4[1][1] * dir->m_y + mat->matrix4x4[2][1] * dir->m_z + mat->matrix4x4[3][1] * dir->m_w;
	vcopy.m_z = mat->matrix4x4[0][2] * dir->m_x + mat->matrix4x4[1][2] * dir->m_y + mat->matrix4x4[2][2] * dir->m_z + mat->matrix4x4[3][2] * dir->m_w;
	vcopy.m_w = mat->matrix4x4[0][3] * dir->m_x + mat->matrix4x4[1][3] * dir->m_y + mat->matrix4x4[2][3] * dir->m_z + mat->matrix4x4[3][3] * dir->m_w;

	vec->Set_XYZW( vcopy.m_x, vcopy.m_y, vcopy.m_z, vcopy.m_w );

}

//void Mat4x4_Vec4_Multiplay( VECTOR4 *vec, MATRIX4x4 *mat, VECTOR4 *dir )
//{
//
//    VECTOR4 vcopy;
//
//	vcopy.m_x = mat->matrix4x4[0][0] * dir->m_x + mat->matrix4x4[0][1] * dir->m_y + mat->matrix4x4[0][2] * dir->m_z + mat->matrix4x4[0][3] * dir->m_w;
//	vcopy.m_y = mat->matrix4x4[1][0] * dir->m_x + mat->matrix4x4[1][1] * dir->m_y + mat->matrix4x4[1][2] * dir->m_z + mat->matrix4x4[1][3] * dir->m_w;
//	vcopy.m_z = mat->matrix4x4[2][0] * dir->m_x + mat->matrix4x4[2][1] * dir->m_y + mat->matrix4x4[2][2] * dir->m_z + mat->matrix4x4[2][3] * dir->m_w;
//	vcopy.m_w = mat->matrix4x4[3][0] * dir->m_x + mat->matrix4x4[3][1] * dir->m_y + mat->matrix4x4[3][2] * dir->m_z + mat->matrix4x4[3][3] * dir->m_w;
//
//	vec->Set_XYZW( vcopy.m_x, vcopy.m_y, vcopy.m_z, vcopy.m_w );
//
//}
