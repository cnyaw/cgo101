
// waync 080913

#ifndef CARDS_HEADER

#include "windows.h"
#pragma comment(lib, "Cards")

extern "C" {

/*!
 * \brief 初始化Cards.dll.
 * \param[in] width 撲克牌寬度.
 * \param[in] height 撲克牌高度.
 * \return 成功回傳true.
 */
bool WINAPI cdtInit( int* width, int* height );

/*!
 * \brief 釋放Cards.dll內部配置.
 */
void WINAPI cdtTerm();

/*!
 * \brief 繪製撲克牌.
 * \param[in] hdc dc handle
 * \param[in] x 繪製目的x座標.
 * \param[in] y 繪製目的y座標.
 * \param[in] card 撲克牌編號.
 * \param[in] mode 0:繪製撲克牌, 1:繪製牌底.
 * \param[in] color ??
 * \note 如果 mode 是 0, card = (color + 4 * number), color(0..3), number(0..12)
 * \note 如果 mode 是 1, card = 54..65, 67..68
 */
bool WINAPI cdtDraw( HDC hdc, int x, int y, int card, int mode, long color );

bool WINAPI cdtDrawExt( HDC hdc, int x, int y, int dx, int dy, int card, int type, long color );

bool WINAPI cdtAnimate( HDC hdc, int cardback, int x, int y, int frame );

}

#endif
