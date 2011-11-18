
/**********************************************************
	�C���N���[�h�t�@�C��
**********************************************************/

#include "main.h"
#include "../Gameprog/Gamemain.h"

/**********************************************************
	�O���[�o���ϐ���`
**********************************************************/

HDC		hDC;								// �f�o�C�X�R���e�L�X�g
HGLRC	hRC;								// OpenGL�f�o�C�X�R���e�L�X�g
HWND	hWnd;								// �E�C���h�E�n���h��

int g_CxFrameSize;							// �E�C���h�E�g�̕�
int g_CyFrameSize;							// �E�C���h�E�g�̍���

int g_CyCaptionSize;						// �^�C�g���o�[�̍���

int g_CxScreenSize;							// �f�X�N�g�b�v�̕�
int g_CyScreenSize;							// �f�X�N�g�b�v�̍���

char OBJFileName[2][20] = {					// �I�u�W�F�N�g�t�@�C����
        "Dat/dat1.pol",
        "Dat/dat2.pol"
};

/**********************************************************
	���b�Z�[�W�v���V�[�W���֐�

	��P�����F�E�B���h�E�n���h��
	��Q�����F�E�B���h�E���b�Z�[�W
	��R�����F���p�����[�^
	��S�����F���p�����[�^
	�@�߂�l�F�v���V�[�W���ŏ����ł��Ȃ����b�Z�[�W�����̌���

	�쐬�����E�C���h�E�ɑ΂��đ���ꂽ���b�Z�[�W����������B
	�d�r�b�L�[�������ꂽ���̏I�������ƃE�B���h�E�T�C�Y��
	�ύX���ꂽ���̏������s���B
**********************************************************/

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg ) {

		case WM_PAINT:											// �ĕ\��
			break;

		case WM_SIZE:											// �T�C�Y�ύX
			
			WGL_Begin();										// OpenGL�`��J�n
			Reshape_Window( LOWORD(lParam), HIWORD(lParam) );	// �E�C���h�E�T�C�Y�ύX�֐� 
			WGL_End();											// OpenGL�`��I��

			break;

		case WM_CHAR:
        
			switch ( wParam ) {
				
				case  27:										// �d�r�b�L�[�������ꂽ���̏���
				case 'q':
				case 'Q':
				
					PostQuitMessage(0);
					break;

				case ' ':
					break;
			}
			break;

		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );

}

/**********************************************************
	�E�C���h�E�T�C�Y�ύX�֐�

	��P�����F�ύX���ꂽ�E�C���h�E�̉���
	��Q�����F�ύX���ꂽ�E�C���h�E�̏c��
	�@�߂�l�F�Ȃ�

	�E�C���h�E�T�C�Y���ύX���ꂽ���Ɏ��s�����֐��B
	�V���������E�c�����g���ĐV�����v���W�F�N�V�����s���
	�쐬�E�ݒ肷��B
**********************************************************/

void Reshape_Window( int width, int height ) 
{

	float w = (float)width;								// �X�V��̃E�C���h�E����
	float h = (float)height;							// �X�V��̃E�C���h�E����

	glViewport( 0, 0, width, height );					// �r���[�|�[�g�ݒ�

	glMatrixMode( GL_PROJECTION );						// �v���W�F�N�V�����s��ɐݒ�
	
	glLoadIdentity();									// �s�񏉊���
	gluPerspective( 45.0f, w / h, 1.0f, 500.0f );

	glMatrixMode( GL_MODELVIEW );						// ���f���r���[�s��ɐݒ�

	glLoadIdentity();									// �s�񏉊���

}

/**********************************************************
	�E�C���h�E�������֐�

	��P�����F�^�C�g���o�[�ɕ\�����镶����
	��Q�����F�쐬����E�C���h�E�̂w���W
	��R�����F�쐬����E�C���h�E�̂x���W
	��S�����F�쐬����E�C���h�E�̉���
	��T�����F�쐬����E�C���h�E�̍���
	�@�߂�l�F�쐬�����E�C���h�E�̃n���h��

	�E�C���h�E�Y�`�o�h���g���ăE�C���h�E�{�̂��쐬����B
	�����Ƃ��ēn���ꂽ�������ɃT�C�Y�������肵�č쐬�B
	�o�^�����̍ۂɂQ�d�N�������Ă��Ȃ����`�F�b�N���Ă���B
**********************************************************/

HWND Init_Window( char *title, int x, int y, int width, int height )
{

	WNDCLASSEX	wc;
	
	static HINSTANCE hInstance = 0;

	g_CxFrameSize		= GetSystemMetrics( SM_CXFRAME );				// �E�C���h�E�g�̕����擾
	g_CyFrameSize		= GetSystemMetrics( SM_CYFRAME );				// �E�C���h�E�g�̍������擾

	g_CyCaptionSize	= GetSystemMetrics( SM_CYCAPTION );					// �^�C�g���o�[�̍������擾

	g_CxScreenSize	= GetSystemMetrics( SM_CXSCREEN );					// �f�X�N�g�b�v�̕����擾
	g_CyScreenSize	= GetSystemMetrics( SM_CYSCREEN );					// �f�X�N�g�b�v�̍������擾

	// �E�C���h�E�C���X�^���X���O�Ȃ�Γo�^�����i��d�N���h�~�����j
	if ( hInstance == 0 ) {

		hInstance		 = GetModuleHandle( NULL );						// �C���X�^���X�̎擾

		wc.cbSize		 = sizeof(wc);									// �E�C���h�E�N���X�̃T�C�Y
		wc.style		 = CS_HREDRAW | CS_VREDRAW;						// �E�C���h�E�X�^�C��
		wc.lpfnWndProc	 = WindowProc;									// ���b�Z�[�W�����֐��̓o�^
		wc.cbClsExtra	 = 0;
		wc.cbWndExtra	 = 0;
		wc.hInstance	 = hInstance;									// �C���X�^���X�̓o�^
		wc.hIcon		 = LoadIcon( NULL, IDI_WINLOGO );				// �W���A�C�R���o�^
		wc.hIconSm		 = LoadIcon( NULL, IDI_APPLICATION );			// �������A�C�R���o�^
		wc.hCursor		 = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );	// �J�[�\���o�^
		wc.hbrBackground = NULL;										// �E�C���h�E�w�i�F
		wc.lpszMenuName	 = NULL;										// ���j���[�ݒ�iNULL�͎g�p���Ȃ��j
		wc.lpszClassName = "OpenGL";									// �E�C���h�E�N���X�̖��O

		// �R�R���E�C���h�E�N���X�o�^����
		if ( !RegisterClassEx( &wc ) ){
			
			// �E�C���h�E�N���X�o�^�G���[���̕\��
			MessageBox( NULL, "RegisterClass() Failed: Cannot register window class", "Error", MB_OK );
			return NULL;

		}
	}

	width  += ( g_CxFrameSize * 2 );										//�@width�T�C�Y�{�i�t���[���g�̕����Q�j
	height += ( g_CyFrameSize * 2 ) + g_CyCaptionSize;						// height�T�C�Y�{�i�t���[���g�̍������Q�j�{�^�C�g���o�[����

	// �E�C���h�E�쐬����
	hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,								// �g���E�C���h�E�T�C�Y
						   "OpenGL",														// �o�^����Ă���N���X��
						    title,															// �E�C���h�E���i�^�C�g���o�[���j
						   WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,			// �E�C���h�E�X�^�C��
						   x,																// �E�C���h�E�������ʒu
						   y,																// �E�C���h�E�c�����ʒu
						   width,															// �E�C���h�E�̕�
						   height,															// �E�C���h�E�̍���
						   NULL,															// �I�[�i�[�E�C���h�E�̃n���h��
						   NULL,															// ���j���[�n���h���܂��͎q�E�C���h�EID
						   hInstance,														// �E�C���h�E�Ɋ֘A�t����C���X�^���X
						   NULL																// �ǉ����
						   );

	// �E�C���h�E�쐬������ɏ������ꂽ���̃`�F�b�N
	if ( hWnd == NULL ) {

		// �E�C���h�E�쐬�G���[���̕\��
		MessageBox( NULL, "CreateWindow() Failed: Cannot create a window.", "Error", MB_OK );
		return NULL;
	
	}

	return hWnd;

}

/**********************************************************
	OpenGL�������֐�

	�@�����F�Ȃ�
	�߂�l�F�����������ɂ�TRUE�i1)��Ԃ��A���s���ɂ�0�iFALSE�j
			��Ԃ��B

	�E�C���h�E�쐬���OpenGL���g�p�ł���悤�ɏ��������s��
	�֐��B��ʂ̃s�N�Z���t�H�[�}�b�g���擾����OpenGL���g�p
	�\���ǂ������ׂ�B�g�p�Ȃ�΃����^�����O�R���e�L�X�g
	���쐬����B
**********************************************************/

int Init_GLWindow( void )
{

	int pf;

	PIXELFORMATDESCRIPTOR pfd;

    pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);	// ���̍\���̂̃T�C�Y
    pfd.nVersion        = 1;								// �\���̂̃o�[�W����
    pfd.dwFlags         = PFD_DRAW_TO_WINDOW				// �E�B���h�E�ɒ��ڕ`�悷��Ƃ��Ɏw��
                        | PFD_SUPPORT_OPENGL				// OpenGL���T�|�[�g
                        | PFD_DOUBLEBUFFER;					// �o�b�t�@�ݒ�
															// PFD_DOUBLEBUFFER  : �_�u���o�b�t�@
															// NULL              : �V���O���o�b�t�@
															//                     �V���O���o�b�t�@�̎���
															//                     SwapBuffers�����Ă͂����Ȃ��B
    pfd.iPixelType      = PFD_TYPE_RGBA;					// �s�N�Z���f�[�^�̃^�C�v���w��
    pfd.cColorBits      = 32;								// �J���[�o�b�t�@�r�b�g��
    pfd.cRedBits        = 8;								// RGBA �� R �r�b�g��
    pfd.cRedShift       = 0;								// R �r�b�g�V�t�g�ݒ�
    pfd.cGreenBits      = 8;								// RGBA �� G �r�b�g��
    pfd.cGreenShift     = 0;								// G �r�b�g�V�t�g�ݒ�
    pfd.cBlueBits       = 8;								// RGBA �� B �r�b�g��
    pfd.cBlueShift      = 0;								// B �r�b�g�V�t�g�ݒ�
    pfd.cAlphaBits      = 8;								// RGBA �� A �r�b�g��
    pfd.cAlphaShift     = 0;								// A �r�b�g�V�t�g�ݒ�
    pfd.cAccumBits      = 0;								// �A�L�������[�V�����o�b�t�@�̃r�b�g��
    pfd.cAccumRedBits   = 0;								// �A�L�������[�V�����o�b�t�@�� R�r�b�g��
    pfd.cAccumGreenBits = 0;								// �A�L�������[�V�����o�b�t�@�� G�r�b�g��
    pfd.cAccumBlueBits  = 0;								// �A�L�������[�V�����o�b�t�@�� B�r�b�g��
    pfd.cAccumAlphaBits = 0;								// �A�L�������[�V�����o�b�t�@�� A�r�b�g��
    pfd.cDepthBits      = 32;								// �f�B�v�X�o�b�t�@�r�b�g��
    pfd.cStencilBits    = 0;								// �X�e���V���o�b�t�@�r�b�g��
    pfd.cAuxBuffers     = 0;								// �⏕�o�b�t�@�̃r�b�g���i���g�p�j
    pfd.iLayerType      = PFD_MAIN_PLANE;					// ���C���[�^�C�v
    pfd.bReserved       = 0;								// �I�[�o�[���C�ƃA���_�[���C�̐��i���g�p�j
    pfd.dwLayerMask     = 0;								// ���C���}�X�N�i���g�p�j
    pfd.dwVisibleMask   = 0;								// �r�W�u���}�X�N�i���g�p�j
    pfd.dwDamageMask    = 0;								// �_���[�W�}�X�N�i���g�p�j

	hDC = GetDC( hWnd );									// �f�o�C�X�R���e�L�X�g�擾

	pf	= ChoosePixelFormat( hDC, &pfd );					// �s�N�Z��ID�̎擾

	if ( pf == 0 ) {
		MessageBox(NULL, "ChoosePixelFormat() Failed: Cannot find a suitable pixel format.", "Error", MB_OK);
        return 0;
	}

	// �f�o�C�X�R���e�L�X�g�Ƀs�N�Z���t�H�|�}�b�g��ݒ肷��
	if ( SetPixelFormat( hDC, pf, &pfd ) == FALSE ) {
		MessageBox(NULL, "SetPixelFormat() Failed: Cannot set format specified.", "Error", MB_OK );
        return 0;
	}

	// �s�N�Z��ID����t�H�[�}�b�g�������擾
	DescribePixelFormat( hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd );

	// �P�U�F�܂��͂Q�T�U�F���̏���
	if ( pfd.dwFlags & PFD_NEED_PALETTE || pfd.iPixelType == PFD_TYPE_COLORINDEX ) {
		MessageBox( NULL, "256 Color un no", "Error", MB_OK );
		return 0;
	}

	hRC = wglCreateContext( hDC );				// �����^�����O�R���e�L�X�g�̍쐬

	ReleaseDC( hWnd, hDC );						// �f�o�C�X�R���e�L�X�g�̉��

	return TRUE;

}
 
/**********************************************************
	�I�������֐�

	�@�����F�Ȃ�
	�߂�l�F�ʏ�ʂ菈�����I�������0���Ԃ�B

	�E�C���h�E���폜���鎞�Ɏ��s�����֐��B�f�o�C�X
	�R���e�L�X�g�E�����^�����O�R���e�L�X�g�̉���폜
	���s���B�E�C���h�E���폜����B
**********************************************************/

int End_Win( void )
{

	wglMakeCurrent( NULL, NULL );				// �����^�����O�R���e�L�X�g�w�����
	ReleaseDC( hWnd, hDC );						// �f�o�C�X�R���e�L�X�g�̉��
	wglDeleteContext( hRC );					// �����^�����O�R���e�L�X�g�폜
	DestroyWindow( hWnd );						// �E�C���h�E�폜

	return 0;

}

/**********************************************************
	���C���֐�
**********************************************************/

int APIENTRY WinMain( HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow )
{

	int ret;
	bool com;
	
	TIMECAPS tc;												// �e�o�r�����p�^�C���ϐ�

	MSG	msg;													// �E�C�����b�Z�[�W�����p�ϐ�

	hWnd = Init_Window( "OpenGL Window", 0, 0, 640, 480 );		// �E�C���h�E�������֐�
	
	if ( hWnd == NULL ) {
		End_Win();												// �I�������֐�
		exit(1);												// �G���[�I��
	}

	ret = Init_GLWindow();										// OpenGL�������֐�

	if ( ret == 0 ) {
		End_Win();												// �I�������֐�
		exit(1);												// �G���[�I��
	}

	WGL_Begin();												// OpenGL�`��J�n
	Init_OpenGL();
	WGL_End();													// OpenGL�`��I��

	srand( (unsigned)time( NULL ) );

	Init_Game();

	ShowWindow( hWnd, SW_SHOW );								// �E�C���h�E�̕\�����[�h�ݒ�
	UpdateWindow( hWnd );										// �E�C���h�E�̕\��

	// �e�o�r�����J�n�ɂ������Ă̏�����
	timeGetDevCaps( &tc, sizeof(TIMECAPS) );					// ���s�\�ȕ���\�擾
	timeBeginPeriod( tc.wPeriodMin );							// timeGettime()�̐��x���グ��

	fpsChk1 = timeGetTime();
	fpsCnt = 0;

	/* �ʐM�̐ڑ����������̊Ԃōs�� */

	CNetClient* cnet = NULL;

	// �C���X�^���X�̍쐬
	cnet = new CNetClient();

	// �쐬�ɐ�������Ύ��s
	if ( cnet != NULL ) {
		
		// �T�[�o�ւ̐ڑ��p�v�����s��
		com = cnet->ServerConnect();

		// �ڑ��Ɏ��s�����ꍇ�A�C���X�^���X���j��
		if ( com == false ) {
			delete(cnet);
			cnet = NULL;
		}

	}

	/* �����܂� */

	while( 1 ) {

		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) == FALSE ) {

				if ( Game_Main( hWnd ) == FALSE ) {

					// �ʐM�ڑ����������ĂΏI�������i�G���f�B���O�֐�����Esc�������ꂽ�Ƃ��͂��������j
					if ( cnet != NULL ) {
						// �I�������֐��i�X���b�h�֘A�j
						cnet->End();
						// �f�X�g���N�^���Ń\�P�b�g�̃N���[�Y���s��
						delete(cnet);
					}

				DestroyWindow( hWnd );							// �E�B���h�E��j��
				return 0;										// GameMain�֐�������I�����Ȃ������狭���I��
			}

		}
		else {

			if ( GetMessage( &msg, NULL, 0, 0 ) == FALSE ) { break; }

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		
		}
	
	}

	timeEndPeriod( tc.wPeriodMin );								// timeGetTime()�̐��x��߂�

	End_Win();													// �I�������֐�

	/* �ʐM�����֘A�̌�Еt�� */

	// �ʐM�ڑ����������ĂΏI������
	if ( cnet != NULL ) {
	
		// �I�������֐��i�X���b�h�֘A�j
		cnet->End();

		// �f�X�g���N�^���Ń\�P�b�g�̃N���[�Y���s��
		delete(cnet);
	
	}

	/* �����܂� */

	return 0;

}
