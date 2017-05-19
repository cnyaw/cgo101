/*
  JniLib.java

  Copyright (c) 2016 Waync Cheng.
  All Rights Reserved.

  2016/5/23 Waync created
 */

package weilican.cgo101;

public class JniLib {

  static {
    System.loadLibrary("cgo101");
  }

  public static native void setClientRect(int w, int h);
  public static native void newGame(int sel);
  public static native void mouseDown(int x, int y);
  public static native void mouseUp(int x, int y);
  public static native void mouseMove(int x, int y);
  public static native int[] drawGame();
}
