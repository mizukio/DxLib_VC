//�@ �����ɃT���v���v���O�������y�[�X�g���Ă�������
// ������Ԃł̓T�E���h�m�x����������`��T���v�������͂���Ă��܂��B
// �@�ׂ̃T���v���v���O���������s����ꍇ�͈ȉ��̃v���O���������ׂ�
// �폜���āA�V���ɃR�s�[���y�[�X�g���Ă��������B

// �T�E���h�m�x����������`��A�e�L�X�g�o�b�t�@�g�p�o�[�W����
#include "DxLib.h"
#include <math.h>

// �����̃T�C�Y
#define MOJI_SIZE 24

// ���z�e�L�X�g�o�b�t�@�̉��T�C�Y�c�T�C�Y
#define STRBUF_WIDTH	24
#define STRBUF_HEIGHT	20

char StringBuf[ STRBUF_HEIGHT ][ STRBUF_WIDTH * 2 + 1 ] ;	// ���z�e�L�X�g�o�b�t�@
int CursorX , CursorY ;						// ���z��ʏ�ł̕����\���J�[�\���̈ʒu
int SP , CP ;							// �Q�Ƃ��镶����ԍ��ƕ����񒆂̕����|�C���^
int EndFlag ;							// �I���t���O
int KeyWaitFlag ;						// �{�^�������҂��t���O
int Count ;							// �t���[���J�E���^

char String[][ 256 ] =
{
	"�@�Q�[���v���O�������K�����邽�߂̈�Ԃ̋ߓ��͂Ƃɂ�����R�v���O������g��",
	"���Ƃł���BB" ,
	"@�@�v���O�����̎Q�l���ɂ̓Q�[���̃v���O�����̕��@�Ȃ�ĉ���������Ă��Ȃ��AB",
	"�ϐ��AB�z��AB�֐��AB���[�v�AB��������cB����炷�ׂĂ̐����̓Q�[���ŉ��Ɏg����",
	"�Ȃ�Ăǂ��ɂ������Ă��Ȃ��AB���������Z���^���ނɂ����Ⴊ������x�ł���BB" ,
	"C�@�v���O�����͏K����芵���Ȃ̂Ńv���O������g�ނɓ������ď����ł��m����",
	"���Ό�͂�������͌@�艺���A�L���Ă����Ηǂ��킯�ŁAB�v���O�����̎Q�l����",
	"�ǂ�ł��ď����ł������o����悤�ȋC�������炻���łƂɂ����v���O����",
	"��ł��Ă݂邱�Ƃ��厖�ł���BE",
} ;

void Kaigyou( void ) ;		// �e�L�X�g�o�b�t�@�̉��s�����֐�


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						 LPSTR lpCmdLine, int nCmdShow )
{
	char OneMojiBuf[ 3 ] ;	// �P�������ꎞ�L���z��
	int i , j ;

	SetGraphMode( 640 , 480 , 16 ) ;
	if( DxLib_Init() == -1 )	// �c�w���C�u��������������
	{
		 return -1;				// �G���[���N�����璼���ɏI��
	}

	// �`��ʒu�̏����ʒu�Z�b�g
	CursorX = 0 ;
	CursorY = 0 ;
	
	// �Q�ƕ����ʒu���Z�b�g
	SP = 0 ;	// �P�s�ڂ�
	CP = 0 ;	// �O����

	// �t�H���g�̃T�C�Y�Z�b�g
	SetFontSize( MOJI_SIZE ) ;

	// �t�H���g�̑�����ύX
	SetFontThickness( 1 ) ;

	// �t�H���g�̃^�C�v���A���`�G�C���A�X�t�H���g�ɕύX
	ChangeFontType( DX_FONTTYPE_ANTIALIASING_EDGE_8X8 ) ;

	// �`���𗠉�ʂɃZ�b�g
	SetDrawScreen( DX_SCREEN_BACK ) ;

	// �t���[���J�E���^������
	Count = 0 ;

	// ���[�v
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		// �T�E���h�m�x����������`�揈�����s��
		// �������I���t���O���P�������ꍇ�͏��������Ȃ�
		if( EndFlag == 0 )
		{
			char  Moji ;

			// �{�^�������҂��t���O�������Ă����ꍇ�̓{�^�����������܂ł����ŏI��
			if( KeyWaitFlag == 1 )
			{
				if( ProcessMessage() == 0 && CheckHitKeyAll() != 0 ) 
				{
					// �{�^����������Ă��������
					KeyWaitFlag = 0 ;
				}
			}
			else
			{
				// �����̕`��
				Moji = String[ SP ][ CP ] ;
				switch( Moji )
				{
				case '@' :	// ���s����

					// ���s��������юQ�ƕ����ʒu����i�߂�
					Kaigyou() ;
					CP ++ ;

					break ;

				case 'B' :	// �{�^�������҂�����

					// �{�^�����������܂ő҂�
					while( ProcessMessage() == 0 && CheckHitKeyAll() != 0 ){}

					// �{�^�������҂��t���O�����Ă�
					KeyWaitFlag = 1 ;
					CP ++ ;

					break ;

				case 'E' :	// �I������

					// �I���t���O�𗧂Ă邨��юQ�ƕ����ʒu����i�߂�
					EndFlag = 1 ;
					CP ++ ;

					break ;

				case 'C' :	// �N���A����

					// ���z�e�L�X�g�o�b�t�@�����������ĕ`�敶���ʒu�������ʒu�ɖ߂�����юQ�ƕ����ʒu����i�߂�
					for( i = 0 ; i < STRBUF_HEIGHT ; i ++ )
					{
						for( j = 0 ; j < STRBUF_WIDTH * 2 ; j ++ )
						{
							StringBuf[ i ][ j ] = 0 ;
						}
					}

					CursorY = 0 ;
					CursorX = 0 ;
					CP ++ ;

					break ;

				default :	// ���̑��̕���

					// �P�����������o��
					OneMojiBuf[ 0 ] = String[ SP ][ CP ] ;
					OneMojiBuf[ 1 ] = String[ SP ][ CP + 1 ] ;
					OneMojiBuf[ 2 ] = '\0' ;

					// �P�����e�L�X�g�o�b�t�@�ɑ��
					StringBuf[ CursorY ][ CursorX * 2 ] = OneMojiBuf[ 0 ] ;
					StringBuf[ CursorY ][ CursorX * 2 + 1 ] = OneMojiBuf[ 1 ] ;

					// �Q�ƕ����ʒu���Q�o�C�g���߂�
					CP += 2 ;

					// �J�[�\�����ꕶ�����i�߂�
					CursorX ++ ;

					// �e�L�X�g�o�b�t�@��������͂ݏo������s����
					if( CursorX >= STRBUF_WIDTH ) Kaigyou() ;

					break ;
				}

				// �Q�ƕ�����̏I�[�܂ōs���Ă�����Q�ƕ������i�߂�
				if( String[ SP ][ CP ] == '\0' )
				{
					SP ++ ;
					CP = 0 ;
				}
			}
		}

		// ��ʂ̃N���A
		ClsDrawScreen() ;

		// �w�i�G�t�F�N�g�̕`��
		{
			int Color ;

			Color = ( int )( sin( Count / 100.0 ) * 80.0 + 125.0 ) ;
			DrawBox( 0 , 0 , 640 , 480 , GetColor( Color , 0 , 0 ) , TRUE ) ;
			Count ++ ;
		}

		// �e�L�X�g�o�b�t�@�̕`��
		for( i = 0 ; i < STRBUF_HEIGHT ; i ++ )
		{
			DrawString( 8 , i * MOJI_SIZE , StringBuf[ i ] , GetColor( 255 , 255 , 255 ) ) ;
		}

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip() ;
	}

	DxLib_End() ;				// �c�w���C�u�����g�p�̏I������

	return 0 ;					// �\�t�g�̏I��
}



// ���s�֐�
void Kaigyou( void )
{
	// �`��s�ʒu���������
	CursorY ++ ;

	// �`�����ŏ��ɖ߂�
	CursorX = 0 ;

	// �����e�L�X�g�o�b�t�@�c������͂ݏo��Ȃ�e�L�X�g�o�b�t�@���c�X�N���[��������
	if( CursorY >= STRBUF_HEIGHT )
	{
		int i , j ;

		for( i = 1 ; i < STRBUF_HEIGHT ; i ++ )
		{
			for( j = 0 ; j < STRBUF_WIDTH * 2 ; j ++ )
			{
				StringBuf[ i - 1 ][ j ] = StringBuf[ i ][ j ] ;
			}
		}

		// �`��s�ʒu���������
		CursorY -- ;
	}
}
