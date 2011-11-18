#if !defined(___MATRIXCAL___H)
#define ___MATRIXCAL___H

/********************************************************
	�C���N���[�h�t�@�C��
********************************************************/

#include <math.h>

/**********************************************************
	�\���̐錾
**********************************************************/

struct VECTOR3 {

	float m_x, m_y, m_z;																			// �R�v�f�i�������j�ϐ�
	VECTOR3() { m_x = 0.0f; m_y = 0.0f; m_z = 0.0f; };												// �R���X�g���N�^�֐�

	// �����o�ϐ� m_x m_y, m_z �ɒl����
	void Set_XYZ( float x, float y, float z ) { m_x = x; m_y = y; m_z = z; }
	
	float Get_Xelement() { return m_x; }	// �����o�ϐ� m_x ��Ԃ�
	float Get_Yelement() { return m_y; }	// �����o�ϐ� m_y ��Ԃ�
	float Get_Zelement() { return m_z; } 	// �����o�ϐ� m_z ��Ԃ�

};

struct VECTOR4 {
	
	float m_x, m_y, m_z, m_w;																		// �S�v�f�i���������j�ϐ�
	VECTOR4() { m_x = 0.0f; m_y = 0.0f; m_z = 0.0f; m_w = 0.0f; };									// �R���X�g���N�^�֐�

	// �����o�ϐ� m_x m_y, m_z, m_w �ɒl����
	void Set_XYZW( float x, float y, float z, float w ) { m_x = x; m_y = y; m_z = z; m_w = w; } 

	float Get_Xelement() { return m_x; }	// �����o�ϐ� m_x ��Ԃ�
	float Get_Yelement() { return m_y; }	// �����o�ϐ� m_y ��Ԃ�
	float Get_Zelement() { return m_z; }	// �����o�ϐ� m_z ��Ԃ�
	float Get_Welement() { return m_w; }	// �����o�ϐ� m_w ��Ԃ�

};

struct MATRIX4x4 {

	float matrix4x4[4][4];																			// �S�w�S�s��ϐ�

	// �R���X�g���N�^�֐��i�P�ʍs����쐬����j
	MATRIX4x4() {																					
		for ( int i = 0; i < 4; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				matrix4x4[i][j] = ( i == j ) ? 1.0f : 0.0f;
			}
		}
	};

};

struct OBJECT_BASE {

	VECTOR4 m_vPos;																			// ���ݍ��W
	
	float m_Speed;																			// �X�s�[�h�i�X�J���[�l�j

	VECTOR4 m_vDir;																			// �����x�N�g��

	float m_AngleX, m_AngleY, m_AngleZ;														// ��]�p�x

	VECTOR4 m_vAxisX, m_vAxisY, m_vAxisZ;													// ��]���i�m�[�}���w�E�x�E�y���j

	MATRIX4x4 m_matRotate, m_matNow_Rotate;													// �I�u�W�F�N�g��]�s��

};

/********************************************************
	�}�N����`
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
	�O���[�o���ϐ��錾
********************************************************/

/********************************************************
	�֐��v���g�^�C�v�錾
********************************************************/

extern void Mat4x4_Identity( MATRIX4x4 );
extern void Mat4x4_Multiplay( MATRIX4x4 *, const MATRIX4x4 *, const MATRIX4x4 * );

extern float Vec3_Length( const VECTOR3 );

extern void Vec3_Normalize( VECTOR3 *, const VECTOR3 * );

extern float Vec3_Dotproduct( const VECTOR3 *, const VECTOR3 * );
extern void Vec3_Crossproduct( VECTOR3 *, const VECTOR3 *, const VECTOR3 * );

extern void Mat4x4_Vec4_Multiplay( VECTOR4 *, MATRIX4x4 *, VECTOR4 * ); 

#endif	// #if !defined(___MATRIXCAL___H)
