/*=============================================================================

		ゲーム[ game.h ]

-------------------------------------------------------------------------------

	■　作成日
		2016/12/15
-------------------------------------------------------------------------------
	■　Update
=============================================================================*/

/*-----------------------------------------------------------------------------
	二重インクルード防止
-----------------------------------------------------------------------------*/
#ifndef _GAME_H_
#define _GAME_H_

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
void InitGame( void );		//	ゲームの初期化
void UninitGame( void );	//	ゲームの終了
void UpdateGame( void );	//	ゲームの更新
void DrawGame( void );		//	ゲームの描画

#endif
