/*----------------------------------------------------------------------*/
/*   Copyright (C) 2006 by DEA                                          */
/*   TrueTypeFont �����o�� �v���O����                                   */
/*                                                                      */
/*   Ver 1.0  : �e�N�X�`�����g�p���ĕ`��                                */
/*----------------------------------------------------------------------*/
#ifndef _MY_TTF_TEXT_INCLUDE
#define _MY_TTF_TEXT_INCLUDE

#if defined __cplusplus
extern "C" {
#endif


#define TTF_MAX_STRING  256                     // ��x�ɕ\���ł���ő啶����

//-----------------------------------------------------------------------------
// �\���̐ݒ�
typedef struct
{
    float x, y;
} TTF_SCALE;

typedef struct {
    float r, g, b, a;
} TTF_RGBA;

// �t�H���g�\���p�\����
typedef struct {
    int             Init_f;                     // �������t���O

    SIZE            DispSize;                   // ���[�U�[�ݒ�\���T�C�Y

    GLuint          TexNo;                      // �e�N�X�`���Ǘ��ԍ�
    SIZE            ImgSize;                    // �摜�̃T�C�Y
    int             Image_f;                    // �����摜�쐬�t���O      0:���쐬  1:�쐬�ς�
    unsigned char   *Image;                     // �摜�f�[�^�i�������G�ɕϊ������f�[�^�j

    int             Front_f;                    // �����\���t���O
    TTF_RGBA        Front_rgba;                 // �����F

    int             Back_f;                     // �w�i�\���t���O
    TTF_RGBA        Back_rgba;                  // �w�i�F

    TTF_SCALE       Scale;                      // �g��k���l

    int             lFont_f;                    // �_���t�H���g�쐬�t���O  0:���쐬  1:�쐬�ς�
    LOGFONT         lFont;                      // �t�H���g�̏���
    HFONT           hFont;                      // �_���t�H���g

    char            string[TTF_MAX_STRING+1];   // �\�������i�e�L�X�g�j
} TTF_TEXT;


//-----------------------------------------------------------------------------
// �t�H���g�f�[�^�ݒ�ύX�p
// ttfTextParameterv�EttfTextParameteri �֐��Ŏg�p����R�}���h
enum
{
    TTF_TEXT_DEF,               // �f�t�H���g�ݒ�       v:i
    TTF_TEXT_FONT,              // �_���t�H���g�쐬     v:i

    TTF_TEXT_ALL,               // �S�f�[�^��������     v:

    TTF_TEXT_DISP_SIZE,         // �T�C�Y�w��           v:
    TTF_TEXT_DISP_SIZE_X,       // �T�C�Y�w�� ����       :i
    TTF_TEXT_DISP_SIZE_Y,       // �T�C�Y�w�� �c��       :i

    TTF_TEXT_SCALE,             // �g��k��             v:

    TTF_TEXT_BACK,              // �w�i�ݒ�              :i
    TTF_TEXT_BACK_COLOR,        // �w�i�F�ݒ�           v:

    TTF_TEXT_FRONT,             // �����ݒ�              :i
    TTF_TEXT_FRONT_COLOR,       // �����F�ݒ�           v:


    TTF_TEXT_HEIGHT,            // ���������ݒ�          :i
    TTF_TEXT_WIDTH,             // �������ݒ�            :i
    TTF_TEXT_WEIGHT,            // �����̑����ݒ�        :i
    TTF_TEXT_ITALIC,            // �C�^���b�N�����ݒ�    :i
    TTF_TEXT_UNDERLINE,         // �����ݒ�              :i
    TTF_TEXT_STRIKEOUT,         // �������ݒ�            :i
    TTF_TEXT_NAME,              // �t�H���g���ݒ�       v:
};


//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾

// �t�H���g�V�X�e��������
extern  int ttfTextSysInit( HWND *hwnd );

// �f�t�H���g�l�ݒ�p
extern  int ttfTextDefault( TTF_TEXT *ttfText );

// �t�H���g�f�[�^�ݒ�ύX�p
extern  void ttfTextParameterv( TTF_TEXT *ttfText, int setcom, void *vp );
extern  void ttfTextParameteri( TTF_TEXT *ttfText, int setcom, int   vp );

// �����F�ύX����
extern  void ttfTextFrontColor( TTF_TEXT *ttfText, float r, float g, float b, float a );

// �w�i�F�ύX����
extern  void ttfTextBackColor( TTF_TEXT *ttfText, float r, float g, float b, float a );

// ������ݒ�p�֐�
extern  void ttfTextString( TTF_TEXT *ttfText, const char *fmt, ... );

// �\���p�֐�
extern  void ttfTextPrint( TTF_TEXT *ttfText, float kx, float ky );


#if defined __cplusplus
} // end of extern "C"
#endif

#endif