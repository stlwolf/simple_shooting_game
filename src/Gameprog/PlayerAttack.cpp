
/**********************************************************
	�C���N���[�h�t�@�C��
**********************************************************/

#include "PlayerAttack.h"

/**********************************************************
	�O���[�o���ϐ�
**********************************************************/

static float s_fColorCode[][3] = {
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f }
};

static float s_fNumMulti[4] = { 0.0f, 3.0f, 4.0f, 5.0f };

/**********************************************************
	�v���C���[�U���������֐�

	�@�����F�Ȃ�
	�߂�l�F�v���C���[�e�z��̗v�f�ԍ�

	�v���C���[�e�̏��������s���֐��B�v���C���[�e�̔��ˈʒu
	��ݒ�A�ړ��x�N�g�����쐬���ăv���C���[�e�ɃZ�b�g�A
	���a�E�����t���O�n�m�E���ˉ񐔁E�F�̐ݒ���s���Ă���B
**********************************************************/

void PAttack_Init( int count )
{
	
	VECTOR4 pos;

	// �v���C���[�e�̈ʒu���W�Ƀv���C���[�ʒu���Z�b�g
	g_Player.PAttack.m_vApos[count].m_x = g_Player.Obj.m_vPos.m_x;
	g_Player.PAttack.m_vApos[count].m_y = g_Player.Obj.m_vPos.m_y;
	g_Player.PAttack.m_vApos[count].m_z = g_Player.Obj.m_vPos.m_z;

	VECTOR4 mov1;

	// �ړ��x�N�g���̏�����
	memset( &mov1, 0x00, sizeof(VECTOR4) );

	// �ړ��ʂ�ݒ�
	mov1.m_z = 1.0f;

	// �ړ��x�N�g�����v���C���[�̉�]�ʕ��A��]�������ړ��x�N�g�����Z�b�g
	Mat4x4_Vec4_Multiplay( &g_Player.PAttack.m_vAmov[count], &g_Player.Obj.m_matRotate, &mov1 );

	// �v���C���[�e�̔��a��ݒ�
	g_Player.PAttack.m_radius[count] = 1.0f;

	// �v���C���[�e�����t���O��1�ɐݒ�
	g_Player.PAttack.m_AttackFlag[count] = 1;

	// �v���C���[�e�̍ő�i�ǁj���ː���ݒ�
	g_Player.PAttack.m_LifeCounter[count] = 3;

	// �v���C���[�e�̐F��ݒ�
	g_Player.PAttack.Rcolor[count] = s_fColorCode[0][0];
	g_Player.PAttack.Gcolor[count] = s_fColorCode[0][1];
	g_Player.PAttack.Bcolor[count] = s_fColorCode[0][2];
		
}

/**********************************************************
	�v���C���[�U���ړ��֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�����t���O��1�̃v���C���[�e�̈ړ����������s���֐��B
	���t���[���A�e�̈ړ����������A�X�ɓ����蔻����s���B
	�܂��A�ʒu���G���A�O�ɂ���ꍇ�␳�i���˕Ԃ�j���s����
	�F�̕ω��A�e���a��傫�����Ă���B
**********************************************************/

void PAttack_Move( void )
{

	// �v���C���[�e�ő唭�ˉ\�������[�v
	for ( int count = 0; count < MAX_ATTACK; count++ ) { 

		// �����t���O��1�ɂȂ��Ă���v���C���[�e�̈ړ������݂̂��s��
		if ( g_Player.PAttack.m_AttackFlag[count] == 1 ) {

			// �v���C���[�e�̈ʒu���W�Ɉړ��x�N�g�����̐��l�𑫂�
			g_Player.PAttack.m_vApos[count].m_x -= g_Player.PAttack.m_vAmov[count].m_x;
			g_Player.PAttack.m_vApos[count].m_y -= g_Player.PAttack.m_vAmov[count].m_y;
			g_Player.PAttack.m_vApos[count].m_z -= g_Player.PAttack.m_vAmov[count].m_z;

			// ���݂̒��˕Ԃ萔�ݒ肪3�łȂ��ꍇ�A�����蔻����s���i���˂��Ă��Ȃ���Ԃ̒e�̓����蔻��͂Ƃ��Ă��Ȃ��j
			if ( g_Player.PAttack.m_LifeCounter[count] != 3 ) {
				// ���v���C���[�e�E�^�[�Q�b�g���Փ˔���֐�
				Sphere_Bullet_Collision( count );
			}

			// �v���C���[�e�ʒu���W���i�G���A�w�͈͒l�{�e���a�j�̏ꍇ�A�␳����
			if ( g_Player.PAttack.m_vApos[count].m_x + g_Player.PAttack.m_radius[count] <= -g_Ereamap.m_x 
			  || g_Player.PAttack.m_vApos[count].m_x - g_Player.PAttack.m_radius[count] >=  g_Ereamap.m_x ) {

				// �e�̒��˕Ԃ萔�ݒ肪0�ɂȂ��Ă��Ȃ��ꍇ
				if ( g_Player.PAttack.m_LifeCounter[count] != 0 ) { 

					// ���˕Ԃ�񐔂ɉ������F�ɕω��i�����������ԁj
					g_Player.PAttack.Rcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][0];
					g_Player.PAttack.Gcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][1];
					g_Player.PAttack.Bcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][2];

					g_Player.PAttack.m_vAmov[count].m_x *= -1;																// �ړ��x�N�g���𔽓]
					g_Player.PAttack.m_radius[count] *= 2.5f;					// �e���a��傫�����ăZ�b�g
					g_Player.PAttack.m_LifeCounter[count]-= 1;																// ���˕Ԃ萔���}�C�i�X1

				}
				else {																										// �ݒ蒵�˕Ԃ萔���A���˕Ԃ��Ă���Ύ��s
					g_Player.PAttack.m_AttackFlag[count] = 0;																// �����t���O��0�ɖ߂�
				}

			}

			// �v���C���[�e�ʒu���W���i�G���A�x�͈͒l�{�e���a�j�̏ꍇ�A�␳����
				if ( g_Player.PAttack.m_vApos[count].m_y + g_Player.PAttack.m_radius[count] <= -g_Ereamap.m_y 
				  || g_Player.PAttack.m_vApos[count].m_y - g_Player.PAttack.m_radius[count] >=  g_Ereamap.m_y ) {

				// �e�̒��˕Ԃ萔�ݒ肪0�ɂȂ��Ă��Ȃ��ꍇ
				if ( g_Player.PAttack.m_LifeCounter[count] != 0 ) {

					// ���˕Ԃ�񐔂ɉ������F�ɕω��i�����������ԁj
					g_Player.PAttack.Rcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][0];
					g_Player.PAttack.Gcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][1];
					g_Player.PAttack.Bcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][2];

					g_Player.PAttack.m_vAmov[count].m_y *= -1;																// �ړ��x�N�g���𔽓]
					g_Player.PAttack.m_radius[count] *= 2.5f;					// �e���a��傫�����ăZ�b�g
					g_Player.PAttack.m_LifeCounter[count]--;																// ���˕Ԃ萔���}�C�i�X1

				}
				else {																										// �ݒ蒵�˕Ԃ萔���A���˕Ԃ��Ă���Ύ��s
					g_Player.PAttack.m_AttackFlag[count] = 0;																// �����t���O��0�ɖ߂�
				}

			}
			
			// �v���C���[�e�ʒu���W���i�G���A�y�͈͒l�{�e���a�j�̏ꍇ�A�␳����
				if ( g_Player.PAttack.m_vApos[count].m_z + g_Player.PAttack.m_radius[count] <= -g_Ereamap.m_z 
				  || g_Player.PAttack.m_vApos[count].m_z - g_Player.PAttack.m_radius[count] >=  g_Ereamap.m_z ) {
				
				// �e�̒��˕Ԃ萔�ݒ肪0�ɂȂ��Ă��Ȃ��ꍇ
				if ( g_Player.PAttack.m_LifeCounter[count] != 0 ) {
					
					// ���˕Ԃ�񐔂ɉ������F�ɕω��i�����������ԁj
					g_Player.PAttack.Rcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][0];
					g_Player.PAttack.Gcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][1];
					g_Player.PAttack.Bcolor[count] = s_fColorCode[g_Player.PAttack.m_LifeCounter[count]][2];

					g_Player.PAttack.m_vAmov[count].m_z *= -1;																// �ړ��x�N�g���𔽓]
					g_Player.PAttack.m_radius[count] *= 2.5f;					// �e���a��傫�����ăZ�b�g
					g_Player.PAttack.m_LifeCounter[count]--;																// ���˕Ԃ萔���}�C�i�X1
					
				}
				else {																										// �ݒ蒵�˕Ԃ萔���A���˕Ԃ��Ă���Ύ��s
					g_Player.PAttack.m_AttackFlag[count] = 0;																// �����t���O��0�ɖ߂�
				}

			}
		
		}
	
	}

}

/**********************************************************
	�v���C���[�U���`��֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�����t���O��1�ɂȂ��Ă���e�̕`�揈�����s���֐��B
	�e�𕽍s�ړ����āA���˕Ԃ�ɉ������F��ݒ�A���̌�
	�`�揈�����s���Ă���B
**********************************************************/

void PAttack_Display( void )
{

	// �ő�e���ˉ\�������[�v���s��
	for ( int count = 0; count < MAX_ATTACK; count++ ) {
		
		// �����t���O��1�ɂȂ��Ă���Βe�̏������s��
		if ( g_Player.PAttack.m_AttackFlag[count] == 1 ) {
			
			glPushMatrix();

			// �e�̍��W���X�V
			glTranslatef( g_Player.PAttack.m_vApos[count].m_x, 
						  g_Player.PAttack.m_vApos[count].m_y, 
						  g_Player.PAttack.m_vApos[count].m_z );
    
			// �F�̐ݒ�
			glColor3f( g_Player.PAttack.Rcolor[count], g_Player.PAttack.Gcolor[count], g_Player.PAttack.Bcolor[count] );
			
			// �e�̔��a���擾
			float radius = g_Player.PAttack.m_radius[count];
			// �e��\��
			gluSphere( globj, radius, 20, 20 );

			glPopMatrix();
	
		}

	}

}

