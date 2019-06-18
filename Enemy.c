/*=============================================================================

		�G[ Enemy.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/01/05
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <time.h>
#include <math.h>
#include "main.h"
#include "Enemy.h"
#include "Shadow.h"
#include "fade.h"
#include "MeshWall.h"
#include "Building.h"
#include "Utility.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define MODEL_NAME "data/MODEL/Enemy/Robo.x"

#define MAX_MAT ( 4 )	//	���f���̍ő�}�e���A����

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

//	�^�[�Q�b�g�̃Z�b�g
void SetEnemy( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point );

//	�����蔻��
void CollisionEnemy( int Cnt );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPD3DXMESH g_pMeshEnemy;	//	���b�V���C���^�[�t�F�[�X�|�C���^

LPD3DXBUFFER g_pBufferMatEnemy;	//	�}�e���A�����

DWORD g_nNumMatEnemy;	//	�}�e���A����

LPDIRECT3DTEXTURE9 g_pTextureEnemy[ MAX_MAT ] = { NULL };//	�e�N�X�`���C���^�[�t�F�[�X

D3DXMATERIAL* pMatEnemy;	//	�}�e���A���|�C���^

ENEMY g_Enemy[ MAX_ENEMY ];	//	�G�\����

/*-----------------------------------------------------------------------------
 �֐���:	void InitTarget( void )
 ����:		
 �߂�l:	
 ����:		������
-----------------------------------------------------------------------------*/
void InitEnemy( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	//	���݂̎������擾
	srand((unsigned)time(NULL));


	HRESULT hr;

	hr = D3DXLoadMeshFromX( MODEL_NAME ,			//	�t�@�C����
							D3DXMESH_MANAGED,
							pDevice,				//	�f�o�C�X
							NULL,					//	�אڃo�b�t�@
							&g_pBufferMatEnemy,		//	�}�e���A�������i�[
							NULL,
							&g_nNumMatEnemy,		//	�}�e���A����
							&g_pMeshEnemy );		//	���b�V��

	//	���f���̃G���[�`�F�b�N
	if( FAILED( hr ) )
	{

		MessageBox( NULL , "[ Enemy.cpp ]\n MODEL_NAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	�}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾
	pMatEnemy = ( D3DXMATERIAL* )g_pBufferMatEnemy -> GetBufferPointer();

	for( int i = 0 ; i < ( int )g_nNumMatEnemy ; i++ )
	{
		if( pMatEnemy[ i ].pTextureFilename != NULL )
		{

			//	�e�N�X�`���̃G���[�`�F�b�N
			if( FAILED( D3DXCreateTextureFromFile( pDevice , pMatEnemy[ i ].pTextureFilename , &g_pTextureEnemy[ i ]  ) ) )
			{

				MessageBox( NULL , "[ Enemy.cpp ]\n MODEL_NAME\n�̉摜�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

			}	//	end of if

		}	//	end of if

	}	//	end of for


	//	�\���̏�����
	for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
	{

		//	���W
		g_Enemy[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�g�嗦
		g_Enemy[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	��]��
		g_Enemy[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�O�̍��W
		g_Enemy[ Cnt ].OldPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�ړ���
		g_Enemy[ Cnt ].Move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	���a
		g_Enemy[ Cnt ].Radius = 3.0f;

		//	�g�p�t��
		g_Enemy[ Cnt ].Use = false;

	}	//	end of for


	//	���[�h���^�C�g���̎�
	if( *Mode == MODE_TITLE )
	{

		//	�^�[�Q�b�g�̃Z�b�g
		SetEnemy( D3DXVECTOR3( 0.0f , 0.0f , -40.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , ( TYPE_POINT )0 );
		SetEnemy( D3DXVECTOR3( -30.0f , 0.0f , -20.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , ( TYPE_POINT )0 );
		SetEnemy( D3DXVECTOR3( 30.0f , 0.0f , -20.0f ), D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , ( TYPE_POINT )0 );

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitTarget( void )
 ����:		
 �߂�l:	
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitEnemy( void )
{

	if( g_pMeshEnemy != NULL )	//	���b�V���C���^�[�t�F�[�X�|�C���^
	{

		g_pMeshEnemy -> Release();
		g_pMeshEnemy = NULL;

	}	//	end of if

	if( g_pBufferMatEnemy != NULL )	//	�}�e���A�����J��
	{

		g_pBufferMatEnemy -> Release();
		g_pBufferMatEnemy = NULL;

	}	//	end of if


}	//	end of func
 
/*-----------------------------------------------------------------------------
 �֐���:	void UpdataTarget( void )
 ����:		
 �߂�l:	
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdateEnemy( void )
{

	//	�e�̏��擾
	SHADOW *Shadow = GetShadow( 0 );

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();



	if( *Mode == MODE_GAME )
	{

		for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
		{

			if( g_Enemy[ Cnt ].Use == true)
			{

				//	�O��̍��W���擾
				g_Enemy[ Cnt ].OldPos = g_Enemy[ Cnt ].World.Pos;


				g_Enemy[ Cnt ].World.Pos += g_Enemy[ Cnt ].Move;


				//	�����蔻��
				CollisionEnemy( Cnt );

			}	//	end of if

		}	//	end of for



		//	�G�̏o���ʒu�̌���
		
		int i = rand() % TYPE_MAX;	//	�o���ʒu�̌��߂�ϐ�
		TYPE_POINT Point = ( TYPE_POINT )i;


		if( Point == TYPE_POINT1 )
		{

			//	�^�[�Q�b�g�̃Z�b�g
			SetEnemy( D3DXVECTOR3( 1000.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 2.5f , 0.0f , 2.5f ) , Point );

		}	//	end of if

		else if( Point == TYPE_POINT2 )
		{

			//	�^�[�Q�b�g�̃Z�b�g
			SetEnemy( D3DXVECTOR3( -1000.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 2.0f , 0.0f , 2.0f ) , Point );

		}	//	end of else if

		else if( Point == TYPE_POINT3 )
		{

			//	�^�[�Q�b�g�̃Z�b�g
			SetEnemy( D3DXVECTOR3( 0.0f , 0.0f , 1000.0f ) , D3DXVECTOR3( 1.5f , 0.0f , 1.5f ) , Point );

		}	//	end of else if
	

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawTarget( void )
 ����:		
 �߂�l:	
 ����:		�`��
-----------------------------------------------------------------------------*/
void DrawEnemy( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();



	//	���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	D3DMATERIAL9 matDef;
	pDevice -> GetMaterial( &matDef );


	for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
	{

		if( g_Enemy[ Cnt ].Use == true)
		{

			//	�t�s��Ȃ��̃��[���h���W�ϊ�
			SetWorld( g_Enemy[ Cnt ].World.Pos , g_Enemy[ Cnt ].World.Rot , g_Enemy[ Cnt ].World.Scl );


			for( int i = 0 ; i < ( int )g_nNumMatEnemy ; i++ )
			{
				//	�f�o�C�X�Ƀ}�e���A����ݒ�
				pDevice -> SetMaterial( &pMatEnemy[ i ].MatD3D );

				//	�e�N�X�`���ݒ�
				pDevice -> SetTexture( 0 , g_pTextureEnemy[ i ] );


				//	�`��
				g_pMeshEnemy -> DrawSubset( i );

			}	//	end of for

		}	//	end of if

		pDevice -> SetMaterial( &matDef );

	}


}	//	end of func


/*-----------------------------------------------------------------------------
 �֐���:	void SetTarget( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point )
 ����:		D3DXVECTOR3 Pos		���W
			D3DXVECTOR3 Move	�ړ���
			TYPE_POINT Point	�o���ʒu
 �߂�l:	
 ����:		�^�[�Q�b�g�̃Z�b�g
-----------------------------------------------------------------------------*/
void SetEnemy( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point )
{

	for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
	{

		if( g_Enemy[ Cnt ].Use == false )
		{

			//	���W
			g_Enemy[ Cnt ].World.Pos = Pos;

			//	�ړ���
			g_Enemy[ Cnt ].Move = Move;

			//	�o���ʒu
			g_Enemy[ Cnt ].Point = Point;

			//	�g�p�t���O
			g_Enemy[ Cnt ].Use = true;

			//	�e�̃Z�b�g
//			SetShadow( g_Enemy[ Cnt ].World.Pos , 15.0f , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

			break;

		}	//	end of if
		
	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void CollisionEnemy( int Cnt )
 ����:		int Cnt		�G�\���̂̃J�E���^
 �߂�l:	
 ����:		�����蔻��
-----------------------------------------------------------------------------*/
void CollisionEnemy( int Cnt )
{

	//	���b�V���E�H�[�����擾
	MESHWALL *MeshWall = GetMeshWall();


	//	�ǂƂ̔���
	if( g_Enemy[ Cnt ].World.Pos.z + 10.0f > MeshWall[ 0 ].World.Pos.z )
	{

		g_Enemy[ Cnt ].Move.z *= -1.0f;

	}	//	end of if

	else if( g_Enemy[ Cnt ].World.Pos.z - 10.0f < MeshWall[ 1 ].World.Pos.z )
	{

		g_Enemy[ Cnt ].Move.z *= -1.0f;

	}	//	end of else if

	else if( g_Enemy[ Cnt ].World.Pos.x - 10.0f < MeshWall[ 3 ].World.Pos.x )
	{

		g_Enemy[ Cnt ].Move.x *= -1.0f;

	}	//	end of else if

	else if( g_Enemy[ Cnt ].World.Pos.x + 10.0f > MeshWall[ 2 ].World.Pos.x )
	{

		g_Enemy[ Cnt ].Move.x *= -1.0f;

	}	//	end of else if



	//	�������̎擾
	BUILDING *Building = GetBuilding();

	for( int CntBuilding = 0 ; CntBuilding < MAX_BUILDING ; CntBuilding++ , Building++ )
	{
		//	���Ƌ��̓����蔻��
		if( SphereCollision( D3DXVECTOR3( g_Enemy[ Cnt ].World.Pos.x , g_Enemy[ Cnt ].World.Pos.y + 10.0f , g_Enemy[ Cnt ].World.Pos.z ) , D3DXVECTOR3( Building -> World.Pos.x , Building -> World.Pos.y + 30.0f , Building -> World.Pos.z ) , g_Enemy[ Cnt ].Radius , Building -> Radius ) )
		{

			if( g_Enemy[ Cnt ].Point = TYPE_POINT1 )
			{

				g_Enemy[ Cnt ].Move.x *= -1.0f;

			}	//	end of if

			else if( g_Enemy[ Cnt ].Point = TYPE_POINT2 )
			{

				g_Enemy[ Cnt ].Move.x *= -1.0f;

			}	//	end of else if

			else if( g_Enemy[ Cnt ].Point = TYPE_POINT2 )
			{

				g_Enemy[ Cnt ].Move.z *= -1.0f;

			}	//	end of else if

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	ENEMY *GetEnemyNor( void )
 ����:		
 �߂�l:	
 ����:		�^�[�Q�b�g���̎擾
-----------------------------------------------------------------------------*/
ENEMY *GetEnemyNor( void )
{

	return &g_Enemy[ 0 ];

}	//	end of func