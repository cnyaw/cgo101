
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/03 Waync.
//

#pragma once

namespace cgo {

//
// Le Cadran.
//

class GameLeCadran : public CardGame
{
public:

  int mNewCard;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_012_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_012_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame2();

    for (int i = 0; i < 10; ++i) {
      mList[0].push_back(-1);
    }

    for (int i = 1; i <= 10; ++i) {
      for (int j = 0; j < 4; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    mNewCard = -1;
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

    if (-1 != mNewCard && 0 != mSel) {
      g.DrawCard(12 + CARD_WIDTH, 4, mNewCard);
    }

    //
    // Draw deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 10; ++i) {
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

    for (int i = 1; i <= 10; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      int n = mSel == i ? mList[i].size() - 1 : mList[i].size();
      for (int j = 0; j < n; ++j) {
        g.DrawCard(rcDeck.left, rcDeck.top + 20 * j, mList[i][j]);
      }
    }

    //
    // Draw waive cards.
    //

    {
      g.DrawEmptyCard(4, 12 + CARD_HEIGHT);

      int n = mSel == 11 ? mList[11].size() - 1 : mList[11].size();
      for (int i = 0; i < n; ++i) {
        g.DrawCard(4, 12 + CARD_HEIGHT + 16 * i, mList[11][i]);
      }
    }

    //
    // Draw select card.
    //

    if (-1 != mSel) {

      int card;
      if (0 == mSel) {
        card = mNewCard;
      } else {
        if (11 == mSel) {
          card = mList[11].back();
        } else {
          card = mList[mSel].back();
        }
      }

      g.DrawCard(mPtCurr.x, mPtCurr.y, card);
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

      mSel = 0;

      mPtDown.x = point.x - rcNewCard.left;
      mPtDown.y = point.y - rcNewCard.top;

      return false;
    }

    //
    // Select waive.
    //

    RECT_t rcWaive(4, 12 + CARD_HEIGHT, 4 + CARD_WIDTH, 12 + 2 * CARD_HEIGHT);
    rcWaive.offset(0, 16 * (mList[11].size() - 1));

    if (rcWaive.ptInRect(point)) {

      if (mList[11].empty()) {
        return false;
      }

      mSel = 11;

      mPtDown.x = point.x - rcWaive.left;
      mPtDown.y = point.y - rcWaive.top;

      return false;
    }

    //
    // Select await?
    //

    RECT_t rcDeck;
    for (int i = 1; i <= 10; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.offset(0, 20 * (mList[i].size() - 1));

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        return false;
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

    int card1;
    if (0 == mSel) {
      card1 = mNewCard;
    } else {
      if (11 == mSel) {
        card1 = mList[11].back();
      } else {
        card1 = mList[mSel].back();
      }
    }

    int number1 = GetNumber(card1);
    int color1 = GetColor(card1);

    RECT_t rcDeck;

    //
    // Drop on waive.
    //

    RECT_t rcWaive(4, 12 + CARD_HEIGHT, 4 + CARD_WIDTH, 12 + 2 * CARD_HEIGHT);
    rcWaive.bottom += 16 * (mList[11].size() - 1);

    if (rcWaive.ptInRect(point)) {
      if (0 == mSel) {
        mList[11].push_back(card1);
        mNewCard = -1;
      }
      goto end;
    }

    //
    // Drop on deck.
    //

    for (int i = 0; i < 10; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[0][i]) {

        if (1 != number1) {
          goto end;
        }
end2:
        if (0 == mSel) {
          mList[0][i] = mNewCard;
          mNewCard = -1;
        } else if (11 == mSel) {
          mList[0][i] = mList[11].back();
          mList[11].pop_back();
        } else {
          mList[0][i] = mList[mSel].back();
          mList[mSel].pop_back();
        }

        goto end;
      }

      int color2 = GetColor(mList[0][i]);
      if (color1 != color2) {
        goto end;
      }

      int number2 = GetNumber(mList[0][i]);
      if (number1 == number2 + 1) {
        goto end2;
      }

      goto end;
    }

    //
    // Drop on await.
    //

    for (int i = 1; i <= 10; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 20 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (i == mSel) {
        goto end;
      }

      if (mList[i].empty()) {
end3:
        mList[i].push_back(card1);

        if (0 == mSel) {
          mNewCard = -1;
        } else if (11 == mSel) {
          mList[11].pop_back();
        } else {
          mList[mSel].pop_back();
        }
        goto end;
      }

      int color2 = GetColor(mList[i].back());
      if (color1 != color2) {
        goto end;
      }

      int number2 = GetNumber(mList[i].back());
      if (number1 + 1 == number2) {
        goto end3;
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

// end of 012_LeCadran.h
