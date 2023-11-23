
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/25 Waync.
//

#pragma once

namespace cgo {

//
// Calculation.
//

class GameCalculation : public CardGame
{
public:

  int mNewCard;
  int mSelCard;                         // mSel, 0:deal, 1..4:await.

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_004_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_004_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < (int)mCards.size(); ++j) {
        int card = mCards[j];
        int number = GetNumber(card);
        if (number == i + 1) {
          mList[0].push_back(card);
          mCards.erase(mCards.begin() + j);
          break;
        }
      }
    }

    mNewCard = mSelCard = -1;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (-1 != mNewCard || mCards.empty()) {
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
    // Draw decks.
    //

    RECT_t rcDeck;
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      int card = mList[0][i];
      GetDeckRect(i, rcDeck);
      g.DrawCard(rcDeck.left, rcDeck.top, card);
    }

    //
    // Draw await cards.
    //

    for (int i = 1; i < 5; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {
        int card = mList[i][j];
        if (mSelCard == card) {
          continue;
        }
        g.DrawCard(rcDeck.left, rcDeck.top + 16 * j, card);
      }
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
    // Drag await card?
    //

    RECT_t rcDeck;
    for (int i = 1; i < 5; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.offset(0, 16 * (mList[i].size() - 1));

      if (rcDeck.ptInRect(point)) {

        if (mList[i].empty()) {
          return false;
        }

        mSel = i;
        mSelCard = mList[i].back();

        mPtDown.x = point.x - rcDeck.left;
        mPtDown.y = point.y - rcDeck.top;

        return false;
      }
    }

    return false;
  } // OnMouseDown

  virtual bool OnMouseUp(POINT_t point, RECT_t const& rcClient)
  {
    if (-1 != mSel) {

      //
      // Drop on deck?
      //

      RECT_t rcDeck;
      for (int i = 0; i < 4; ++i) {

        GetDeckRect(i, rcDeck);

        if (rcDeck.ptInRect(point)) {

          int number1 = GetNumber(mList[0][i]);
          int number2 = GetNumber(mSelCard);
          int const offset[] = {1, 2, 3, 4};

          if (number2 == ((number1 + offset[i]) % 13)) {

            mList[0][i] = mSelCard;

            if (0 < mSel) {
              mList[mSel].pop_back();
            } else {
              mNewCard = -1;
            }

            mSel = mSelCard = -1;
            return true;
          }

          break;
        }
      }

      //
      // Drop on await card deck?
      //

      if (0 == mSel) {

        for (int i = 1; i < 5; ++i) {

          GetAwaitRect(i - 1, rcDeck);
          rcDeck.offset(0, 16 * (mList[i].size() - 1));

          if (rcDeck.ptInRect(point)) {
            mList[i].push_back(mSelCard);
            mNewCard = -1;
            break;
          }
        }
      }

      mSel = mSelCard = -1;

      return true;
    }

    return false;
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

// end of 004_Calculation.h
