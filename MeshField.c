/*=============================================================================

		メッシュフィールド[ MeshField.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/11/09
-------------------------------------------------------------------------------
	■　Update
		2016/11/02
=============================================================================*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "MeshField.h"
#include "fade.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/Field.jpg"

#define MAX_FIELD ( 1 )	//	最大フィールド数

#define FIELD00_WIDTH ( 100 )	//	横の枚数
#define FIELD00_HEIGHT ( 100 )	//	縦の枚数

#define ONE_SIZE ( 50 )	//	1辺の大きさ

#define VERTEX ( ( ( 2 + 2 * FIELD00_WIDTH ) * FIELD00_HEIGHT + ( FIELD00_HEIGHT - 1 ) * 2 ) ) //	頂点数

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	ワールド変換用変数
	bool Use;	//	使用フラグ

}MESHFIELD;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexMeshField( LPDIRECT3DDEVICE9 pDevice , UINT NumBlock_X , UINT NumBlock_Y , float SizeBlock_X , float SizeBlock_Z );

//	頂点の変更
void VerTexField( void );

//	メッシュフィールドのセット
void SetField( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferField = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBufferField = NULL;	//	インデックスバッファインタフェースへのポインタのアドレス
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;//	テクスチャインターフェース

MESHFIELD g_MeshField[ MAX_FIELD ];	//	メッシュフィールド構造体

float g_VSet;	//	V座標

/*-----------------------------------------------------------------------------
 関数名:	void InitMeshField( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitMeshField( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureField  ) ) )
	{
		MessageBox( NULL , "[ MeshField.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexMeshField( pDevice , FIELD00_WIDTH , FIELD00_HEIGHT , ONE_SIZE , ONE_SIZE );


	//	構造体初期化
	for( int Cnt = 0 ; Cnt < MAX_FIELD ; Cnt++ )
	{
		//	座標
		g_MeshField[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	拡大率
		g_MeshField[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	回転量
		g_MeshField[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	使用フラグ
		g_MeshField[ Cnt ].Use = false;

	}	//	end of for


	//	変数初期化

	//	V座標
	g_VSet = 0.0f;


	//	メッシュフィールドのセット
	SetField( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitMeshField( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitMeshField( void )
{

	if( g_pVtxBufferField != NULL )	//	頂点バッファインタフェース開放
	{

		g_pVtxBufferField -> Release();
		g_pVtxBufferField = NULL;

	}	//	end of if

	if( g_pIdxBufferField != NULL )	//	インデックスバッファインタフェース開放
	{

		g_pIdxBufferField -> Release();
		g_pIdxBufferField = NULL;

	}	//	end of if

	if( g_pTextureField != NULL )	//	テクスチャポリゴン開放
	{

		g_pTextureField -> Release();
		g_pTextureField = NULL;

	}	//	end of if

}	//	end of func
 
/*-----------------------------------------------------------------------------
 関数名:	void UpdateMeshField( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateMeshField( void )
{

	//	次のモードの取得
	MODE *ModeNext = GetNextMode();

	//	モードの取得
	MODE *Mode = GetMode();


	if( *ModeNext == MODE_TITLE )
	{

		//	頂点の変更
		VerTexField();


		if( *Mode == MODE_RESULT )
		{

			//	V座標をずらす
			g_VSet -= 0.07f;

		}	//	end of if
		else
		{

			//	V座標をずらす
			g_VSet += 0.07f;

		}	//	end of else

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawMeshField( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawMeshField( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	パイプラインのストリーム
	pDevice -> SetStreamSource( 0 , g_pVtxBufferField , 0 , sizeof( VERTEX_3D ));

	pDevice -> SetIndices( g_pIdxBufferField );

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureField );


	for( int Cnt = 0 ; Cnt < MAX_FIELD ; Cnt++ )
	{

		if( g_MeshField[ Cnt ].Use == true )
		{

			//	逆行列なしのワールド座標変換
			SetWorld( g_MeshField[ Cnt ].World.Pos , g_MeshField[ Cnt ].World.Rot , g_MeshField[ Cnt ].World.Scl );

			//	ポリゴンの描画
			pDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 0 , VERTEX , 0 , VERTEX - 2 );

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexMeshField( LPDIRECT3DDEVICE9 pDevice , UINT NumBlock_X , UINT NumBlock_Y , float SizeBlock_X , float SizeBlock_Z )
 引数:		LPDIRECT3DDEVICE9 pDevice		デバイス
			UINT NumBlock_X					横のブロック数
			UINT NumBlock_Y					縦のブロック数
			float SizeBlock_X				X軸の大きさ
			float SizeBlock_Z				Z軸の大きさ
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexMeshField( LPDIRECT3DDEVICE9 pDevice , UINT NumBlock_X , UINT NumBlock_Y , float SizeBlock_X , float SizeBlock_Z )
{

	VERTEX_3D* pVtx;	//	仮想アドレスを取得するポインタ変数

	WORD* pIdx;	//	ポインタ変数

	HRESULT hr;

	D3DXVECTOR3 Pos;



	//	頂点数の計算
	int VerTex = ( NumBlock_X + 1 ) * ( NumBlock_Y + 1 );


	//	インデックスバッファの計算
	int IdxVerTex = ( 2 + 2 * NumBlock_X ) * NumBlock_Y + ( NumBlock_Y - 1 ) * 2;


	//	頂点バッファの生成
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * VerTex , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferField , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	//	インデックスバッファの生成
	hr = pDevice -> CreateIndexBuffer(
		sizeof( WORD ) * IdxVerTex ,	//	バッファ量
		D3DUSAGE_WRITEONLY ,	//	使用方法フラグ
		D3DFMT_INDEX16 , //	インデックスフォーマット
		D3DPOOL_MANAGED , //	メモリの管理方法
		&g_pIdxBufferField ,	//	インデックスバッファインターフェースへのポインタのアドレス
		NULL);


	//	インデックスバッファのエラーチェック
	if( FAILED( hr ) )
	{

		return E_FAIL;

	}	//	end of if


	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferField -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	//	縦
	for( UINT Cnt1 = 0 ; Cnt1 < NumBlock_Y + 1 ; Cnt1++ )
	{

		//	横
		for( UINT Cnt2 = 0 ; Cnt2 < NumBlock_X + 1 ; Cnt2++ )
		{

			//	座標の設定
			Pos.x = ( SizeBlock_X * NumBlock_X / 2 ) - NumBlock_X * SizeBlock_X + ( SizeBlock_X * Cnt2 );
			Pos.y = 0.0f;
			Pos.z = ( SizeBlock_Z * NumBlock_Y / 2 ) - ( SizeBlock_Z * Cnt1 );


			//	頂点座標の設定
			pVtx[ 0 ].pos = D3DXVECTOR3( Pos.x , Pos.y , Pos.z );


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
	g_pVtxBufferField -> Unlock();



	//	インデックスバッファをロック
	g_pIdxBufferField -> Lock( 0 , 0 , (void**)&pIdx , 0 );


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
	g_pIdxBufferField -> Unlock();

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTexField( void )
 引数:		
 戻り値:	
 説明:		頂点の変更
-----------------------------------------------------------------------------*/
void VerTexField( void )
{

	VERTEX_3D* pVtx;	//	仮想アドレスを取得するポインタ変数


	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferField -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	//	縦
	for( UINT Cnt1 = 0 ; Cnt1 < FIELD00_HEIGHT + 1 ; Cnt1++ )
	{

		//	横
		for( UINT Cnt2 = 0 ; Cnt2 < FIELD00_WIDTH + 1 ; Cnt2++ )
		{

			//	テクスチャ座標の設定
			pVtx[ 0 ].tex = D3DXVECTOR2( ( float )Cnt2 , ( float )Cnt1 + g_VSet );


			pVtx ++;	//	ポインタをずらす

		}	//	end of for

	}	//	end of for


	//	バッファのアンロック
	g_pVtxBufferField -> Unlock();

}

/*-----------------------------------------------------------------------------
 関数名:	void SetField( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Rot		回転量
 戻り値:	
 説明:		メッシュフィールドのセット
-----------------------------------------------------------------------------*/
void SetField( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
{
	for( int CntField = 0 ; CntField < MAX_FIELD ; CntField++ )
	{

		if( g_MeshField[ CntField ].Use == false )
		{

			//	座標
			g_MeshField[ CntField ].World.Pos = Pos;

			//	回転量
			g_MeshField[ CntField ].World.Rot = Rot;

			//	使用フラグ
			g_MeshField[ CntField ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func