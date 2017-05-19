
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/08 Waync.
//

#pragma once

namespace cgo {

//
// King Albert.
//

class GameKingAlbert : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_020_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_020_DESC;
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

    for (int i = 1; i <= 9; ++i) {
      for (int j = 0; j < 9 - i + 1; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    for (int i = 0; i < 7; ++i) {
      mList[10].push_back(GetNewCard());
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
      if (-1 == mList[0][i]) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
      }
    }

    //
    // Draw await.
    //

    for (int i = 1; i <= 9; ++i) {

      GetAwaitRect(i - 1, rcDeck);
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

    //
    // Draw stock.
    //

    for (int i = 0; i < 7; ++i) {
      GetStockRect(i, rcDeck);
      if (-1 == mList[10][i] || mSel - 100 == i) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[10][i]);
      }
    }

    //
    // Draw select.
    //

    if (-1 != mSel) {
      int card = 100 <= mSel ? mList[10][mSel - 100] : mList[mSel].back();
      g.DrawCard(mPtCurr.x, mPtCurr.y, card);
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    //
    // Select await?
    //

    RECT_t rcDeck;
    for (int i = 1; i <= 9; ++i) {

      GetAwaitRect(i - 1, rcDeck);
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

    //
    // Select stock?
    //

    for (int i = 0; i < 7; ++i) {

      GetStockRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[10][i]) {
        return false;
      }

      mSel = i + 100;

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

    int card1 = 100 <= mSel ? mList[10][mSel - 100] : mList[mSel].back();
    int number1 = GetNumber(card1);

    //
    // Drop deck.
    //

    RECT_t rcDeck;
    for (int i = 0; i < 4; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[0][i]) {
        if (1 == number1) {
          goto end2;
        }
        goto end;
      }

      if (GetColor(card1) == GetColor(mList[0][i]) &&
          number1 == GetNumber(mList[0][i]) + 1) {
end2:
        mList[0][i] = card1;
        if (100 <= mSel) {
          mList[10][mSel - 100] = -1;
        } else {
          mList[mSel].pop_back();
        }
      }

      goto end;
    }

    //
    // Drop await.
    //

    for (int i = 1; i <= 9; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 22 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSel == i) {
        goto end;
      }

      if (mList[i].empty()) {
end3:
        mList[i].push_back(card1);

        if (100 <= mSel) {
          mList[10][mSel - 100] = -1;
        } else {
          mList[mSel].pop_back();
        }
        goto end;
      }

      int color1 = GetColor(card1);
      int color2 = GetColor(mList[i].back());
      int number2 = GetNumber(mList[i].back());

      if (color1 != color2 &&
          color1 + color2 != 3 &&
          number1 + 1 == number2) {
        goto end3;
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
    rcDeck.left = 4 + OFFSETW * iDeck;
    rcDeck.top = 4 + OFFSETH;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetAwaitRect

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * iDeck;
    rcDeck.top = 4;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetDeckRect

  void GetStockRect(int iDeck, RECT_t& rcDeck) const
  {
    rcDeck.left = 4 + OFFSETW * iDeck;
    rcDeck.top = 4 + OFFSETH * 5;
    rcDeck.right = rcDeck.left + CARD_WIDTH;
    rcDeck.bottom = rcDeck.top + CARD_HEIGHT;
  } // GetStockRect
};

} // namespace cgo

// end of 020_KingAlbert.h
