
/**********************************************************
	�C���N���[�h�t�@�C��
**********************************************************/

#include "Player.h"

/**********************************************************
	�O���[�o���ϐ�
**********************************************************/

PLAYER g_Player( -0.2f );

/**********************************************************
	�v���C���[�������֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�v���C���[���̏��������s���B
	�v���C���[�̉�]���̐ݒ�A�J�����x�N�g���E���[�h�̐ݒ�
	�v���C���[�e�̐����t���O�̏��������s���B
**********************************************************/

void Player_Init( void )
{

	MATRIX4x4 mat;

	// �v���C���[��]���������ݒ�
	// ���[�J���w��
	g_Player.Obj.m_vAxisX.m_x = 1.0f;
	g_Player.Obj.m_vAxisX.m_y = 0.0f;
	g_Player.Obj.m_vAxisX.m_z = 0.0f;
	g_Player.Obj.m_vAxisX.m_w = 1.0f;

	// ���[�J���x��
	g_Player.Obj.m_vAxisY.m_x = 0.0f;
	g_Player.Obj.m_vAxisY.m_y = 1.0f;
	g_Player.Obj.m_vAxisY.m_z = 0.0f;
	g_Player.Obj.m_vAxisY.m_w = 1.0f;

	// ���[�J���y��
	g_Player.Obj.m_vAxisZ.m_x = 0.0f;
	g_Player.Obj.m_vAxisZ.m_y = 0.0f;
	g_Player.Obj.m_vAxisZ.m_z = 1.0f;
	g_Player.Obj.m_vAxisZ.m_w = 1.0f;

	// �v���C���[�ʒu��񏉊���
	g_Player.Obj.m_vPos.m_x = 0.0f;
	g_Player.Obj.m_vPos.m_y = 0.0f;
	g_Player.Obj.m_vPos.m_z = 0.0f;
	
	// �v���C���[��]�ʂ̏�����
	g_Player.Obj.m_matRotate     = mat;
	g_Player.Obj.m_matNow_Rotate = mat;

	// �J�������[�h���O�̃x�N�g��������
	// ���_�x�N�g��������
	g_Player.m_vEye[0].m_x = 0.0f;
	g_Player.m_vEye[0].m_y = 1.2f;
	g_Player.m_vEye[0].m_z = 5.5f;
	g_Player.m_vEye[0].m_w = 1.0f;

	// �����_�̏�����
	g_Player.m_vUp[0].m_x = 0.0f;
	g_Player.m_vUp[0].m_y = 1.0f;
	g_Player.m_vUp[0].m_z = 0.0f;
	g_Player.m_vUp[0].m_w = 1.0f;


	// �J�������[�h���P�̃x�N�g��������
	// ���_�x�N�g��������
	g_Player.m_vEye[1].m_x = 0.0f;
	g_Player.m_vEye[1].m_y = 0.2f;
	g_Player.m_vEye[1].m_z = 0.0f;
	g_Player.m_vEye[1].m_w = 1.0f;

	// �����_�̏�����
	g_Player.m_vLook[1].m_x =  0.0f;
	g_Player.m_vLook[1].m_y =  0.2f;
	g_Player.m_vLook[1].m_z = -0.5f;
	g_Player.m_vLook[1].m_w =  1.0f;

	// Up�x�N�g���̏�����
	g_Player.m_vUp[1].m_x = 0.0f;
	g_Player.m_vUp[1].m_y = 1.0f;
	g_Player.m_vUp[1].m_z = 0.0f;
	g_Player.m_vUp[1].m_w = 1.0f;

	// �����J�������[�h���O�ɐݒ�
	g_Player.m_nCameraNo = 0;

	// �v���C���[�e�̃t���O������
	for ( int count = 0; count < MAX_ATTACK; count++ ) {
		g_Player.PAttack.m_AttackFlag[count] = 0;
	}

}

/**********************************************************
	�v���C���[�ړ������֐�

	�@�����F�Ȃ�
	�߂�l�F�Ȃ�

	�v���C���[�̈ړ����������s���֐��B���͂��ꂽ�L�[��
	�����āA�ړ��E��]�E�U���E�J�������[�h�̕ύX���s���B
	�܂��ړ��̍ۂɔ͈͊O�ɏo�Ȃ��悤�ɕ␳���s���B
**********************************************************/

void Player_Move( void )
{

	if ( g_nKeyData & KEY_UP ) {							// [8]�L�[
		g_Player.Obj.m_AngleX = -2.0f;						// �v���C���[�̂w����]
	}
	else if ( g_nKeyData & KEY_DOWN ) {						// [2]�L�[
		g_Player.Obj.m_AngleX = 2.0f;						// �v���C���[�̂w����]
	}

	if ( g_nKeyData & KEY_LEFT ) {							// [4]�L�[
		g_Player.Obj.m_AngleY = 1.0f;						// �v���C���[�̂x����]
	}
	else if ( g_nKeyData & KEY_RIGHT ) {					// [6]�L�[
		g_Player.Obj.m_AngleY = -1.0f;						// �v���C���[�̂x����]
	}

	if ( g_nKeyData & KEY_ROTL ) {							// [1]�L�[
		g_Player.Obj.m_AngleZ =  1.0f;						// �v���C���[�̂y����]		
	}
	else if ( g_nKeyData & KEY_ROTR ) {						// [3]�L�[
		g_Player.Obj.m_AngleZ = -1.0f;						// �v���C���[�̂y����]
	}

	if ( g_nKeyEdgeData & KEY_BUTTON2 ) {					// [X]�L�[

		// ���݂̃J�������[�h���擾
		int key = g_Player.m_nCameraNo;

		// �J�������[�h����0�Ȃ���s
		if ( key < 1 ) {
			// �J�������[�h��1�ɕύX
			g_Player.m_nCameraNo++;
		}
		// �J�������[�hNo��1�Ȃ���s
		else if ( key > 0 ) {
			// �J�������[�h��0�ɕύX
			g_Player.m_nCameraNo--; 
		}

	}

	if ( g_nKeyEdgeData & KEY_BUTTON1 ) {					// [Z]�L�[

		// �v���C���[�e�����t���O��0�̏ꍇ���s
		if ( g_Player.PAttack.m_AttackFlag[g_Player.PAttack.m_FlagCounter] == 0 ) {

			// �v���C���[�e�������֐������s����
			PAttack_Init( g_Player.PAttack.m_FlagCounter );
				
			// �����t���O�J�E���g���i�ő唭�ˉ\��-1�j��菬������΃J�E���^���v���X
			if ( g_Player.PAttack.m_FlagCounter < ( MAX_ATTACK - 1 ) ) {
				g_Player.PAttack.m_FlagCounter++;
			}
			else {
				g_Player.PAttack.m_FlagCounter = 0;			// �J�E���^��0�ɖ߂�
			}

		}

	}

	// �P�t���[���O�̉�]�ʂ���V������]�����쐬����
	Mat4x4_Vec4_Multiplay( &g_Player.Obj.m_vAxisX, &g_Player.Obj.m_matNow_Rotate, &g_Player.Obj.m_vAxisX );
	Mat4x4_Vec4_Multiplay( &g_Player.Obj.m_vAxisY, &g_Player.Obj.m_matNow_Rotate, &g_Player.Obj.m_vAxisY );  
	Mat4x4_Vec4_Multiplay( &g_Player.Obj.m_vAxisZ, &g_Player.Obj.m_matNow_Rotate, &g_Player.Obj.m_vAxisZ );

	VECTOR4 mov1, mov2;

	memset( &mov1, 0x00, sizeof(VECTOR4) );

	// �ړ��X�s�[�h���擾
	mov1.m_z = g_Player.Obj.m_Speed;

	// �ړ��x�N�g���ɉ�]�s����|���ĐV�����ړ��x�N�g�����쐬����
	Mat4x4_Vec4_Multiplay( &mov2, &g_Player.Obj.m_matRotate, &mov1 );

	// �v���C���[�̈ʒu�Ɉړ��x�N�g���̒l�𑫂��čX�V
	g_Player.Obj.m_vPos.m_x += mov2.m_x;
	g_Player.Obj.m_vPos.m_y += mov2.m_y;
	g_Player.Obj.m_vPos.m_z += mov2.m_z;

	// �v���C���[�ʒu���W���G���A�͈͊O�ɏo���Ƃ��Ɏ��s�i�ʒu��␳�j
	if ( g_Player.Obj.m_vPos.m_x - 0.8 <= -g_Ereamap.m_x
	  || g_Player.Obj.m_vPos.m_x + 0.8 >=  g_Ereamap.m_x ) {

		// �ړ��x�N�g�����ʒu��߂�
		g_Player.Obj.m_vPos.m_x -= mov2.m_x;
		g_Player.Obj.m_vPos.m_y -= mov2.m_y;
		g_Player.Obj.m_vPos.m_z -= mov2.m_z;

	}

	// �v���C���[�ʒu���W���G���A�͈͊O�ɏo���Ƃ��Ɏ��s�i�ʒu��␳�j
	if ( g_Player.Obj.m_vPos.m_y - 0.8 <= -g_Ereamap.m_y
	  || g_Player.Obj.m_vPos.m_y + 0.8 >=  g_Ereamap.m_y ) {

		// �ړ��x�N�g�����ʒu��߂�
		g_Player.Obj.m_vPos.m_x -= mov2.m_x;
		g_Player.Obj.m_vPos.m_y -= mov2.m_y;
		g_Player.Obj.m_vPos.m_z -= mov2.m_z;

	}
	
	// �v���C���[�ʒu���W���G���A�͈͊O�ɏo���Ƃ��Ɏ��s�i�ʒu��␳�j
	if ( g_Player.Obj.m_vPos.m_z - 0.8 <= -g_Ereamap.m_z
	  || g_Player.Obj.m_vPos.m_z + 0.8 >=  g_Ereamap.m_z ) {

		// �ړ��x�N�g�����ʒu��߂�
		g_Player.Obj.m_vPos.m_x -= mov2.m_x;
		g_Player.Obj.m_vPos.m_y -= mov2.m_y;
		g_Player.Obj.m_vPos.m_z -= mov2.m_z;

	}
	
}

/**********************************************************
	�v���C���[�`�揈���֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�L�[���͂ŕύX���ꂽ��]�ʕ��A�v���C���[�̉�]����
	�ύX���Ă���B�܂��A���_�̍X�V�A�ړ��ʂɉ������ʒu��
	�X�V�A�v���C���[�I�u�W�F�N�g�̕\�����s���B
**********************************************************/

void Player_Display( void )
{

	// ���f���r���[�s��ɐݒ�
	glMatrixMode( GL_MODELVIEW );

	// �s��̏�����
	glLoadIdentity();

	glPushMatrix();

	// �v���C���[��]�������ɉ�]���s��
	glRotatef( g_Player.Obj.m_AngleX, g_Player.Obj.m_vAxisX.m_x, g_Player.Obj.m_vAxisX.m_y, g_Player.Obj.m_vAxisX.m_z );
	glRotatef( g_Player.Obj.m_AngleY, g_Player.Obj.m_vAxisY.m_x, g_Player.Obj.m_vAxisY.m_y, g_Player.Obj.m_vAxisY.m_z );
	glRotatef( g_Player.Obj.m_AngleZ, g_Player.Obj.m_vAxisZ.m_x, g_Player.Obj.m_vAxisZ.m_y, g_Player.Obj.m_vAxisZ.m_z );

	// ��]�ʂ̏�����
	g_Player.Obj.m_AngleX = 0.0f;
	g_Player.Obj.m_AngleY = 0.0f;
	g_Player.Obj.m_AngleZ = 0.0f;

	// ����̃t���[���̉�]�ʂ����o��
	glGetFloatv( GL_MODELVIEW_MATRIX, (float *)&g_Player.Obj.m_matNow_Rotate );

	// ����]�ʍs������f���r���[�s��Ɋ|����
	glMultMatrixf( (float *)&g_Player.Obj.m_matRotate );

	// ����]�ʂ����o��
	glGetFloatv( GL_MODELVIEW_MATRIX, (float *)&g_Player.Obj.m_matRotate );

	glPopMatrix();

	// �J�������[�h�ɉ��������_��ݒ�
	Player_Camera( g_Player.m_nCameraNo );

	glPushMatrix();

	// �v���C���[�ʒu���W�̍X�V�i���s�ړ����s���j
	glTranslatef( g_Player.Obj.m_vPos.m_x, g_Player.Obj.m_vPos.m_y, g_Player.Obj.m_vPos.m_z );

	glMultMatrixf( (float *)&g_Player.Obj.m_matRotate );

	// �|���S���\������
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );

	glDrawArrays( GL_TRIANGLES, 0, g_pObjBuf2->m_nPolcnt * 3 );

	glPopMatrix();

}

/**********************************************************
	�v���C���[�J�����쐬�֐�

	�@�����F�Ȃ�
	�߂�l�F�J�������[�h���̒l

	�v���C���[�̉�]�ʕ��ɍ��킹�āA���_�E�����_�EUp�x�N�g
	������]������B���v���C���[���ړ��������������W���X
	�V����B�J�������W���X�V������̐ݒu�������ōs���B
**********************************************************/

void Player_Camera( int camerano )
{

	VECTOR4 eye, look, up;

	// �J�����̊e���W����]������i�s����|�����킹��j
	Mat4x4_Vec4_Multiplay( &eye, &g_Player.Obj.m_matRotate, &g_Player.m_vEye[camerano] );
	Mat4x4_Vec4_Multiplay( &up, &g_Player.Obj.m_matRotate, &g_Player.m_vUp[camerano] );
	Mat4x4_Vec4_Multiplay( &look, &g_Player.Obj.m_matRotate, &g_Player.m_vLook[camerano] );

	// �J�����̏ꏊ���v�Z
	eye.m_x += g_Player.Obj.m_vPos.m_x;
	eye.m_y += g_Player.Obj.m_vPos.m_y;
	eye.m_z += g_Player.Obj.m_vPos.m_z;

	// �J�����̎��_���v�Z
	look.m_x += g_Player.Obj.m_vPos.m_x;
	look.m_y += g_Player.Obj.m_vPos.m_y;
	look.m_z += g_Player.Obj.m_vPos.m_z;

	// �J�����s��̐ݒ�
	gluLookAt(  eye.m_x,  eye.m_y,  eye.m_z,
			   look.m_x, look.m_y, look.m_z,
			     up.m_x,   up.m_y,   up.m_z );

}
