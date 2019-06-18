/*=============================================================================

		爆発[ explosin.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/12/13
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "explosion.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/Explosin.png"

#define MAX_EXPLOSION ( 100 )	//	爆発最大数

#define MAX_TEX ( 8 )	//	何枚のテクスチャがあるか
#define MAX_TEX_X ( 4 )	//	横のテクスチャが何枚あるか

#define TEX_ONE_WIDTH ( 0.25f )	//	一つのテクスチャの横幅
#define TEX_ONE_HEIGHT ( 0.5f )	//	一つのテクスチャの縦幅

#define TEX_WIDTH ( TEX_ONE_WIDTH * ( MAX_TEX_X - 1 ) )	//	テクスチャの横幅

#define TEX_SPEED ( 5 )	//	テクスチャの描画スピード( 数値が0になるにつれて描画が速くなる )

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	ワールド変換用変数
	D3DXVECTOR2 UVSet;	//	UV座標
	int Frame;	//	フレーム
	float Radius;	//	半径
	bool Use;	//	使用フラグ

}EXPLOSION;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexExplosin( LPDIRECT3DDEVICE9 pDevice );

//	爆発のテクスチャ変更
void VerTexExplosin( VERTEX_3D* pVtx , int Cnt );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferExplosin = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureExplosin = NULL;//	テクスチャインターフェース

EXPLOSION g_Explosion[ MAX_EXPLOSION ];		//	爆発構造体

/*-----------------------------------------------------------------------------
 関数名:	void InitExplosin( void )
 引数:		
 戻り値:	
 説明:		爆発の初期化
-----------------------------------------------------------------------------*/
void InitExplosin( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureExplosin  ) ) )
	{
		MessageBox( NULL , "[ explosin.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexExplosin( pDevice );


	//	構造体初期化
	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{
		//	座標
		g_Explosion[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	拡大率
		g_Explosion[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	回転量
		g_Explosion[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	フレーム
		g_Explosion[ Cnt ].Frame = 0;

		//	U座標
		g_Explosion[ Cnt ].UVSet.x = 0.0f;

		//	V座標
		g_Explosion[ Cnt ].UVSet.y = 0.0f;

		//	半径
		g_Explosion[ Cnt ].Radius = 0.0f;

		//	使用フラグ
		g_Explosion[ Cnt ].Use = false;

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitExplosin( void )
 引数:		
 戻り値:	
 説明:		爆発の終了
-----------------------------------------------------------------------------*/
void UninitExplosin( void )
{

	if( g_pVtxBufferExplosin != NULL )	//	頂点バッファインタフェース開放
	{
		g_pVtxBufferExplosin -> Release();
		g_pVtxBufferExplosin = NULL;

	}	//	end of if

	if( g_pTextureExplosin != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureExplosin -> Release();
		g_pTextureExplosin = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateExplosin( void )
 引数:		
 戻り値:	
 説明:		爆発の更新
-----------------------------------------------------------------------------*/
void UpdateExplosin( void )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_3D* pVtx;


	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferExplosin -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{
		if( g_Explosion[ Cnt ].Use == true )
		{
			
			//	フレームを増加
			g_Explosion[ Cnt ].Frame++;


			//	テクスチャアニメーション
			g_Explosion[ Cnt ].UVSet.x = ( ( ( g_Explosion[ Cnt ].Frame / TEX_SPEED ) % MAX_TEX_X ) * TEX_ONE_WIDTH );	//	上部テクスチャ
			g_Explosion[ Cnt ].UVSet.y = ( ( ( g_Explosion[ Cnt ].Frame / TEX_SPEED ) % MAX_TEX ) / MAX_TEX_X * TEX_ONE_HEIGHT );	//	下部テクスチャ


			//	テクスチャアニメーションが終わったら
			if( g_Explosion[ Cnt ].UVSet.x == TEX_WIDTH && g_Explosion[ Cnt ].UVSet.y == TEX_ONE_HEIGHT )
			{

				g_Explosion[ Cnt ].Use = false;

			}	//	end of if


			//	頂点の変更
			VerTexExplosin( pVtx , Cnt );

		}	//	end of if

	}	//	end of for


	//	バッファのアンロック
	g_pVtxBufferExplosin -> Unlock();


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawExplosin( void )
 引数:		
 戻り値:	
 説明:		弾の描画
-----------------------------------------------------------------------------*/
void DrawExplosin( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	パイプラインのストリーム
	pDevice -> SetStreamSource( 0 , g_pVtxBufferExplosin , 0 , sizeof( VERTEX_3D ));

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureExplosin );

	//	Zバッファ
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , FALSE );


	//	全体のライトを有効にする
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );



	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{

		if( g_Explosion[ Cnt ].Use == true )
		{

			//	逆行列ありのワールド座標変換
			SetWorldInv( g_Explosion[ Cnt ].World.Pos , g_Explosion[ Cnt ].World.Rot , g_Explosion[ Cnt ].World.Scl );


			//	ポリゴンの描画
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , Cnt * NUM_VERTEX , NUM_POLYGON );

		}	//	end of if

	}	//	end of for


	//	全体のライトを有効にする
	pDevice -> SetRenderState( D3DRS_LIGHTING , TRUE );

	//	元に戻す
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexExplosin( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexExplosin( LPDIRECT3DDEVICE9 pDevice )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_3D* pVtx;

	//	頂点バッファの生成
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * MAX_EXPLOSION , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferExplosin , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if


	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferExplosin -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{

		//	頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );


		//	法線の設定
		pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 1 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 2 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 3 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );

		//	頂点色の設定
		pVtx[ 0 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 1 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 2 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 3 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

		//	テクスチャ座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

		pVtx += 4;	//	pVtxをずらす

	}	//	end of for


	//	バッファのアンロック
	g_pVtxBufferExplosin -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void VerTexExplosin( VERTEX_3D* pVtx , int Cnt )
 引数:		VERTEX_3D* pVtx		仮想アドレスを取得するポインタ変数
			int Cnt				爆発構造体のカウンタ
 戻り値:	
 説明:		頂点の変更
-----------------------------------------------------------------------------*/
void VerTexExplosin( VERTEX_3D* pVtx , int Cnt )
{

	//	pVtxをずらす
	pVtx += Cnt * NUM_VERTEX;


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( -g_Explosion[ Cnt ].Radius , g_Explosion[ Cnt ].Radius , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3(  g_Explosion[ Cnt ].Radius , g_Explosion[ Cnt ].Radius , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( -g_Explosion[ Cnt ].Radius , -g_Explosion[ Cnt ].Radius , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3(  g_Explosion[ Cnt ].Radius , -g_Explosion[ Cnt ].Radius , 0.0f );


	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( g_Explosion[ Cnt ].UVSet.x                 , g_Explosion[ Cnt ].UVSet.y );
	pVtx[ 1 ].tex = D3DXVECTOR2( g_Explosion[ Cnt ].UVSet.x + TEX_ONE_WIDTH , g_Explosion[ Cnt ].UVSet.y );
	pVtx[ 2 ].tex = D3DXVECTOR2( g_Explosion[ Cnt ].UVSet.x                 , g_Explosion[ Cnt ].UVSet.y + TEX_ONE_HEIGHT );
	pVtx[ 3 ].tex = D3DXVECTOR2( g_Explosion[ Cnt ].UVSet.x + TEX_ONE_WIDTH , g_Explosion[ Cnt ].UVSet.y + TEX_ONE_HEIGHT );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetExplosin( D3DXVECTOR3 Pos , float Radius )
 引数:		D3DXVECTOR3 Pos		座標
			float Radius		半径
 戻り値:	
 説明:		爆発のセット
-----------------------------------------------------------------------------*/
void SetExplosin( D3DXVECTOR3 Pos , float Radius )
{

	for( int Cnt = 0 ; Cnt < MAX_EXPLOSION ; Cnt++ )
	{

		if( g_Explosion[ Cnt ].Use == false )
		{

			//	座標
			g_Explosion[ Cnt ].World.Pos = Pos;

			//	フレーム
			g_Explosion[ Cnt ].Frame = 0;

			//	U座標
			g_Explosion[ Cnt ].UVSet.x = 0.0f;

			//	V座標
			g_Explosion[ Cnt ].UVSet.y = 0.0f;

			//	半径
			g_Explosion[ Cnt ].Radius = Radius;

			//	使用フラグ
			g_Explosion[ Cnt ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func
