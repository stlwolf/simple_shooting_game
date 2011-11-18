#if !defined(___PLAYER___H)
#define ___PLAYER___H

/********************************************************
	�C���N���[�h�t�@�C��
********************************************************/

#include "../Winsprog/main.h"
#include "../Winsprog/MatrixCal.h"

#include "../Gameprog/PlayerAttack.h"

/********************************************************
	�}�N����`
********************************************************/

#define MAX_ATTACK (20)

/********************************************************
	�\���̒�`
********************************************************/

struct ATTACK {																// �v���C���[�e�\����

	float m_radius[MAX_ATTACK];												// �v���C���[�e���a

	// �v���C���[�e�̔��a���X�J���[�{����i�����͔{���ƌv�Z���s���e�̔z��ԍ��j
	void Radius_Multiplay( float num, int no ) { m_radius[no] *= num; }

	VECTOR4 m_vApos[MAX_ATTACK], m_vAmov[MAX_ATTACK];						// �v���C���[�e�̈ʒu�x�N�g���ƕ����x�N�g��

	int m_AttackFlag[MAX_ATTACK];											// �v���C���[�e�̐����t���O
	int m_FlagCounter;														// �v���C���[�e�̃t�����t���O���J�E���^�[

	int m_LifeCounter[MAX_ATTACK];											// �v���C���[�e�̒��˕Ԃ�񐔃J�E���^�[

	float Rcolor[MAX_ATTACK], Gcolor[MAX_ATTACK], Bcolor[MAX_ATTACK];		// �v���C���[�e�̂q�f�a�l���
	
};

struct PLAYER {

	// �I�u�W�F�N�g���ʗv�f�\����
	OBJECT_BASE Obj;
	
	// �v���C���[�e�\���̕ϐ�
	ATTACK PAttack;

	int m_nCameraNo;														// �J�������[�h�ϐ�

	VECTOR4 m_vEye[3], m_vLook[3], m_vUp[3];								// ���_�A�����_�AUp�x�N�g��

	// �R���X�g���N�^�֐�
	// �����Ƃ��ēn���ꂽ�l���X�s�[�h�l�ɃZ�b�g����
	PLAYER ( float speed ) {

		Obj.m_Speed = speed;
	
	}

};

/********************************************************
	�O���[�o���ϐ��錾
********************************************************/

extern PLAYER g_Player;

/********************************************************
	�֐��v���g�^�C�v�錾
********************************************************/

extern void Player_Init( void );

extern void Player_Move( void );
extern void Player_Display( void );

extern void Player_Camera( int camerano );

#endif	// #if !defined(___PLAYER___H)
