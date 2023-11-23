
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/21 Waync.
//

#pragma once

namespace cgo {

//
// Accordion.
//

class GameAccordion : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_001_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_001_DESC;
  } // GetDesc

  //
  // New card.
  //

  virtual bool NewCard()
  {
    //
    // End game?
    //

    if (mCards.empty()) {
      return false;
    }

    //
    // Test all decks.
    //

    for (int i = 0; i < (int)mList[0].size(); ++i) {

      int card = mList[0][i];
      int color = GetColor(card);
      int number = GetNumber(card);

      //
      // 1) test previous card, if has same number or color?
      //

      if (0 < i) {
        int card2 = mList[0][i - 1];
        int color2 = GetColor(card2);
        int number2 = GetNumber(card2);
        if (color == color2 || number == number2) {
          return false;
        }
      }

      //
      // 2) test previous previous previous card, if has same number or color?
      //

      if (2 < i) {
        int card2 = mList[0][i - 3];
        int color2 = GetColor(card2);
        int number2 = GetNumber(card2);
        if (color == color2 || number == number2) {
          return false;
        }
      }
    }

    //
    // Deal new card.
    //

    mList[0].push_back(GetNewCard());

    return true;
  } // NewCard

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    RECT_t rcDeck;

    //
    // Draw deal new card deck.
    //

    if (!mCards.empty()) {
      g.DrawCover(4, 4);
    }

    //
    // Draw empty decks.
    //

    int iDeck = mCards.empty() ? 0 : 1;
    while (GetDeckRect(g.rcClient, iDeck++, rcDeck)) {
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);
    }

    //
    // Draw decks.
    //

    for (int i = 0; i < (int)mList[0].size(); ++i) {

      if (mSel == i) {
        continue;
      }

      if (!GetDeckRect(g.rcClient, 1 + i, rcDeck)) {
        break;
      }

      g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
    }

    //
    // Draw selected card.
    //

    if (-1 != mSel) {
      g.DrawCard(mPtCurr.x, mPtCurr.y, mList[0][mSel]);
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    //
    // Hittest, get select deck.
    //

    RECT_t rcDeck;
    int sel = GetHitDeck(rcClient, point, rcDeck);

    //
    // Deal new card?
    //

    if (0 == sel) {
      return NewCard();
    }

    //
    // Hit other deck?
    //

    if (-1 != sel && (int)mList[0].size() >= sel) {
      mSel = sel - 1;
      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - rcDeck.top;
    }

    return false;
  } // OnMouseDown

  virtual bool OnMouseUp(POINT_t point, RECT_t const& rcClient)
  {
    if (-1 != mSel) {

      RECT_t rcDeck;
      int sel = GetHitDeck(rcClient, point, rcDeck) - 1;

      if (sel == mSel - 1 || sel == mSel - 3) {

        int card = mList[0][mSel];
        int color = GetColor(card);
        int number = GetNumber(card);

        int card2 = mList[0][sel];
        int color2 = GetColor(card2);
        int number2 = GetNumber(card2);

        if (color == color2 || number == number2) {
          mList[0][sel] = mList[0][mSel];
          mList[0].erase(mList[0].begin() + mSel);
        }
      }

      mSel = -1;

      return true;
    }

    return false;
  } // OnMouseUp

  //
  // Helper.
  //

  bool GetDeckRect(RECT_t const& rcClient, int iDeck, RECT_t& rcDeck) const
  {
    int cxDeck = rcClient.right / OFFSETW;
    int cyDeck = rcClient.bottom / OFFSETH;

    int c = 0;
    for (int j = 0; j < cyDeck; ++j) {
      for (int i = 0; i < cxDeck; ++i, ++c) {
        if (c == iDeck) {
          InitCardRect(rcDeck, 4 + i * OFFSETW, 4 + j * OFFSETH);
          return true;
        }
      }
    }

    return false;
  } // GetDeckRect

  int GetHitDeck(RECT_t const& rcClient, POINT_t point, RECT_t& rcDeck) const
  {
    int sel = -1;
    int iDeck = 0;

    while (GetDeckRect(rcClient, iDeck++, rcDeck)) {
      if (rcDeck.ptInRect(point)) {
        sel = iDeck - 1;
        break;
      }
    }

    return sel;
  } // GetHitDeck
};

} // namespace cgo

// end of 001_Accordion.h
