
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/09/28 Waync.
//

#pragma once

namespace cgo {

//
// Above And Below.
//

class GameAboveAndBelow : public CardGame
{
public:

  int mSolitaireBottom[8];
  int mSolitaire[8];

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_026_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_026_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame2();

    for (int i = 0; i < 8; i++) {
      mList[i].push_back(GetNewCard());
    }

    for (int i = 8; i < 16; i++) {
      for (int j = 0; j < 11; j++) {
        mList[i].push_back(GetNewCard());
      }
    }

    for (int i = 16; i < 24; i++) {
      mList[i].push_back(GetNewCard());
    }

    for (int i = 0; i < 8; i++) {
      mSolitaireBottom[i] = -1;
      mSolitaire[i] = -1;
    }

  } // NewGame

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    RECT_t rcDeck;

    //
    // Solitaire.
    //

    for (int i = 0; i < 8; ++i) {
      GetSolitaireRect(i, rcDeck);
      if (999 == mSolitaire[i]) {
        g.DrawCover(rcDeck.left, rcDeck.top);
      } else if (-1 == mSolitaire[i]) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mSolitaire[i]);
      }
    }

    //
    // Decks.
    //

    for (int i = 0; i < 24; ++i) {

      if (mList[i].empty()) {
        continue;
      }

      int card = mList[i].back();
      if (mSel == i) {
        if (1 == mList[i].size()) {
          continue;
        } else {
          card = mList[i][mList[i].size() - 2];
        }
      }

      GetDeckRect(i, rcDeck);
      g.DrawCard(rcDeck.left, rcDeck.top, card);
    }

    //
    // Sel.
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
    RECT_t rcDeck;
    for (int i = 0; i < 24; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        break;
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

    int card1 = mList[mSel].back();
    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    //
    // Drop on solitaire.
    //

    for (int i = 0; i < 8; ++i) {

      GetSolitaireRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int card2 = mSolitaire[i];
      if (-1 == card2) {

        if (1 != number1 && 13 != number1) {
          goto end;
        }

        for (int j = 0; j < 8; j++) {
          if (mSolitaireBottom[j] == card1) {
            goto end;
          }
        }

        mSolitaireBottom[i] = card1;
        mSolitaire[i] = card1;
        mList[mSel].pop_back();

      } else {

        int color2 = GetColor(card2);
        int number2 = GetNumber(card2);

        if (color1 != color2) {
          goto end;
        }

        if (1 == GetNumber(mSolitaireBottom[i])) {

          if (number1 != number2 + 1) {
            goto end;
          }

          if (13 == number2 + 1) {
            mSolitaire[i] = 999;
          } else {
            mSolitaire[i] = card1;
          }

        } else {

          if (number1 != number2 - 1) {
            goto end;
          }

          if (1 == number2 - 1) {
            mSolitaire[i] = 999;
          } else {
            mSolitaire[i] = card1;
          }
        }

        mList[mSel].pop_back();
      }

      goto end;
    }

    //
    // Drop on decks.
    //

    for (int i = 0; i < 24; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (i == mSel) {
        goto end;
      }

      if (mList[i].empty()) {

        if (!(8 <= i && 16 > i) &&
             (8 <= mSel && 16 > mSel) &&
             (i % 8) == (mSel % 8)) {
          mList[i].push_back(card1);
          mList[mSel].pop_back();
        } else if ((8 <= i && 16 > i) &&
                   (i == mSel + 1 || i == mSel - 1)) {
          mList[i].push_back(card1);
          mList[mSel].pop_back();
        }

        goto end;
      }

      if (8 <= i && 16 > i) {
        goto end;
      }

      int card2 = mList[i].back();
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 != color2) {
        goto end;
      }

      if (number1 == number2 + 1 || number1 == number2 - 1) {
        mList[i].push_back(card1);
        mList[mSel].pop_back();
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

  void GetSolitaireRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * iDeck;
    rcDeck.top = 4;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetSolitaireRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (iDeck % 8);
    rcDeck.top = 4 + OFFSETH * (1 + (iDeck / 8));
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect
};

} // namespace cgo

// end of 026_AboveAndBelow.h
