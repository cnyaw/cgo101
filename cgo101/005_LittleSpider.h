
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/26 Waync.
//

#pragma once

namespace cgo {

//
// Little Spider.
//

class GameLittleSpider : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_005_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_005_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    int c[] = {0, 1, 2, 3};
    const int n[] = {13, 13, 1, 1};

    std::random_shuffle(c, c + 4);

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < (int)mCards.size(); ++j) {
        int card = mCards[j];
        int color = GetColor(card);
        int number = GetNumber(card);
        if (c[i] == color && n[i] == number) {
          mList[0].push_back(card);
          mCards.erase(mCards.begin() + j);
          break;
        }
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

    for (int i = 1; i < 9; ++i) {
      mList[i].push_back(GetNewCard());
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

    RECT_t rcDeck;
    GetDeckRect(0, rcDeck);
    rcDeck.offset(-(12 + CARD_WIDTH), 0);

    if (mCards.empty()) {
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);
    } else {
      g.DrawCover(rcDeck.left, rcDeck.top);
    }

    //
    // Draw decks.
    //

    for (int i = 0; i < 4; ++i) {
      GetDeckRect(i, rcDeck);
      g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
    }

    //
    // Draw await cards.
    //

    for (int i = 1; i < 9; ++i) {
      GetAwaitRect(i - 1, rcDeck);
      if (mSel == i) {
        if (1 == mList[i].size()) {
          g.DrawEmptyCard(rcDeck.left, rcDeck.top);
        } else {
          g.DrawCard(rcDeck.left, rcDeck.top, mList[i][mList[i].size() - 2]);
        }
      } else {

        if (mList[i].empty()) {
          g.DrawEmptyCard(rcDeck.left, rcDeck.top);
        } else {
          g.DrawCard(rcDeck.left, rcDeck.top, mList[i].back());
        }
      }
    }

    //
    // Draw selected card.
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
    // Deal new card?
    //

    RECT_t rcDeck;
    GetDeckRect(0, rcDeck);
    rcDeck.offset(-(12 + CARD_WIDTH), 0);

    if (rcDeck.ptInRect(point)) {
      return NewCard();
    }

    //
    // Select await deck?
    //

    for (int i = 1; i <= 8; ++i) {

      GetAwaitRect(i - 1, rcDeck);

      if (rcDeck.ptInRect(point)) {

        if (mList[i].empty()) {
          break;
        }

        mSel = i;
        mPtDown.x = point.x - rcDeck.left;
        mPtDown.y = point.y - rcDeck.top;

        break;
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

      if (rcDeck.ptInRect(point)) {

        if (!mCards.empty() && 3 < (mSel - 1) && i != (mSel - 1) - 4) {
          goto end;
        }

        int card1 = mList[0][i];
        int card2 = mList[mSel].back();
        int color1 = GetColor(card1);
        int color2 = GetColor(card2);
        int number1 = GetNumber(card1);
        int number2 = GetNumber(card2);

        if (color1 != color2) {
          goto end;
        }

        if ((2 > i && 1 == number1 - number2) ||
            (2 <= i && -1 == number1 - number2)) {
          mList[0][i] = card2;
          mList[mSel].pop_back();
        }

        goto end;
      }
    }

    //
    // Drop on await deck?
    //

    if (!mCards.empty()) {
      goto end;
    }

    for (int i = 1; i <= 8; ++i) {

      GetAwaitRect(i - 1, rcDeck);

      if (rcDeck.ptInRect(point)) {

        if (i == mSel) {
          goto end;
        }

        int card1 = mList[i].back();
        int card2 = mList[mSel].back();
        int number1 = GetNumber(card1);
        int number2 = GetNumber(card2);

        if (1 == ::abs(number1 - number2)) {
          mList[mSel].pop_back();
          mList[i].push_back(card2);
        }

        goto end;
      }
    }

end:
    mSel = -1;
    return true;
  }

  //
  // Helper.
  //

  void GetAwaitRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (2 + (iDeck % 4));
    rcDeck.top = 42 + (3 < iDeck ? 2 * OFFSETH : 0);
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (2 + iDeck);
    rcDeck.top = 42 + OFFSETH;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect
};

} // namespace cgo

// end of 005_LittleSpider.h
