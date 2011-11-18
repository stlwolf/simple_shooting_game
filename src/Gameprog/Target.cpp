
/**********************************************************
	�C���N���[�h�t�@�C��
**********************************************************/

#include "Target.h"

/**********************************************************
	�O���[�o���ϐ�
**********************************************************/

// �^�[�Q�b�g�̂q�f�a�l�ϐ�
float R = 0.0f;
float G = 1.0f;
float B = 0.0f;

// �^�[�Q�b�g�\����
TARGET g_Target[MAX_TARGET];

/**********************************************************
	�^�[�Q�b�g�������֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�^�[�Q�b�g�𐶐����邽�߂̏��������s���֐��B
	���a�A�ʒu���W�i�����_���l�j�̏��������s���A�����t���O
	��1�ɐݒ肷��B
**********************************************************/

void Target_Init( void )
{

	// �ő�^�[�Q�b�g�������[�v���s��
	for ( int count = 0; count < MAX_TARGET; count++ ) {

		// �^�[�Q�b�g�̔��a��ݒ菉��������
		g_Target[count].m_radius = 4.0f;

		float x, y, z;

		// �����_���̂w�E�x�E�y�l�𐶐�
		x = ( rand() % 150 ) - 75;
		y = ( rand() % 110 ) - 55;
		z = ( rand() % 150 ) - 75;

		// �����_���ɎZ�o�����l���^�[�Q�b�g�̈ʒu���W�ɃZ�b�g
		g_Target[count].Obj.m_vPos.m_x = x;
		g_Target[count].Obj.m_vPos.m_y = y;
		g_Target[count].Obj.m_vPos.m_z = z;

		// �^�[�Q�b�g�����t���O��1�ŃZ�b�g
		g_Target[count].m_nLifeFlag = 1;


	}

}

/**********************************************************
	�^�[�Q�b�g�`��֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�����t���O��1�̃^�[�Q�b�g�̕`�揈�����s���֐��B
	�F�̐ݒ�A���ꂼ��̃^�[�Q�b�g�ʒu���W�����ɕ`���
	�s���Ă���B
**********************************************************/

void Target_Display( void )
{

	// �ő�^�[�Q�b�g�������[�v���s��
	for ( int count = 0; count < MAX_TARGET; count++ ) { 

		// �����t���O��1�̃^�[�Q�b�g�Ȃ�Ώ��������s
		if ( g_Target[count].m_nLifeFlag == 1 ) {

			glPushMatrix();

			// �^�[�Q�b�g�̐F��ݒ�
			glColor3f( R, G, B );

			// �^�[�Q�b�g�̈ʒu���W�ɕ��s�ړ�����
			glTranslatef( g_Target[count].Obj.m_vPos.m_x, 
						  g_Target[count].Obj.m_vPos.m_y,
						  g_Target[count].Obj.m_vPos.m_z );

			// �^�[�Q�b�g�̕\������
			gluSphere( globj, g_Target[count].m_radius, 20, 20 );

			glPopMatrix();

		}

	}
	
}