/*=============================================================================

		スカイドーム[ skydome.h ]

-------------------------------------------------------------------------------

	■　作成日
		2016/12/14
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	二重インクルード防止
-----------------------------------------------------------------------------*/
#ifndef _SKYDOME_H_
#define _SKYDOME_H_

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
void InitSkydome( void );		//	スカイドームの初期化
void UninitSkydome( void );		//	スカイドームの終了
void UpdateSkydome( void );		//	スカイドームの更新
void DrawSkydome( void );		//	スカイドームの描画

#endif
