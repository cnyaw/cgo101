
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/04 Waync.
//

#pragma once

namespace cgo {

//
// Klondike.
//

class GameKlondike : public CardGame
{
public:

  int mSelIndex;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_014_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_014_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 0; i < 4; ++i) {
      mList[0].push_back(-1);
    }

    for (int i = 1; i <= 7; ++i) {
      for (int j = 0; j < 7 - i + 1; ++j) {
        mList[i].push_back(GetNewCard() + (j == 7 - i ? 0 : 100));
      }
    }

  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (mCards.empty()) {
      return false;
    }

    mList[8].push_back(GetNewCard());
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

    if (mCards.empty()) {
      g.DrawEmptyCard(4, 4);
    } else {
      g.DrawCover(4, 4);
    }

    //
    // Draw new card.
    //

    {
      int n = 8 == mSel ? mList[8].size() - 1: mList[8].size();
      for (int i = 0; i < n; ++i) {
        g.DrawCard(12 + CARD_WIDTH, 4 + 20 * i, mList[8][i]);
      }
    }

    //
    // Draw deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {
      GetDeckRect(i, rcDeck);
      if (-1 == mList[0][i]) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
      }
    }

    //
    // Draw await.
    //

    for (int i = 1; i <= 7; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {

        if (mSel == i && mSelIndex == j) {
          break;
        }

        if (100 <= mList[i][j]) {
          g.DrawCover(rcDeck.left, rcDeck.top + 20 * j);
        } else {
          g.DrawCard(rcDeck.left, rcDeck.top + 20 * j, mList[i][j]);
        }
      }
    }

    //
    // Draw select card.
    //

    if (-1 != mSel) {
      if (8 == mSel) {
        g.DrawCard(mPtCurr.x, mPtCurr.y, mList[8].back());
      } else {
        for (int i = mSelIndex; i < (int)mList[mSel].size(); ++i) {
          g.DrawCard(mPtCurr.x, mPtCurr.y + 20 * (i - mSelIndex), mList[mSel][i]);
        }
      }
    }
  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    //
    // Deal new card?
    //

    RECT_t rcDealNewCard(4, 4, 4 + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcDealNewCard.ptInRect(point)) {
      return NewCard();
    }

    //
    // Select new card?
    //

    RECT_t rcNewCard(12 + CARD_WIDTH, 4, 12 + 2 * CARD_WIDTH, 4 + CARD_HEIGHT);
    rcNewCard.offset(0, 20 * (mList[8].size() - 1));

    if (rcNewCard.ptInRect(point)) {

      if (mList[8].empty()) {
        return false;
      }

      mSel = 8;

      mPtDown.x = point.x - rcNewCard.left;
      mPtDown.y = point.y - rcNewCard.top;

      return false;
    }

    //
    // Select await?
    //

    RECT_t rcDeck;
    for (int i = 1; i <= 7; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 20 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        return false;
      }

      int offset = (rcDeck.bottom - CARD_HEIGHT - point.y);
      int idx;
      if (0 < offset) {
        idx = mList[i].size() - offset / 20 - 2;
      } else {
        idx = mList[i].size() - 1;
      }

      if (100 <= mList[i][idx]) {
        if (idx == mList[i].size() - 1) {
          mList[i][idx] -= 100;
          return true;
        }
        return false;
      }

      if (idx != mList[i].size() - 1) {

        int color1 = GetColor(mList[i][idx]);
        int number1 = GetNumber(mList[i][idx]);

        for (int j = idx + 1; j < (int)mList[i].size(); ++j) {

          int color2 = GetColor(mList[i][j]);
          int number2 = GetNumber(mList[i][j]);

          if (color1 == color2 ||
              color1 + color2 == 3 ||
              number1 != number2 + 1) {
            return false;
          }

          color1 = color2;
          number1 = number2;
        }
      }

      mSel = i;
      mSelIndex = idx;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - (rcDeck.top + 20 * idx);

      return false;
    }

    return false;
  } // OnMouseDown

  virtual bool OnMouseUp(POINT_t point, RECT_t const& rcClient)
  {
    if (-1 == mSel) {
      return false;
    }

    //
    // Drop on deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int card1 = 8 == mSel ? mList[8].back() : mList[mSel][mSelIndex];
      int color1 = GetColor(card1);
      int number1 = GetNumber(card1);

      if (card1 == mList[mSel].back()) {

        if (1 == number1 && -1 == mList[0][i]) {
end2:
          mList[0][i] = card1;
          mList[mSel].pop_back();
          goto end;
        }

        int color2 = GetColor(mList[0][i]);
        int number2 = GetNumber(mList[0][i]);
        if (color1 == color2 && number1 == number2 + 1) {
          goto end2;
        }
      }

      goto end;
    }

    //
    // Drop on await.
    //

    for (int i = 1; i <= 7; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 20 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (8 == mSel) {

        if (mList[i].empty() && 13 == GetNumber(mList[8].back())) {
end5:
          mList[i].push_back(mList[8].back());
          mList[8].pop_back();
          goto end;
        }

        int color1 = GetColor(mList[8].back());
        int number1 = GetNumber(mList[8].back());
        int color2 = GetColor(mList[i].back());
        int number2 = GetNumber(mList[i].back());
        if (color1 != color2 &&
            color1 + color2 != 3 &&
            number1 + 1 == number2) {
          goto end5;
        }

        goto end;
      }

      if (mSel == i) {
        goto end;
      }

      if (mList[i].empty() && 13 == GetNumber(mList[mSel][mSelIndex])) {
end3:
        mList[i].insert(
                   mList[i].end(),
                   mList[mSel].begin() + mSelIndex,
                   mList[mSel].end());
        mList[mSel].erase(
                      mList[mSel].begin() + mSelIndex,
                      mList[mSel].end());
        goto end;
      }

      int color1 = GetColor(mList[mSel][mSelIndex]);
      int number1 = GetNumber(mList[mSel][mSelIndex]);
      int color2 = GetColor(mList[i].back());
      int number2 = GetNumber(mList[i].back());
      if (color1 != color2 &&
          color1 + color2 != 3 &&
          number1 + 1 == number2) {
        goto end3;
      }

      goto end;
    }
end:
    mSel = -1;
    return true;
  }

  //
  // Helper.
  //

  void GetAwaitRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (2 + iDeck);
    rcDeck.top = 4 + OFFSETH;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (2 + iDeck);
    rcDeck.top = 4;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect
};

} // namespace cgo

// end of 014_Klondike.h
