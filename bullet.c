/*=============================================================================

		弾[ bullet.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/11/24
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <math.h>
#include "main.h"
#include "input.h"
#include "sound.h"
#include "bullet.h"
#include "Utility.h"
#include "explosion.h"
#include "MeshWall.h"
#include "Enemy.h"
#include "EnemyFly.h"
#include "Score.h"
#include "Shadow.h"
#include "Effect.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/bullet000.png"

#define MAX_BULLET ( 100 )	//	弾最大数

#define POLYGON_X ( 5.0f )	//	ポリゴンのＸ
#define POLYGON_Y ( 5.0f )	//	ポリゴンのＹ

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	ワールド変換用変数
	D3DXVECTOR3 Move;	//	移動量
	int LIfe;			//	寿命
	float length;	//	長さ
	float Radius;	//	半径
	bool Use;	//	使用フラグ

}BULLET;

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexBullet( LPDIRECT3DDEVICE9 pDevice );

//	当たり判定
void Collision( int CntBullet );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;//	テクスチャインターフェース

BULLET g_Bullet[ MAX_BULLET ];	//	弾の構造体

/*-----------------------------------------------------------------------------
 関数名:	void InitBullet( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitBullet( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureBullet  ) ) )
	{
		MessageBox( NULL , "[ bullet.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexBullet( pDevice );


	//	構造体初期化
	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{
		//	座標
		g_Bullet[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	拡大率
		g_Bullet[ Cnt ].World.Scl =  D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	回転量
		g_Bullet[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	移動量
		g_Bullet[ Cnt ].Move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	寿命
		g_Bullet[ Cnt ].LIfe = 0;

		//	長さ
		g_Bullet[ Cnt ].length = 1.0f;

		//	半径
		g_Bullet[ Cnt ].Radius = 30.0f;

		//	使用フラグ
		g_Bullet[ Cnt ].Use = false;

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitBullet( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitBullet( void )
{

	if( g_pVtxBufferBullet != NULL )	//	頂点バッファインタフェース開放
	{
		g_pVtxBufferBullet -> Release();
		g_pVtxBufferBullet = NULL;

	}	//	end of if

	if( g_pTextureBullet != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureBullet -> Release();
		g_pTextureBullet = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateBullet( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateBullet( void )
{

	//	影の情報取得
	SHADOW *Shadow = GetShadow( 1 );


	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{

		if( g_Bullet[ Cnt ].Use == true )
		{			

			//	座標移動
			g_Bullet[ Cnt ].World.Pos += g_Bullet[ Cnt ].Move;

			//	ターゲットとの当たり判定
			Collision( Cnt );

			//	エフェクトのセット
			SetEffect( g_Bullet[ Cnt ].World.Pos , D3DXCOLOR( 255 , 0 , 0 , 255 ) , 5 , 15.0f );


		}	//	end of if

	}	//	end of for


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawBullet( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawBullet( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	パイプラインのストリーム
	pDevice -> SetStreamSource( 0 , g_pVtxBufferBullet , 0 , sizeof( VERTEX_3D ));

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureBullet );

	//	Zバッファ
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , FALSE );


	//	全体のライトを有効にする
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );



	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{

		if( g_Bullet[ Cnt ].Use == true )
		{

			//	逆行列ありのワールド座標変換
			SetWorldInv( g_Bullet[ Cnt ].World.Pos , g_Bullet[ Cnt ].World.Rot , g_Bullet[ Cnt ].World.Scl );

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
 関数名:	HRESULT MakeVertexBullet( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBullet( LPDIRECT3DDEVICE9 pDevice )
{

	VERTEX_3D* pVtx;	//	仮想アドレスを取得するポインタ変数


	//	頂点バッファの生成
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * MAX_BULLET , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferBullet , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if


	//	バッファをロックし仮想アドレスを取得する
	g_pVtxBufferBullet -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{

		//	頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( -POLYGON_X , POLYGON_Y  , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( POLYGON_X  , POLYGON_Y  , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( -POLYGON_X , -POLYGON_Y  , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( POLYGON_X  , -POLYGON_Y  , 0.0f );


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
	g_pVtxBufferBullet -> Unlock();

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetBullet( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , float Length )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Rot		回転量
			float Length		長さ
 戻り値:	
 説明:		弾のセット
-----------------------------------------------------------------------------*/
void SetBullet( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , float Length )
{

	float rot = Rot.y + 180;

	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{

		if( g_Bullet[ Cnt ].Use == false )
		{

			//	座標
			//g_Bullet[ CntBullet ].World.Pos.x = cosf( D3DXToRadian( Rot.x ) ) * sinf( D3DXToRadian( rot ) ) * Length + Pos.x;
			//g_Bullet[ CntBullet ].World.Pos.y = sinf( D3DXToRadian( Rot.x ) ) * Length + Pos.y;
			//g_Bullet[ CntBullet ].World.Pos.z = cosf( D3DXToRadian( Rot.x ) ) * cosf( D3DXToRadian( rot ) ) * Length + Pos.z;

			g_Bullet[ Cnt ].World.Pos.x = Pos.x;
			g_Bullet[ Cnt ].World.Pos.y = Pos.y;
			g_Bullet[ Cnt ].World.Pos.z = Pos.z;


			//	移動量
			g_Bullet[ Cnt ].Move.x = cosf( D3DXToRadian( Rot.x ) ) * sinf( D3DXToRadian( rot ) ) * Length;
			g_Bullet[ Cnt ].Move.y = sinf( D3DXToRadian( Rot.x ) ) * Length;
			g_Bullet[ Cnt ].Move.z = cosf( D3DXToRadian( Rot.x ) ) * cosf( D3DXToRadian( rot ) ) * Length;

			//	寿命
			g_Bullet[ Cnt ].LIfe = 100;

			//	使用フラグ
			g_Bullet[ Cnt ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void Collision( int Cnt )
 引数:		int Cnt	弾構造体のカウンタ
 戻り値:	
 説明:		当たり判定
-----------------------------------------------------------------------------*/
void Collision( int Cnt )
{

	//	スコア情報の取得
	int *Score = GetScore();

	//	ターゲット情報の取得
	ENEMY *Enemy = GetEnemyNor();
	ENEMY *EnemyFly = GetEnemyFly();

	//	メッシュウォール情報取得
	MESHWALL *MeshWall = GetMeshWall();


	for( int CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{

		if( Enemy -> Use == true )
		{

			//	球の当たり判定
			if( SphereCollision( g_Bullet[ Cnt ].World.Pos , D3DXVECTOR3( Enemy -> World.Pos.x , Enemy -> World.Pos.y + 20.0f , Enemy -> World.Pos.z ) , g_Bullet[ Cnt ].Radius , Enemy -> Radius ) )
			{

				//	敵の使用フラグをOFFにする
				Enemy -> Use = false;

				//	弾の使用フラグをOFFにする
				g_Bullet[ Cnt ].Use = false;

				//	ターゲットの影を消す（ +1はプレイヤーから先に作っているから ）
				IndexShadow( CntEnemy + 1 );


				//	スコアの加算
				*Score += 50;

				//	爆発のセット
				SetExplosin( D3DXVECTOR3( Enemy -> World.Pos.x , Enemy -> World.Pos.y + 20.0f , Enemy -> World.Pos.z ) , 30.0f );

				//	音楽再生
				PlaySound( SOUND_LABEL_SE_HIT );

			}	//	end of if

		}	//	end of if
				
	}	//	end of for

	for( int CntEnemyFly = 0 ; CntEnemyFly < MAX_ENEMY_FLY ; CntEnemyFly++ , EnemyFly++ )
	{

		if( EnemyFly -> Use == true )
		{

			//	球の当たり判定
			if( SphereCollision( g_Bullet[ Cnt ].World.Pos , D3DXVECTOR3( EnemyFly -> World.Pos.x , EnemyFly -> World.Pos.y + 20.0f , EnemyFly -> World.Pos.z ) , g_Bullet[ Cnt ].Radius , EnemyFly -> Radius ) )
			{

				//	敵の使用フラグをOFFにする
				EnemyFly -> Use = false;

				//	弾の使用フラグをOFFにする
				g_Bullet[ Cnt ].Use = false;

				//	ターゲットの影を消す（ +1はプレイヤーから先に作っているから ）
				IndexShadow( CntEnemyFly + 11 );


				//	スコアの加算
				*Score += 100;

				//	爆発のセット
				SetExplosin( D3DXVECTOR3( EnemyFly -> World.Pos.x , EnemyFly -> World.Pos.y + 20.0f , EnemyFly -> World.Pos.z ) , 30.0f );

				//	音楽再生
				PlaySound( SOUND_LABEL_SE_HIT );

			}

		}

	}	//	end of for

	//	メッシュウォールとの当たり判定

	//	Ｚ軸判定
	if( g_Bullet[ Cnt ].World.Pos.z + 20.0f > MeshWall[ 0 ].World.Pos.z || g_Bullet[ Cnt ].World.Pos.z - 20.0f < MeshWall[ 1 ].World.Pos.z )
	{

		g_Bullet[ Cnt ].Use = false;

	}	//	end of if

	//	X軸判定
	if( g_Bullet[ Cnt ].World.Pos.x + 20.0f > MeshWall[ 2 ].World.Pos.x || g_Bullet[ Cnt ].World.Pos.x - 20.0f < MeshWall[ 3 ].World.Pos.x )
	{

		g_Bullet[ Cnt ].Use = false;

	}	//	end of if

}

/*-----------------------------------------------------------------------------
 関数名:	BULLET *GetBullet( void )
 引数:		
 戻り値:	return &g_Bullet[ 0 ];
 説明:		弾情報取得
-----------------------------------------------------------------------------*/
BULLET *GetBullet( void )
{
	return &g_Bullet[ 0 ];

}	//	end of func