#if !defined( __MAIN__H)
#define __MAIN__H

/**********************************************************
	�C���N���[�h�w�b�_
**********************************************************/

// �b����֘A�w�b�_�[
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <mbstring.h>

// �E�C���h�E�v���O�����֘A�w�b�_�[
#include <windows.h>

#	pragma comment ( lib, "winmm.lib" )

// OpenGL�֘A�w�b�_�[
//#if GLUT
//	#include <GL/glut.h>							// GLUT�錾 
//#else
	#include <GL/gl.h>								// OpenGL�w�b�_�t�@�C��
	#include <GL/glu.h>
	#pragma comment ( lib, "opengl32.lib" )			// link with Microsoft OpenGL lib
	#pragma comment ( lib, "glu32.lib" )			// link with Microsoft OpenGL Utility lib
//#endif

#include "../GL_Utility/glext.h"

// �����o�͗p�ݒ�
#include "../GL_Utility/ttftext.h"
#pragma comment( lib, "ttftext_r.lib" )

// �Q�[���v���O�����w�b�_�[
#include "Input.h"
#include "MatrixCal.h"

#include "../Gameprog/GameMain.h"

/**********************************************************
	�O���[�o���ϐ���`
**********************************************************/

extern HDC		hDC;								// �f�o�C�X�R���e�L�X�g
extern HGLRC	hRC;								// OpenGL�f�o�C�X�R���e�L�X�g
extern HWND		hWnd;								// �E�C���h�E�n���h��

extern int g_CxFrameSize;							// �E�C���h�E�g�̕�
extern int g_CyFrameSize;							// �E�C���h�E�g�̍���

extern int g_CyCaptionSize;							// �^�C�g���o�[�̍���

extern int g_CxScreenSize;							// �f�X�N�g�b�v�̕�
extern int g_CyScreenSize;							// �f�X�N�g�b�v�̍���

extern char OBJFileName[2][20];						// �I�u�W�F�N�g�t�@�C����

/**********************************************************
	�萔��`
**********************************************************/

#define PI	  (3.1415926535897932384626433832795)	// �~����
#define FPS	  (1000 / 60)							// FPS�l

#define WHITE (1 << 0)
#define RED   (1 << 1)
#define GREEN (1 << 2)
#define BULE  (1 << 3)

/**********************************************************
	�v���g�^�C�v�錾
**********************************************************/

extern int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, int );
extern LRESULT CALLBACK WindowProc( HWND, UINT, WPARAM, LPARAM );
extern void Reshape_Window( int ,int );

extern HWND Init_Window( char *, int, int, int, int );
extern int Init_GLWindow( void );
extern int End_Win( void );

#endif