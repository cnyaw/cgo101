
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/12 Waync.
//

#pragma once

namespace cgo {

//
// Bisley.
//

class GameBisley : public CardGame
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_025_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_025_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 0; i < 4; ++i) {
      mList[i].push_back(GetNewCard(1));
    }

    for (int i = 0; i < 13; ++i) {
      if (4 > i) {
        for (int j = 1; j < 4; ++j) {
          mList[i].push_back(GetNewCard());
        }
      } else {
        for (int j = 0; j < 4; ++j) {
          mList[i].push_back(GetNewCard());
        }
      }
    }
  } // NewGame

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    RECT_t rc;
    for (int i = 0; i < 13; ++i) {
      if (4 >= mList[i].size()) {

        for (size_t j = 0; j < mList[i].size(); ++j) {
          if (j == mList[i].size() - 1 && i == mSel) {
            break;
          }
          GetRect(i, j, rc);
          g.DrawCard(rc.left, rc.top, mList[i][j]);
        }

      } else {

        for (size_t j = 0; j < 3; ++j) {
          GetRect(i, j, rc);
          g.DrawCard(rc.left, rc.top, mList[i][j]);
        }

        GetRect(i, 3, rc);

        if (i == mSel) {
          g.DrawCard(rc.left, rc.top, mList[i][mList[i].size() - 2]);
        } else {
          g.DrawCard(rc.left, rc.top, mList[i].back());
        }
      }
    }

    if (-1 != mSel) {
      g.DrawCard(mPtCurr.x, mPtCurr.y, mList[mSel].back());
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    RECT_t rc;
    for (int i = 0; i < 13; ++i) {

      if (1 == mList[i].size() && 4 > i) {
        continue;
      }
      GetRect(i, (std::min)(3, (int)mList[i].size() - 1), rc);
      if (!rc.ptInRect(point)) {
        continue;
      }

      mSel = i;

      mPtDown.x = point.x - rc.left;
      mPtDown.y = point.y - rc.top;

      break;
    }

    return false;
  } // OnMouseDown

  virtual bool OnMouseUp(POINT_t point, RECT_t const& rcClient)
  {
    if (-1 == mSel) {
      return false;
    }

    RECT_t rc;

    //
    // Drop on first 4 deck.
    //

    for (int i = 0; i < 4; ++i) {

      GetRect(i, 0, rc);
      if (!rc.ptInRect( point)) {
        continue;
      }

      int number1 = GetNumber(mList[i][0]);
      int color1 = GetColor(mList[i][0]);
      int number2 = GetNumber(mList[mSel].back());
      int color2 = GetColor(mList[mSel].back());

      if (color1 != color2) {
        goto end;
      }

      if (1 == number1 && 13 == number2) { // Stack K on A.
        mList[i][0] = mList[mSel].back();
        mList[mSel].pop_back();
      } else if (number1 == number2 + 1 || number1 == number2 - 1) {

        //
        // Stack by increases or decreases.
        //

        mList[i][0] = mList[mSel].back();
        mList[mSel].pop_back();
      }

      goto end;
    }

    //
    // Drop on column tail.
    //

    for (int i = 0; i < 13; ++i) {

      if (mList[i].empty()) {
        continue;
      }
      GetRect(i, (std::min)(3, (int)mList[i].size() - 1), rc);
      if (!rc.ptInRect(point)) {
        continue;
      }

      if (mSel == i) {
        break;
      }

      int number1 = GetNumber(mList[i].back());
      int color1 = GetColor(mList[i].back());
      int number2 = GetNumber(mList[mSel].back());
      int color2 = GetColor(mList[mSel].back());

      if (color1 != color2) {
        goto end;
      }

      if (number1 == number2 + 1 || number1 == number2 - 1) {

        //
        // Stack by increases or decreases.
        //

        mList[i].push_back(mList[mSel].back());
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

  void GetRect(int i, int j, RECT_t& rc) const
  {
    rc.left = 4 + OFFSETW * i;
    rc.top = 4 + OFFSETH * j;
    rc.right = rc.left + CARD_WIDTH;
    rc.bottom = rc.top + CARD_HEIGHT;
  } // GetRect
};

} // namespace cgo

// end of 025_Bisley.h
