/*=============================================================================

		����[ Building.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2017/01/19
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Building.h"
#include "fade.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define MODEL_NAME "data/MODEL/Building/Saloon.x"

#define MAX_MAT ( 5 )	//	���f���̍ő�}�e���A����

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

//	�����̃Z�b�g
void SetBuilding( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPD3DXMESH g_pMeshBuilding;	//	���b�V���C���^�[�t�F�[�X�|�C���^

LPD3DXBUFFER g_pBufferMatBuilding;	//	�}�e���A�����

DWORD g_nNumMatBuilding;	//	�}�e���A����

LPDIRECT3DTEXTURE9 g_pTextureBuilding[ MAX_MAT ] = { NULL };//	�e�N�X�`���C���^�[�t�F�[�X

D3DXMATERIAL* pMatBuilding;	//	�}�e���A���|�C���^

BUILDING g_Building[ MAX_BUILDING ];	//	�����̍\����

float g_Move;	//	�ړ���

/*-----------------------------------------------------------------------------
 �֐���:	void InitBuilding( void )
 ����:		
 �߂�l:	
 ����:		������
-----------------------------------------------------------------------------*/
void InitBuilding( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	HRESULT hr;

	hr = D3DXLoadMeshFromX( MODEL_NAME ,			//	�t�@�C����
							D3DXMESH_MANAGED,
							pDevice,				//	�f�o�C�X
							NULL,					//	�אڃo�b�t�@
							&g_pBufferMatBuilding,		//	�}�e���A�������i�[
							NULL,
							&g_nNumMatBuilding,		//	�}�e���A����
							&g_pMeshBuilding );		//	���b�V��

	//	���f���̃G���[�`�F�b�N
	if( FAILED( hr ) )
	{

		MessageBox( NULL , "[ Building.cpp ]\n MODEL_NAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	�}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾
	pMatBuilding = ( D3DXMATERIAL* )g_pBufferMatBuilding -> GetBufferPointer();

	for( int i = 0 ; i < ( int )g_nNumMatBuilding ; i++ )
	{
		if( pMatBuilding[ i ].pTextureFilename != NULL )
		{

			//	�e�N�X�`���̃G���[�`�F�b�N
			if( FAILED( D3DXCreateTextureFromFile( pDevice , pMatBuilding[ i ].pTextureFilename , &g_pTextureBuilding[ i ]  ) ) )
			{

				MessageBox( NULL , "[ Building.cpp ]\n MODEL_NAME\n�̉摜�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

			}	//	end of if

		}	//	end of if

	}	//	end of for


	//	�\���̏�����
	for( int Cnt = 0 ; Cnt < MAX_BUILDING ; Cnt++ )
	{

		//	���W
		g_Building[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�g�嗦
		g_Building[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	��]��
		g_Building[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	���a
		g_Building[ Cnt ].Radius = 110.0f;

		//	�g�p�t���O
		g_Building[ Cnt ].Use = false;

	}	//	end of for


	//	�ϐ�������
	g_Move = 3.0f;



	//	�Z�b�g

	//	�E��̌���
	SetBuilding( D3DXVECTOR3( 1000.0f , 0.0f, 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 840.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 680.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 540.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 475.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 650.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 825.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 1000.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );


	//	����̌���
	SetBuilding( D3DXVECTOR3( -1000.0f , 0.0f, 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -840.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -680.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -540.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 475.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 650.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 825.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 1000.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );


	//	�E���̌���
	SetBuilding( D3DXVECTOR3( 1000.0f , 0.0f, -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 840.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 680.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 540.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -475.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -650.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -825.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -1000.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );


	//	�����̌���
	SetBuilding( D3DXVECTOR3( -1000.0f , 0.0f, -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -840.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -680.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -540.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , -475.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , -650.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , -825.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , -1000.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitBuilding( void )
 ����:		
 �߂�l:	
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitBuilding( void )
{

	if( g_pMeshBuilding != NULL )	//	���b�V���C���^�[�t�F�[�X�|�C���^
	{

		g_pMeshBuilding -> Release();
		g_pMeshBuilding = NULL;

	}	//	end of if

	if( g_pBufferMatBuilding != NULL )	//	�}�e���A�����J��
	{

		g_pBufferMatBuilding -> Release();
		g_pBufferMatBuilding = NULL;

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateBuilding( void )
 ����:		
 �߂�l:	
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdateBuilding( void )
{

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();


	if( *Mode == MODE_TITLE )
	{

		for( int Cnt = 0 ; Cnt < MAX_BUILDING ; Cnt++ )
		{

			if( g_Building[ Cnt ].Use == true )
			{

				g_Building[ Cnt ].World.Pos.z -= g_Move;

			}	//	end of if

		}	//	end of for

	}

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawBuilding( void )
 ����:		
 �߂�l:	
 ����:		�`��
-----------------------------------------------------------------------------*/
void DrawBuilding( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();



	//	���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	D3DMATERIAL9 matDef;
	pDevice -> GetMaterial( &matDef );


	for( int Cnt = 0 ; Cnt < MAX_BUILDING ; Cnt++ )
	{

		if( g_Building[ Cnt ].Use == true )
		{

			//	�t�s��Ȃ��̃��[���h���W�ϊ�
			SetWorld( g_Building[ Cnt ].World.Pos , g_Building[ Cnt ].World.Rot , g_Building[ Cnt ].World.Scl );

			for( int i = 0 ; i < ( int )g_nNumMatBuilding ; i++ )
			{
				//	�f�o�C�X�Ƀ}�e���A����ݒ�
				pDevice -> SetMaterial( &pMatBuilding[ i ].MatD3D );

				//	�e�N�X�`���ݒ�
				pDevice -> SetTexture( 0 , g_pTextureBuilding[ i ] );


				//	�`��
				g_pMeshBuilding -> DrawSubset( i );

			}	//	end of for

		}	//	end of if

		pDevice -> SetMaterial( &matDef );

	}

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	SetBuilding( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
 ����:		D3DXVECTOR3 Pos		���W
			D3DXVECTOR3 Rot		��]��
 �߂�l:	
 ����:		�����̃Z�b�g
-----------------------------------------------------------------------------*/
void SetBuilding( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
{

	for( int Cnt = 0 ; Cnt < MAX_BUILDING ; Cnt++ )
	{

		if( g_Building[ Cnt ].Use == false )
		{

			//	���W
			g_Building[ Cnt ].World.Pos = Pos;

			//	��]
			g_Building[ Cnt ].World.Rot = Rot;

			//	�g�p�t���O
			g_Building[ Cnt ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	BUILDING *GetBuilding( void )
 ����:		
 �߂�l:	
 ����:		�������̎擾
-----------------------------------------------------------------------------*/
BUILDING *GetBuilding( void )
{

	return &g_Building[ 0 ];

}	//	end of func