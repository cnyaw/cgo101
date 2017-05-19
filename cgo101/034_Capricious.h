
//
// Copyright (c) 2012 Waync Cheng.
// All Rights Reserved.
//
// 2012/03/20 Waync.
//

#pragma once

namespace cgo {

//
// Capricious.
//

class GameCapricious : public CardGame
{
public:

  int mRetry;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_034_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_034_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame2();

    for (int i = 0; i < 4; ++i) {
      mList[0].push_back(GetNewCard(i, 1));
    }

    for (int i = 0; i < 4; ++i) {
      mList[0].push_back(GetNewCard(i, 13));
    }

    mRetry = 2;

    NewCard();
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (0 == mRetry) {
      return false;
    }

    mRetry--;

    for (int i = 12; i >= 1; i--) {
      if (!mList[i].empty()) {
        mCards.insert(mCards.end(), mList[i].begin(), mList[i].end());
        mList[i].clear();
      }
    }

    while (true) {

      if (mCards.empty()) {
        break;
      }

      for (int i = 1; i <= 12; i++) {

        if (mCards.empty()) {
          break;
        }

        int card = GetNewCard();
        for (int j = 0; j < 8; j++) {
          if (CheckSolitai(card, j)) {
            mList[0][j] = card;
            card = -1;
            break;
          }
        }

        if (-1 != card) {
          mList[i].push_back(card);
        }
      }
    }

    return true;
  } // NewCard

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    //
    // Draw deal new card deck.
    //

    if (mRetry) {
      g.DrawCover(4 + CARD_WIDTH, 4 + 2 * CARD_HEIGHT);
    } else {
      g.DrawEmptyCard(4 + CARD_WIDTH, 4 + 2 * CARD_HEIGHT);
    }

    //
    // Draw Solitaire.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 8; ++i) {
      GetSolitaireRect(i, rcDeck);
      int card = mList[0][i];
      int num = GetNumber(card);
      if ((1 == num && i >= 4) || (13 == num && i < 4)) {
        g.DrawCover(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, card);
      }
    }

    //
    // Draw deck.
    //

    for (int i = 1; i <= 12; ++i) {

      GetDeckRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (i == mSel) {
        if (1 < mList[i].size()) {
          g.DrawCard(rcDeck.left, rcDeck.top, mList[i][mList[i].size() - 2]);
        }
        continue;
      }

      if (!mList[i].empty()) {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[i].back());
      }
    }

    //
    // Draw sel.
    //

    if (-1 != mSel) {
      int card = mList[mSel].back();
      g.DrawCard(mPtCurr.x, mPtCurr.y, card);
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    RECT_t rc(0, 0, CARD_WIDTH, CARD_HEIGHT);
    rc.offset(4 + CARD_WIDTH, 4 + 2 * CARD_HEIGHT);

    //
    // Deal again?
    //

    if (rc.ptInRect(point)) {
      return NewCard();
    }

    //
    // Select deck?
    //

    RECT_t rcDeck;
    for (int i = 1; i <= 12; ++i) {

      GetDeckRect(i - 1, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
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

    int card = mList[mSel].back();

    //
    // On solitai?
    //

    RECT_t rcDeck;
    for (int i = 0; i < 8; ++i) {

      GetSolitaireRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (CheckSolitai(card, i)) {
        mList[0][i] = card;
        mList[mSel].pop_back();
      }

      goto end;
    }

    //
    // On deck?
    //

    for (int i = 1; i <= 12; ++i) {

      GetDeckRect(i - 1, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSel == i) {
        goto end;
      }

      int color = GetColor(card);
      int num = GetNumber(card);

      if (mList[i].empty()) {
putit:
        mList[i].push_back(card);
        mList[mSel].pop_back();
        goto end;
      }

      int card0 = mList[i].back();
      int color0 = GetColor(card0);
      int num0 = GetNumber(card0);

      if (color0 != color) {
        goto end;
      }

      if ((1 != num0 && num0 - 1 == num) ||
          (13 != num0 && num0 + 1 == num)) {
        goto putit;
      }

      goto end;
    }

end:
    mSel = -1;
    return true;
  } // OnMouseUp

  //
  // Helper.
  //

  bool CheckSolitai(int card, int index)
  {
    int color = GetColor(card);
    int num = GetNumber(card);

    if (0 <= index && 4 > index) {
      int card0 = mList[0][index];
      if (GetColor(card0) == color && GetNumber(card0) + 1 == num) {
        return true;
      }
    }

    if (4 <= index && 8 > index) {
      int card0 = mList[0][index];
      if (GetColor(card0) == color && GetNumber(card0) - 1 == num) {
        return true;
      }
    }

    return false;
  } // CheckSolitai

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    int r = iDeck / 4;
    int c = iDeck % 4;

    rcDeck.left = 4 + OFFSETW * (3 + c);
    rcDeck.top = 4 + OFFSETH * (1 + r);
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect

  void GetSolitaireRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (1 + iDeck);
    rcDeck.top = 4;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetSolitaireRect
};

} // namespace cgo

// end of 034_Capricious.h
