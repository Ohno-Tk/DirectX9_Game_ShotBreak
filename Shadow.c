/*=============================================================================

		影[ Shadow.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2016/11/09
-------------------------------------------------------------------------------
	■　Update
		2016/11/09
=============================================================================*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Shadow.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/shadow000.jpg"

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
HRESULT MakeVertexShadow( LPDIRECT3DDEVICE9 pDevice );

//	頂点の変更
void VerTexPos( VERTEX_3D* pVtx , int Cnt );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferShadow = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;//	テクスチャインターフェース

SHADOW g_Shadow[ MAX_SHADOW ];	//	影構造体

/*-----------------------------------------------------------------------------
 関数名:	void InitShadow( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitShadow( void )
{
	
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureShadow  ) ) )
	{

		MessageBox( NULL , "[ Shadow.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexShadow( pDevice );


	//	構造体初期化
	for( int Cnt = 0 ; Cnt < MAX_SHADOW ; Cnt++ )
	{
		//	座標
		g_Shadow[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	拡大率
		g_Shadow[ Cnt ].World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	回転量
		g_Shadow[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	カラー
		g_Shadow[ Cnt ].Color = D3DXCOLOR( 255 , 255 , 255 , 255 );

		//	使用フラグ
		g_Shadow[ Cnt ].Use = false;

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitShadow( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitShadow( void )
{

	if(g_pVtxBufferShadow != NULL)	//頂点バッファのインターフェースポインタの解放
	{

		g_pVtxBufferShadow -> Release();
		g_pVtxBufferShadow = NULL;

	}	//	end of if

	if( g_pTextureShadow != NULL )	//	テクスチャポリゴン開放
	{

		g_pTextureShadow -> Release();
		g_pTextureShadow = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdataShadow( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateShadow( void )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_3D* pVtx;


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferShadow -> Lock ( 0 , 0 ,( void** )&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_SHADOW ; Cnt++ )
	{

		//	頂点の変更
		VerTexPos( pVtx , Cnt );


	}	//	end of for


	//	バッファのアンロック
	g_pVtxBufferShadow -> Unlock();

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawShadow( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawShadow( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferShadow , 0 , sizeof( VERTEX_3D ) );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureShadow );


	//	減算合成
	pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_REVSUBTRACT );
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );
	

	for( int Cnt = 0 ; Cnt < MAX_SHADOW ; Cnt++ )
	{

		if( g_Shadow[ Cnt ].Use == true )
		{

			//	逆行列なしのワールド座標変換
			SetWorld( g_Shadow[ Cnt ].World.Pos , g_Shadow[ Cnt ].World.Rot , g_Shadow[ Cnt ].World.Scl );


			//	ポリゴンの描画
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , Cnt * NUM_VERTEX, NUM_POLYGON);

		}	//	end of if

	}	//	end of for


	//	元に戻す
	pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexShadow( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexShadow( LPDIRECT3DDEVICE9 pDevice )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_3D* pVtx;

	//	頂点バッファの生成
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * MAX_SHADOW , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferShadow , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferShadow -> Lock( 0 , 0 ,( void** )&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_SHADOW ; Cnt++ )
	{
		//	頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	法線の設定
		pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
		pVtx[ 1 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
		pVtx[ 2 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
		pVtx[ 3 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );

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
	g_pVtxBufferShadow -> Unlock();

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTexPos( VERTEX_3D* pVtx , int Cnt )
 引数:		VERTEX_3D* pVtx		仮想アドレスを取得するポインタ変数
			int Cnt				影構造体のカウンタ
 戻り値:	
 説明:		頂点の変更
-----------------------------------------------------------------------------*/
void VerTexPos( VERTEX_3D* pVtx , int Cnt )
{

	//	pVtxをCnt分ずらす
	pVtx += Cnt * NUM_VERTEX;


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3(  g_Shadow[ Cnt ].Radius , 0.0f , -g_Shadow[ Cnt ].Radius );
	pVtx[ 1 ].pos = D3DXVECTOR3( -g_Shadow[ Cnt ].Radius , 0.0f , -g_Shadow[ Cnt ].Radius );
	pVtx[ 2 ].pos = D3DXVECTOR3(  g_Shadow[ Cnt ].Radius , 0.0f ,  g_Shadow[ Cnt ].Radius );
	pVtx[ 3 ].pos = D3DXVECTOR3( -g_Shadow[ Cnt ].Radius , 0.0f ,  g_Shadow[ Cnt ].Radius );


	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_Shadow[ Cnt ].Color.r , g_Shadow[ Cnt ].Color.g , g_Shadow[ Cnt ].Color.b , g_Shadow[ Cnt ].Color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Shadow[ Cnt ].Color.r , g_Shadow[ Cnt ].Color.g , g_Shadow[ Cnt ].Color.b , g_Shadow[ Cnt ].Color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Shadow[ Cnt ].Color.r , g_Shadow[ Cnt ].Color.g , g_Shadow[ Cnt ].Color.b , g_Shadow[ Cnt ].Color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Shadow[ Cnt ].Color.r , g_Shadow[ Cnt ].Color.g , g_Shadow[ Cnt ].Color.b , g_Shadow[ Cnt ].Color.a );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetShadow( D3DXVECTOR3 Pos , float Radius , D3DXCOLOR Color )
 引数:		D3DXVECTOR3 Pos		座標
			float Radius		半径
			D3DXCOLOR Color		色
 戻り値:	
 説明:		影のセット
-----------------------------------------------------------------------------*/
void SetShadow( D3DXVECTOR3 Pos , float Radius , D3DXCOLOR Color )
{

	for( int Cnt = 0 ; Cnt < MAX_SHADOW ; Cnt++ )
	{

		if( g_Shadow[ Cnt ].Use == false )
		{
			
			//	座標
			g_Shadow[ Cnt ].World.Pos.x = Pos.x;
			g_Shadow[ Cnt ].World.Pos.z = Pos.z;

			//	色
			g_Shadow[ Cnt ].Color.r = Color.r;
			g_Shadow[ Cnt ].Color.g = Color.g;
			g_Shadow[ Cnt ].Color.b = Color.b;
			g_Shadow[ Cnt ].Color.a = Color.a;

			//	半径
			g_Shadow[ Cnt ].Radius = Radius;

			//	使用フラグ
			g_Shadow[ Cnt ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void IndexShadow( int Index )
 引数:		int Index
 戻り値:	
 説明:		特定の影の使用フラグをOFFにする
-----------------------------------------------------------------------------*/
void IndexShadow( int Index )
{

	g_Shadow[ Index ].Use = false;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	SHADOW *GetShadow( int Index )
 引数:		int Index
 戻り値:	
 説明:		影の情報取得
-----------------------------------------------------------------------------*/
SHADOW *GetShadow( int Index )
{

	return &g_Shadow[ Index ];

}	//	end of func
