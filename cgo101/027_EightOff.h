
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/09/28 Waync.
//

#pragma once

namespace cgo {

//
// Eight Off.
//

class GameEightOff : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_027_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_027_DESC;
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

    for (int i = 1; i <= 8; ++i) {
      for (int j = 0; j < 6; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    for (int i = 0; i < 8; ++i) {
      mList[9].push_back(-1);
    }

  } // NewGame

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    RECT_t rcDeck;

    for (int i = 0; i < 4; ++i) {
      GetSolitaireRect(i, rcDeck);
      if (13 == GetNumber(mList[0][i])) {
        g.DrawCover(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
      }
    }

    for (int i = 1; i <= 8; ++i) {

      if (mList[i].empty()) {
        continue;
      }

      GetDeckRect(i, rcDeck);

      int n = (int)mList[i].size();
      if (mSel == i) {
        n--;
      }

      for (int j = 0; j < n; ++j) {
        g.DrawCard(rcDeck.left, rcDeck.top + 22 * j, mList[i][j]);
      }
    }

    for (int i = 0; i < 8; ++i) {
      GetReserveRect(i, rcDeck);
      if (-1 == mList[9][i] || 9 + i == mSel) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[9][i]);
      }
    }

    if (-1 != mSel) {
      if (9 <= mSel) {
        g.DrawCard(mPtCurr.x, mPtCurr.y, mList[9][mSel - 9]);
      } else {
        g.DrawCard(mPtCurr.x, mPtCurr.y, mList[mSel].back());
      }
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    RECT_t rcDeck;

    for (int i = 1; i <= 8; ++i) {

      if (mList[i].empty()) {
        continue;
      }

      GetDeckRect(i, rcDeck);

      rcDeck.offset(0, 22 * (mList[i].size() - 1));
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      mSel = i;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - rcDeck.top;

      return false;
    }

    for (int i = 0; i < 8; ++i) {

      GetReserveRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[9][i]) {
        break;
      }

      mSel = i + 9;

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

    int card1 = 9 <= mSel ? mList[9][mSel - 9] : mList[mSel].back();
    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    RECT_t rcDeck;

    //
    // Drop on solitaire?
    //

    for (int i = 0; i < 4; ++i) {

      GetSolitaireRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int card2 = mList[0][i];
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 != color2) {
        goto end;
      }

      if (number1 != number2 + 1) {
        goto end;
      }

      mList[0][i] = card1;

      if (9 <= mSel) {
        mList[9][mSel - 9] = -1;
      } else {
        mList[mSel].pop_back();
      }

      goto end;
    }

    //
    // Drop on deck?
    //

    for (int i = 1; i <= 8; ++i) {

      GetDeckRect(i, rcDeck);

      if (mList[i].empty()) {

        if (rcDeck.ptInRect(point) && 13 == number1) {

          mList[i].push_back(card1);

          if (9 <= mSel) {
            mList[9][mSel - 9] = -1;
          } else {
            mList[mSel].pop_back();
          }

          goto end;
        }

        continue;
      }

      rcDeck.offset(0, 22 * (mList[i].size() - 1));
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (i == mSel) {
        goto end;
      }

      int card2 = mList[i].back();
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 != color2) {
        goto end;
      }

      if (number1 != number2 - 1) {
        goto end;
      }

      mList[i].push_back(card1);

      if (9 <= mSel) {
        mList[9][mSel - 9] = -1;
      } else {
        mList[mSel].pop_back();
      }

      goto end;
    }

    //
    // Drop on reserved area.
    //

    if (9 <= mSel) {
      goto end;
    }

    for (int i = 0; i < 8; ++i) {

      GetReserveRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 != mList[9][i]) {
        goto end;
      }

      mList[9][i] = card1;
      mList[mSel].pop_back();

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
    InitCardRect(rcDeck, 4 + OFFSETW * (1 + iDeck), 4);
  } // GetSolitaireRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * iDeck, 4 + OFFSETH);
  } // GetDeckRect

  void GetReserveRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 16 + OFFSETW * (9 + (iDeck % 2)), 4 + OFFSETH * (iDeck / 2));
  } // GetReserveRect
};

} // namespace cgo

// end of 027_EightOff.h
