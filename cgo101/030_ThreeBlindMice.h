
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/09/29 Waync.
//

#pragma once

namespace cgo {

//
// Three Blind Mice.
//

class GameThreeBlindMice : public CardGame
{
public:

  int mSelIndex;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_030_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_030_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 1; i <= 7; ++i) {
      for (int j = 0; j < 5; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    for (int i = 8; i <= 10; ++i) {
      for (int j = 0; j < 3; ++j) {
        mList[i].push_back(GetNewCard() + 100);
      }
      for (int j = 0; j < 2; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    mList[0].push_back(GetNewCard());
    mList[0].push_back(GetNewCard());

  } // NewGame

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    RECT_t rcDeck;

    for (int i = 1; i <= 10; ++i) {

      GetDeckRect(i, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {

        if (mSel == i && mSelIndex == j) {
          break;
        }

        if (100 <= mList[i][j]) {
          g.DrawCover(rcDeck.left, rcDeck.top + 24 * j);
        } else {
          g.DrawCard(rcDeck.left, rcDeck.top + 24 * j, mList[i][j]);
        }
      }
    }

    for (int i = 0; i < 2; ++i) {
      GetReserveRect(i, rcDeck);
      if (-1 == mList[0][i] || i + 100 == mSel) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
      }
    }

    if (-1 != mSel) {
      if (100 <= mSel) {
        g.DrawCard(mPtCurr.x, mPtCurr.y, mList[0][mSel - 100]);
      } else {
        for (int i = mSelIndex; i < (int)mList[mSel].size(); ++i) {
          g.DrawCard(mPtCurr.x, mPtCurr.y + 24 * (i - mSelIndex), mList[mSel][i]);
        }
      }
    }
  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    RECT_t rcDeck;

    for (int i = 1; i <= 10; ++i) {

      GetDeckRect(i, rcDeck);
      rcDeck.bottom += 24 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int offset = (rcDeck.bottom - CARD_HEIGHT - point.y);
      int idx;
      if (0 < offset) {
        idx = mList[i].size() - offset / 24 - 2;
      } else {
        idx = mList[i].size() - 1;
      }

      if (100 <= mList[i][idx]) {
        if (idx == mList[i].size() - 1) {
          mList[i][idx] -= 100;
          return true;
        }
        return false;
      }

      mSel = i;
      mSelIndex = idx;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - (rcDeck.top + 24 * mSelIndex);

      return false;
    }

    for (int i = 0; i < 2; ++i) {

      GetReserveRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[0][i]) {
        return false;
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

    int card1 = 100 <= mSel ? mList[0][mSel - 100] : mList[mSel][mSelIndex];
    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    RECT_t rcDeck;
    for (int i = 1; i <= 10; ++i) {

      GetDeckRect(i, rcDeck);
      rcDeck.bottom += 24 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSel == i) {
        goto end;
      }

      if (mList[i].empty()) {

        if (13 == number1) {
end2:
          if (100 <= mSel) {
            mList[i].push_back(card1);
            mList[0][mSel - 100] = -1;
          } else {
            mList[i].insert(
                       mList[i].end(),
                       mList[mSel].begin() + mSelIndex,
                       mList[mSel].end());
            mList[mSel].erase(
                          mList[mSel].begin() + mSelIndex,
                          mList[mSel].end());
          }
        }

        goto end;
      }

      int card2 = mList[i].back();
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 == color2 && number1 + 1 == number2) {
        goto end2;
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

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (iDeck - 1), 4);
  } // GetDeckRect

  void GetReserveRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 12 + OFFSETW * 10, 12 + OFFSETH * iDeck);
  } // GetReserveRect
};

} // namespace cgo

// end of 030_ThreeBlindMice.h
