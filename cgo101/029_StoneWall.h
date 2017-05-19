
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/09/28 Waync.
//

#pragma once

namespace cgo {

//
// Stone Wall.
//

class GameStoneWall : public CardGame
{
public:

  int mSelIndex;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_029_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_029_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 0; i < 4; ++i) {
      mList[0].push_back(-1);
    }

    for (int i = 1; i <= 6; ++i) {
      for (int j = 0; j < 6; ++j) {
        mList[i].push_back(
                   GetNewCard() + (0 == j || 2 == j || 4 == j ? 100 : 0));
      }
    }

    for (int i = 0; i < 16; ++i) {
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
      int card = mList[0][i];
      if (-1 == card) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else if (13 == GetNumber(card)) {
        g.DrawCover(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, card);
      }
    }

    for (int i = 1; i <= 6; ++i) {

      GetDeckRect(i - 1, rcDeck);
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

    for (int i = 0; i < 16; ++i) {
      GetReserveRect(i, rcDeck);
      if (-1 == mList[10][i] || i + 10 == mSel) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[10][i]);
      }
    }

    if (-1 != mSel) {
      if (10 <= mSel) {
        g.DrawCard(mPtCurr.x, mPtCurr.y, mList[10][mSel - 10]);
      } else {
        for (int i = mSelIndex; i < (int)mList[mSel].size(); ++i) {
          g.DrawCard(mPtCurr.x, mPtCurr.y + 22 * (i - mSelIndex), mList[mSel][i]);
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

    for (int i = 1; i <= 6; ++i) {

      if (mList[i].empty()) {
        continue;
      }

      GetDeckRect(i - 1, rcDeck);
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

    for (int i = 0; i < 16; ++i) {

      GetReserveRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      mSel = 10 + i;

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

    int card1 = 10 <= mSel ? mList[10][mSel - 10] : mList[mSel][mSelIndex];
    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    RECT_t rcDeck;

    //
    // On solitaire.
    //

    for (int i = 0; i < 4; ++i) {

      GetSolitaireRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      int card2 = mList[0][i];
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if ((-1 == card2 && 1 == number1) ||
          (color1 == color2 && number1 == number2 + 1)) {
        if (10 <= mSel) {
          mList[0][i] = card1;
          mList[10][mSel - 10] = -1;
        } else if (mSelIndex == (int)mList[mSel].size() - 1) {
          mList[0][i] = card1;
          mList[mSel].pop_back();
        }
      }

      goto end;
    }

    //
    // On deck.
    //

    for (int i = 1; i <= 6; ++i) {

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
        if (10 <= mSel) {
          mList[i].push_back(mList[10][mSel - 10]);
          mList[10][mSel - 10] = -1;
        } else {
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

      int color2 = GetColor(mList[i].back());
      int number2 = GetNumber(mList[i].back());

      if (color1 != color2 &&
          color1 + color2 != 3 &&
          number1 + 1 == number2) {
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

  void GetSolitaireRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * iDeck;
    rcDeck.top = 4;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetSolitaireRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * iDeck;
    rcDeck.top = 4 + OFFSETH;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect

  void GetReserveRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * (6 + (iDeck % 4));
    rcDeck.top = 4 + OFFSETH * (iDeck / 4);
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetReserveRect
};

} // namespace cgo

// end of 029_StoneWall.h
