
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/01 Waync.
//

#pragma once

namespace cgo {

//
// Sir Tommy.
//

class GameSirTommy : public CardGame
{
public:

  int mNewCard;
  int mSelCard;                         // mSel, 0:new, 1..4:await.

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_010_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_010_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 1; i <= 4; ++i) {
      mList[0].push_back(-1);
      mList[i].clear();
      mList[i].push_back(GetNewCard());
    }

    mNewCard = mSelCard = -1;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (mCards.empty() || -1 != mNewCard) {
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

    if (-1 != mNewCard && mSelCard != mNewCard) {
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

    for (int i = 1; i <= 4; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      if (mList[i].back() == mSelCard) {
        if (1 < mList[i].size()) {
          g.DrawCard(rcDeck.left, rcDeck.top, mList[i][mList[i].size() - 2]);
        }
        continue;
      }

      g.DrawCard(rcDeck.left, rcDeck.top, mList[i].back());
    }

    //
    // Draw selected card.
    //

    if (-1 != mSel) {
      g.DrawCard(mPtCurr.x, mPtCurr.y, mSelCard);
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

    RECT_t rcNewCard(4, 4, 4 + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcNewCard.ptInRect(point)) {
      return NewCard();
    }

    //
    // Drag new card?
    //

    RECT_t rcDealCard(12 + CARD_WIDTH, 4, 12 + CARD_WIDTH + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcDealCard.ptInRect(point)) {

      mSel = 0;
      mSelCard = mNewCard;

      mPtDown.x = point.x - rcDealCard.left;
      mPtDown.y = point.y - rcDealCard.top;

      return false;
    }

    //
    // Drag await.
    //

    RECT_t rcDeck;

    for (int i = 1; i <= 4; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        break;
      }

      mSel = i;
      mSelCard = mList[i].back();

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - rcDeck.top;
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

      int number1 = GetNumber(mSelCard);

      if (-1 == mList[0][i]) {

        if (1 == number1) {
          mList[0][i] = mSelCard;
          if (0 == mSel) {
            mNewCard = -1;
          } else {
            mList[mSel].pop_back();
          }
        }

        goto end;
      }

      int number2 = GetNumber(mList[0][i]);
      if (number2 + 1 == number1) {
        mList[0][i] = mSelCard;
        if (0 == mSel) {
          mNewCard = -1;
        } else {
          mList[mSel].pop_back();
        }
      }

      goto end;
    }

    //
    // Drop on await.
    //

    for (int i = 1; i <= 4; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (i == mSel) {
        break;
      }

      mList[i].push_back(mSelCard);

      if (0 == mSel) {
        mNewCard = -1;
      } else {
        mList[mSel].pop_back();
      }

      goto end;
    }

end:
    mSel = mSelCard = -1;
    return true;
  } // OnMouseUp

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

// end of 010_SirTommy.h
