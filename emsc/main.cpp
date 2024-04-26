//
// main.cpp
// CGO101 web C API.
//
// 2020/7/5 Waync created.
//

#include <time.h>
#include <stdlib.h>

#include <emscripten.h>

#include "../resource.h"

#define CARD_WIDTH 71
#define CARD_HEIGHT 96
#include "../cgo101/cgo101.h"

int mSel = 0;
cgo::CardGame* mGame = 0;
cgo::DrawListCardGameRenderer mRenderer;

extern "C" {

int EMSCRIPTEN_KEEPALIVE cSetClientRect(int width, int height)
{
  mRenderer.rcClient = cgo::RECT_t(0, 0, width, height);
  return 1;
}

int EMSCRIPTEN_KEEPALIVE cNewGame(int sel)
{
  mSel = sel;
  mGame = cgo::CardGameManager::inst().mGames[mSel];
  mGame->NewGame();
  return 1;
}

int EMSCRIPTEN_KEEPALIVE cMouseDown(int x, int y)
{
  if (mGame) {
    return mGame->OnMouseDown(cgo::POINT_t(x, y), mRenderer.rcClient);
  }
  return 0;
}

int EMSCRIPTEN_KEEPALIVE cMouseUp(int x, int y)
{
  if (mGame) {
    return mGame->OnMouseUp(cgo::POINT_t(x, y), mRenderer.rcClient);
  }
  return 0;
}

int EMSCRIPTEN_KEEPALIVE cMouseMove(int x, int y)
{
  if (mGame) {
    return mGame->OnMouseMove(cgo::POINT_t(x, y), mRenderer.rcClient);
  }
  return 0;
}

int EMSCRIPTEN_KEEPALIVE cDrawGame()
{
  if (mGame) {
    mRenderer.mDrawList.clear();
    mRenderer.EraseBkgnd();
    mGame->DrawGame(mRenderer);
    return (int)mRenderer.mDrawList.size();
  } else {
    return 0;
  }
}

int EMSCRIPTEN_KEEPALIVE cGetDrawGameData(int index)
{
  if (0 > index || (int)mRenderer.mDrawList.size() <= index) {
    return -1;
  } else {
    return mRenderer.mDrawList[index];
  }
}

} // extern "C"

int main(int argc, char* argv[])
{
  srand(time(0));
  emscripten_run_script("ongameload()");
  emscripten_exit_with_live_runtime();
}
