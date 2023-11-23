
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/02 Waync.
//

#pragma once

namespace cgo {

//
// Six By Six.
//

class GameSixBySix : public CardGame
{
public:

  int mNewCard;                         // mSel, 0:new card, 1..6:await.
  int mSelIndex;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_011_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_011_DESC;
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

    for (int i = 1; i <= 6; ++i) {
      for (int j = 0; j < 6; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    mNewCard = mSelIndex = -1;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (mCards.empty() ||-1 != mNewCard) {
      return false;
    }

    mNewCard = GetNewCard();
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

    if (-1 != mNewCard && 0 != mSel) {
      g.DrawCard(12 + CARD_WIDTH, 4, mNewCard);
    }

    //
    // Draw deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {
      GetDeckRect(i, rcDeck);
      int card = mList[0][i];
      if (-1 == card) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, card);
      }
    }

    //
    // Draw await.
    //

    for (int i = 1; i <= 6; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {
        if (mSel == i && mSelIndex == j) {
          break;
        }
        g.DrawCard(rcDeck.left, rcDeck.top + 20 * j, mList[i][j]);
      }
    }

    //
    // Draw select card.
    //

    if (-1 != mSel) {
      if (0 == mSel) {
        g.DrawCard(mPtCurr.x, mPtCurr.y, mNewCard);
      } else {
        for (int i = mSelIndex; i < (int)mList[mSel].size(); ++i) {
          int card = mList[mSel][i];
          g.DrawCard(mPtCurr.x, mPtCurr.y + 20 * (i - mSelIndex), card);
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

    RECT_t rcNewCard(12 + CARD_WIDTH, 4, 12 + CARD_WIDTH + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcNewCard.ptInRect(point)) {

      mSel = 0;

      mPtDown.x = point.x - rcNewCard.left;
      mPtDown.y = point.y - rcNewCard.top;

      return false;
    }

    //
    // Select await cards?
    //

    RECT_t rcDeck;
    for (int i = 1; i <= 6; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 20 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int offset = (rcDeck.bottom - CARD_HEIGHT - point.y);
      int idx;
      if (0 < offset) {
        idx = (mList[i].size() - offset / 20 - 2);
      } else {
        idx = (mList[i].size() - 1);
      }

      if (idx != mList[i].size() - 1) {
        int number1 = GetNumber(mList[i][idx]);
        for (int j = idx + 1; j < (int)mList[i].size(); ++j) {
          if (number1 != GetNumber(mList[i][j]) + 1) {
            return false;
          }
          number1--;
        }
      }

      mSel = i;
      mSelIndex = idx;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - (rcDeck.top + 20 * mSelIndex);

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
    // Drop on deck?
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (0 == mSel) {

        int number1 = GetNumber(mNewCard);
        if (1 == number1 && -1 == mList[0][i]) {
          mList[0][i] = mNewCard;
          mNewCard = -1;
          goto end;
        }

        int color1 = GetColor(mNewCard);
        int color2 = GetColor(mList[0][i]);
        int number2 = GetNumber(mList[0][i]);
        if (color1 == color2 && number1 == number2 + 1) {
          mList[0][i] = mNewCard;
          mNewCard = -1;
        }

        goto end;
      }

      if (mSelIndex == (int)mList[mSel].size() - 1) {

        int card1 = mList[mSel][mSelIndex];
        int number1 = GetNumber(card1);
        if (1 == number1 && -1 == mList[0][i]) {
          mList[0][i] = card1;
          mList[mSel].pop_back();
          goto end;
        }

        int color1 = GetColor(card1);
        int color2 = GetColor(mList[0][i]);
        int number2 = GetNumber(mList[0][i]);
        if (color1 == color2 && number1 == number2 + 1) {
          mList[0][i] = card1;
          mList[mSel].pop_back();
        }
      }

      goto end;
    }

    //
    // Drop on await?
    //

    for (int i = 1; i <= 6; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 20 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (0 == mSel) {

        if (1 == i || mList[i].empty()) {
          mList[i].push_back(mNewCard);
          mNewCard = -1;
          goto end;
        }

        int number1 = GetNumber(mNewCard);
        int number2 = GetNumber(mList[i].back());
        if (number1 + 1 == number2) {
          mList[i].push_back(mNewCard);
          mNewCard = -1;
        }

        goto end;
      }

      if (mSel == i) {
        goto end;
      }

      if (mList[i].empty()) {
end2:
        mList[i].insert(
                   mList[i].end(),
                   mList[mSel].begin() + mSelIndex,
                   mList[mSel].end());
        mList[mSel].erase(
                      mList[mSel].begin() + mSelIndex,
                      mList[mSel].end());
        goto end;
      }

      int number1 = GetNumber(mList[mSel][mSelIndex]);
      int number2 = GetNumber(mList[i].back());
      if (number1 + 1 == number2) {
        goto end2;
      }

      goto end;
    }

end:
    mSel = mSelIndex = -1;
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

// end of 011_SixBySix.h
