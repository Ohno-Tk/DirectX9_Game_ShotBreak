/*=============================================================================

		����[ explosin.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/12/13
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"
#include "explosion.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/Explosin.png"

#define MAX_EXPLOSION ( 100 )	//	�����ő吔

#define MAX_TEX ( 8 )	//	�����̃e�N�X�`�������邩
#define MAX_TEX_X ( 4 )	//	���̃e�N�X�`�����������邩

#define TEX_ONE_WIDTH ( 0.25f )	//	��̃e�N�X�`���̉���
#define TEX_ONE_HEIGHT ( 0.5f )	//	��̃e�N�X�`���̏c��

#define TEX_WIDTH ( TEX_ONE_WIDTH * ( MAX_TEX_X - 1 ) )	//	�e�N�X�`���̉���

#define TEX_SPEED ( 5 )	//	�e�N�X�`���̕`��X�s�[�h( ���l��0�ɂȂ�ɂ�ĕ`�悪�����Ȃ� )

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	���[���h�ϊ��p�ϐ�
	D3DXVECTOR2 UVSet;	//	UV���W
	int Frame;	//	�t���[��
	float Radius;	//	���a
	bool Use;	//	�g�p�t���O

}EXPLOSION;

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

//	���_�̍쐬
HRESULT MakeVertexExplosin( LPDIRECT3DDEVICE9 pDevice );

//	�����̃e�N�X�`���ύX
void VerTexExplosin( VERTEX_3D* pVtx , int Cnt );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferExplosin = NULL;	//	���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureExplosin = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

EXPLOSION g_Explosion[ MAX_EXPLOSION ];		//	�����\����

/*-----------------------------------------------------------------------------
 �֐���:	void InitExplosin( void )
 ����:		
 �߂�l:	
 ����:		�����̏�����
-----------------------------------------------------------------------------*/
void InitExplosin( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureExplosin  ) ) )
	{
		MessageBox( NULL , "[ explosin.cpp ]\n POLYGON00_TEXTURENAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	���_�̍쐬
	MakeVertexExplosin( pDevice );


	//	�\���̏�����
	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{
		//	���W
		g_Explosion[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�g�嗦
		g_Explosion[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	��]��
		g_Explosion[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�t���[��
		g_Explosion[ Cnt ].Frame = 0;

		//	U���W
		g_Explosion[ Cnt ].UVSet.x = 0.0f;

		//	V���W
		g_Explosion[ Cnt ].UVSet.y = 0.0f;

		//	���a
		g_Explosion[ Cnt ].Radius = 0.0f;

		//	�g�p�t���O
		g_Explosion[ Cnt ].Use = false;

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitExplosin( void )
 ����:		
 �߂�l:	
 ����:		�����̏I��
-----------------------------------------------------------------------------*/
void UninitExplosin( void )
{

	if( g_pVtxBufferExplosin != NULL )	//	���_�o�b�t�@�C���^�t�F�[�X�J��
	{
		g_pVtxBufferExplosin -> Release();
		g_pVtxBufferExplosin = NULL;

	}	//	end of if

	if( g_pTextureExplosin != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureExplosin -> Release();
		g_pTextureExplosin = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateExplosin( void )
 ����:		
 �߂�l:	
 ����:		�����̍X�V
-----------------------------------------------------------------------------*/
void UpdateExplosin( void )
{

	//	���z�A�h���X���擾����|�C���^�ϐ�
	VERTEX_3D* pVtx;


	//	�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferExplosin -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{
		if( g_Explosion[ Cnt ].Use == true )
		{
			
			//	�t���[���𑝉�
			g_Explosion[ Cnt ].Frame++;


			//	�e�N�X�`���A�j���[�V����
			g_Explosion[ Cnt ].UVSet.x = ( ( ( g_Explosion[ Cnt ].Frame / TEX_SPEED ) % MAX_TEX_X ) * TEX_ONE_WIDTH );	//	�㕔�e�N�X�`��
			g_Explosion[ Cnt ].UVSet.y = ( ( ( g_Explosion[ Cnt ].Frame / TEX_SPEED ) % MAX_TEX ) / MAX_TEX_X * TEX_ONE_HEIGHT );	//	�����e�N�X�`��


			//	�e�N�X�`���A�j���[�V�������I�������
			if( g_Explosion[ Cnt ].UVSet.x == TEX_WIDTH && g_Explosion[ Cnt ].UVSet.y == TEX_ONE_HEIGHT )
			{

				g_Explosion[ Cnt ].Use = false;

			}	//	end of if


			//	���_�̕ύX
			VerTexExplosin( pVtx , Cnt );

		}	//	end of if

	}	//	end of for


	//	�o�b�t�@�̃A�����b�N
	g_pVtxBufferExplosin -> Unlock();


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawExplosin( void )
 ����:		
 �߂�l:	
 ����:		�e�̕`��
-----------------------------------------------------------------------------*/
void DrawExplosin( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	�p�C�v���C���̃X�g���[��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferExplosin , 0 , sizeof( VERTEX_3D ));

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	�e�N�X�`���̃Z�b�g
	pDevice -> SetTexture( 0 , g_pTextureExplosin );

	//	Z�o�b�t�@
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , FALSE );


	//	�S�̂̃��C�g��L���ɂ���
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );



	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{

		if( g_Explosion[ Cnt ].Use == true )
		{

			//	�t�s�񂠂�̃��[���h���W�ϊ�
			SetWorldInv( g_Explosion[ Cnt ].World.Pos , g_Explosion[ Cnt ].World.Rot , g_Explosion[ Cnt ].World.Scl );


			//	�|���S���̕`��
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , Cnt * NUM_VERTEX , NUM_POLYGON );

		}	//	end of if

	}	//	end of for


	//	�S�̂̃��C�g��L���ɂ���
	pDevice -> SetRenderState( D3DRS_LIGHTING , TRUE );

	//	���ɖ߂�
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexExplosin( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexExplosin( LPDIRECT3DDEVICE9 pDevice )
{

	//	���z�A�h���X���擾����|�C���^�ϐ�
	VERTEX_3D* pVtx;

	//	���_�o�b�t�@�̐���
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * MAX_EXPLOSION , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferExplosin , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if


	//	�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferExplosin -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{

		//	���_���W�̐ݒ�
		pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );


		//	�@���̐ݒ�
		pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 1 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 2 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 3 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );

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

		pVtx += 4;	//	pVtx�����炷

	}	//	end of for


	//	�o�b�t�@�̃A�����b�N
	g_pVtxBufferExplosin -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void VerTexExplosin( VERTEX_3D* pVtx , int Cnt )
 ����:		VERTEX_3D* pVtx		���z�A�h���X���擾����|�C���^�ϐ�
			int Cnt				�����\���̂̃J�E���^
 �߂�l:	
 ����:		���_�̕ύX
-----------------------------------------------------------------------------*/
void VerTexExplosin( VERTEX_3D* pVtx , int Cnt )
{

	//	pVtx�����炷
	pVtx += Cnt * NUM_VERTEX;


	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( -g_Explosion[ Cnt ].Radius , g_Explosion[ Cnt ].Radius , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3(  g_Explosion[ Cnt ].Radius , g_Explosion[ Cnt ].Radius , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( -g_Explosion[ Cnt ].Radius , -g_Explosion[ Cnt ].Radius , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3(  g_Explosion[ Cnt ].Radius , -g_Explosion[ Cnt ].Radius , 0.0f );


	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( g_Explosion[ Cnt ].UVSet.x                 , g_Explosion[ Cnt ].UVSet.y );
	pVtx[ 1 ].tex = D3DXVECTOR2( g_Explosion[ Cnt ].UVSet.x + TEX_ONE_WIDTH , g_Explosion[ Cnt ].UVSet.y );
	pVtx[ 2 ].tex = D3DXVECTOR2( g_Explosion[ Cnt ].UVSet.x                 , g_Explosion[ Cnt ].UVSet.y + TEX_ONE_HEIGHT );
	pVtx[ 3 ].tex = D3DXVECTOR2( g_Explosion[ Cnt ].UVSet.x + TEX_ONE_WIDTH , g_Explosion[ Cnt ].UVSet.y + TEX_ONE_HEIGHT );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void SetExplosin( D3DXVECTOR3 Pos , float Radius )
 ����:		D3DXVECTOR3 Pos		���W
			float Radius		���a
 �߂�l:	
 ����:		�����̃Z�b�g
-----------------------------------------------------------------------------*/
void SetExplosin( D3DXVECTOR3 Pos , float Radius )
{

	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{

		if( g_Explosion[ Cnt ].Use == false )
		{

			//	���W
			g_Explosion[ Cnt ].World.Pos = Pos;

			//	�t���[��
			g_Explosion[ Cnt ].Frame = 0;

			//	U���W
			g_Explosion[ Cnt ].UVSet.x = 0.0f;

			//	V���W
			g_Explosion[ Cnt ].UVSet.y = 0.0f;

			//	���a
			g_Explosion[ Cnt ].Radius = Radius;

			//	�g�p�t���O
			g_Explosion[ Cnt ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func
