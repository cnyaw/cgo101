
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/05 Waync.
//

#pragma once

namespace cgo {

//
// Royal Parade.
//

class GameRoyalParade : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_016_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_016_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame2();

    for (int i = 0; i < 24; ++i) {
      int card = GetNewCard();
      if (2 + i / 8 == GetNumber(card)) {
        card += 100;
      }
      mList[0].push_back(card);
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

    for (int i = 1; i <= 8; ++i) {
      if (!mCards.empty()) {
        mList[i].push_back(GetNewCard());
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
    for (int i = 0; i < 24; ++i) {

      GetDeckRect(i, rcDeck);

      if (-1 == mList[0][i] || i == mSel) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else if (500 < mList[0][i]) {
        g.DrawCover(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i] % 100);
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

      if (i + 100 == mSel) {
        if (1 < mList[i].size()) {
          g.DrawCard(rcDeck.left, rcDeck.top, mList[i][mList[i].size() - 2]);
        }
        continue;
      }

      g.DrawCard(rcDeck.left, rcDeck.top, mList[i].back());
    }

    //
    // Draw sel.
    //

    if (-1 != mSel) {
      int card = 100 < mSel ? mList[mSel - 100].back() : mList[0][mSel];
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
    // Select deck?
    //

    RECT_t rcDeck;
    for (int i = 0; i < 24; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[0][i] || 100 <= mList[0][i]) {
        return false;
      }

      if (1 == GetNumber(mList[0][i])) {
        mList[0][i] = -1;
        return true;
      }

      mSel = i;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - rcDeck.top;

      return false;
    }

    //
    // Select await?
    //

    for (int i = 1; i <= 8; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        return false;
      }

      if (1 == GetNumber(mList[i].back())) {
        mList[i].pop_back();
        return true;
      }

      mSel = 100 + i;

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
    for (int i = 0; i < 24; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int card1 = 100 < mSel ? mList[mSel - 100].back() : mList[0][mSel];
      int number1 = GetNumber(card1);

      if (-1 == mList[0][i]) {

        if (2 + i / 8 != number1) {
          goto end;
        }

      } else {

        int card2 = mList[0][i];
        if (500 <= card2) {
          goto end;
        }

        int number2 = GetNumber(card2 % 100);

        if (2 + i / 8 < number2 && 100 > card2) {
          goto end;
        }

        card2 %= 100;

        if (GetColor(card1) != GetColor(card2) ||
            number1 != number2 + 3 || (2 + i / 8 + 9 < number1)) {
          goto end;
        }

        if (11 <= number1) {
          card1 = 500;
        }
      }

      mList[0][i] = card1 + 100;

      if (100 < mSel) {
        mList[mSel - 100].pop_back();
      } else {
        mList[0][mSel] = -1;
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
    InitCardRect(rcDeck, 4 + OFFSETW * (1 + iDeck), 4 + OFFSETH * 3);
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (1 + (iDeck % 8)), 4 + OFFSETH * (iDeck / 8));
  } // GetDeckRect
};

} // namespace cgo

// end of 016_RoyalParade.h
