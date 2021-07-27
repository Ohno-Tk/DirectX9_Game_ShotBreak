/*=============================================================================

		スカイドーム[ skydome.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2016/11/14
-------------------------------------------------------------------------------
	■　Update
		2016/11/14
=============================================================================*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "skydome.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/Skydome.jpg"

#define FIELD_WIDTH ( 20 )	//	横の枚数
#define FIELD_HEIGHT ( 30 )	//	縦の枚数

#define RADIUS ( 2600.0f )	//	半径

#define VERTEX ( ( ( 2 + 2 * FIELD_WIDTH ) * FIELD_HEIGHT + ( FIELD_HEIGHT - 1 ) * 2 ) ) //	頂点数

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

}SKYDOME;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexSkyDome( LPDIRECT3DDEVICE9 pDevice , int NumBlock_X , int NumBlock_Y , float Radius );

//	メッシュフィールドのセット
void SetSkyDome( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferSkyDome = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBufferSkyDome = NULL;	//	インデックスバッファインタフェースへのポインタのアドレス

LPDIRECT3DTEXTURE9 g_pTextureSkyDome = NULL;	//	テクスチャインターフェース

SKYDOME g_SkyDome;		//	スカイドーム構造体

/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitSkyDome( void )
 引数:		なし
 戻り値:	なし
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitSkydome( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureSkyDome  ) ) )
	{
		MessageBox( NULL , "[ skydome.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexSkyDome( pDevice , FIELD_WIDTH , FIELD_HEIGHT , RADIUS );


	//	構造体初期化

	//	座標
	g_SkyDome.World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//	回転量
	g_SkyDome.World.Rot =  D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//	拡大率
	g_SkyDome.World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

	//	使用フラグ
	g_SkyDome.Use = false;



	//	メッシュフィールドのセット
	SetSkyDome( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitSkyDome( void )
 引数:		なし
 戻り値:	なし
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitSkydome( void )
{

	if( g_pVtxBufferSkyDome != NULL )	//	頂点バッファインタフェース開放
	{
		g_pVtxBufferSkyDome -> Release();
		g_pVtxBufferSkyDome = NULL;

	}	//	end of if

	if( g_pIdxBufferSkyDome != NULL )	//	インデックスバッファインタフェース開放
	{
		g_pIdxBufferSkyDome -> Release();
		g_pIdxBufferSkyDome = NULL;

	}	//	end of if

	if( g_pTextureSkyDome != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureSkyDome -> Release();
		g_pTextureSkyDome = NULL;

	}	//	end of if

}	//	end of func
 
/*-----------------------------------------------------------------------------
 関数名:	void UpdateSkyDome( void )
 引数:		なし
 戻り値:	なし
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateSkydome( void )
{

	g_SkyDome.World.Rot.y += 0.01f;

	if( g_SkyDome.World.Rot.y >= 360 )
	{
		g_SkyDome.World.Rot.y = 0;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawSkyDome( void )
 引数:		なし
 戻り値:	なし
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawSkydome( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	パイプラインのストリーム
	pDevice -> SetStreamSource( 0 , g_pVtxBufferSkyDome , 0 , sizeof( VERTEX_3D ));

	pDevice -> SetIndices( g_pIdxBufferSkyDome );

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureSkyDome );

	if( g_SkyDome.Use == true )
	{

		//	逆行列なしのワールド座標変換
		SetWorld( g_SkyDome.World.Pos , g_SkyDome.World.Rot , g_SkyDome.World.Scl );

		//	ポリゴンの描画
		pDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 0 , VERTEX , 0 , VERTEX - 2 );

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertex( LPDIRECT3DDEVICE9 pDevice , UINT NumBlock_X , UINT NumBlock_Y , float Radius )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
			UINT NumBlock_X				横の枚数
			UINT NumBlock_Y				縦の枚数
			float Radius				半径
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexSkyDome( LPDIRECT3DDEVICE9 pDevice , int NumBlock_X , int NumBlock_Y , float Radius )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_3D* pVtx;

	//	ポインタ変数
	WORD* pIdx;

	HRESULT hr;

	//	座標の格納変数
	D3DXVECTOR3 Pos;

	//	頂点数の計算
	int VerTex = ( NumBlock_X + 1 ) * ( NumBlock_Y + 1 );

	//	インデックスバッファの計算
	int IdxVerTex = ((2 + 2 * NumBlock_X) * NumBlock_Y+ ( NumBlock_Y - 1 ) * 2);

	//	360度の一つ分の角度求める
	float Rot_XZ = ( float )360 / NumBlock_X;

	//	180度の一つ分の角度求める( 半円の場合は90にする , 円の場合は180にする )
	float Rot_Y = ( float )90 / NumBlock_Y;

	//	一つのUV座標を求める
	float Uset = 1.0f / NumBlock_X;
	float Vset = 1.0f / NumBlock_Y;



	//	頂点バッファの生成
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * VerTex , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferSkyDome , NULL ) ) )
	{
		return E_FAIL;

	}	//	end of if


	//	インデックスバッファの生成
	hr = pDevice -> CreateIndexBuffer(
		sizeof( WORD ) * IdxVerTex ,	//	バッファ量
		D3DUSAGE_WRITEONLY ,	//	使用方法フラグ
		D3DFMT_INDEX16 , //	インデックスフォーマット
		D3DPOOL_MANAGED , //	メモリの管理方法
		&g_pIdxBufferSkyDome ,	//	インデックスバッファインターフェースへのポインタのアドレス
		NULL);

	if( FAILED( hr ) )
	{

		return E_FAIL;

	}	//	end of if

	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferSkyDome -> Lock( 0 , 0 , (void**)&pVtx , 0 );

	//	縦
	for( int Cnt1 = 0 ; Cnt1 < NumBlock_Y + 1 ; Cnt1++ )
	{
		//	角度の設定
		float θy = 90 - ( Cnt1 * Rot_Y );

		//	最初に90度から求める
		//	90度から引いていく
		Pos.y = Radius * sinf( D3DXToRadian( θy ) );

		//	横
		for( int Cnt2 = 0 ; Cnt2 < NumBlock_X + 1 ; Cnt2++ )
		{
			//	角度の設定
			float θxz = ( -1 * Cnt2 ) * Rot_XZ;

			//	XZ軸で360度求める
			Pos.x = Radius * cosf( D3DXToRadian( θy ) ) * cosf( D3DXToRadian( θxz ) );
			Pos.z = Radius * cosf( D3DXToRadian( θy ) ) * sinf( D3DXToRadian( θxz ) );


			//	頂点座標の設定
			pVtx[ 0 ].pos = D3DXVECTOR3( Pos.x , Pos.y , Pos.z );


			//	法線の設定
			pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );


			//	頂点色の設定
			pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );


			//	テクスチャ座標の設定
			pVtx[ 0 ].tex = D3DXVECTOR2( Cnt2 * Uset , Cnt1 * Vset );

			pVtx ++;	//	ポインタをずらす

		}	//	end of for

	}	//	end of for

	//	バッファのアンロック
	g_pVtxBufferSkyDome -> Unlock();


	//	インデックスバッファをロック
	g_pIdxBufferSkyDome -> Lock( 0 , 0 , (void**)&pIdx , 0 );

#if 1
	//	時計回りを裏面
	//	縦
	for( int Cnt1 = 0 ; Cnt1 < NumBlock_Y ; Cnt1++ )
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
		for( int Cnt2 = 0 ; Cnt2 < NumBlock_X ; Cnt2++ )
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
#endif

	//for(int h = 0; h < NumBlock_Y; h ++)
	//{
	//	if( h != 0 )
	//	{

	//		pIdx[ 0 ] = (h - 1) * ( NumBlock_X + 1 ) + NumBlock_X;

	//		pIdx ++;

	//		pIdx[ 0 ] = h * ( NumBlock_X + 1 ) + ( NumBlock_X + 1 );

	//		pIdx ++;
	//	}

	//	pIdx[ 0 ] = h * ( NumBlock_X + 1 ) + ( NumBlock_X + 1 );

	//	pIdx ++;


	//	pIdx[ 0 ] = h * ( NumBlock_X + 1 );

	//	pIdx ++;

	//	for(int w = 0; w < NumBlock_X; w ++)
	//	{
	//		pIdx[ 0 ] = h * ( NumBlock_X + 1 ) + (w + 1) + ( NumBlock_X + 1 );

	//		pIdx ++;

	//		pIdx[ 0 ] = h * ( NumBlock_X + 1 ) + (w + 1) ;

	//		pIdx ++;

	//	}

	//
	//}

	//	逆時計回りを裏面とする
	//	縦
	//for( int Cnt1 = 0 ; Cnt1 < NumBlock_Y ; Cnt1++ )
	//{
	//	//	はじめじゃなかったら
	//	//	縮退ポリゴン
	//	if( Cnt1 != 0 )
	//	{
	//		//	1点打つ

	//		pIdx[ 0 ] = ( Cnt1 + 1 ) * ( NumBlock_X + 1 ) - 1;

	//		pIdx++;	//	ポインタをずらす

	//	}	//	end of if

	//	//	2点打つ
	//	pIdx[ 1 ] = ( Cnt1 + 1 ) * ( NumBlock_X + 1 );
	//	pIdx[ 0 ] = ( Cnt1 + 1 ) * ( NumBlock_X + 1 ) - ( NumBlock_X + 1 );

	//	pIdx += 2;	//	ポインタをずらす

	//	//	横
	//	for( int Cnt2 = 0 ; Cnt2 < NumBlock_X ; Cnt2++ )
	//	{
	//		//	2点打つ
	//		pIdx[ 1 ] = ( ( Cnt1 + 1 ) * ( NumBlock_X + 1 ) + 1 ) + Cnt2;
	//		pIdx[ 0 ] = ( ( Cnt1 + 1 ) * ( NumBlock_X + 1 ) + 1 ) - ( NumBlock_X + 1 ) + Cnt2;

	//		pIdx += 2;	//	ポインタをずらす

	//	}	//	end of for

	//	//	最後じゃなかったら
	//	//	縮退ポリゴン
	//	if( Cnt1 != NumBlock_Y - 1 )
	//	{
	//		//	1点打つ
	//		pIdx[ 0 ] = ( Cnt1 + 1 ) * ( NumBlock_X + 1 );

	//		pIdx++;	//	ポインタをずらす

	//	}	//	end of if

	//}	//	end of for



	//	インデックスバッファのアンロック
	g_pIdxBufferSkyDome -> Unlock();

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetField( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Rot		回転量
 戻り値:	なし
 説明:		メッシュフィールドのセット
-----------------------------------------------------------------------------*/
void SetSkyDome( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot )
{

	if( g_SkyDome.Use == false )
	{
		//	座標
		g_SkyDome.World.Pos = Pos;

		//	回転量
		g_SkyDome.World.Rot = Rot;

		//	使用フラグ
		g_SkyDome.Use = true;


	}	//	end of if

}	//	end of func
