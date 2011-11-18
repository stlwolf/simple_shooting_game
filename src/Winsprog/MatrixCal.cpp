
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "MatrixCal.h"

/**********************************************************
	�P�ʍs��쐬�֐�

	�@�����F�P�ʍs��ɂ���MATRIX4x4�^�ϐ��̃A�h���X
	�߂�l�F�Ȃ�

	�����Ƃ��ēn���ꂽ�s���P�ʍs��ɂ���֐�
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
	�s��̏�Z�֐�

	��P�����F��Z�������ʂ��i�[����MATRIX4x4�^�ϐ��̃A�h���X
	��Q�����F��Z����MATRIX4x4�^�ϐ��̃A�h���X
	��R�����F��Q�����ɏ�Z����MATRIX4x4�^�̕ϐ��̃A�h���X
	�@�߂�l�F�Ȃ�

	�����Ƃ��ēn���ꂽMATRIX4x4�^�ϐ����m����Z����֐��B
	���ʂ��R�����Ƃ��ēn���ꂽ�A�h���X�ɃZ�b�g����B
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
	�x�N�g���̒��������߂�֐�

	�@�����FVECTOR3�^�̃x�N�g��
	�߂�l�F���߂��x�N�g���̒���

	�^����ꂽ�x�N�g���̒��������߂ĕԂ��֐��B
**********************************************************/

float Vec3_Length( const VECTOR3 dir )
{

	return sqrt( dir.m_x * dir.m_x + dir.m_y * dir.m_y + dir.m_z * dir.m_z );

}

/**********************************************************
	�x�N�g�����K���֐�

	��P�����F���K�������x�N�g���̒l���i�[����ϐ��̃A�h���X
	��Q�����F���K���������x�N�g���ϐ��̃A�h���X
	�@�߂�l�F�Ȃ�

	��Q�����Ƃ��ė^����ꂽ�x�N�g���̐��K�����s���֐��B
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
	�x�N�g�����ϊ֐�

	��P�����F���ς��s��VECTOR3�^�̕ϐ��̃A�h���X
	��Q�����F���ς��s��VECTOR3�^�̕ϐ��̃A�h���X
	�@�߂�l�F���ς̌v�Z����

	�����Ƃ��ė^����ꂽ�׃N�g���̓��ς��s�����ʂ�Ԃ��B
**********************************************************/

float Vec3_Dotproduct( const VECTOR3 *dir1, const VECTOR3 *dir2 )
{

	 return ( dir1->m_x * dir2->m_x + dir1->m_y * dir2->m_y + dir1->m_z * dir2->m_z );

}

/**********************************************************
	�x�N�g���O�ϊ֐�


	��P�����F�O�ς��s��VECTOR3�^�̕ϐ��̃A�h���X
	��Q�����F�O�ς��s��VECTOR3�^�̕ϐ��̃A�h���X
	��R�����F�O�ς̌v�Z���ʂ��i�[����VECTOR3�^�̕ϐ��̃A�h���X
	�@�߂�l�F�Ȃ�

	�����Ƃ��ė^����ꂽ�׃N�g���̊O�ς��s�����ʂ�Ԃ��B
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
	�s��w�x�N�g���֐��iOpenGL�d�l�j

	��P�����FMATRIX4x4�^�ϐ��̃A�h���X
	��Q�����FVECTOR4�^�ϐ��̃A�h���X
	��R�����F���ʂ��i�[����VECTOR4�^�ϐ��̃A�h���X

	��Q�����Ƃ��ēn���ꂽ�S���S�s��Ƒ�R�����Ƃ���
	�n���ꂽ�x�N�g���̏�Z���s���A���̌��ʂ�Ԃ��B
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
