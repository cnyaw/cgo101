
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/08 Waync.
//

#pragma once

namespace cgo {

//
// Golf.
//

class GameGolf : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_019_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_019_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    mList[0].push_back(GetNewCard());

    for (int i = 1; i <= 7; ++i) {
      for (int j = 0; j < 5; ++j) {
        mList[i].push_back(GetNewCard());
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

    mList[0][0] = GetNewCard();
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
    GetDeckRect(0, rcDeck);
    g.DrawCard(rcDeck.left, rcDeck.top, mList[0][0]);

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
        if (mSel == i && (int)mList[i].size() - 1 == j) {
          break;
        }
        g.DrawCard(rcDeck.left, rcDeck.top + 22 * j, mList[i][j]);
      }
    }

    //
    // Draw select.
    //

    if (-1 != mSel) {
      g.DrawCard(mPtCurr.x, mPtCurr.y, mList[mSel].back());
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    //
    // New card?
    //

    RECT_t rcDealNewCard(4, 4, 4 + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcDealNewCard.ptInRect(point)) {
      return NewCard();
    }

    //
    // Select await?
    //

    RECT_t rcDeck;
    for (int i = 1; i <= 7; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.offset(0,  22 * (mList[i].size() - 1));

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        return false;
      }

      mSel = i;

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
    // Drop on deck?
    //

    RECT_t rcDeck;
    GetDeckRect(0, rcDeck);

    if (rcDeck.ptInRect(point)) {
      int number1 = GetNumber(mList[0][0]);
      int number2 = GetNumber(mList[mSel].back());
      if (number1 + 1 == number2 || number1 == number2 + 1) {
        mList[0][0] = mList[mSel].back();
        mList[mSel].pop_back();
      }
    }

    mSel = -1;
    return true;
  } // OnMouseUp

  //
  // Helper.
  //

  void GetAwaitRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (1 + iDeck), 4 + OFFSETH);
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (1 + iDeck), 4);
  } // GetDeckRect
};

} // namespace cgo

// end of 019_Golf.h
