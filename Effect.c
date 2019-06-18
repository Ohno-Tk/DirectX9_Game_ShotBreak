/*=============================================================================

		�G�t�F�N�g[ Effect.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2017/01/22
-------------------------------------------------------------------------------
	���@Update
=============================================================================*/

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Effect.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/effect000.jpg"

#define MAX_EFFECT ( 100 )	//	�G�t�F�N�g�̍ő吔

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	���[���h�ϊ��p�ϐ�
	D3DXCOLOR Color;	//	�F
	int Life;	//	����
	float Radius;	//	���a
	float RadiusValue;	//	���a�̕ω���
	bool Use;	//	�g�p�t���O

}EFFECT;

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

//	���_�̍쐬
HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice );

//	���_�̕ύX
void VerTex( VERTEX_3D* pVtx , int Cnt );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEffect = NULL;	//	���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

EFFECT g_Effect[ MAX_EFFECT ];	//	�G�t�F�N�g�\����

/*-----------------------------------------------------------------------------
 �֐���:	void InitEffect( void )
 ����:		
 �߂�l:	
 ����:		������
-----------------------------------------------------------------------------*/
void InitEffect( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureEffect  ) ) )
	{
		MessageBox( NULL , "[ Effect.cpp ]\n POLYGON00_TEXTURENAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	���_�̍쐬
	MakeVertexEffect( pDevice );


	//	�\���̏�����
	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
	{

		//	���W
		g_Effect[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	��]��
		g_Effect[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�g�嗦
		g_Effect[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	�F
		g_Effect[ Cnt ].Color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f );

		//	���a
		g_Effect[ Cnt ].Radius = 0.0f;

		//	���a�̕ω���
		g_Effect[ Cnt ].RadiusValue = 0.0f;

		//	����
		g_Effect[ Cnt ].Life = 0;

		//	�g�p�t���O
		g_Effect[ Cnt ].Use = false;

	}	//	end of for


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitEffect( void )
 ����:		
 �߂�l:	
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitEffect( void )
{
	if( g_pVtxBufferEffect != NULL )	//	���_�o�b�t�@�C���^�t�F�[�X�J��
	{
		g_pVtxBufferEffect -> Release();
		g_pVtxBufferEffect = NULL;

	}	//	end of if

	if( g_pTextureEffect != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureEffect -> Release();
		g_pTextureEffect = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateEffect( void )
 ����:		
 �߂�l:	
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdateEffect( void )
{

	//	���z�A�h���X���擾����|�C���^�ϐ�
	VERTEX_3D* pVtx;


	//	�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferEffect -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
	{

		if( g_Effect[ Cnt ].Use == true )
		{

			//	���a�����炷
			g_Effect[ Cnt ].Radius -= g_Effect[ Cnt ].RadiusValue;

			//	���������炷
			g_Effect[ Cnt ].Life--;

			if( g_Effect[ Cnt ].Life < 0 )
			{

				g_Effect[ Cnt ].Use = false;

			}	//	end of if

			//	���_�̕ύX
			VerTex( pVtx , Cnt );

		}	//	end of if

	}	//	end of for


	//	�o�b�t�@�̃A�����b�N
	g_pVtxBufferEffect -> Unlock();

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawEffect( void )
 ����:		
 �߂�l:	
 ����:		�`��
-----------------------------------------------------------------------------*/
void DrawEffect( void )
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	�p�C�v���C���̃X�g���[��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferEffect , 0 , sizeof( VERTEX_3D ));

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	�e�N�X�`���̃Z�b�g
	pDevice -> SetTexture( 0 , g_pTextureEffect );

	//	Z�o�b�t�@
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , FALSE );


	//	�S�̂̃��C�g��L���ɂ���
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );


	//	a�u�����h�i���Z�����j
	pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );	//	Init�ɏ����Ă���̂ŏȗ���
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );	//	Init�ɏ����Ă���̂ŏȗ���
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );



	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
	{

		if( g_Effect[ Cnt ].Use == true )
		{

			//	�t�s�񂠂�̃��[���h���W�ϊ�
			SetWorldInv( g_Effect[ Cnt ].World.Pos , g_Effect[ Cnt ].World.Rot , g_Effect[ Cnt ].World.Scl );

			//	�|���S���̕`��
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , Cnt * NUM_VERTEX , NUM_POLYGON );

		}	//	end of if

	}	//	end of for


	//	���ɖ߂�
	pDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );


	//	�S�̂̃��C�g��L���ɂ���
	pDevice -> SetRenderState( D3DRS_LIGHTING , TRUE );

	//	���ɖ߂�
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice	�f�o�C�X
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice )
{

	//	���z�A�h���X���擾����|�C���^�ϐ�
	VERTEX_3D* pVtx;

	//	���_�o�b�t�@�̐���
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * MAX_EFFECT , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferEffect , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if


	//	�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferEffect -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
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
	g_pVtxBufferEffect -> Unlock();

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void VerTex( VERTEX_3D* pVtx , int Cnt )
 ����:		VERTEX_3D* pVtx		���z�A�h���X���擾����|�C���^�ϐ�
			int Cnt				�G�t�F�N�g�\���̂̃J�E���^
 �߂�l:	
 ����:		���_�̕ύX
-----------------------------------------------------------------------------*/
void VerTex( VERTEX_3D* pVtx , int Cnt )
{

	//	pVtx�����炷
	pVtx += Cnt * NUM_VERTEX;


	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( -g_Effect[ Cnt ].Radius , g_Effect[ Cnt ].Radius , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3(  g_Effect[ Cnt ].Radius , g_Effect[ Cnt ].Radius , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( -g_Effect[ Cnt ].Radius , -g_Effect[ Cnt ].Radius , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3(  g_Effect[ Cnt ].Radius , -g_Effect[ Cnt ].Radius , 0.0f );


	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( g_Effect[ Cnt ].Color.r , g_Effect[ Cnt ].Color.g , g_Effect[ Cnt ].Color.b , g_Effect[ Cnt ].Color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Effect[ Cnt ].Color.r , g_Effect[ Cnt ].Color.g , g_Effect[ Cnt ].Color.b , g_Effect[ Cnt ].Color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Effect[ Cnt ].Color.r , g_Effect[ Cnt ].Color.g , g_Effect[ Cnt ].Color.b , g_Effect[ Cnt ].Color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Effect[ Cnt ].Color.r , g_Effect[ Cnt ].Color.g , g_Effect[ Cnt ].Color.b , g_Effect[ Cnt ].Color.a );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void SetEffect( D3DXVECTOR3 Pos , D3DXCOLOR Color , int Life , float Radius )
 ����:		D3DXVECTOR3 Pos			���W
			D3DXVECTOR2 VerTexPos	���_���W
			int Life				����
			float Radius			���a
 �߂�l:	
 ����:		�G�t�F�N�g�̃Z�b�g 
-----------------------------------------------------------------------------*/
void SetEffect( D3DXVECTOR3 Pos , D3DXCOLOR Color , int Life , float Radius )
{

	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
	{

		if( g_Effect[ Cnt ].Use == false )
		{

			//	���W
			g_Effect[ Cnt ].World.Pos = Pos;

			//	�F
			g_Effect[ Cnt ].Color = Color;

			//	����
			g_Effect[ Cnt ].Life = Life;

			//	���a
			g_Effect[ Cnt ].Radius = Radius;

			//	���a�̕ω���
			g_Effect[ Cnt ].RadiusValue = g_Effect[ Cnt ].Radius / g_Effect[ Cnt ].Life;

			//	�g�p�t���O
			g_Effect[ Cnt ].Use = true;


			break;

		}	//	end of if
	}	//	end of for

}	//	end of func