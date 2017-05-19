
// waync 080913

#ifndef CARDS_HEADER

#include "windows.h"
#pragma comment(lib, "Cards")

extern "C" {

/*!
 * \brief ��l��Cards.dll.
 * \param[in] width ���J�P�e��.
 * \param[in] height ���J�P����.
 * \return ���\�^��true.
 */
bool WINAPI cdtInit( int* width, int* height );

/*!
 * \brief ����Cards.dll�����t�m.
 */
void WINAPI cdtTerm();

/*!
 * \brief ø�s���J�P.
 * \param[in] hdc dc handle
 * \param[in] x ø�s�ت�x�y��.
 * \param[in] y ø�s�ت�y�y��.
 * \param[in] card ���J�P�s��.
 * \param[in] mode 0:ø�s���J�P, 1:ø�s�P��.
 * \param[in] color ??
 * \note �p�G mode �O 0, card = (color + 4 * number), color(0..3), number(0..12)
 * \note �p�G mode �O 1, card = 54..65, 67..68
 */
bool WINAPI cdtDraw( HDC hdc, int x, int y, int card, int mode, long color );

bool WINAPI cdtDrawExt( HDC hdc, int x, int y, int dx, int dy, int card, int type, long color );

bool WINAPI cdtAnimate( HDC hdc, int cardback, int x, int y, int frame );

}

#endif
