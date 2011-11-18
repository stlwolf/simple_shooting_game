
/*********************************************************
	�C���N���[�h�t�@�C��
*********************************************************/

#include "main.h"						// �A�v���P�[�V�����S�̂Ɋւ��`
#include "Input.h"

/**********************************************************
	�O���[�o���ϐ�
**********************************************************/

int g_nKeyData     = 0;											// ���A���^�C���ȃL�[���
int g_nKeyEdgeData = 0;											// �L�[�̃G�b�W���

int naKeyData[][2] = {
	{ VK_NUMPAD8, KEY_UP      },	// ������i�e���L�[�W�j
	{ VK_NUMPAD2, KEY_DOWN    },	// �������i�e���L�[�Q�j
	{ VK_NUMPAD4, KEY_LEFT    },	// �������i�e���L�[�S�j
	{ VK_NUMPAD6, KEY_RIGHT   },	// �E�����i�e���L�[�U�j
	{ VK_NUMPAD1, KEY_ROTL	  },	// �e���L�[�P
	{ VK_NUMPAD3, KEY_ROTR    },	// �e���L�[�R
	{        'Z', KEY_BUTTON1 },	// �y�L�[
	{		 'X', KEY_BUTTON2 },	// �w�L�[
	{  VK_RETURN, KEY_ENTER	  },	// �d�m�s�d�q�L�[
	{  VK_ESCAPE, KEY_ESC	  },	// �d�b�r�L�[
	{          0, 0           }		// while���[�v�I���`�F�b�N
};

/**********************************************************
	���͏����֐�

	out	:	g_nKeyData		---	bit 0: Up			0x01
								bit 1: Down			0x02
								bit 2: Left			0x04
								bit 3: Right		0x08
								bit 4: Button 0		0x10
								bit 5: Button 1		0x20
	
	g_nKeyEdgeData	---	��L�̃G�b�W���

	�@�����F�Ȃ�
	�߂�l�F�Ȃ�

	�����ꂽ�L�[�����擾����֐��B�����ꂽ�L�[�������
	���f�[�^�����A�X�ɂ��ꂩ��G�b�W�f�[�^���쐬����B
**********************************************************/

void Input_Device( void )
{
	int	nKeyCode;												// GetAsyncKeyState�ɓn�����z�L�[�R�[�h
	int	nCount = 0;												// ���[�v�J�E���^��naKeyData�pindex
	
	static int nOld = 0;										// �O��̏o�̓f�[�^
	
	nOld = g_nKeyData;											// �O��̏o�̓f�[�^���o���Ă���
	
	g_nKeyData = 0;												// ����̏o�̓f�[�^��������
	
	while ( nKeyCode = naKeyData[nCount][0] ) {					// Key Data��0�ɂȂ�܂Ń��[�v
		
		if ( GetAsyncKeyState(nKeyCode) & 0x8000 ) {			// �ŏ�ʃr�b�g�������Ă���΃L�[��������Ă���
			g_nKeyData |= naKeyData[nCount][1];					// �����ꂽ�L�[�ɑΉ�����r�b�g��1�ɂ���
		}
		++nCount;												// ����Key Data�ɐi�߂�
	}
	g_nKeyEdgeData = ( g_nKeyData ^ nOld ) & g_nKeyData;		// �G�b�W�f�[�^�����
}
