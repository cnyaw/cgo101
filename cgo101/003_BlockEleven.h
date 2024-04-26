
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/25 Waync.
//

#pragma once

namespace cgo {

//
// Block Eleven.
//

class GameBlockEleven : public CardGame
{
public:

  //
  // property.
  //

  virtual int GetName() const
  {
    return IDS_003_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_003_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    //
    // Prepare 12 cards, number < 11.
    //

    for (int i = 0; i < 12;) {

      int card = GetNewCard();
      int number = GetNumber(card);

      if (10 < number) {
        mCards.push_back(card);
        continue;
      }

      mList[0].push_back(card);

      i++;
    }

    //
    // Shuffle again.
    //

    random_shuffle(mCards.begin(), mCards.end());

    //
    // Find a card that number > 10 and put to the end of mCards.
    //

    for (int i = 0; i < (int)mCards.size(); ++i) {
      int card = mCards[i];
      int number = GetNumber(card);
      if (10 < number) {
        mCards.erase(mCards.begin() + i);
        mCards.push_back(card);
        break;
      }
    }
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      if (-1 == mList[0][i] && !mCards.empty()) {
        mList[0][i] = GetNewCard();
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
    // Draw decks.
    //

    RECT_t rcDeck;
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      GetDeckRect(i, rcDeck);
      int card = mList[0][i];
      if (-1 == card || mSel == i) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, card);
      }
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
    // Deal new card?
    //

    RECT_t rcNewCard(4, 4, 4 + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcNewCard.ptInRect(point)) {
      return NewCard();
    }

    //
    // Start to drag card to collapse?
    //

    RECT_t rcDeck;
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      GetDeckRect(i, rcDeck);
      if (rcDeck.ptInRect(point)) {
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
    if (-1 != mSel) {

      RECT_t rcDeck;
      int sel = GetHitDeck(point, rcDeck);
      if (CanClear(sel, mSel)) {
        mList[0][sel] = -1;
        mList[0][mSel] = -1;
      }

      mSel = -1;
      return true;
    }

    return false;
  } // OnMouseUp

  //
  // Helper.
  //

  bool CanClear(int iDeck1, int iDeck2) const
  {
    int card1 = mList[0][iDeck1];
    int card2 = mList[0][iDeck2];
    int number1 = GetNumber(card1);
    int number2 = GetNumber(card2);

    return 11 == number1 + number2;
  } // CanClear

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    int x = iDeck % 4;
    int y = iDeck / 4;
    InitCardRect(rcDeck, 4 + OFFSETW * (1 + x), 4 + OFFSETH * y);
  } // GetDeckRect

  int GetHitDeck(POINT_t point, RECT_t& rcDeck) const
  {
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      GetDeckRect(i, rcDeck);
      if (rcDeck.ptInRect(point)) {
        return i;
      }
    }

    return -1;
  } // GetHitDeck
};

} // namespace cgo

// end of 003_BlockEleven.h
