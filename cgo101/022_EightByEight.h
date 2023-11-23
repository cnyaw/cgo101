
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/10 Waync.
//

#pragma once

namespace cgo {

//
// Eight By Eight.
//

class GameEightByEight : public CardGame
{
public:

  int mTurn;
  int mSelIndex;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_022_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_022_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame2();

    for (int i = 0; i < 8; ++i) {
      mList[0].push_back(-1);
    }

    for (int i = 1; i <= 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    mTurn = 3;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (0 == mTurn) {
      return false;
    }

    if (mCards.empty()) {
      mTurn--;
      if (0 < mTurn) {
        mCards.insert(mCards.begin(), mList[9].begin(), mList[9].end());
        mList[9].clear();
      }
      return true;
    }

    if (mTurn <= (int)mCards.size()) {
      for (int i = 0; i < mTurn; ++i) {
        mList[9].push_back(GetNewCard());
      }
    } else {
      mList[9].push_back(GetNewCard());
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

    if (mCards.empty()) {
      g.DrawEmptyCard(4, 4);
    } else {
      g.DrawCover(4, 4);
    }

    //
    // Draw new card.
    //

    if (mSel == 9) {
      if (1 < mList[9].size()) {
        g.DrawCard(12 + CARD_WIDTH, 4, mList[9][mList[9].size() - 2]);
      }
    } else {
      if (!mList[9].empty()) {
        g.DrawCard(12 + CARD_WIDTH, 4, mList[9].back());
      }
    }

    //
    // Draw deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 8; ++i) {
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

    for (int i = 1; i <= 8; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {
        if (mSel == i && mSelIndex == j) {
          break;
        }
        g.DrawCard(rcDeck.left, rcDeck.top + 22 * j, mList[i][j]);
      }
    }

    //
    // Draw select.
    //

    if (-1 != mSel) {
      if (9 == mSel) {
        g.DrawCard(mPtCurr.x, mPtCurr.y, mList[9].back());
      } else {
        for (int i = mSelIndex; i < (int)mList[mSel].size(); ++i) {
          g.DrawCard(mPtCurr.x, mPtCurr.y + 22 * (i - mSelIndex), mList[mSel][i]);
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
    // New card?
    //

    RECT_t rcNewCard(12 + CARD_WIDTH, 4, 12 + 2 * CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcNewCard.ptInRect(point)) {

      if (mList[9].empty()) {
        return false;
      }

      mSel = 9;

      mPtDown.x = point.x - rcNewCard.left;
      mPtDown.y = point.y - rcNewCard.top;

      return false;
    }

    //
    // Await?
    //

    RECT_t rcDeck;
    for (int i = 1; i <= 8; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 22 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        return false;
      }

      int offset = (rcDeck.bottom - CARD_HEIGHT - point.y);
      int idx;
      if (0 < offset) {
        idx = mList[i].size() - offset / 22 - 2;
      } else {
        idx = mList[i].size() - 1;
      }

      if (idx != mList[i].size() - 1) {

        int number1 = GetNumber(mList[i][idx]);
        for (int j = idx + 1; j < (int)mList[i].size(); ++j) {
          int number2 = GetNumber(mList[i][j]);
          if (number1 != number2 + 1) {
            return false;
          }
          number1 = number2;
        }
      }

      mSel = i;
      mSelIndex = idx;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - (rcDeck.top + 22 * idx);

      return false;
    }

    return false;
  } // OnMouseDown

  virtual bool OnMouseUp(POINT_t point, RECT_t const& rcClient)
  {
    if (-1 == mSel) {
      return false;
    }

    int card1 = 9 == mSel ? mList[9].back() : mList[mSel][mSelIndex];
    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    //
    // Drop on deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 8; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[mSel].back() == card1) {

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

    for (int i = 1; i <= 8; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 22 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (9 == mSel) {

        if (mList[i].empty()) {
          mList[i].push_back(card1);
          mList[9].pop_back();
        } else if (number1 + 1 == GetNumber(mList[i].back())) {
          mList[i].push_back(card1);
          mList[9].pop_back();
        }

        goto end;
      }

      if (mSel == i) {
        goto end;
      }

      if (mList[i].empty()) {

        if (mList[mSel].back() == card1) {

          mList[i].push_back(card1);
          mList[mSel].pop_back();

        } else {

          for (int j = mSelIndex + 1; j < (int)mList[mSel].size(); ++j) {
            int color2 = GetColor(mList[mSel][j]);
            int number2 = GetNumber(mList[mSel][j]);
            if (color1 != color2 || number1 != number2 + 1) {
              goto end;
            }
            number1--;
          }

          goto end3;
        }

        goto end;
      }

      if (number1 + 1 == GetNumber(mList[i].back())) {
end3:
        mList[i].insert(
                   mList[i].end(),
                   mList[mSel].begin() + mSelIndex,
                   mList[mSel].end());
        mList[mSel].erase(
                      mList[mSel].begin() + mSelIndex,
                      mList[mSel].end());
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

  void GetAwaitRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (2 + iDeck), 4 + OFFSETH);
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (2 + iDeck), 4);
  } // GetDeckRect
};

} // namespace cgo

// end of 022_EightByEight.h
