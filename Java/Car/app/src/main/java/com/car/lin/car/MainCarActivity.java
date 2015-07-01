package com.car.lin.car;

import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.ImageView;
import android.util.Log;
import android.widget.Button;
import android.view.View.OnClickListener;
import android.view.View;
import android.view.Window;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.widget.RelativeLayout;
import android.graphics.Matrix;
import android.widget.EditText;

import java.lang.ref.WeakReference;

public class MainCarActivity extends ActionBarActivity {
    private MainThread t = null;
    boolean m_bInit = false;
    CarImageView m_Image;
    EditText m_ctlText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_car);

        m_Image = (CarImageView)findViewById(R.id.imageView1);
        m_Image.getViewTreeObserver().addOnGlobalLayoutListener(mLayoutListener);
        Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(mGlobal_OnClickListener);
        Button button2 = (Button) findViewById(R.id.button2);
        button2.setOnClickListener(mGlobal_OnClickListener);
    }
    public void OnTest()
    {
        //m_Image.SetMoving(1);
        //m_Image.SetMoving(2);
        m_Image.SetMoving(m_Image.crossQ[0]);

        Log.i("Car", "OnClick Test");
    }

    public void OnQuestionB()
    {
        Log.i("Car","OnClick Qb");
        m_Image.isSameDirection = true;
        m_Image.m_crossIndex = 3;
        m_Image.SetMoving(4);
        m_Image.SetMoving(3);


    }
    //Global On click listener for all views
    final OnClickListener mGlobal_OnClickListener = new OnClickListener()
    {
        @Override
        public void onClick(final View v)
        {
            switch(v.getId())
            {
                case R.id.button:
                    OnTest();
                    break;
                case R.id.button2:
                    OnQuestionB();
                    break;
                default:
                    break;
            }
        }
    };

    final OnGlobalLayoutListener mLayoutListener = new OnGlobalLayoutListener()
    {
        @Override
        public void onGlobalLayout()
        {
            if(!m_bInit)
            {
                int CenterX = m_Image.getWidth();
                int x = m_Image.getLeft();
                Log.i("Car", "layout initialize."+CenterX + ","+x);
                Bitmap[] images = new Bitmap[]{
                        BitmapFactory.decodeResource(getResources(), R.drawable.bg),
                        BitmapFactory.decodeResource(getResources(), R.drawable.car1),
                        BitmapFactory.decodeResource(getResources(), R.drawable.car2),
                        BitmapFactory.decodeResource(getResources(), R.drawable.car3),
                        BitmapFactory.decodeResource(getResources(), R.drawable.car4),
                };
                for(int i=0;i<5;i++)
                {
                    m_Image.addLayer(i,images[i]);
                }
                m_Image.reset();
                m_bInit = true;
                t.setControls(mHandler);
            }
        }
    };

        @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main_car, menu);
        return true;
    }

    @Override
    protected void onPause ()
    {
        Log.i("Car", "OnPause");
        super.onPause();
        t.setRunning(false);
        while (true)
        {
            try
            {
                t.join();
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
            break;
        }
        t = null;
        finish();
    }

    @Override
    protected void onResume ()
    {
        Log.i("Car", "OnResume");
        super.onResume();
        t = new MainThread ();
        t.setRunning(true);
        t.start();
    }

    public static class MyHandler extends Handler {
        private final WeakReference<MainCarActivity> mActivity;

        public MyHandler(MainCarActivity activity) {
            mActivity = new WeakReference<MainCarActivity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            MainCarActivity activity = mActivity.get();

            if (activity != null) {

                int ID = msg.what - 1000;
                int state = msg.arg1;
                int direction = msg.arg2;
                Log.i("Car","receive thread msg:"+ID+";"+state);
                if (0 <= ID && ID < 4) {
                    if (state == 1 /*Moving*/) {
                        activity.m_Image.SetMoving(ID + 1);
                    }
                } else {
                    Log.e("Car", "CarID in message is wrong!");
                }

            }
        }
    }

    public final MyHandler mHandler = new MyHandler(this);
}
