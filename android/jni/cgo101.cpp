/*
  cgo101.cpp
  Card Game for One.

  Copyright (c) 2016 Waync Cheng.
  All Rights Reserved.

  2016/5/21 Waync created
 */

#include <jni.h>
#include <android/log.h>

#include <time.h>

#include <vector>

#define CARD_WIDTH 36
#define CARD_HEIGHT 49

#include "../../resource.h"
#include "../../cgo101/cgo101.h"

class AndroidCardGameRenderer : public cgo::CardGameRenderer
{
public:

  std::vector<int> mDrawList;

  void DrawCard(int x, int y, int card)
  {
    mDrawList.push_back(3);
    mDrawList.push_back(x);
    mDrawList.push_back(y);
    mDrawList.push_back(card);
  }

  void DrawCover(int x, int y)
  {
    mDrawList.push_back(2);
    mDrawList.push_back(x);
    mDrawList.push_back(y);
  }

  void DrawEmptyCard(int x, int y)
  {
    mDrawList.push_back(1);
    mDrawList.push_back(x);
    mDrawList.push_back(y);
  }

  void EraseBkgnd()
  {
    mDrawList.push_back(0);
  }
};

int mSel = 0;
cgo::CardGame* mGame = 0;
cgo::RECT_t mClientRect;

extern "C" {

JNIEXPORT void JNICALL Java_weilican_cgo101_JniLib_setClientRect(JNIEnv * env, jobject obj, jint width, jint height)
{
  mClientRect = cgo::RECT_t(0, 0, width, height);
}

JNIEXPORT void JNICALL Java_weilican_cgo101_JniLib_newGame(JNIEnv * env, jobject obj, jint sel)
{
  srand(time(0));
  mSel = sel;
  mGame = cgo::CardGameManager::inst().mGames[mSel];
  mGame->NewGame();
}

JNIEXPORT void JNICALL Java_weilican_cgo101_JniLib_mouseDown(JNIEnv * env, jobject obj, jint x, jint y)
{
  if (mGame) {
    mGame->OnMouseDown(cgo::POINT_t(x, y), mClientRect);
  }
}

JNIEXPORT void JNICALL Java_weilican_cgo101_JniLib_mouseUp(JNIEnv * env, jobject obj, jint x, jint y)
{
  if (mGame) {
    mGame->OnMouseUp(cgo::POINT_t(x, y), mClientRect);
  }
}

JNIEXPORT void JNICALL Java_weilican_cgo101_JniLib_mouseMove(JNIEnv * env, jobject obj, jint x, jint y)
{
  if (mGame) {
    mGame->OnMouseMove(cgo::POINT_t(x, y), mClientRect);
  }
}

JNIEXPORT jintArray JNICALL Java_weilican_cgo101_JniLib_drawGame(JNIEnv * env, jobject obj)
{
  if (mGame) {
    AndroidCardGameRenderer g;
    g.rcClient = mClientRect;
    g.EraseBkgnd();
    mGame->DrawGame(g);
    jintArray iarr = env->NewIntArray(g.mDrawList.size());
    env->SetIntArrayRegion(iarr, 0, g.mDrawList.size(), (const jint*)&g.mDrawList[0]);
    return iarr;
  } else {
    return (jintArray)0;
  }
}

};
