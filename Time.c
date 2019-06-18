/*=============================================================================

		�^�C��[ Time.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/01/05
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Time.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/number000.png"
#define POLYGON01_TEXTURENAME "data/TEXTURE/GAME/Time.png"

#define MAX_TEXTURE ( 2 )	//	�ő�e�N�X�`����

#define SCORE_DIGIT ( 2 )	//	�����̐����̕\��

//#define FRAME ( 0.06f )	//	�t���[����
#define FRAME ( 60 )	//	�t���[����

#define TIME_MIN ( 0 )	//	�^�C���̍ŏ��l
#define TIME_COLORCHANGE ( 10 )	//	�F��ς��鎞��

#define SECOND ( 90 )	//	�b��


#define POLYGON01_X ( SCREEN_WIDTH * 0.36f )	//	�^�C��������X
#define POLYGON01_Y ( -10.0f )					//	�^�C��������Y
#define POLYGON01_WIDTH  ( 150.0f )				//	�^�C��������Width
#define POLYGON01_HEIGHT ( 75.0f )				//	�^�C��������Height

#define POLYGON00_X ( SCREEN_WIDTH * 0.5f )		//	�^�C����X
#define POLYGON00_Y ( 50.0f )					//	�^�C����Y
#define POLYGON00_WIDTH  ( 70.0f )				//	�^�C����Width
#define POLYGON00_HEIGHT ( 70.0f )				//	�^�C����Height

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

//	���_�̍쐬
HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice );

//	�e�N�X�`���̕ύX
void VerTexTime( VERTEX_2D* pVtx , int Cnt , int num );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTime = NULL;	//	���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTime[ MAX_TEXTURE ] = { NULL };	//	�e�N�X�`���C���^�[�t�F�[�X

D3DXCOLOR g_TimeColor;	//	�F

int g_Time;	//	�^�C��

bool g_TimeFlag;	//	�^�C���t���O

/*-----------------------------------------------------------------------------
 �֐���:	void InitTime( void )
 ����:		
 �߂�l:	
 ����:		������
-----------------------------------------------------------------------------*/
void InitTime( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	�e�N�X�`���̃G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureTime[ 0 ] ) ) )
	{
		MessageBox( NULL , "[ Time.cpp ]\n POLYGON00_TEXTURENAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON01_TEXTURENAME , &g_pTextureTime[ 1 ] ) ) )
	{
		MessageBox( NULL , "[ Time.cpp ]\n POLYGON01_TEXTURENAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	���_�̍쐬
	MakeVertexTime( pDevice );


	//	�ϐ�������

	//	�F
	g_TimeColor = D3DXCOLOR( 255 , 255 , 255 , 255 );

	//	�^�C��
	g_Time = SECOND * FRAME;

	//	�^�C���t���O
	g_TimeFlag = false;

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitTime( void )
 ����:		
 �߂�l:	
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitTime( void )
{

	if(g_pVtxBufferTime != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferTime -> Release();
		g_pVtxBufferTime  = NULL;

	}	//	end of if


	for( int CntTime = 0 ; CntTime < MAX_TEXTURE ; CntTime++ )
	{

		if( g_pTextureTime[ CntTime ] != NULL )	//	�e�N�X�`���|���S���J��
		{
			g_pTextureTime[ CntTime ] -> Release();
			g_pTextureTime[ CntTime ] = NULL;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateTime( void )
 ����:		
 �߂�l:	
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdateTime( void )
{

	int num;	//	������\������ϐ�
	int value = g_Time / FRAME;


	//	���z�A�h���X���擾����|�C���^�ϐ�
	VERTEX_2D* pVtx;


	//�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferTime -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	for( int Cnt = 0 ; Cnt < SCORE_DIGIT ; Cnt++ )
	{

		num = value % 10;
		value /= 10;

		//	�e�N�X�`���̕ύX
		VerTexTime( pVtx , Cnt , num );

	}	//	end of for


	g_pVtxBufferTime -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�


	//	�^�C����0�ȉ��ɂȂ�����
	if( g_Time <= 0 )
	{

		g_Time = TIME_MIN;

		g_TimeFlag = true;

	}	//	end of if

	else
	{

		g_Time--;	//	�^�C�������炷

		//	�^�C�������̒l�����������
		if( g_Time / FRAME < TIME_COLORCHANGE )
		{

			//	�F�ύX
			g_TimeColor.r = 255;
			g_TimeColor.g = 0;
			g_TimeColor.b = 0;

		}	//	end of if
	
	}	//	end of else


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawTime( void )
 ����:		
 �߂�l:	
 ����:		�`��
-----------------------------------------------------------------------------*/
void DrawTime( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTime , 0 , sizeof( VERTEX_2D ) );

	//	�e�N�X�`���̃Z�b�g
	pDevice -> SetTexture( 0 , g_pTextureTime[ 0 ] );


	for( int CntTime = 0 ; CntTime < SCORE_DIGIT ; CntTime++ )
	{

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , CntTime * NUM_VERTEX , NUM_POLYGON);

	}	//	end of for


	//	�e�N�X�`���̃Z�b�g
	pDevice -> SetTexture( 0 , g_pTextureTime[ 1 ] );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , SCORE_DIGIT * NUM_VERTEX , NUM_POLYGON);


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice	�f�o�C�X
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice )
{

	int num;	//	������\������ϐ�
	int value = g_Time / FRAME;	//	�^�C���̒l


	//	���z�A�h���X���擾����|�C���^�ϐ�
	VERTEX_2D* pVtx;


	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * SCORE_DIGIT * MAX_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTime , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferTime -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	for( int Cnt = 0 ; Cnt < SCORE_DIGIT ; Cnt++ )
	{

		num = value % 10;
		value /= 10;


		//	���_���W�̐ݒ�
		pVtx[ 0 ].pos = D3DXVECTOR3( POLYGON00_X                   - Cnt * POLYGON00_WIDTH , POLYGON00_Y                    , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( POLYGON00_X + POLYGON00_WIDTH - Cnt * POLYGON00_WIDTH , POLYGON00_Y                    , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( POLYGON00_X                   - Cnt * POLYGON00_WIDTH , POLYGON00_Y + POLYGON00_HEIGHT , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( POLYGON00_X + POLYGON00_WIDTH - Cnt * POLYGON00_WIDTH , POLYGON00_Y + POLYGON00_HEIGHT , 0.0f );

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ 0 ].rhw = 1.0f;
		pVtx[ 1 ].rhw = 1.0f;
		pVtx[ 2 ].rhw = 1.0f;
		pVtx[ 3 ].rhw = 1.0f;

		//	���_�F�̐ݒ�
		pVtx[ 0 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 1 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 2 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 3 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.1f * num        , 0 );
		pVtx[ 1 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 0 );
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.1f * num        , 1 );
		pVtx[ 3 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 1 );

		pVtx += 4;	//	�|�C���^�����炷

	}	//	end of for


	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( POLYGON01_X                   , POLYGON01_Y                    , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( POLYGON01_X + POLYGON01_WIDTH , POLYGON01_Y                    , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( POLYGON01_X                   , POLYGON01_Y + POLYGON01_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( POLYGON01_X + POLYGON01_WIDTH , POLYGON01_Y + POLYGON01_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 1 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 2 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 3 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );


	g_pVtxBufferTime -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void VerTexTime( VERTEX_2D* pVtx , int Cnt , int num )
 ����:		VERTEX_2D* pVtx		���z�A�h���X���擾����|�C���^�ϐ�
			int Cnt				�^�C���̃J�E���^
			int num				�e�N�X�`�������炷�ϐ�
 �߂�l:	
 ����:		�e�N�X�`���̕ύX
-----------------------------------------------------------------------------*/
void VerTexTime( VERTEX_2D* pVtx , int Cnt , int num )
{

	pVtx += Cnt * NUM_VERTEX;


	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( g_TimeColor.r , g_TimeColor.g , g_TimeColor.b , g_TimeColor.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_TimeColor.r , g_TimeColor.g , g_TimeColor.b , g_TimeColor.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_TimeColor.r , g_TimeColor.g , g_TimeColor.b , g_TimeColor.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_TimeColor.r , g_TimeColor.g , g_TimeColor.b , g_TimeColor.a );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.1f * num        , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.1f * num        , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 1 );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	int *GetTime( void )
 ����:		
 �߂�l:	return &g_Time;
 ����:		�^�C�����̎擾
-----------------------------------------------------------------------------*/
bool *GetTime( void )
{

	return &g_TimeFlag;

}	//	end of func