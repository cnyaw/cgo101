
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/29 Waync.
//

#pragma once

namespace cgo {

//
// Easy Go.
//

class GameEasyGo : public CardGame
{
public:

  int mNewCard;
  int mSelCard;                         // mSel, 0:new card, 1..12:await, 100:waive.

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_008_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_008_DESC;
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
        if (1 == number) {
          mList[0].push_back(card);
          mCards.erase(mCards.begin() + j);
          break;
        }
      }
    }

    for (int i = 1; i < 13; ++i) {
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

    if (mSelCard != mNewCard) {
      g.DrawCard(12 + CARD_WIDTH, 4, mNewCard);
    }

    //
    // Draw decks.
    //

    RECT_t rcDeck;
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      const int card = mList[0][i];
      GetDeckRect(i, rcDeck);
      g.DrawCard(rcDeck.left, rcDeck.top, card);
    }

    for (int i = 1; i <= 12; ++i) {

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
    // Draw waive.
    //

    GetWaiveRect(rcDeck);
    g.DrawEmptyCard(rcDeck.left, rcDeck.top);

    for (int i = 0; i < (int)mList[13].size(); ++i) {
      int card = mList[13][i];
      if (mSelCard == card) {
        break;
      }
      g.DrawCard(rcDeck.left, rcDeck.top + 16 * i, card);
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

    RECT_t rcDealNewCard(4, 4, 4 + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcDealNewCard.ptInRect(point)) {
      return NewCard();
    }

    //
    // New card?
    //

    RECT_t rcNewCard(12 + CARD_WIDTH, 4, 12 + 2 * CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcNewCard.ptInRect(point)) {

      mSel = 0;
      mSelCard = mNewCard;

      mPtDown.x = point.x - rcNewCard.left;
      mPtDown.y = point.y - rcNewCard.top;

      return false;
    }

    //
    // Waive?
    //

    RECT_t rcDeck;
    GetWaiveRect(rcDeck);
    rcDeck.offset(0, 16 * (mList[13].size() - 1));

    if (rcDeck.ptInRect(point)) {
      if (!mList[13].empty()) {
        mSel = 100;
        mSelCard = mList[13].back();
        mPtDown.x = point.x - rcDeck.left;
        mPtDown.y = point.y - rcDeck.top;
      }
      return false;
    }

    //
    // Await?
    //

    for (int i = 1; i <= 12; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        return false;
      }

      mSel = i;
      mSelCard = mList[i].back();

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

    //
    // Drop on waive?
    //

    RECT_t rcDeck;
    GetWaiveRect(rcDeck);
    rcDeck.offset(0, 16 * (mList[13].size() - 1));

    if (rcDeck.ptInRect(point)) {

      if (100 == mSel) {
        goto end;
      }

      if (!CanPushBackWaive()) {
        goto end;
      }

      mList[13].push_back(mSelCard);
      if (0 == mSel) {
        mNewCard = -1;
      } else {
        mList[mSel].pop_back();
      }

      goto end;
    }

    //
    // Drop on deck?
    //

    for (int i = 0; i < 4; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int card = mList[0][i];
      int color1 = GetColor(card);
      int color2 = GetColor(mSelCard);
      int number1 = GetNumber(card);
      int number2 = GetNumber(mSelCard);

      if (color1 == color2 && number1 + 1 == number2) {
        mList[0][i] = mSelCard;
        if (0 == mSel) {
          mNewCard = -1;
        } else if (100 == mSel) {
          mList[13].pop_back();
        } else {
          mList[mSel].pop_back();
        }
      }

      goto end;
    }

    //
    // Drop on await?
    //

    for (int i = 1; i <= 12; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (i == mSel) {
        goto end;
      }

      if (mList[i].empty()) {
        if (0 == mSel) {
          mList[i].push_back(mSelCard);
          mNewCard = -1;
        } else if (100 == mSel) {
          mList[i].push_back(mSelCard);
          mList[13].pop_back();
        }
        goto end;
      }

      int card = mList[i].back();
      int color1 = GetColor(card);
      int color2 = GetColor(mSelCard);
      int number1 = GetNumber(card);
      int number2 = GetNumber(mSelCard);

      if (color1 == color2 && number1 == number2 + 1) {
        mList[i].push_back(mSelCard);
        if (0 == mSel) {
          mNewCard = -1;
        } else if (100 == mSel) {
          mList[13].pop_back();
        } else {
          mList[mSel].pop_back();
        }
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

  bool CanPushBackWaive() const
  {
    for (int i = 1; i < 13; ++i) {

      if (mList[i].empty()) {
        return false;
      }

      int card1 = mList[i].back();
      int color1 = GetColor(card1);
      int number1 = GetNumber(card1);

      for (int j = 0; j < 4; ++j) {
        int card2 = mList[0][j];
        int color2 = GetColor(card2);
        int number2 = GetNumber(card2);
        if (color1 == color2 && number1 == number2 + 1) {
          return false;
        }
      }

      for (int j = 1; j < 13; ++j) {

        if (i == j) {
          continue;
        }

        int card2 = mList[j].back();
        int color2 = GetColor(card2);
        int number2 = GetNumber(card2);

        if (color1 == color2 && number1 + 1 == number2) {
          return false;
        }
      }
    }

    if (-1 != mNewCard) {

      int card1 = mNewCard;
      int color1 = GetColor(card1);
      int number1 = GetNumber(card1);

      for (int j = 0; j < 4; ++j) {
        int card2 = mList[0][j];
        int color2 = GetColor(card2);
        int number2 = GetNumber(card2);
        if (color1 == color2 && number1 == number2 + 1) {
          return false;
        }
      }

      for (int j = 1; j < 13; ++j) {
        int card2 = mList[j].back();
        int color2 = GetColor(card2);
        int number2 = GetNumber(card2);
        if (color1 == color2 && number1 + 1 == number2) {
          return false;
        }
      }
    }

    return true;
  } // CanPushBackWaive

  void GetAwaitRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (2 + (iDeck % 4)), 4 + OFFSETH * (1 + (iDeck / 4)));
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (2 + iDeck), 4);
  } // GetDeckRect

  void GetWaiveRect(RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4, 4 + OFFSETH);
  } // GetWaiveRect
};

} // namespace cgo

// end of 008_EasyGo.h
