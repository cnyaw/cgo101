
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/07 Waync.
//

#pragma once

namespace cgo {

//
// Martha.
//

class GameMartha : public CardGame
{
public:

  int mSelIndex;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_017_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_017_DESC;
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

    for (int i = 1; i <= 12; ++i) {
      for (int j = 0; j < 4; ++j) {
        mList[i].push_back(GetNewCard() + (0 == j || 2 == j ? 100 : 0));
      }
    }
  } // NewGame

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    //
    // Draw deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {
      GetDeckRect(i, rcDeck);
      g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
    }

    //
    // Draw await.
    //

    for (int i = 1; i <= 12; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {

        if (mSel == i && mSelIndex == j) {
          break;
        }

        if (100 <= mList[i][j]) {
          g.DrawCover(rcDeck.left, rcDeck.top + 22 * j);
        } else {
          g.DrawCard(rcDeck.left, rcDeck.top + 22 * j, mList[i][j]);
        }
      }
    }

    //
    // Draw select.
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
    for (int i = 1; i <= 12; ++i) {

      if (mList[i].empty()) {
        continue;
      }

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 22 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int offset = (rcDeck.bottom - CARD_HEIGHT - point.y);
      int idx;
      if (0 < offset) {
        idx = mList[i].size() - offset / 22 - 2;
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

      if (idx != mList[i].size() - 1) {

        int color1 = GetColor(mList[i][idx]);
        int number1 = GetNumber(mList[i][idx]);

        for (int j = idx + 1; j < (int)mList[i].size(); ++j) {

          int color2 = GetColor(mList[i][j]);
          int number2 = GetNumber(mList[i][j]);

          if (color1 == color2 ||
              color1 + color2 == 3 ||
              number1 != number2 + 1) {
            return false;
          }

          color1 = color2;
          number1 = number2;
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

    //
    // Drop on deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSelIndex == (int)mList[mSel].size() - 1) {
        int card1 = mList[mSel][mSelIndex];
        int color1 = GetColor(card1);
        int number1 = GetNumber(card1);
        int color2 = GetColor(mList[0][i]);
        int number2 = GetNumber(mList[0][i]);
        if (color1 == color2 && number1 == number2 + 1) {
          mList[0][i] = card1;
          mList[mSel].pop_back();
        }
      }

      goto end;
    }

    //
    // Drop on await.
    //

    for (int i = 1; i <= 12; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 22 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSel == i) {
        goto end;
      }

      if (mList[i].empty()) {
        if (mSelIndex == (int)mList[mSel].size() - 1) {
          mList[i].push_back(mList[mSel].back());
          mList[mSel].pop_back();
        }
        goto end;
      }

      int color1 = GetColor(mList[mSel][mSelIndex]);
      int number1 = GetNumber(mList[mSel][mSelIndex]);
      int color2 = GetColor(mList[i].back());
      int number2 = GetNumber(mList[i].back());

      if (color1 != color2 &&
          color1 + color2 != 3 &&
          number1 + 1 == number2) {
        mList[i].insert(
                   mList[i].end(),
                   mList[mSel].begin() + mSelIndex,
                   mList[mSel].end());
        mList[mSel].erase(
                      mList[mSel].begin() + mSelIndex,
                      mList[mSel].end());
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
    InitCardRect(rcDeck, 4 + OFFSETW * iDeck, 4 + OFFSETH);
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * iDeck, 4);
  } // GetDeckRect
};

} // namespace cgo

// end of 017_Martha.h
