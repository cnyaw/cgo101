
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/05/05 waync
//

#pragma once

namespace cgo {

//
// Mrs Mop.
//

class GameMrsMop : public GameSimpleSimon
{
public:

  //
  // Property.
  //

  virtual int GetName() const
  {
    return IDS_015_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_015_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame2();

    for (int i = 1; i <= 13; ++i) {
      for (int j = 0; j < 8; ++j) {
        mList[i].push_back(GetNewCard());
      }
    }

    mN = 13;
  } // NewGame
};

} // namespace cgo

// end of 015_MrsMop.h
