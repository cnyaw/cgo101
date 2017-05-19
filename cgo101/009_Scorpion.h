
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/01 Waync.
//

#pragma once

namespace cgo {

//
// Scorpion.
//

class GameScorpion : public CardGame
{
public:

  int mSelIndex;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_009_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_009_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 0; i < 7; ++i) {
      for (int j = 0; j < 7; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        mList[i][j] += 100;
      }
    }

    mSelIndex = -1;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (mCards.empty()) {
      return false;
    }

    for (int i = 0; i < 3; ++i) {
      mList[i].push_back(mCards[i]);
    }

    mCards.clear();

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

    for (int i = 0; i < 7; ++i) {

      GetDeckRect(i, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {

        if (mSel == i && mSelIndex == j) {
          break;
        }

        int card = mList[i][j];
        if (100 <= card) {
          g.DrawCover(rcDeck.left, rcDeck.top + 24 * j);
        } else {
          g.DrawCard(rcDeck.left, rcDeck.top + 24 * j, card);
        }
      }
    }

    //
    // Draw select card.
    //

    if (-1 != mSel) {
      for (int i = mSelIndex; i < (int)mList[mSel].size(); ++i) {
        int card = mList[mSel][i];
        g.DrawCard(mPtCurr.x, mPtCurr.y + 24 * (i - mSelIndex), card);
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
    // Check select.
    //

    RECT_t rcDeck;

    for (int i = 0; i < 7; ++i) {

      GetDeckRect(i, rcDeck);
      rcDeck.bottom += 24 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int offset = (rcDeck.bottom - CARD_HEIGHT - point.y);
      int idx;
      if (0 < offset) {
        idx = mList[i].size() - offset / 24 - 2;
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

      mSel = i;
      mSelIndex = idx;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - (rcDeck.top + 24 * mSelIndex);

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
    // Check drop.
    //

    RECT_t rcDeck;

    for (int i = 0; i < 7; ++i) {

      GetDeckRect(i, rcDeck);
      rcDeck.bottom += 24 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSel == i) {
        break;
      }

      int card1 = mList[mSel][mSelIndex];
      int color1 = GetColor(card1);
      int number1 = GetNumber(card1);

      if (mList[i].empty()) {
        if (13 == number1) {
          mList[i].insert(
                     mList[i].end(),
                     mList[mSel].begin() + mSelIndex,
                     mList[mSel].end());
          mList[mSel].erase(
                        mList[mSel].begin() + mSelIndex,
                        mList[mSel].end());
        }
        break;
      }

      int card2 = mList[i].back();
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 == color2 && number1 + 1 == number2) {
        mList[i].insert(
                   mList[i].end(),
                   mList[mSel].begin() + mSelIndex,
                   mList[mSel].end());
        mList[mSel].erase(
                      mList[mSel].begin() + mSelIndex,
                      mList[mSel].end());
      }

      break;
    }

    mSel = mSelIndex = -1;
    return true;
  } // OnMouseUp

  //
  // Helper.
  //

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (2 + iDeck);
    rcDeck.top = 4;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // OnMouseUp
};

} // namespace cgo

// end of 009_Scorpion.h
