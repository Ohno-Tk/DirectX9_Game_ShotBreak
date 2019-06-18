/*=============================================================================

		�G( ���V�^�C�v )[ EnemyFly.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/01/11
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <time.h>
#include "main.h"
#include "Enemy.h"
#include "EnemyFly.h"
#include "Shadow.h"
#include "fade.h"
#include "MeshWall.h"
#include "Building.h"
#include "Utility.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define MODEL_NAME "data/MODEL/Enemy/RoboFly.x"

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
void SetEnemyFly( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point );

//	�����蔻��
void CollisionEnemyFly( int Cnt );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPD3DXMESH g_pMeshEnemyFly;	//	���b�V���C���^�[�t�F�[�X�|�C���^

LPD3DXBUFFER g_pBufferMatEnemyFly;	//	�}�e���A�����

DWORD g_nNumMatEnemyFly;	//	�}�e���A����

LPDIRECT3DTEXTURE9 g_pTextureEnemyFly[ MAX_MAT ] = { NULL };//	�e�N�X�`���C���^�[�t�F�[�X

D3DXMATERIAL* pMatEnemyFly;	//	�}�e���A���|�C���^

ENEMY g_EnemyFly[ MAX_ENEMY_FLY ];	//	�G�\����


/*-----------------------------------------------------------------------------
 �֐���:	void InitTargetFly( void )
 ����:		
 �߂�l:	
 ����:		������
-----------------------------------------------------------------------------*/
void InitEnemyFly( void )
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
							&g_pBufferMatEnemyFly,		//	�}�e���A�������i�[
							NULL,
							&g_nNumMatEnemyFly,		//	�}�e���A����
							&g_pMeshEnemyFly );		//	���b�V��

	//	���f���̃G���[�`�F�b�N
	if( FAILED( hr ) )
	{

		MessageBox( NULL , "[ EnemyFly.cpp ]\n MODEL_NAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	�}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾
	pMatEnemyFly = ( D3DXMATERIAL* )g_pBufferMatEnemyFly -> GetBufferPointer();

	for( int i = 0 ; i < ( int )g_nNumMatEnemyFly ; i++ )
	{
		if( pMatEnemyFly[ i ].pTextureFilename != NULL )
		{

			//	�e�N�X�`���̃G���[�`�F�b�N
			if( FAILED( D3DXCreateTextureFromFile( pDevice , pMatEnemyFly[ i ].pTextureFilename , &g_pTextureEnemyFly[ i ]  ) ) )
			{

				MessageBox( NULL , "[ EnemyFly.cpp ]\n MODEL_NAME\n�̉摜�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

			}	//	end of if

		}	//	end of if

	}	//	end of for


	//	�\���̏�����
	for( int Cnt = 0 ; Cnt < MAX_ENEMY_FLY ; Cnt++ )
	{

		//	���W
		g_EnemyFly[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�g�嗦
		g_EnemyFly[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	��]��
		g_EnemyFly[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�O�̍��W
		g_EnemyFly[ Cnt ].OldPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�ړ���
		g_EnemyFly[ Cnt ].Move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	���a
		g_EnemyFly[ Cnt ].Radius = 3.0f;

		//	�g�p�t���O
		g_EnemyFly[ Cnt ].Use = false;

	}	//	end of for


	//	���[�h���^�C�g���̎�
	if( *Mode == MODE_TITLE )
	{

		//	�^�[�Q�b�g�̃Z�b�g
		SetEnemyFly( D3DXVECTOR3( -50.0f , 40.0f , -18.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , ( TYPE_POINT )0 );
		SetEnemyFly( D3DXVECTOR3( 50.0f , 40.0f , -19.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , ( TYPE_POINT )0 );

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitEnemyFly( void )
 ����:		
 �߂�l:	
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitEnemyFly( void )
{

	if( g_pMeshEnemyFly != NULL )	//	���b�V���C���^�[�t�F�[�X�|�C���^
	{

		g_pMeshEnemyFly -> Release();
		g_pMeshEnemyFly = NULL;

	}	//	end of if

	if( g_pBufferMatEnemyFly != NULL )	//	�}�e���A�����J��
	{

		g_pBufferMatEnemyFly -> Release();
		g_pBufferMatEnemyFly = NULL;

	}	//	end of if


}	//	end of func
 
/*-----------------------------------------------------------------------------
 �֐���:	void UpdataEnemyFly( void )
 ����:		
 �߂�l:	
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdateEnemyFly( void )
{

	//	�e�̏��擾
	SHADOW *Shadow = GetShadow( 0 );

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();


	if( *Mode == MODE_GAME )
	{

		for( int Cnt = 0 ; Cnt < MAX_ENEMY ; Cnt++ )
		{

			if( g_EnemyFly[ Cnt ].Use == true)
			{

				//	�O��̍��W���擾
				g_EnemyFly[ Cnt ].OldPos = g_EnemyFly[ Cnt ].World.Pos;


				g_EnemyFly[ Cnt ].World.Pos += g_EnemyFly[ Cnt ].Move;


				//	�����蔻��
				CollisionEnemyFly( Cnt );

			}	//	end of if

		}	//	end of for



		//	�G�̏o���ʒu�̌���
		
		int i = rand() % TYPE_MAX;	//	�o���ʒu�̌��߂�ϐ�
		TYPE_POINT Point = ( TYPE_POINT )i;

		if( Point == TYPE_POINT1 )
		{

			//	�^�[�Q�b�g�̃Z�b�g
			SetEnemyFly( D3DXVECTOR3( 1000.0f , rand()%50 + 50 , 0.0f ) , D3DXVECTOR3( 2.0f , 0.0f , 1.0f ) , Point );

		}	//	end of if

		else if( Point == TYPE_POINT2 )
		{

			//	�^�[�Q�b�g�̃Z�b�g
			SetEnemyFly( D3DXVECTOR3( -1000.0f , rand()%50 + 50 , 0.0f ) , D3DXVECTOR3( 1.5f , 0.0f , 1.5f ) , Point );

		}	//	end of else if

		else if( Point == TYPE_POINT3 )
		{

			//	�^�[�Q�b�g�̃Z�b�g
			SetEnemyFly( D3DXVECTOR3( 0.0f , rand()%50 + 50 , 1000.0f ) , D3DXVECTOR3( 1.0f , 0.0f , 2.0f ) , Point );

		}	//	end of else if

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawEnemyFly( void )
 ����:		
 �߂�l:	
 ����:		�`��
-----------------------------------------------------------------------------*/
void DrawEnemyFly( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();



	//	���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	D3DMATERIAL9 matDef;
	pDevice -> GetMaterial( &matDef );


	for( int Cnt = 0 ; Cnt < MAX_ENEMY_FLY ; Cnt++ )
	{

		if( g_EnemyFly[ Cnt ].Use == true)
		{

			//	�t�s��Ȃ��̃��[���h���W�ϊ�
			SetWorld( g_EnemyFly[ Cnt ].World.Pos , g_EnemyFly[ Cnt ].World.Rot , g_EnemyFly[ Cnt ].World.Scl );


			for( int i = 0 ; i < ( int )g_nNumMatEnemyFly ; i++ )
			{
				//	�f�o�C�X�Ƀ}�e���A����ݒ�
				pDevice -> SetMaterial( &pMatEnemyFly[ i ].MatD3D );

				//	�e�N�X�`���ݒ�
				pDevice -> SetTexture( 0 , g_pTextureEnemyFly[ i ] );


				//	�`��
				g_pMeshEnemyFly -> DrawSubset( i );

			}	//	end of for

		}	//	end of if

		pDevice -> SetMaterial( &matDef );

	}	//	end of for


}	//	end of func


/*-----------------------------------------------------------------------------
 �֐���:	void SetEnemyFly( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point )
 ����:		D3DXVECTOR3 Pos		���W
			D3DXVECTOR3 Move	�ړ���
			TYPE_POINT Point	�o���ʒu
 �߂�l:	
 ����:		�^�[�Q�b�g�̃Z�b�g
-----------------------------------------------------------------------------*/
void SetEnemyFly( D3DXVECTOR3 Pos , D3DXVECTOR3 Move , TYPE_POINT Point )
{

	for( int Cnt = 0 ; Cnt < MAX_ENEMY_FLY ; Cnt++ )
	{

		if( g_EnemyFly[ Cnt ].Use == false )
		{

			//	���W
			g_EnemyFly[ Cnt ].World.Pos = Pos;

			//	�ړ���
			g_EnemyFly[ Cnt ].Move = Move;

			//	�o���ʒu
			g_EnemyFly[ Cnt ].Point = Point;

			//	�g�p�t���O
			g_EnemyFly[ Cnt ].Use = true;

			//	�e�̃Z�b�g
//			SetShadow( g_EnemyFly[ Cnt ].World.Pos , 15.0f , D3DXCOLOR( 0.5f , 0.5f , 0.5f , 1.0f ) );

			break;

		}	//	end of if
		
	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void CollisionEnemyFly( int Cnt )
 ����:		int Cnt		�G�\���̂̃J�E���^
 �߂�l:	
 ����:		�����蔻��
-----------------------------------------------------------------------------*/
void CollisionEnemyFly( int Cnt )
{

	//	���b�V���E�H�[�����擾
	MESHWALL *MeshWall = GetMeshWall();


	//	�ǂƂ̔���
	if( g_EnemyFly[ Cnt ].World.Pos.z + 10.0f > MeshWall[ 0 ].World.Pos.z )
	{

		g_EnemyFly[ Cnt ].Move.z *= -1.0f;

	}	//	end of if

	else if( g_EnemyFly[ Cnt ].World.Pos.z - 10.0f < MeshWall[ 1 ].World.Pos.z )
	{

		g_EnemyFly[ Cnt ].Move.z *= -1.0f;

	}	//	end of else if

	else if( g_EnemyFly[ Cnt ].World.Pos.x - 10.0f < MeshWall[ 3 ].World.Pos.x )
	{

		g_EnemyFly[ Cnt ].Move.x *= -1.0f;

	}	//	end of else if

	else if( g_EnemyFly[ Cnt ].World.Pos.x + 10.0f > MeshWall[ 2 ].World.Pos.x )
	{

		g_EnemyFly[ Cnt ].Move.x *= -1.0f;

	}	//	end of else if


	//	�������̎擾
	BUILDING *Building = GetBuilding();

	for( int CntBuilding = 0 ; CntBuilding < MAX_BUILDING ; CntBuilding++ , Building++ )
	{
		//	���Ƌ��̓����蔻��
		if( SphereCollision( D3DXVECTOR3( g_EnemyFly[ Cnt ].World.Pos.x , g_EnemyFly[ Cnt ].World.Pos.y + 10.0f , g_EnemyFly[ Cnt ].World.Pos.z ) , D3DXVECTOR3( Building -> World.Pos.x , Building -> World.Pos.y + 30.0f , Building -> World.Pos.z ) , g_EnemyFly[ Cnt ].Radius , Building -> Radius ) )
		{

			if( g_EnemyFly[ Cnt ].Point = TYPE_POINT1 )
			{

				g_EnemyFly[ Cnt ].Move.x *= -1.0f;

			}	//	end of if

			else if( g_EnemyFly[ Cnt ].Point = TYPE_POINT2 )
			{

				g_EnemyFly[ Cnt ].Move.x *= -1.0f;

			}	//	end of else if

			else if( g_EnemyFly[ Cnt ].Point = TYPE_POINT2 )
			{

				g_EnemyFly[ Cnt ].Move.z *= -1.0f;

			}	//	end of else if

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	ENEMY *GetEnemyFly( void )
 ����:		
 �߂�l:	
 ����:		�G( ���V�^�C�v )���̎擾
-----------------------------------------------------------------------------*/
ENEMY *GetEnemyFly( void )
{

	return &g_EnemyFly[ 0 ];

}	//	end of func