
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/09/28 Waync.
//

#pragma once

namespace cgo {

//
// The Carpet.
//

class GameTheCarpet : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_028_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_028_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 0; i < 4; ++i) {
      mList[0].push_back(GetNewCard(1));
    }

    for (int i = 0; i < 20; ++i) {
      mList[3].push_back(GetNewCard());
    }

  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    return false;
  } // NewCard

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    RECT_t rcDeck;

    if (1 != mSel) {
      if (mCards.empty()) {
        g.DrawEmptyCard(4, 4);
      }else {
        g.DrawCard(4, 4, mCards.front());
      }
    } else {
      if (1 == mCards.size()) {
        g.DrawEmptyCard(4, 4);
      } else {
        g.DrawCard(4, 4, mCards[1]);
      }
    }

    if (2 != mSel) {
      if (mList[2].empty()) {
        g.DrawEmptyCard(4, 4 + OFFSETH);
      } else {
        g.DrawCard(4, 4 + OFFSETH, mList[2].back());
      }
    } else {
      if (1 == mList[2].size()) {
        g.DrawEmptyCard(4, 4 + OFFSETH);
      } else {
        g.DrawCard(4, 4 + OFFSETH, mList[2][mList[2].size() - 2]);
      }
    }

    for (int i = 0; i < 4; ++i) {
      GetSolitaireRect(i, rcDeck);
      if (13 == GetNumber(mList[0][i])) {
        g.DrawCover(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
      }
    }

    for (int i = 0; i < 20; ++i) {
      GetDeckRect(i, rcDeck);
      if (-1 == mList[3][i] || mSel == i + 3) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[3][i]);
      }
    }

    if (-1 != mSel) {
      int card = -1;
      if (1 == mSel) {
        card = mCards.front();
      } else if (2 == mSel) {
        card = mList[2].back();
      } else if (3 <= mSel) {
        card = mList[3][mSel - 3];
      }
      g.DrawCard(mPtCurr.x, mPtCurr.y, card);
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    //
    // On reserve (1).
    //

    RECT_t rc1(4, 4, 4 + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rc1.ptInRect(point)) {

      if (mCards.empty()) {
        return false;
      }

      mSel = 1;

      mPtDown.x = point.x - rc1.left;
      mPtDown.y = point.y - rc1.top;

      return false;
    }

    //
    // On trash (2).
    //

    RECT_t rc2(4, 4 + OFFSETH, 4 + CARD_WIDTH, 4 + OFFSETH + CARD_HEIGHT);
    if (rc2.ptInRect(point)) {

      if (mList[2].empty()) {
        return false;
      }

      mSel = 2;

      mPtDown.x = point.x - rc2.left;
      mPtDown.y = point.y - rc2.top;

      return false;
    }

    //
    // On carpet (>=3).
    //

    RECT_t rcDeck;
    for (int i = 0; i < 20; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[3][i]) {
        return false;
      }

      mSel = 3 + i;

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

    int card1;
    if (1 == mSel) {
      card1 = mCards.front();
    } else if (2 == mSel) {
      card1 = mList[2].back();
    } else if (3 <= mSel) {
      card1 = mList[3][mSel - 3];
    }

    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    RECT_t rcDeck;

    //
    // Drop on solitaire.
    //

    for (int i = 0; i < 4; ++i) {

      GetSolitaireRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int card2 = mList[0][i];
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 != color2) {
        goto end;
      }

      if (number1 != number2 + 1) {
        goto end;
      }

      mList[0][i] = card1;

      goto end2;
    }

    //
    // Drop on trash.
    //

    if (1 == mSel) {
      RECT_t rc2(4, 4 + OFFSETH, 4 + CARD_WIDTH, 4 + OFFSETH + CARD_HEIGHT);
      if (rc2.ptInRect(point)) {
        mList[2].push_back(GetNewCard());
        goto end;
      }
    }

    //
    // Drop on carpet.
    //

    if (3 > mSel) {
      for (int i = 0; i < 20; ++i) {

        GetDeckRect(i, rcDeck);
        if (!rcDeck.ptInRect(point)) {
          continue;
        }

        if (-1 != mList[3][i]) {
          goto end;
        }

        mList[3][i] = card1;

        goto end2;
      }
    }

end:
    mSel = -1;
    return true;

end2:
    if (1 == mSel) {
      (void)GetNewCard();
    } else if (2 == mSel) {
      mList[2].pop_back();
    } else if (3 <= mSel) {
      mList[3][mSel - 3] = -1;
    }
    goto end;

  } // OnMouseUp

  //
  // Helper.
  //

  void GetSolitaireRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (1 + iDeck);
    rcDeck.top = 4;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetSolitaireRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (1 + (iDeck % 5));
    rcDeck.top = 4 + OFFSETH * (1 + (iDeck / 5));
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect
};

} // namespace cgo

// end of 028_TheCarpet.h
