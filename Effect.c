/*=============================================================================

		エフェクト[ Effect.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2017/01/22
-------------------------------------------------------------------------------
	■　Update
=============================================================================*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Effect.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/effect000.jpg"

#define MAX_EFFECT ( 100 )	//	エフェクトの最大数

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	ワールド変換用変数
	D3DXCOLOR Color;	//	色
	int Life;	//	寿命
	float Radius;	//	半径
	float RadiusValue;	//	半径の変化量
	bool Use;	//	使用フラグ

}EFFECT;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice );

//	頂点の変更
void VerTex( VERTEX_3D* pVtx , int Cnt );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEffect = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//	テクスチャインターフェース

EFFECT g_Effect[ MAX_EFFECT ];	//	エフェクト構造体

/*-----------------------------------------------------------------------------
 関数名:	void InitEffect( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitEffect( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureEffect  ) ) )
	{
		MessageBox( NULL , "[ Effect.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexEffect( pDevice );


	//	構造体初期化
	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
	{

		//	座標
		g_Effect[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	回転量
		g_Effect[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	拡大率
		g_Effect[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	色
		g_Effect[ Cnt ].Color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f );

		//	半径
		g_Effect[ Cnt ].Radius = 0.0f;

		//	半径の変化量
		g_Effect[ Cnt ].RadiusValue = 0.0f;

		//	寿命
		g_Effect[ Cnt ].Life = 0;

		//	使用フラグ
		g_Effect[ Cnt ].Use = false;

	}	//	end of for


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitEffect( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitEffect( void )
{
	if( g_pVtxBufferEffect != NULL )	//	頂点バッファインタフェース開放
	{
		g_pVtxBufferEffect -> Release();
		g_pVtxBufferEffect = NULL;

	}	//	end of if

	if( g_pTextureEffect != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureEffect -> Release();
		g_pTextureEffect = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateEffect( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateEffect( void )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_3D* pVtx;


	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferEffect -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
	{

		if( g_Effect[ Cnt ].Use == true )
		{

			//	半径を減らす
			g_Effect[ Cnt ].Radius -= g_Effect[ Cnt ].RadiusValue;

			//	寿命を減らす
			g_Effect[ Cnt ].Life--;

			if( g_Effect[ Cnt ].Life < 0 )
			{

				g_Effect[ Cnt ].Use = false;

			}	//	end of if

			//	頂点の変更
			VerTex( pVtx , Cnt );

		}	//	end of if

	}	//	end of for


	//	バッファのアンロック
	g_pVtxBufferEffect -> Unlock();

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawEffect( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawEffect( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	パイプラインのストリーム
	pDevice -> SetStreamSource( 0 , g_pVtxBufferEffect , 0 , sizeof( VERTEX_3D ));

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureEffect );

	//	Zバッファ
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , FALSE );


	//	全体のライトを有効にする
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );


	//	aブレンド（加算合成）
	pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );	//	Initに書いてあるので省略可
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );	//	Initに書いてあるので省略可
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );



	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
	{

		if( g_Effect[ Cnt ].Use == true )
		{

			//	逆行列ありのワールド座標変換
			SetWorldInv( g_Effect[ Cnt ].World.Pos , g_Effect[ Cnt ].World.Rot , g_Effect[ Cnt ].World.Scl );

			//	ポリゴンの描画
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , Cnt * NUM_VERTEX , NUM_POLYGON );

		}	//	end of if

	}	//	end of for


	//	元に戻す
	pDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );


	//	全体のライトを有効にする
	pDevice -> SetRenderState( D3DRS_LIGHTING , TRUE );

	//	元に戻す
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_3D* pVtx;

	//	頂点バッファの生成
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * MAX_EFFECT , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferEffect , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if


	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferEffect -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
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
	g_pVtxBufferEffect -> Unlock();

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTex( VERTEX_3D* pVtx , int Cnt )
 引数:		VERTEX_3D* pVtx		仮想アドレスを取得するポインタ変数
			int Cnt				エフェクト構造体のカウンタ
 戻り値:	
 説明:		頂点の変更
-----------------------------------------------------------------------------*/
void VerTex( VERTEX_3D* pVtx , int Cnt )
{

	//	pVtxをずらす
	pVtx += Cnt * NUM_VERTEX;


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( -g_Effect[ Cnt ].Radius , g_Effect[ Cnt ].Radius , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3(  g_Effect[ Cnt ].Radius , g_Effect[ Cnt ].Radius , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( -g_Effect[ Cnt ].Radius , -g_Effect[ Cnt ].Radius , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3(  g_Effect[ Cnt ].Radius , -g_Effect[ Cnt ].Radius , 0.0f );


	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_Effect[ Cnt ].Color.r , g_Effect[ Cnt ].Color.g , g_Effect[ Cnt ].Color.b , g_Effect[ Cnt ].Color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Effect[ Cnt ].Color.r , g_Effect[ Cnt ].Color.g , g_Effect[ Cnt ].Color.b , g_Effect[ Cnt ].Color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Effect[ Cnt ].Color.r , g_Effect[ Cnt ].Color.g , g_Effect[ Cnt ].Color.b , g_Effect[ Cnt ].Color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Effect[ Cnt ].Color.r , g_Effect[ Cnt ].Color.g , g_Effect[ Cnt ].Color.b , g_Effect[ Cnt ].Color.a );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetEffect( D3DXVECTOR3 Pos , D3DXCOLOR Color , int Life , float Radius )
 引数:		D3DXVECTOR3 Pos			座標
			D3DXVECTOR2 VerTexPos	頂点座標
			int Life				寿命
			float Radius			半径
 戻り値:	
 説明:		エフェクトのセット 
-----------------------------------------------------------------------------*/
void SetEffect( D3DXVECTOR3 Pos , D3DXCOLOR Color , int Life , float Radius )
{

	for( int Cnt = 0 ; Cnt < MAX_EFFECT ; Cnt++ )
	{

		if( g_Effect[ Cnt ].Use == false )
		{

			//	座標
			g_Effect[ Cnt ].World.Pos = Pos;

			//	色
			g_Effect[ Cnt ].Color = Color;

			//	寿命
			g_Effect[ Cnt ].Life = Life;

			//	半径
			g_Effect[ Cnt ].Radius = Radius;

			//	半径の変化量
			g_Effect[ Cnt ].RadiusValue = g_Effect[ Cnt ].Radius / g_Effect[ Cnt ].Life;

			//	使用フラグ
			g_Effect[ Cnt ].Use = true;


			break;

		}	//	end of if
	}	//	end of for

}	//	end of func