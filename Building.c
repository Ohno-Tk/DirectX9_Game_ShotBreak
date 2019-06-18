/*=============================================================================

		建物[ Building.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2017/01/19
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Building.h"
#include "fade.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define MODEL_NAME "data/MODEL/Building/Saloon.x"

#define MAX_MAT ( 5 )	//	モデルの最大マテリアル数

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	建物のセット
void SetBuilding( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPD3DXMESH g_pMeshBuilding;	//	メッシュインターフェースポインタ

LPD3DXBUFFER g_pBufferMatBuilding;	//	マテリアル情報

DWORD g_nNumMatBuilding;	//	マテリアル数

LPDIRECT3DTEXTURE9 g_pTextureBuilding[ MAX_MAT ] = { NULL };//	テクスチャインターフェース

D3DXMATERIAL* pMatBuilding;	//	マテリアルポインタ

BUILDING g_Building[ MAX_BUILDING ];	//	建物の構造体

float g_Move;	//	移動量

/*-----------------------------------------------------------------------------
 関数名:	void InitBuilding( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitBuilding( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	HRESULT hr;

	hr = D3DXLoadMeshFromX( MODEL_NAME ,			//	ファイル名
							D3DXMESH_MANAGED,
							pDevice,				//	デバイス
							NULL,					//	隣接バッファ
							&g_pBufferMatBuilding,		//	マテリアル情報を格納
							NULL,
							&g_nNumMatBuilding,		//	マテリアル数
							&g_pMeshBuilding );		//	メッシュ

	//	モデルのエラーチェック
	if( FAILED( hr ) )
	{

		MessageBox( NULL , "[ Building.cpp ]\n MODEL_NAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	マテリアル情報のポインタとしてバッファのアドレスを取得
	pMatBuilding = ( D3DXMATERIAL* )g_pBufferMatBuilding -> GetBufferPointer();

	for( int i = 0 ; i < ( int )g_nNumMatBuilding ; i++ )
	{
		if( pMatBuilding[ i ].pTextureFilename != NULL )
		{

			//	テクスチャのエラーチェック
			if( FAILED( D3DXCreateTextureFromFile( pDevice , pMatBuilding[ i ].pTextureFilename , &g_pTextureBuilding[ i ]  ) ) )
			{

				MessageBox( NULL , "[ Building.cpp ]\n MODEL_NAME\nの画像の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

			}	//	end of if

		}	//	end of if

	}	//	end of for


	//	構造体初期化
	for( int Cnt = 0 ; Cnt < MAX_BUILDING ; Cnt++ )
	{

		//	座標
		g_Building[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	拡大率
		g_Building[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	回転量
		g_Building[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	半径
		g_Building[ Cnt ].Radius = 110.0f;

		//	使用フラグ
		g_Building[ Cnt ].Use = false;

	}	//	end of for


	//	変数初期化
	g_Move = 3.0f;



	//	セット

	//	右上の建物
	SetBuilding( D3DXVECTOR3( 1000.0f , 0.0f, 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 840.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 680.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 540.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 475.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 650.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 825.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , 1000.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );


	//	左上の建物
	SetBuilding( D3DXVECTOR3( -1000.0f , 0.0f, 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -840.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -680.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -540.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 300.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 475.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 650.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 825.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( -340.0f , 0.0f , 1000.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );


	//	右下の建物
	SetBuilding( D3DXVECTOR3( 1000.0f , 0.0f, -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 840.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 680.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 540.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );
	
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -475.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -650.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -825.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );
	SetBuilding( D3DXVECTOR3( 340.0f , 0.0f , -1000.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );


	//	左下の建物
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
 関数名:	void UninitBuilding( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitBuilding( void )
{

	if( g_pMeshBuilding != NULL )	//	メッシュインターフェースポインタ
	{

		g_pMeshBuilding -> Release();
		g_pMeshBuilding = NULL;

	}	//	end of if

	if( g_pBufferMatBuilding != NULL )	//	マテリアル情報開放
	{

		g_pBufferMatBuilding -> Release();
		g_pBufferMatBuilding = NULL;

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateBuilding( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateBuilding( void )
{

	//	次のモードの取得
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
 関数名:	void DrawBuilding( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawBuilding( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();



	//	現在デバイスに設定されているマテリアル情報の取得
	D3DMATERIAL9 matDef;
	pDevice -> GetMaterial( &matDef );


	for( int Cnt = 0 ; Cnt < MAX_BUILDING ; Cnt++ )
	{

		if( g_Building[ Cnt ].Use == true )
		{

			//	逆行列なしのワールド座標変換
			SetWorld( g_Building[ Cnt ].World.Pos , g_Building[ Cnt ].World.Rot , g_Building[ Cnt ].World.Scl );

			for( int i = 0 ; i < ( int )g_nNumMatBuilding ; i++ )
			{
				//	デバイスにマテリアルを設定
				pDevice -> SetMaterial( &pMatBuilding[ i ].MatD3D );

				//	テクスチャ設定
				pDevice -> SetTexture( 0 , g_pTextureBuilding[ i ] );


				//	描画
				g_pMeshBuilding -> DrawSubset( i );

			}	//	end of for

		}	//	end of if

		pDevice -> SetMaterial( &matDef );

	}

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	SetBuilding( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Rot		回転量
 戻り値:	
 説明:		建物のセット
-----------------------------------------------------------------------------*/
void SetBuilding( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
{

	for( int Cnt = 0 ; Cnt < MAX_BUILDING ; Cnt++ )
	{

		if( g_Building[ Cnt ].Use == false )
		{

			//	座標
			g_Building[ Cnt ].World.Pos = Pos;

			//	回転
			g_Building[ Cnt ].World.Rot = Rot;

			//	使用フラグ
			g_Building[ Cnt ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	BUILDING *GetBuilding( void )
 引数:		
 戻り値:	
 説明:		建物情報の取得
-----------------------------------------------------------------------------*/
BUILDING *GetBuilding( void )
{

	return &g_Building[ 0 ];

}	//	end of func