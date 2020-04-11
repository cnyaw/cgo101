/*
  Cgo101Activity.java

  Copyright (c) 2016 Waync Cheng.
  All Rights Reserved.

  2016/5/23 Waync created
 */

package weilican.cgo101;

import android.app.Activity;
import android.os.Bundle;
import android.net.Uri;
import android.content.Context;
import android.content.Intent;
import android.view.*;
import android.graphics.*;
import android.widget.*;
import android.widget.AdapterView.OnItemClickListener;

public class Cgo101Activity extends Activity
{
  static final String[] name_values = {
    "001_Accordion",
    "002_MonteCarlo",
    "003_BlockEleven",
    "004_Calculation",
    "005_LittleSpider",
    "006_PussInTheConer",
    "007_Pyramid",
    "008_EasyGo",
    "009_Scorpion",
    "010_SirTommy",
    "011_SixBySix",
    "012_LeCadran",
    "013_SimpleSimon",
    "014_Klondike",
    "015_MrsMop",
    "016_RoyalParade",
    "017_Martha",
    "018_BakersDozen",
    "019_Golf",
    "020_KingAlbert",
    "021_Diavolo",
    "022_EightByEight",
    "023_Labyrinth",
    "024_Dieppe",
    "025_Bisley",
    "026_AboveAndBelow",
    "027_EightOff",
    "028_TheCarpet",
    "029_StoneWall",
    "030_ThreeBlindMice",
    "031_Vacancies",
    "032_Raglan",
    "033_KingsWay",
    "034_Capricious",
  };

  static final String[] wiki_values = {
    "Accordion_(solitaire)",
    "Monte_Carlo_(solitaire)",
    "", // 003_BlockEleven
    "Calculation_(card_game)",
    "Little_Spider",
    "Puss_in_the_Corner",
    "Pyramid_(solitaire)",
    "", // 008_EasyGo
    "Scorpion_(solitaire)",
    "Sir_Tommy",
    "Six_By_Six",
    "Forty_Thieves_(card_game)",
    "Simple_Simon_(solitaire)",
    "Klondike_(solitaire)",
    "Mrs._Mop",
    "Virginia_Reel_(solitaire)",
    "Martha_(solitaire)",
    "Baker%27s_Dozen_(solitaire)",
    "Golf_(patience)",
    "King_Albert_(solitaire)",
    "", // 021_Diavolo
    "", // 022_EightByEight
    "Labyrinth_(solitaire)",
    "", // 024_Dieppe
    "Bisley_(solitaire)",
    "", // 026_AboveAndBelow
    "Eight_Off",
    "Carpet_(solitaire)",
    "Stonewall_(solitaire)",
    "Scorpion_(solitaire)",
    "Gaps",
    "", // 032_Raglan
    "", // 033_KingsWay
    "Capricieuse",
  };

  int mSelGame = -1;
  float mScaleFactor = 1.5f;
  int mViewW, mViewH;

  void newGame() {
    JniLib.newGame(mSelGame);
    setContentView(new Cgo101View(this));
  }

  void selGame() {
    mSelGame = -1;
    ListView listView = new ListView(this);
    setContentView(listView);
    ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, android.R.id.text1, name_values);
    listView.setAdapter(adapter);
    listView.setOnItemClickListener(new OnItemClickListener() {
      @Override
      public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        mSelGame = position;
        newGame();
        setTitle(getResources().getString(R.string.app_name) + " - " + name_values[position]);
      }
    });
  }

  @Override
  protected void onCreate(Bundle b)
  {
    super.onCreate(b);
    selGame();
  }

  @Override
  public boolean onPrepareOptionsMenu(Menu menu) {
    menu.clear();
    if (-1 != mSelGame) {
      menu.add(0, 0, 0, "New Game");
      menu.add(0, 1, 1, "Game Menu");
      if (!wiki_values[mSelGame].isEmpty()) {
        menu.add(0, 2, 2, "Help");
      }
    }
    return super.onPrepareOptionsMenu(menu);
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
    switch (item.getItemId())
    {
    case 0:
      newGame();
      return true;
    case 1:
      selGame();
      return true;
    case 2:
      startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://en.wikipedia.org/wiki/" + wiki_values[mSelGame])));
      return true;
    }
    return super.onOptionsItemSelected(item);
  }

  @Override
  public boolean onKeyDown(int keyCode, KeyEvent event) {
    if (KeyEvent.KEYCODE_BACK == keyCode && -1 != mSelGame) {
      openOptionsMenu();
      return true;
    }
    return super.onKeyDown(keyCode, event);
  }

  public class Cgo101View extends View {

    Bitmap mCards;
    static final int CW = 36;
    static final int CH = 49;
    ScaleGestureDetector mScaleDetector;

    public Cgo101View(Context context) {
      super(context);
      BitmapFactory.Options options = new BitmapFactory.Options();
      options.inScaled = false;
      mCards = BitmapFactory.decodeResource(context.getResources(), R.drawable.card1, options);
      mScaleDetector = new ScaleGestureDetector(context, new ScaleListener());
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
      super.onSizeChanged(w, h, oldw, oldh);
      JniLib.setClientRect((int)(w / mScaleFactor), (int)(h / mScaleFactor));
      mViewW = w;
      mViewH = h;
    }

    void drawCard(Canvas canvas, int left, int top, int suit, int number) {
      Rect src = new Rect(0,0,73,99);
      src.offset(number * 73, suit * 99);
      RectF dst = new RectF(left, top, left + CW, top + CH);
      canvas.drawBitmap(mCards, src, dst, null);
    }

    @Override
    protected void onDraw(Canvas canvas) {
      super.onDraw(canvas);
      canvas.save();
      canvas.scale(mScaleFactor, mScaleFactor);
      int drawList[] = JniLib.drawGame();
      for (int i = 0; i < drawList.length; i++) {
        switch (drawList[i])
        {
        case 0: // erase bk.
          {
            Paint paint = new Paint();
            paint.setStyle(Paint.Style.FILL);
            paint.setColor(Color.rgb(0, 128, 0));
            canvas.drawPaint(paint);
          }
          break;
        case 1: // empty.
          {
            Paint paint = new Paint();
            paint.setStyle(Paint.Style.STROKE);
            paint.setColor(Color.WHITE);
            paint.setStrokeWidth(2);
            int left = drawList[i + 1];
            int top = drawList[i + 2];
            RectF rc = new RectF(left, top, left + CW, top + CH);
            rc.inset(1, 1);
            canvas.drawRoundRect(rc, 5, 5, paint);
            i += 2;
          }
          break;
        case 2: // cover.
          {
            int left = drawList[i + 1];
            int top = drawList[i + 2];
            drawCard(canvas, left, top, 2, 13);
            i += 2;
          }
          break;
        case 3: // card.
          {
            int left = drawList[i + 1];
            int top = drawList[i + 2];
            int card = drawList[i + 3];
            int suit = card / 13;
            int number = card % 13;
            drawCard(canvas, left, top, suit, number);
            i += 3;
          }
          break;
        }
      }
      canvas.restore();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
      mScaleDetector.onTouchEvent(event);
      int x = (int)(event.getX() / mScaleFactor);
      int y = (int)(event.getY() / mScaleFactor);
      switch (event.getAction())
      {
      case MotionEvent.ACTION_DOWN:
        JniLib.mouseDown(x, y);
        //invalidate();
        break;
      case MotionEvent.ACTION_MOVE:
        JniLib.mouseMove(x, y);
        invalidate();
        break;
      case MotionEvent.ACTION_UP:
        JniLib.mouseUp(x, y);
        invalidate();
        break;
      }
      return true;
    }

    class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
      @Override
      public boolean onScale(ScaleGestureDetector detector) {
        mScaleFactor *= detector.getScaleFactor();
        JniLib.setClientRect((int)(mViewW / mScaleFactor), (int)(mViewH / mScaleFactor));
        invalidate();
        return true;
      }
    }
  }
}
