
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/09/29 Waync.
//

#pragma once

namespace cgo {

//
// Vacancies.
//

class GameVacancies : public CardGame
{
public:

  int mCountNew;

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_031_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_031_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 0; i < 52; ++i) {
      int card = GetNewCard();
      mList[0].push_back(1 == GetNumber(card) ? -1 : card);
    }

    mCountNew = 3;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    if (0 == mCountNew) {
      return false;
    }

    mCountNew--;

    for (int i = 0; i < 52; ++i) {
      int card = mList[0][i];
      if (-1 != card && GetNumber(card) != (2 + (i % 13))) {
        mCards.push_back(card);
        mList[0][i] = -1;
      }
    }

    std::random_shuffle(mCards.begin(), mCards.end(), myrand);

    for (int i = 0; i < 52; ++i) {

      if (mCards.empty()) {
        break;
      }

      if (-1 != mList[0][i]) {
        continue;
      }

      if (0 == i % 13) {
        continue;
      }

      mList[0][i] = GetNewCard();
    }

    return true;
  } // NewCard

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    if (0 == mCountNew) {
      g.DrawEmptyCard(4, 4 + 4 * OFFSETH);
    } else {
      g.DrawCover(4, 4 + 4 * OFFSETH);
    }

    RECT_t rcDeck;
    for (int i = 0; i < 52; ++i) {
      GetDeckRect(i, rcDeck);
      if (-1 == mList[0][i] || i == mSel) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, mList[0][i]);
      }
    }

    if (-1 != mSel) {
      g.DrawCard(mPtCurr.x, mPtCurr.y, mList[0][mSel]);
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    RECT_t rcNew(4, 4 + 4 * OFFSETH, 4 + CARD_WIDTH, 4 + 4 * OFFSETH + CARD_HEIGHT);
    if (rcNew.ptInRect(point)) {
      return NewCard();
    }

    RECT_t rcDeck;
    for (int i = 0; i < 52; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (-1 == mList[0][i]) {
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

    int card1 = mList[0][mSel];
    int color1 = GetColor(card1);
    int number1 = GetNumber(card1);

    RECT_t rcDeck;
    for (int i = 0; i < 52; ++i) {

      GetDeckRect(i, rcDeck);
      if (!rcDeck.ptInRect(point)) {
        continue;
      }

      if (mSel == i || -1 != mList[0][i]) {
        goto end;
      }

      if (0 == i % 13) {
        if (2 == number1) {
          std::swap(mList[0][i], mList[0][mSel]);
        }
        goto end;
      }

      int card2 = mList[0][i - 1];
      int color2 = GetColor(card2);
      int number2 = GetNumber(card2);

      if (color1 == color2 && number1 == number2 + 1) {
        std::swap(mList[0][i], mList[0][mSel]);
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

  void GetDeckRect(int i, RECT_t& rcDeck) const
  {
    InitCardRect(rcDeck, 4 + OFFSETW * (i % 13), 4 + OFFSETH * (i / 13));
  } // GetDeckRect
};

} // namespace cgo

// end of 031_Vacancies.h
