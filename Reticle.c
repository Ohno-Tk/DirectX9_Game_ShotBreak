/*=============================================================================

		照準[ Reticle.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2017/01/18
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
						scanf のwarning防止 
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "input.h"
#include "Reticle.h"
#include "bullet.h"
#include "Player.h"
#include "Enemy.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/reticle.png"

#define POLYGON_POS_X ( 5.0f )	//	X
#define POLYGON_POS_Y ( 5.0f )	//	Y

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	ワールド変換用変数
	D3DXVECTOR3 Move;	//	移動量
	D3DXCOLOR Color;	//	色
	float Length;	//	長さ
	bool Use;	//	使用フラグ

}RETICLE;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexReticle( LPDIRECT3DDEVICE9 pDevice );

//	頂点の変更
void VerTexReticle( VERTEX_3D* pVtx );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferReticle = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureReticle = NULL;//	テクスチャインターフェース

RETICLE g_Reticle;	//	レティクル構造体

#ifdef _DEBUG

	LPD3DXFONT g_pFontReticle = NULL;

#endif	//	_DEBUG

/*-----------------------------------------------------------------------------
 関数名:	void InitReticle( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitReticle( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureReticle ) ) )
	{

		MessageBox( NULL , "[ Reticle.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexReticle( pDevice );


	//	構造体初期化

	//	座標
	g_Reticle.World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//	拡大率
	g_Reticle.World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

	//	回転量
	g_Reticle.World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//	移動量
	g_Reticle.Move = D3DXVECTOR3( 5.0f , 5.0f , 5.0f );

	//	長さ
	g_Reticle.Length = 30.0f;

	//	色
	g_Reticle.Color = D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f );

		
#ifdef _DEBUG
	//	フォントの設定
	D3DXCreateFont( pDevice ,
				18 ,	//	文字高さ
				0 ,	//	文字幅
				0 ,	//	文字の太さ
				0 ,	//	ミップマップ
				FALSE ,	//	イタリック
				SHIFTJIS_CHARSET ,
				OUT_DEFAULT_PRECIS ,
				DEFAULT_QUALITY ,
				DEFAULT_PITCH ,
				"Terminal" ,
				&g_pFontReticle );

#endif
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitReticle( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitReticle( void )
{

	if(g_pVtxBufferReticle != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferReticle -> Release();
		g_pVtxBufferReticle  = NULL;

	}	//	end of if



	if( g_pTextureReticle != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureReticle -> Release();
		g_pTextureReticle = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateReticle( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateReticle( void )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_3D* pVtx;


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferReticle -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );


	//	頂点の変更
	VerTexReticle( pVtx );


	g_pVtxBufferReticle -> Unlock(); //これ以降触れてはいけない


	PLAYER *Player = GetPlayer();

	if( GetKeyboardTrigger( DIK_SPACE ) )
	{

		//	弾のセット
		SetBullet( D3DXVECTOR3( g_Reticle.World.Pos.x , g_Reticle.World.Pos.y - 5.0f , g_Reticle.World.Pos.z ) , Player -> World.Rot , 50.0f );

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawReticle( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawReticle( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	逆行列ありのワールド座標変換
	SetWorld( g_Reticle.World.Pos , g_Reticle.World.Rot , g_Reticle.World.Scl );
	

	//	パイプラインのストリーム
	pDevice -> SetStreamSource( 0 , g_pVtxBufferReticle , 0 , sizeof( VERTEX_3D ));


	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_3D );


	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureReticle );


	//	全体のライトを有効にする
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );


	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON );


	//	全体のライトを有効にする
	pDevice -> SetRenderState( D3DRS_LIGHTING , TRUE );


#ifdef _DEBUG
/*
	RECT rect = { 0 , 40 , SCREEN_WIDTH , SCREEN_HEIGHT };
	RECT rect01 = { 0 , 60 , SCREEN_WIDTH , SCREEN_HEIGHT };

	char aStr[ 256 ] , aStr01[ 256 ];

	sprintf( &aStr[ 0 ] , "レティクル座標 : %.3f , %.3f , %.3f" , g_Reticle.World.Pos.x , g_Reticle.World.Pos.y , g_Reticle.World.Pos.z );
	sprintf( &aStr01[ 0 ] , "レティクル角度 : %.3f , %.3f , %.3f" , g_Reticle.World.Rot.x , g_Reticle.World.Rot.y , g_Reticle.World.Rot.z );

	g_pFontReticle -> DrawText( NULL ,
						&aStr[ 0 ] ,
						-1 ,
						&rect ,
						DT_LEFT ,
						D3DCOLOR_RGBA( 255 , 255 , 255 , 255));

	g_pFontReticle -> DrawText( NULL ,
						&aStr01[ 0 ] ,
						-1 ,
						&rect01 ,
						DT_LEFT ,
						D3DCOLOR_RGBA( 255 , 255 , 255 , 255));
*/
#endif


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexReticle( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexReticle( LPDIRECT3DDEVICE9 pDevice )
{

	// 構造体のポインタ宣言
	VERTEX_3D* pVtx;


	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_3D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferReticle , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferReticle -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( POLYGON_POS_X , POLYGON_POS_Y  , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( -POLYGON_POS_X  , POLYGON_POS_Y  , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( POLYGON_POS_X , -POLYGON_POS_Y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( -POLYGON_POS_X  , -POLYGON_POS_Y , 0.0f );

	//	法線の設定
	pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	pVtx[ 1 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	pVtx[ 2 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	pVtx[ 3 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );

	//	頂点色の設定
	pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );


	g_pVtxBufferReticle -> Unlock(); //これ以降触れてはいけない

	return S_OK;


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTexReticle( VERTEX_3D* pVtx )
 引数:		VERTEX_3D* pVtx
 戻り値:	
 説明:		頂点の変更
-----------------------------------------------------------------------------*/
void VerTexReticle( VERTEX_3D* pVtx )
{


	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_Reticle.Color.r , g_Reticle.Color.g , g_Reticle.Color.b , g_Reticle.Color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Reticle.Color.r , g_Reticle.Color.g , g_Reticle.Color.b , g_Reticle.Color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Reticle.Color.r , g_Reticle.Color.g , g_Reticle.Color.b , g_Reticle.Color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Reticle.Color.r , g_Reticle.Color.g , g_Reticle.Color.b , g_Reticle.Color.a );



}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetReticle( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXCOLOR Color )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Rot		回転量
			D3DXCOLOR Color		色
 戻り値:	
 説明:		レティクルのセット
-----------------------------------------------------------------------------*/
void SetReticle( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXCOLOR Color )
{

	float rot = Rot.y + 180;

	//	座標
	g_Reticle.World.Pos.x = cosf( D3DXToRadian( Rot.x ) ) * sinf( D3DXToRadian( rot ) ) * g_Reticle.Length + Pos.x;
	g_Reticle.World.Pos.y = sinf( D3DXToRadian( Rot.x ) ) * g_Reticle.Length + Pos.y;
	g_Reticle.World.Pos.z = cosf( D3DXToRadian( Rot.x ) ) * cosf( D3DXToRadian( rot ) ) * g_Reticle.Length + Pos.z;

	//	回転量
	g_Reticle.World.Rot = Rot;

	//	色
	g_Reticle.Color = Color;

}	//	end of func
