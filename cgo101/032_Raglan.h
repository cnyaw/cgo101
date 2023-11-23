
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/09/30 Waync.
//

#pragma once

namespace cgo {

//
// Raglan.
//

class GameRaglan : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_032_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_032_DESC;
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

    for (int i = 1; i <= 3; ++i) {
      for (int j = 0; j < 7; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    for (int i = 4; i <= 9; ++i) {
      for (int j = 0; j < 9 - i + 1; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    for (int i = 0; i < 6; ++i) {
      mList[10].push_back(GetNewCard());
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

    for (int i = 1; i <= 9; ++i) {

      GetDeckRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {
        if (mSel == i && (int)mList[i].size() - 1 == j) {
          break;
        }
        g.DrawCard(rcDeck.left, rcDeck.top + 22 * j, mList[i][j]);
      }
    }

    for (int i = 0; i < 6; ++i) {
      GetReserveRect(i, rcDeck);
      if (-1 == mList[10][i] || 10 + i == mSel) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[10][i]);
      }
    }

    if (-1 != mSel) {
      int card = 10 <= mSel ? mList[10][mSel - 10] : mList[mSel].back();
      g.DrawCard(mPtCurr.x, mPtCurr.y, card);
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    RECT_t rcDeck;

    for (int i = 1; i <= 9; ++i) {

      GetDeckRect(i - 1, rcDeck);
      rcDeck.offset(0,  22 * (mList[i].size() - 1));

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

    for (int i = 0; i < 6; ++i) {

      GetReserveRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[10][i]) {
        return false;
      }

      mSel = i + 10;

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

    int card1 = 10 <= mSel ? mList[10][mSel - 10] : mList[mSel].back();
    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    RECT_t rcDeck;

    for (int i = 0; i < 4; ++i) {

      GetSolitaireRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int color2 = GetColor(mList[0][i]);
      int number2 = GetNumber(mList[0][i]);

      if (color1 == color2 && number1 == number2 + 1) {
        mList[0][i] = card1;
        if (10 <= mSel) {
          mList[10][mSel - 10] = -1;
        } else {
          mList[mSel].pop_back();
        }
      }

      goto end;
    }

    for (int i = 1; i <= 9; ++i) {

      GetDeckRect(i - 1, rcDeck);
      rcDeck.offset(0,  22 * (mList[i].size() - 1));

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (i == mSel) {
        goto end;
      }

      if (mList[i].empty()) {
end2:
        mList[i].push_back(card1);

        if (10 <= mSel) {
          mList[10][mSel - 10] = -1;
        } else {
          mList[mSel].pop_back();
        }

        goto end;
      }

      int card2 = mList[i].back();
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 == color2 ||
          color1 + color2 == 3 ||
          number2 != number1 + 1) {
        goto end;
      }

      goto end2;
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
    InitCardRect(rcDeck, 4 + OFFSETW * iDeck, 4);
  } // GetSolitaireRect

  void GetDeckRect(int i, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * i, 4 + OFFSETH);
  } // GetDeckRect

  void GetReserveRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 12 + OFFSETW * (9 + (iDeck % 2)), 12 + OFFSETH * (iDeck / 2));
  } // GetReserveRect
};

} // namespace cgo

// end of 032_Raglan.h
