
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/27 Waync.
//

#pragma once

namespace cgo {

//
// Puss In The Corner.
//

class GamePussInTheCorner : public CardGame
{
public:

  int mRound;
  int mSelCard;                         // mSel, 0..3:new card, 4..7:await.

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_006_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_006_DESC;
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

    mList[6].clear();
    for (int i = 1; i < 5; ++i) {
      mList[i].clear();
      mList[6].push_back(-1);
    }

    mRound = 1;
    mSelCard = -1;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    for (int i = 0; i < 4; ++i) {
      if (-1 != mList[6][i]) {
        return false;
      }
    }

    if (mCards.empty() && 0 == mRound) {
      return false;
    }

    if (mCards.empty()) {
      mRound--;
      for (int i = 1; i < 5; ++i) {
        mCards.insert(mCards.end(), mList[i].begin(), mList[i].end());
        mList[i].clear();
      }
      return true;
    }

    for (int i = 0; i < 4; ++i) {
      if (mCards.empty()) {
        break;
      }
      mList[6][i] = GetNewCard();
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
    // Draw decks.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {
      GetDeckRect(i, rcDeck);
      g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
    }

    //
    // Draw await cards.
    //

    for (int i = 1; i <= 4; ++i) {

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
    // Draw new cards.
    //

    for (int i = 0; i < 4; ++i) {
      GetNewRect(i, rcDeck);
      if (-1 == mList[6][i] || mSel == i) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[6][i]);
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
    // Select new card?
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {
      GetNewRect(i, rcDeck);
      if (rcDeck.ptInRect(point)) {

        mSel = i;
        mSelCard = mList[6][i];

        mPtDown.x = point.x - rcDeck.left;
        mPtDown.y = point.y - rcDeck.top;

        return false;
      }
    }

    //
    // Select await card?
    //

    for (int i = 1; i <= 4; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.offset(0, 16 * (mList[i].size() - 1));

      if (rcDeck.ptInRect(point)) {

        if (mList[i].empty()) {
          return false;
        }

        mSel = i + 4;
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

      int card1 = mList[0][i];
      int color1 = GetColor(card1);
      int number1 = GetNumber(card1);
      int card2 = mSelCard;
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (number2 == number1 + 1) {

        if (((0 == color1 || 3 == color1) && (0 == color2 || 3 == color2)) ||
            ((1 == color1 || 2 == color1) && (1 == color2 || 2 == color2))) {

          mList[0][i] = mSelCard;

          if (4 > mSel) {
            mList[6][mSel] = -1;
          } else {
            mList[mSel - 4].pop_back();
          }
        }
      }

      mSel = mSelCard = -1;
      return true;
    }

    //
    // Drop on await?
    //

    if (4 > mSel) {
      for (int i = 1; i <= 4; ++i) {

        GetAwaitRect(i - 1, rcDeck);
        rcDeck.offset(0, 16 * (mList[i].size() - 1));

        if (!rcDeck.ptInRect(point)) {
          continue;
        }

        mList[i].push_back(mSelCard);
        mList[6][mSel] = -1;

        break;
      }
    }

    mSel = mSelCard = -1;
    return true;
  } // OnMouseUp

  //
  // Helper.
  //

  void GetAwaitRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (0 == (iDeck % 2) ? 2 : 5);
    rcDeck.top = 4 + (0 == (iDeck / 2) ? 0 : OFFSETH * 3);
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (3 + (iDeck % 2));
    rcDeck.top = 4 + OFFSETH * (1 + (iDeck / 2));
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect

  void GetNewRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (iDeck % 2);
    rcDeck.top = 4 + OFFSETH * (1 + (iDeck / 2));
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetNewRect
};

} // namespace cgo

// end of 006_PussInTheCorner.h
