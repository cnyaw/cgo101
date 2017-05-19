
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/28 Waync.
//

#pragma once

namespace cgo {

//
// Pyramid.
//

class GamePyramid : public CardGame
{
public:

  int mRow, mN;                         // 0:new card, 1:pyramid(mRow,mN), -1:invalid.
  int mSelCard;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_007_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_007_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 1; i <= 7; ++i) {
      for (int j = 0; j < i; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    mSelCard = -1;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (mCards.empty()) {
      return false;
    }

    mList[0].push_back(GetNewCard());
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
    // Draw deck, new card list.
    //

    RECT_t rcDeck;
    GetDeckRect(rcDeck);
    g.DrawEmptyCard(rcDeck.left, rcDeck.top);

    for (int i = 0; i < (int)mList[0].size(); ++i) {
      int card = mList[0][i];
      if (mSelCard == card) {
        break;
      }
      g.DrawCard(rcDeck.left, rcDeck.top + 16 * i, card);
    }

    //
    // Draw pyramid deck.
    //

    for (int i = 1; i < 8; ++i) {
      for (int j = 0; j < i; ++j) {
        int card = mList[i][j];
        if (-1 == card || mSelCard == card) {
          continue;
        }
        GetDeckRect(i - 1, j, rcDeck);
        g.DrawCard(rcDeck.left, rcDeck.top, card);
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
    if (!mList[0].empty()) {

      GetDeckRect(rcDeck);
      rcDeck.offset(0, 16 * (mList[0].size() - 1));

      if (rcDeck.ptInRect(point)) {

        int card = mList[0].back();
        int number = GetNumber(card);
        if (13 == number) {
          mList[0].pop_back();
          return true;
        }

        mSel = 0;
        mSelCard = card;

        mPtDown.x = point.x - rcDeck.left;
        mPtDown.y = point.y - rcDeck.top;

        return false;
      }
    }

    //
    // Select deck?
    //

    for (int i = 7; i >= 1; --i) {
      for (int j = 0; j < i; ++j) {
        GetDeckRect(i - 1, j, rcDeck);
        if (rcDeck.ptInRect(point)) {

          int card = mList[i][j];
          if (-1 == card) {
            continue;
          }

          if (!CanSelect(i, j)) {
            return false;
          }

          int number = GetNumber(card);
          if (13 == number) {
            mList[i][j] = -1;
            return true;
          }

          mSel = 1;
          mSelCard = card;
          mRow = i;
          mN = j;

          mPtDown.x = point.x - rcDeck.left;
          mPtDown.y = point.y - rcDeck.top;

          return false;
        }
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
    // Drop on pyramid deck.
    //

    RECT_t rcDeck;
    for (int i = 7; i >= 1; --i) {
      for (int j = 0; j < i; ++j) {
        GetDeckRect(i - 1, j, rcDeck);
        if (rcDeck.ptInRect(point)) {

          int card = mList[i][j];
          if (-1 == card) {
            continue;
          }

          if (!CanSelect(i, j)) {
            goto end;
          }

          if (mSelCard == card) {
            goto end;
          }

          int number1 = GetNumber(card);
          int number2 = GetNumber(mSelCard);

          if (13 == number1 + number2) {
            if (0 == mSel) {
              mList[0].pop_back();
            } else {
              mList[mRow][mN] = -1;
            }
            mList[i][j] = -1;
          }

          goto end;
        }
      }
    }

end:
    mSel = mSelCard = -1;
    return true;
  } // OnMouseUp

  //
  // Helper.
  //

  bool CanSelect(int row, int n) const
  {
    if (7 == row) {
      return true;
    }

    return -1 == mList[row + 1][n] && -1 == mList[row + 1][n + 1];
  } // CanSelect

  void GetDeckRect(RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW;
    rcDeck.top = 4;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect

  void GetDeckRect(int row, int n, RECT_t& rcDeck) const
  {
    int const offseth = 8 + 6 * CARD_HEIGHT / 13;

    rcDeck.left = 4 + OFFSETW * (5 + n) - row * CARD_WIDTH / 2;
    rcDeck.top = 4 + offseth * row;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect
};

} // namespace cgo

// end of 007_Pyramid.h
