/*=============================================================================

		ライト[ light.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2016/10/19
-------------------------------------------------------------------------------
	■　Update
		2016/10/19
=============================================================================*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "light.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 関数名:	void InitLight( void )
 引数:		なし
 戻り値:	なし
 説明:		ライトの初期化
-----------------------------------------------------------------------------*/
void InitLight( void )
{
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitLight( void )
 引数:		なし
 戻り値:	なし
 説明:		ライトの終了
-----------------------------------------------------------------------------*/
void UninitLight( void )
{
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdataLight( void )
 引数:		なし
 戻り値:	なし
 説明:		ライトの更新
-----------------------------------------------------------------------------*/
void UpdateLight( void )
{
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawLight( void )
 引数:		なし
 戻り値:	なし
 説明:		ライトの描画
-----------------------------------------------------------------------------*/
void DrawLight( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	ライトの作成
	D3DLIGHT9 light;

	//	全て0にする
	ZeroMemory( &light , sizeof( D3DLIGHT9 ) );

	light.Type = D3DLIGHT_DIRECTIONAL;

	//	ディレクショナルライトの色
	light.Diffuse = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	//	アンビエントライトの色
	light.Ambient = D3DXCOLOR( 0.5f , 0.0f , 0.5f , 1.0f );

	//	ディレクショナルライトの向き
	D3DXVECTOR3 vecDir( 0.0f , -5.0f , 1.0f );

	//	単位ベクトルにしてlight.Directionに計算結果を代入
	D3DXVec3Normalize( ( D3DXVECTOR3 * )&light.Direction , &vecDir );

	//	デバイスにライトを設定
	pDevice -> SetLight( 0 , &light );

	//	ライトをONにする
	pDevice -> LightEnable( 0 , TRUE );

	//	全体のライトを有効にする
	pDevice -> SetRenderState( D3DRS_LIGHTING , TRUE );


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetLight( void )
 引数:		なし
 戻り値:	なし
 説明:		ライトのセット
-----------------------------------------------------------------------------*/
void SetLight( void )
{
}	//	end of func
