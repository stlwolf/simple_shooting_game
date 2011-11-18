
/**********************************************************
	�C���N���[�h�t�@�C��
**********************************************************/

#include "CollisionJudge.h"

/**********************************************************
	���v���C���[�e�E�^�[�Q�b�g���Փ˔���֐�

	�@�����F�Փˏ������s���v���C���[�e�̔ԍ�
	�߂�l�F�Ȃ�

	�v���C���[�e�ƃ^�[�Q�b�g�Ƃ̋����𑪂蓖���蔻����s���B
	�e�ƃ^�[�Q�b�g�Ƌ����x�N�g�����쐬���Ă��̒������A
	�e�{�^�[�Q�b�g�̔��a���Z����Γ����菈�����s���B
	�^�[�Q�b�g�̑��݃t���O���I�t�ɂ��āA���^�[�Q�b�g����
	����炷�B
**********************************************************/

void Sphere_Bullet_Collision( int count )
{

	float l, r;

	VECTOR3 APvec, BPvec, BP1vec, Pvec;

	// �^�[�Q�b�g���̏���
	for ( int no = 0; no < MAX_TARGET; no++ ) {

		// �Y���^�[�Q�b�g�̍��W���擾�i�ʒu�x�N�g���j
		Pvec.m_x = g_Target[no].Obj.m_vPos.m_x;
		Pvec.m_y = g_Target[no].Obj.m_vPos.m_y;
		Pvec.m_z = g_Target[no].Obj.m_vPos.m_z;

		// �^�[�Q�b�g�ƒe�Ƃ̈ʒu�̍�����x�N�g��������i�e�ƃ^�[�Q�b�g�̋����j
		BPvec.Set_XYZ( g_Player.PAttack.m_vApos[count].m_x - Pvec.m_x,
					   g_Player.PAttack.m_vApos[count].m_y - Pvec.m_y,
					   g_Player.PAttack.m_vApos[count].m_z - Pvec.m_z );


		// �x�N�g���̒��������߂�
		l = Vec3_Length( BPvec );

		// �^�[�Q�b�g�ƒe�̔��a�����ꂼ�ꋁ�߁A����𑫂��ē����蔼�a���Z�o
		r = g_Target[no].m_radius + g_Player.PAttack.m_radius[count];

		// �����^�[�Q�b�g�E�e�x�N�g���̒����������蔼�a���Z����Ύ��s
		if ( l <= r ) {
			
			// �e�̑��݃t���O��0�Ƀ��Z�b�g�i�`����I���j
			g_Player.PAttack.m_AttackFlag[count] = 0;

			// �^�[�Q�b�g�̑��݃t���O��1�Ȃ�Ύ��s
			if ( g_Target[no].m_nLifeFlag == 1 ) {
				// �G���A�}�b�v�ϐ�����c��^�[�Q�b�g�����擾���Ĉ���炵�ăZ�b�g
				g_Ereamap.m_TargetNum--;
			}

			//�@�^�[�Q�b�g�̑��݃t���O��0���Z�b�g
			g_Target[no].m_nLifeFlag = 0;

			return;

		}

	}

}

