
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/10/05 Waync.
//

#pragma once

namespace cgo {

//
// King's Way.
//

class GameKingsWay : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_033_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_033_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame2();

    for (int i = 0; i < 8; ++i) {
      mList[0].push_back(GetNewCard(13));
    }

    for (int i = 1; i <= 8; ++i) {
      for (int j = 0; j < 5; ++j) {
        mList[i].push_back(GetNewCard() + (4 == j ? 0 : 100));
      }
    }

    mList[10].push_back(GetNewCard());
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (mCards.empty()) {
      return false;
    }

    mList[10][0] = GetNewCard();
    return true;
  } // NewCard

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    RECT_t rcDeck;

    if (mCards.empty()) {
      g.DrawEmptyCard(4, 4);
    } else {
      g.DrawCover(4, 4);
    }

    g.DrawCard(4, 4 + OFFSETH, mList[10].back());

    for (int i = 0; i < 8; ++i) {
      GetSolitaireRect(i, rcDeck);
      g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
    }

    for (int i = 1; i <= 8; ++i) {

      GetDeckRect(i, rcDeck);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {

        if (i == mSel && mList[i].size() - 1 == j) {
          break;
        }

        if (100 <= mList[i][j]) {
          g.DrawCover(rcDeck.left, rcDeck.top + 22 * j);
        } else {
          g.DrawCard(rcDeck.left, rcDeck.top + 22 * j, mList[i][j]);
        }
      }
    }

    if (-1 != mSel) {
      g.DrawCard(mPtCurr.x, mPtCurr.y, mList[mSel].back());
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    RECT_t rcDealNewCard(4, 4, 4 + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcDealNewCard.ptInRect(point)) {
      return NewCard();
    }

    RECT_t rcDeck;
    for (int i = 1; i <= 8; ++i) {

      GetDeckRect(i, rcDeck);
      rcDeck.offset(0,  22 * (mList[i].size() - 1));

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        break;
      }

      if (100 <= mList[i].back()) {
        mList[i][mList[i].size() - 1] -= 100;
        return true;
      }

      mSel = i;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - rcDeck.top;

      return false;
    }

    return false;
  } // OnMouseDown

  virtual bool OnMouseUp(POINT_t point, RECT_t const& rcClient)
  {
    if (-1 == mSel) {
      return false;
    }

    int card1 = mList[mSel].back();
    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    RECT_t rc1(4, 4 + OFFSETH, 4 + CARD_WIDTH, 4 + OFFSETH + CARD_HEIGHT);
    if (rc1.ptInRect(point)) {

      int card2 = mList[10].back();
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 == color2) {
        goto end;
      }

      if (number1 + 1 != number2 && number1 != number2 + 1) {
        goto end;
      }

      mList[10][0] = mList[mSel].back();
      mList[mSel].pop_back();

      goto end;
    }

end:
    mSel = -1;
    return true;
  } // OnMouseUp

  //
  // Helper.
  //

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * iDeck, 4 + OFFSETH);
  } // GetDeckRect

  void GetSolitaireRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (1 + iDeck), 4);
  } // GetSolitaireRect
};

} // namespace cgo

// end of 033_KingsWay.h
