
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/11 Waync.
//

#pragma once

namespace cgo {

//
// Dieppe.
//

class GameDieppe : public CardGame
{
public:

  int mSelIndex;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_024_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_024_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame2();

    for (int i = 0; i < 8; ++i) {
      mList[0].push_back(GetNewCard(1));
    }

    for (int i = 1; i <= 8; ++i) {
      mList[i].push_back(GetNewCard());
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

    mList[9].push_back(GetNewCard());
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
    // Draw deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 8; ++i) {
      GetDeckRect(i, rcDeck);
      if (13 == GetNumber(mList[0][i])) {
        g.DrawCover(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
      }
    }

    //
    // Draw new card.
    //

    if (!mList[9].empty()) {
      if (9 != mSel) {
        g.DrawCard(12 + CARD_WIDTH, 4, mList[9].back());
      } else if (2 <= mList[9].size()) {
        g.DrawCard(12 + CARD_WIDTH, 4, mList[9][mList[9].size() - 2]);
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
    // Draw select card.
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
    // Select new card?
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
    // Select await?
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

      if (GetColor(mList[0][i]) == color1 &&
          GetNumber(mList[0][i]) + 1 == number1) {

        if (mList[mSel].back() != card1) {
          goto end;
        }

        mList[0][i] = card1;
        mList[mSel].pop_back();
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

      if (i == mSel) {
        goto end;
      }

      if (mList[i].empty() ||
          number1 + 1 == GetNumber(mList[i].back())) {
        if (9 == mSel) {
          mList[i].push_back(mList[mSel].back());
          mList[mSel].pop_back();
        } else {
          mList[i].insert(
                     mList[i].end(),
                     mList[mSel].begin() + mSelIndex,
                     mList[mSel].end());
          mList[mSel].erase(
                        mList[mSel].begin() + mSelIndex,
                        mList[mSel].end());
        }
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

// end of 024_Dieppe.h
