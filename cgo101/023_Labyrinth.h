
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/10 Waync.
//

#pragma once

namespace cgo {

//
// Labyrinth.
//

class GameLabyrinth : public CardGame
{
public:

  int mTurn;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_023_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_023_DESC;
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

    for (int i = 0; i < 32; ++i) {
      mList[1].push_back(-1);
    }

    mTurn = 0;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (mCards.empty()) {
      return false;
    }

    if (4 > mTurn) {
      for (int i = 0; i < 8; ++i) {
        if (!mCards.empty()) {
          mList[1][8 * mTurn + i] = GetNewCard();
        }
      }
      mTurn++;
    } else {
      for (int i = 0; i < 8; ++i) {
        if (-1 == mList[1][i] && !mCards.empty()) {
          mList[1][i] = GetNewCard();
        }
      }
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

    for (int i = 0; i < 32; ++i) {
      GetAwaitRect(i, rcDeck);
      if (-1 == mList[1][i] || mSel == i) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[1][i]);
      }
    }

    //
    // Draw select card.
    //

    if (-1 != mSel) {
      g.DrawCard(mPtCurr.x, mPtCurr.y, mList[1][mSel]);
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
    // Select card.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 32; ++i) {

      GetAwaitRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[1][i]) {
        return false;
      }

      int c = i % 8;
      int r = i / 8;

      if (1 == r) {
        if (-1 != mList[1][c + 8 * (r - 1)]) {
          return false;
        }
      } else if (2 == r) {
        if (-1 != mList[1][c + 8 * (r + 1)]) {
          return false;
        }
      }

      mSel = i;

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

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (GetColor(mList[0][i]) != GetColor(mList[1][mSel]) ||
          GetNumber(mList[0][i]) + 1 != GetNumber(mList[1][mSel])) {
        goto end;
      }

      mList[0][i] = mList[1][mSel];
      mList[1][mSel] = -1;

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
    int r = iDeck / 8;
    int c = iDeck % 8;
    InitCardRect(rcDeck, 4 + OFFSETW * (2 + c), 4 + OFFSETH * (1 + r));
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (2 + iDeck), 4);
  } // GetDeckRect
};

} // namespace cgo

// end of 023_Labyrinth.h
