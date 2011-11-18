
/**********************************************************
	�C���N���[�h�w�b�_�[
**********************************************************/

#include "Gamemain.h"

/**********************************************************
	�O���[�o���ϐ�
**********************************************************/

// �e�o�r�����p�ϐ�
DWORD fpsChk1;
DWORD fpsChk2;

int fpsCnt;

float g_Stringx = -100.0f, g_Stringy = 120.0f;		   // ���@�^�[�Q�b�g���̕����\���ʒu
float g_eStringx = -100.0f, g_eStringy = -220.0f;      // �ΐ푊��^�[�Q�b�g���̕����\���ʒu

OBJ_VARIABLE	*g_pObjData;

OBJ_VERTEX_BUF  *g_pObjBuf;
OBJ_VERTEX_BUF2 *g_pObjBuf2;

GLuint g_nVerbufNo[3];

GLUquadricObj  *globj;

// �t���O�\���̕ϐ�
FLAG g_Flag;

TTF_TEXT g_Ttftext[6];								// �����\���pTTF�ϐ�

/**********************************************************
	OpenGL�������֐�

	�@�����F�Ȃ�
	�߂�l�F�����������ɂ�TRUE���A�������s���ɂ�FALSE��Ԃ��B

	OpenGL���g�p���ĕ`����s�����߂ɕK�v�ȏ��������s���B
	�X��pol�f�[�^���e�L�X�g�f�[�^����ǂݍ���Ő��l�f�[�^
	�ɕϊ�����B���̃f�[�^�����Ɋe���_�f�[�^�̗̈���m�ۂ��A
	�܂��I�u�W�F�N�g�̒��_���𒸓_�o�b�t�@����ǂݍ���
	�悤�ɐݒ肷��B
**********************************************************/

int Init_OpenGL( void )
{

	int ret;
	int nbufsize;

	ret = Get_GLprocs();													// �g���@�\�擾�֐�

	if ( ret == FALSE ) {
		MessageBox( NULL, "OpenGL Expand Error", "Error", MB_OK );
		return FALSE;
	}

	wglSwapIntervalEXT(0);													// V-sync�҂�����

	glEnable( GL_DEPTH_TEST );

	globj = gluNewQuadric();

	// �w�肵���e�L�X�g�t�@�C������I�u�W�F�N�g�f�[�^��ǂݍ���
	g_pObjData = Load_ObjFile( OBJFileName[0] );							// �|���S���t�@�C���ǂݍ��݊֐�

	if ( g_pObjData == NULL ) { return FALSE; }								// �|���S���t�@�C���ǂݍ��ݎ��s����

	g_pObjBuf2 = Set_VertexBuf( g_pObjData );								// �|���S���f�[�^���璸�_�f�[�^���擾					

	if ( g_pObjBuf2 == NULL ) { return FALSE; }								// ���_�f�[�^�擾���s

	Free_ObjIndex( g_pObjData );											// �|���S���f�[�^���
	g_pObjData = NULL;

	// ���_�o�b�t�@�ݒ�
	glGenBuffersARB( 3, &g_nVerbufNo[0] );									// �o�b�t�@�Ǘ��ԍ��擾

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_nVerbufNo[0] );					// �o�b�t�@�Ǘ��ԍ��I��

	nbufsize = g_pObjBuf2->m_nObjsize;										// �o�b�t�@�T�C�Y���

	glBufferDataARB( GL_ARRAY_BUFFER_ARB, nbufsize, g_pObjBuf2->pcObjData, GL_STATIC_DRAW_ARB );

	// ���_�z��@�ǂݏo���A�h���X
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_nVerbufNo[0] );					// �o�b�t�@�Ǘ��ԍ��I��

	glInterleavedArrays( g_pObjBuf2->m_nPolmode, 0, NULL );					// ���_�o�b�t�@����`��

	glClearColor( 0.0f, 0.0f, 0.0f, 2.0f );									// �w�i�F�N���A

	int i = 0;

	i = ttfTextSysInit( &hWnd );											// �����\�����C�u����������

	memset( &g_Ttftext, 0x00, sizeof(TTF_TEXT) );							// �����\���\���̏�����
	
	for ( int i = 0; i < 6; i++ ) {
		ttfTextDefault( &g_Ttftext[i] );									// �����\���p�������֐�
	}

	return TRUE;

}

/**********************************************************
	�Q�[���������֐�

	�@�����F�Ȃ�
	�߂�l�F������������TRUE��Ԃ��B

	�e�������֐��A�}�b�v�E�v���C���[�E�^�[�Q�b�g�̏�����
	�֐������s����B�t���O�\���̂̃t���O�����Z�b�g�B
**********************************************************/

BOOL Init_Game( void )
{

	// �}�b�v�������֐�
	Map_Init();

	// �v���C���[�������֐�
	Player_Init();

	// �^�[�Q�b�g�������֐�
	Target_Init();

	// �I�[�v�j���O�E�G���f�B���O�t���O��0�ɏ�����
	g_Flag.m_OpeningFlag = 0;
	g_Flag.m_EndingFlag	 = 0;

	return TRUE;
}

/**********************************************************
	�Q�[�����C���֐�

	�@�����F�\�����s���E�B���h�E�̃n���h��
	�߂�l�F�����������ɂ�TRUE��Ԃ��A���s�ɂ�FALSE��Ԃ��B

	�Q�[���{�̂̊֐��B�I�[�v�j���O�E�G���f�B���O�t���O��
	�P�̏ꍇ�A���ꂼ��̊֐������s���ĕ\�����s���B
	�����̊֐������s����Ă��Ȃ���Ίe�I�u�W�F�N�g��
	�ړ��E�`�揈���֐������s����B�����łe�o�r�������s���B
**********************************************************/

BOOL Game_Main( HWND hWnd )
{

	int ctime;

	// �G���f�B���O�t���O��1�̎�
	if ( g_Flag.m_EndingFlag == 1 ) {
				
		// �G���f�B���O��ʂ�\������
		if ( FALSE == Ending_Display() ) {
			// �G���f�B���O�֐����łd�r�b�L�[�������ꂽ�ꍇ
			return FALSE;
		}

		// �G���f�B���O��ʕ\����A�t���O��0�ɐݒ肳��Ă���ꍇ
		// �i�G���f�B���O�֐����łd�m�s�d�q�L�[�������ꂽ�ꍇ�j
		if ( g_Flag.m_EndingFlag == 0 ) {

			// ��ʂ̓h��Ԃ�
			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

			// �ēx�A�Q�[���������֐������s����
			Init_Game();
	
			// �e�o�r�������s�����߂̍Đݒ�
			fpsChk1 = timeGetTime();
			fpsCnt = 0;
		
		}
		// �G���f�B���O�֐����ŉ���������Ȃ��ꍇ
		else { return TRUE; }

	}

	// �I�[�v�j���O�t���O�������l�i0)�̏ꍇ
	if ( g_Flag.m_OpeningFlag == 0 ) {
		
		// �I�[�v�j���O��ʂ�\������
		if ( FALSE == Opening_Display() ) {
			// �I�[�v�j���O�֐����łd�r�b�L�[�������ꂽ�ꍇ
			return FALSE;
		}

		// �I�[�v�j���O��ʕ\����t���O�����̂܂܂̏ꍇ�֐��𔲂���
		if ( g_Flag.m_OpeningFlag == 0 ) {
			return TRUE;
		}

	}

	// �e�o�r�����̂��߂̐ݒ�
	fpsChk2 = timeGetTime();
	ctime = (int)( fpsChk2 - fpsChk1 );

	// �e�o�r����
	if ( ctime > FPS ) {
		
		// �L�[���͂��擾����
		Input_Device();

		// �ړ��������C���֐�
		Move_Main();

		// �ړ���������ݒ莞�Ԉȏォ���čs���Ă��Ȃ���Ύ��s
		if ( ctime < ( FPS * 2 ) ) {

			WGL_Begin();								// OpenGL�`��J�n

			Display_Main();								// �`�揈�����C���֐�

			WGL_End();									// OpenGL�`��I��

			fpsChk1 = fpsChk2;							// �`���̎�����fpsChk1�ɐݒ�

		}
		else {
			fpsChk1 += FPS;
		}
			
	}

	// �ړ��v�Z�E�\�������S�ďI���������_�Ń^�[�Q�b�g�����݂��Ȃ���΃G���f�B���O�t���O�𗧂Ă�
	// �ʐM������̃^�[�Q�b�g�����O�̏ꍇ���ǉ�
	if ( g_Ereamap.m_TargetNum == 0 || g_Ereamap.m_EnemyTargetNum == 0 ) {
		g_Flag.m_EndingFlag = 1;
	}

	// CPU�g�p����������
	SleepEx( 1, TRUE );									// �P�~���b

	return TRUE;

}

/**********************************************************
	�ړ��������C���֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�e�I�u�W�F�N�g�̈ړ������֘A�̊֐������s����B
	���i�K�ł̓v���C���[�ƃv���C���[�e�̈ړ������֐���
	���s����B
**********************************************************/

void Move_Main( void )
{

	Player_Move();
	PAttack_Move();

}

/**********************************************************
	���C���`�揈���֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�e�I�u�W�F�N�g�̕`�揈���֘A�̊֐������s����B
	�v���C���[�E�v���C���[�e�E�^�[�Q�b�g�E���C���[�ǂ̕`��
	�Ǝc��^�[�Q�b�g���̕�����\�����Ă���B
**********************************************************/

void Display_Main( void )
{

	// ���C�g�֘A�ݒ�
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	// �f�B�X�v���C�o�b�t�@���N���A
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// �v���C���[�`��֐�
	Player_Display();

	// �v���C���[�e�`��֐�
	PAttack_Display();

	// �^�[�Q�b�g�`��֐�
	Target_Display();

	glDisable( GL_LIGHTING );

	// �����\���p�������֐�
	//ttfTextDefault( &g_Ttftext[5] );

	// �c��^�[�Q�b�g���̕����\�����s��
	ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_HEIGHT, 25 );
    ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextString    ( &g_Ttftext[5], "�v���C���[�c��^�[�Q�b�g���@%d", g_Ereamap.m_TargetNum );
    ttfTextPrint     ( &g_Ttftext[5], g_Stringx, g_Stringy + 20 );

	ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_HEIGHT, 25 );
    ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextString    ( &g_Ttftext[5], "�ΐ푊��̎c��^�[�Q�b�g���@%d", g_Ereamap.m_EnemyTargetNum );
    ttfTextPrint     ( &g_Ttftext[5], g_eStringx, g_eStringy - 20 );

	// ���C���[�Ǖ`��֐�
	WireWall_Disp( ( g_Ereamap.m_x * 2.0f ), 
				   ( g_Ereamap.m_y * 2.0f ), 
				   ( g_Ereamap.m_z * 2.0f ), 20.0 );

	glFlush();																// �����`��
	SwapBuffers( hDC );														// �o�b�t�@�̐؂�ւ�

}

/**********************************************************
	�I�[�v�j���O�֐�

	�@�����F�Ȃ�
	�߂�l�F�d�m�s�d�q�L�[����������TRUE��Ԃ��A�d�r�b
			�L�[����������FALSE��Ԃ�

	�I�[�v�j���O��ʁi�����j��\������B
	�d�m�s�d�q���d�r�b�L�[���������΂��ꂼ�ꌋ�ʂ�Ԃ��B
**********************************************************/

BOOL Opening_Display( void )
{

	// �����g��p�ϐ�
	TTF_SCALE ttfsc = { 2.0f, 2.0f };

	// �L�[���͎擾
	Input_Device();

	WGL_Begin();														// OpenGL�`��J�n
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// �I�[�v�j���O�����\��
	ttfTextParameteri( &g_Ttftext[0], TTF_TEXT_HEIGHT, 40 );
    ttfTextParameteri( &g_Ttftext[0], TTF_TEXT_DISP_SIZE_X, 1024 );
    ttfTextParameteri( &g_Ttftext[0], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextFrontColor( &g_Ttftext[0], 0, 0, 255, 100 );
    ttfTextString    ( &g_Ttftext[0], "�^�}�^�}�V���[�e�B���O�Q�[��" );
    ttfTextPrint     ( &g_Ttftext[0], g_Stringx - 180, g_Stringy - 40 );

	ttfTextParameteri( &g_Ttftext[1], TTF_TEXT_HEIGHT, 35 );
    ttfTextParameteri( &g_Ttftext[1], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[1], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextString    ( &g_Ttftext[1], "�d�m�s�d�q�L�[�ŃX�^�[�g" );
    ttfTextPrint     ( &g_Ttftext[1], g_Stringx - 110, g_Stringy - 200 );

	glFlush();															// �����`��
	SwapBuffers( hDC );													// �o�b�t�@�̐؂�ւ�

	WGL_End();															// OpenGL�`��I��

	// �d�m�s�d�q�L�[��������Ă���Ύ��s
	if ( g_nKeyData & KEY_ENTER ) {
		// �I�[�v�j���O�t���O��1�ɐݒ��A�e�o�r�����̂��߂̒l���Đݒ�
		g_Flag.m_OpeningFlag = 1;
		fpsChk1 = timeGetTime();
		fpsCnt = 0;
		return TRUE;
	}
	// �d�r�b�L�[���������Ǝ��s
	if ( g_nKeyData & KEY_ESC ) {
		return FALSE;
	}

	return TRUE;

}

/**********************************************************
	�G���f�B���O�֐�

	�@�����F�Ȃ�
	�߂�l�F�������ʏ�ʂ�I�������TRUE��Ԃ��A�d�r�b
			�L�[����������FALSE��Ԃ�

	�G���f�B���O��ʁi�����j��\������B
	�ʏ�ʂ���s������TRUE��Ԃ����A�r���łd�m�s�d�q�L�[
	��������Ă����ꍇ�̓G���f�B���O�t���O��0�ɂ���B
	�܂��A�d�r�b�L�[����������FALSE��Ԃ��B
**********************************************************/

BOOL Ending_Display( void )
{

	// �L�[���͎擾
	Input_Device();

	WGL_Begin();														// OpenGL�`��J�n

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// �G���f�B���O�����\��
	ttfTextParameteri( &g_Ttftext[2], TTF_TEXT_HEIGHT, 60 );
    ttfTextParameteri( &g_Ttftext[2], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[2], TTF_TEXT_DISP_SIZE_Y, 64 );

	if ( g_Ereamap.m_TargetNum == 0 ) {

		ttfTextString( &g_Ttftext[2], "���Ȃ��̏����ł�" );
		ttfTextPrint( &g_Ttftext[2], g_Stringx - 120, g_Stringy - 20 );

	}
	else if ( g_Ereamap.m_EnemyTargetNum == 0 )	{

		ttfTextString( &g_Ttftext[2], "���Ȃ��̔s�k�ł�" );
		ttfTextPrint( &g_Ttftext[2], g_Stringx - 120, g_Stringy - 20 );

	}
	else if ( g_Ereamap.m_EnemyTargetNum == 99 && g_Ereamap.m_TargetNum == 0 ) {
		
		ttfTextString( &g_Ttftext[2], "�Q�[���I��" );
		ttfTextPrint( &g_Ttftext[2], g_Stringx - 60, g_Stringy - 20 );
	
	}

	ttfTextParameteri( &g_Ttftext[3], TTF_TEXT_HEIGHT, 30 );
    ttfTextParameteri( &g_Ttftext[3], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[3], TTF_TEXT_DISP_SIZE_Y, 64 );	
	ttfTextString    ( &g_Ttftext[3], "�d�m�s�d�q�L�[�ōăX�^�[�g" );
    ttfTextPrint     ( &g_Ttftext[3], g_Stringx - 80, g_Stringy - 180 );

	ttfTextParameteri( &g_Ttftext[4], TTF_TEXT_HEIGHT, 30 );
    ttfTextParameteri( &g_Ttftext[4], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[4], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextString    ( &g_Ttftext[4], "�d�r�b�L�[�ŏI��" );
    ttfTextPrint     ( &g_Ttftext[4], g_Stringx - 30, g_Stringy - 230 );

	glFlush();															// �����`��
	SwapBuffers( hDC );													// �o�b�t�@�̐؂�ւ�

	WGL_End();															// OpenGL�`��I��

	// �d�m�s�d�q�L�[���������Ύ��s
	if ( g_nKeyData & KEY_ENTER ) {
		// �G���f�B���O�t���O��0�ɐݒ�
		g_Flag.m_EndingFlag = 0;
	}
	// �d�r�b�L�[���������Ύ��s
	if ( g_nKeyData & KEY_ESC ) {
		return FALSE;
	}

	return TRUE;

}

/**********************************************************
	OpenGL�`�揉���������֐�

	�@�����F�Ȃ�
	�߂�l�G���������������TRUE��Ԃ��A���s���ɂ�FALSE��
			�Ԃ��B

	OpenGL���g���ĕ`�悷�邽�߂̏������s���֐��B�f�o�C�X
	�R���e�L�X�g�ƃ����^�����O�R���e�L�X�g���w�肵�Ă���
	�����т���B
**********************************************************/

int WGL_Begin()
{

	// �f�o�C�X�R���e�L�X�g���擾
	hDC = GetDC( hWnd );

	if ( hDC == 0 ) { return FALSE; }

	// �����^�����O�R���e�L�X�g�̎w��
	wglMakeCurrent( hDC, hRC );

	return TRUE;

}

/**********************************************************
	OpenGL�`��I���������֐�

	�@�����F�Ȃ�
	�߂�l�G���������������TRUE��Ԃ��A���s���ɂ�FALSE��
			�Ԃ��B

	OpenGL���g�����`�悪�I���������Ɏ��s����֐��B
	�f�o�C�X�R���e�L�X�g�ƃ����^�����O�R���e�L�X�g��
	���т�����������B
**********************************************************/

int WGL_End()
{

	if ( hDC == 0 ) { return FALSE; }

	// �����^�����O�R���e�L�X�g�����
	wglMakeCurrent( NULL, NULL );

	// �f�o�C�X�R���e�L�X�g�����
	ReleaseDC( hWnd, hDC );
	hDC = 0;

	return TRUE;

}
