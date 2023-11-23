
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/04 Waync.
//

#pragma once

namespace cgo {

//
// Simple Simon.
//

class GameSimpleSimon : public CardGame
{
public:

  int mSelIndex;
  int mN;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_013_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_013_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    int const n[] = {8,8,8,7,6,5,4,3,2,1};
    for (int i = 1; i <= 10; ++i) {
      for (int j = 0; j < n[i - 1]; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    mN = 10;
  } // NewGame

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    RECT_t rcDeck;
    for (int i = 1; i <= mN; ++i) {

      GetDeckRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {
        if (mSel == i && mSelIndex == j) {
          break;
        }
        g.DrawCard(rcDeck.left, rcDeck.top + 22 * j, mList[i][j]);
      }
    }

    //
    // Draw select card.
    //

    if (-1 != mSel) {
      for (int i = mSelIndex; i < (int)mList[mSel].size(); ++i) {
        g.DrawCard(mPtCurr.x, mPtCurr.y + 22 * (i - mSelIndex), mList[mSel][i]);
      }
    }
  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    RECT_t rcDeck;
    for (int i = 1; i <= mN; ++i) {

      GetDeckRect(i - 1, rcDeck);
      rcDeck.bottom += 22 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mList[i].empty()) {
        return false;
      }

      int offset = (rcDeck.bottom - CARD_HEIGHT - point.y);
      int idx;
      if (0 < offset) {
        idx = mList[i].size() - offset / 22 - 2;
      } else {
        idx = mList[i].size() - 1;
      }

      if (idx != mList[i].size() - 1) {

        int color1 = GetColor(mList[i][idx]);
        int number1 = GetNumber(mList[i][idx]);

        for (int j = idx + 1; j < (int)mList[i].size(); ++j) {
          int color2 = GetColor(mList[i][j]);
          int number2 = GetNumber(mList[i][j]);
          if (color1 != color2 || number1 != number2 + 1) {
            return false;
          }
          number1--;
        }
      }

      mSel = i;
      mSelIndex = idx;

      mPtDown.x = point.x - rcDeck.left;
      mPtDown.y = point.y - (rcDeck.top + 22 * idx);

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
    for (int i = 1; i <= mN; ++i) {

      GetDeckRect(i - 1, rcDeck);
      rcDeck.bottom += 22 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSel == i) {
        goto end;
      }

      if (mList[i].empty()) {
end2:
        mList[i].insert(
                   mList[i].end(),
                   mList[mSel].begin() + mSelIndex,
                   mList[mSel].end());
        mList[mSel].erase(
                      mList[mSel].begin() + mSelIndex,
                      mList[mSel].end());
        goto end;
      }

      int number1 = GetNumber(mList[i].back());
      int number2 = GetNumber(mList[mSel][mSelIndex]);
      if (number1 == number2 + 1) {
        goto end2;
      }

      goto end;
    }

end:
    mSel = mSelIndex = -1;
    return true;
  } // OnMouseUp

  //
  // Helper.
  //

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * iDeck, 4);
  } // GetDeckRect
};

} // namespace cgo

// end of 013_SimpleSimon.h
