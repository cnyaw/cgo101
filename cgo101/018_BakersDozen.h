
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/07 Waync.
//

#pragma once

namespace cgo {

//
// Baker's Dozen.
//

class GameBakersDozen : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_018_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_018_DESC;
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

    for (int i = 1; i <= 13; ++i) {
      for (int j = 0; j < 4; ++j) {
        mList[i].push_back(GetNewCard());
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
      if (-1 == mList[0][i]) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
      }
    }

    //
    // Draw await.
    //

    for (int i = 1; i <= 13; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      g.DrawEmptyCard(rcDeck.left, rcDeck.top);

      if (mList[i].empty()) {
        continue;
      }

      for (int j = 0; j < (int)mList[i].size(); ++j) {
        if (mSel == i && j == (int)mList[i].size() - 1) {
          break;
        }
        g.DrawCard(rcDeck.left, rcDeck.top + 22 * j, mList[i][j]);
      }
    }

    //
    // Draw select.
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
    for (int i = 1; i <= 13; ++i) {

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
      if (!rcDeck.ptInRect(point))
          continue;

      if (-1 == mList[0][i]) {
        if (1 == GetNumber(mList[mSel].back())) {
          mList[0][i] = mList[mSel].back();
          mList[mSel].pop_back();
        }
        goto end;
      }

      if (GetColor(mList[0][i]) != GetColor(mList[mSel].back())) {
        goto end;
      }

      if (GetNumber(mList[0][i]) + 1 != GetNumber(mList[mSel].back())) {
        goto end;
      }

      mList[0][i] = mList[mSel].back();
      mList[mSel].pop_back();

      goto end;
    }

    //
    // Drop on await.
    //

    for (int i = 1; i <= 13; ++i) {

      GetAwaitRect(i - 1, rcDeck);
      rcDeck.bottom += 22 * (mList[i].size() - 1);

      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSel == i) {
        goto end;
      }

      if (mList[i].empty()) {
        goto end2;
      }

      if (GetNumber(mList[i].back()) != GetNumber(mList[mSel].back() + 1)) {
        goto end;
      }

end2:
      mList[i].push_back(mList[mSel].back());
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

// end of 018_BakersDozen.h
