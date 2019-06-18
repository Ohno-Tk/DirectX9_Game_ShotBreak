/*=============================================================================

		メッシュウォール[ MeshWall.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/11/09
-------------------------------------------------------------------------------
	■　Update
		2016/11/09
=============================================================================*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "MeshWall.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/MeshWall.png"

#define MAX_WALL ( 4 )	//	壁の最大数

#define WALL_WIDTH ( 10 )	//	横の枚数
#define WALL_HEIGHT ( 1 )	//	縦の枚数

#define WIDTH_ONE_SIZE ( 220 )	//	横の1辺の大きさ
#define HEIGHT_ONE_SIZE ( 50 )	//	縦の1辺の大きさ

#define WALL_POSITON ( WALL_WIDTH * WIDTH_ONE_SIZE / 2 )	//	壁の位置

#define VERTEX ( ( ( 2 + 2 * WALL_WIDTH ) * WALL_HEIGHT + ( WALL_HEIGHT - 1 ) * 2 ) )	//	頂点数

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexMeshWall( LPDIRECT3DDEVICE9 pDevice , UINT NumBlock_X , UINT NumBlock_Y , float SizeBlock_X , float SizeBlock_Y );

//	壁のセット
void SetWall( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMeshWall = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBufferMeshWall = NULL;	//	インデックスバッファインタフェースへのポインタのアドレス
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;//	テクスチャインターフェース

MESHWALL g_MeshWall[ MAX_WALL ];	//	メッシュウォール構造体

/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitMeshWall( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitMeshWall( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureMeshWall  ) ) )
	{
		MessageBox( NULL , "[ MeshWall.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexMeshWall( pDevice , WALL_WIDTH , WALL_HEIGHT , WIDTH_ONE_SIZE , HEIGHT_ONE_SIZE );


	//	構造体初期化
	for( int Cnt = 0 ; Cnt < MAX_WALL ; Cnt++ )
	{
		//	座標
		g_MeshWall[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	拡大率
		g_MeshWall[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	回転量
		g_MeshWall[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	使用フラグ
		g_MeshWall[ Cnt ].Use = false;

	}	//	end of for


	//	壁のセット

	//	奥の壁
	SetWall( D3DXVECTOR3( 0.0f , 0.0f , WALL_POSITON ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );


	//	手前の壁
	SetWall( D3DXVECTOR3( 0.0f , 0.0f , -WALL_POSITON ) , D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) );


	//右の壁
	SetWall( D3DXVECTOR3( WALL_POSITON , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) );


	//	左の壁
	SetWall( D3DXVECTOR3( -WALL_POSITON , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) );


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitMeshWall( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitMeshWall( void )
{

	if( g_pVtxBufferMeshWall != NULL )	//	頂点バッファインタフェース開放
	{
		g_pVtxBufferMeshWall -> Release();
		g_pVtxBufferMeshWall = NULL;

	}	//	end of if

	if( g_pIdxBufferMeshWall != NULL )	//	インデックスバッファインタフェース開放
	{
		g_pIdxBufferMeshWall -> Release();
		g_pIdxBufferMeshWall = NULL;

	}	//	end of if

	if( g_pTextureMeshWall != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureMeshWall -> Release();
		g_pTextureMeshWall = NULL;

	}	//	end of if

}	//	end of func
 
/*-----------------------------------------------------------------------------
 関数名:	void UpdateMeshWall( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateMeshWall( void )
{

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawMeshWall( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawMeshWall( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	パイプラインのストリーム
	pDevice -> SetStreamSource( 0 , g_pVtxBufferMeshWall , 0 , sizeof( VERTEX_3D ));


	//	インデックスのセット
	pDevice -> SetIndices( g_pIdxBufferMeshWall );


	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_3D );


	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureMeshWall );



	for( int Cnt = 0 ; Cnt < MAX_WALL ; Cnt++ )
	{

		if( g_MeshWall[ Cnt ].Use == true )
		{

			//	逆行列なしのワールド座標変換
			SetWorld( g_MeshWall[ Cnt ].World.Pos , g_MeshWall[ Cnt ].World.Rot , g_MeshWall[ Cnt ].World.Scl );

			//	ポリゴンの描画
			pDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 0 , VERTEX , 0 , VERTEX - 2 );

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void MakeVertexMeshWall( LPDIRECT3DDEVICE9 pDevice , UINT NumBlock_X , UINT NumBlock_Y , float SizeBlock_X , float SizeBlock_Y )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
			UINT NumBlock_X				横のブロック数
			UINT NumBlock_Y				縦のブロック数
			float SizeBlock_X			X軸の大きさ
			float SizeBlock_Y			Y軸の大きさ
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexMeshWall( LPDIRECT3DDEVICE9 pDevice , UINT NumBlock_X , UINT NumBlock_Y , float SizeBlock_X , float SizeBlock_Y )
{

	VERTEX_3D* pVtx;	//	仮想アドレスを取得するポインタ変数

	WORD* pIdx;	//	ポインタ変数

	HRESULT hr;

	//	頂点数の計算
	int VerTex = ( NumBlock_X + 1 ) * ( NumBlock_Y + 1 );

	//	インデックスバッファの計算
	int IdxVerTexMeshWall = ( 2 + 2 * NumBlock_X ) * NumBlock_Y + ( NumBlock_Y - 1 ) * 2;


	//	頂点バッファの生成
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * VerTex , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferMeshWall , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if

	//	インデックスバッファの生成
	hr = pDevice -> CreateIndexBuffer(
		sizeof( WORD ) * IdxVerTexMeshWall ,	//	バッファ量
		D3DUSAGE_WRITEONLY ,	//	使用方法フラグ
		D3DFMT_INDEX16 , //	インデックスフォーマット
		D3DPOOL_MANAGED , //	メモリの管理方法
		&g_pIdxBufferMeshWall ,	//	インデックスバッファインターフェースへのポインタのアドレス
		NULL);

	if( FAILED( hr ) )
	{

		return E_FAIL;

	}	//	end of if


	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferMeshWall -> Lock( 0 , 0 , (void**)&pVtx , 0 );

	//	縦
	for( UINT Cnt1 = 0 ; Cnt1 < NumBlock_Y + 1 ; Cnt1++ )
	{
		//	横
		for( UINT Cnt2 = 0 ; Cnt2 < NumBlock_X + 1 ; Cnt2++ )
		{

			//	頂点座標の設定
			//	奥の壁
			pVtx[ 0 ].pos = D3DXVECTOR3( ( SizeBlock_X * NumBlock_X / 2 ) - NumBlock_X * SizeBlock_X + ( SizeBlock_X * Cnt2 ) , ( SizeBlock_Y * NumBlock_Y ) - ( SizeBlock_Y * Cnt1 ) , 0.0f );


			//	法線の設定
			pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );


			//	頂点色の設定
			pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );


			//	テクスチャ座標の設定
			pVtx[ 0 ].tex = D3DXVECTOR2( ( float )Cnt2 , ( float )Cnt1 );

			pVtx ++;	//	ポインタをずらす

		}	//	end of for

	}	//	end of for

	//	バッファのアンロック
	g_pVtxBufferMeshWall -> Unlock();



	//	インデックスバッファをロック
	g_pIdxBufferMeshWall -> Lock( 0 , 0 , (void**)&pIdx , 0 );

	//	縦
	for( UINT Cnt1 = 0 ; Cnt1 < NumBlock_Y ; Cnt1++ )
	{
		//	はじめじゃなかったら
		//	縮退ポリゴン
		if( Cnt1 != 0 )
		{
			//	1点打つ

			pIdx[ 0 ] = ( Cnt1 + 1 ) * ( NumBlock_X + 1 );

			pIdx++;	//	ポインタをずらす

		}	//	end of if

		//	2点打つ

		pIdx[ 0 ] = ( Cnt1 + 1 ) * ( NumBlock_X + 1 );
		pIdx[ 1 ] = ( Cnt1 + 1 ) * ( NumBlock_X + 1 ) - ( NumBlock_X + 1 );

		pIdx += 2;	//	ポインタをずらす

		//	横
		for( UINT Cnt2 = 0 ; Cnt2 < NumBlock_X ; Cnt2++ )
		{
			//	2点打つ

			pIdx[ 0 ] = ( ( Cnt1 + 1 ) * ( NumBlock_X + 1 ) + 1 ) + Cnt2;
			pIdx[ 1 ] = ( ( Cnt1 + 1 ) * ( NumBlock_X + 1 ) + 1 ) - ( NumBlock_X + 1 ) + Cnt2;

			pIdx += 2;	//	ポインタをずらす

		}	//	end of for

		//	最後じゃなかったら
		//	縮退ポリゴン
		if( Cnt1 != NumBlock_Y - 1 )
		{
			//	1点打つ

			pIdx[ 0 ] = ( Cnt1 + 1 ) * ( NumBlock_X + 1 ) - 1;

			pIdx++;	//	ポインタをずらす

		}	//	end of if

	}	//	end of for


	//	インデックスバッファのアンロック
	g_pIdxBufferMeshWall -> Unlock();

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetWall( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Rot		回転量
 戻り値:	
 説明:		壁のセット
-----------------------------------------------------------------------------*/
void SetWall( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
{

	for( int Cnt = 0 ; Cnt < MAX_WALL ; Cnt++ )
	{

		if( g_MeshWall[ Cnt ].Use == false )
		{

			//	座標
			g_MeshWall[ Cnt ].World.Pos = Pos;

			//	回転量
			g_MeshWall[ Cnt ].World.Rot = Rot;

			//	使用フラグ
			g_MeshWall[ Cnt ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	MESHWALL *GetMeshWall( )
 引数:		
 戻り値:	なし
 説明:		メッシュウォール情報取得
-----------------------------------------------------------------------------*/
MESHWALL *GetMeshWall( void )
{

	return &g_MeshWall[ 0 ];

}	//	end of func