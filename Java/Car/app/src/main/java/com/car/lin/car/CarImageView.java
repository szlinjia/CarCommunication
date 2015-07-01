package com.car.lin.car;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.ImageView;
import java.util.ArrayList;

/**
 * Created by Linda on 3/13/2015.
 */

public class CarImageView extends ImageView{
    enum CarState
    {
        Stop,
        Moving,
        RequestToCross,
        Waiting,
    }

    enum CarDirection
    {
        LefttoRight,
        RighttoLeft,
    }

    float init_x = 0;
    float init_y = 0;
    float m_x[] = new float[5];
    float m_y[] = new float[5];
    Bitmap alteredBridge = null;
    float dst_left;
    float dst_right;
    ArrayList<Bitmap> mLayers;
    Matrix m;
    SoundManager m_sound;
    public CarState[] m_state;
    public CarDirection[] m_direction;
    public int[] crossQ = new int[4];
    public int m_crossIndex = 0;
    public boolean isSameDirection = false;

    public CarImageView(Context context) {
        super(context);
    }

    public CarImageView(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.i("Car","Start to init CarImageView1");
        mLayers = new ArrayList<Bitmap>();
        m = new Matrix();
        m_state = new CarState[4];
        for(int i=0;i<4;i++) {
            m_state[i] = CarState.Stop;
        }
        m_direction = new CarDirection[4];
        m_sound = SoundManager.getInstance();
        m_sound.Initialize(getContext());

        crossQ[0] = 4;
        crossQ[1] = 1;
        crossQ[2] = 2;
        crossQ[3] = 3;
    }

    void reset()
    {
        if(mLayers.size() < 5) return;
        Bitmap bridge = mLayers.get(0);
        Bitmap car = mLayers.get(1);
        init_x = 90 + car.getWidth();
        m_x[0] = 0;
        m_y[0] = 0;
        for(int i=1;i<5;i++)
        {
            m_x[i] = init_x + (float)(car.getWidth()+30) *(i-1);
            m_y[i] = init_y;
            m_direction[i-1] = CarDirection.LefttoRight;
            m_state[i-1] = CarState.Stop;
            if(i>2)
            {
                m_y[i] = 1300;
                m_direction[i-1] = CarDirection.RighttoLeft;
            }
            //Log.i("Car","car"+"i"+" coordinate:" + m_x[i]+" "+m_y[i]);
        }

        dst_left = m_y[1];
        dst_right = m_y[3];
    }

    void Reset()
    {
        if(mLayers.size()<5) return;
    }
    public void Move(int index)
    {
        if(m_state[index-1] != CarState.Moving) return;

        if(m_direction[index -1] == CarDirection.LefttoRight)
        {

            if ((m_y[index] - dst_right) < 0.001)
            {
                m_y[index] += 2.5;
            }
            else
            {
                m_direction[index -1] = CarDirection.RighttoLeft;
                m_state[index-1] = CarState.Stop;
                m_sound.playsound("leave");

                //for test
                if(!isSameDirection) {
                    m_crossIndex++;
                    if (m_crossIndex < 4) {
                        SetMoving(crossQ[m_crossIndex]);
                    }
                }
                else
                {
                    m_crossIndex-=2;
                    if(m_crossIndex >= 0)
                    {
                        SetMoving(1);
                        SetMoving(2);
                        m_crossIndex -=2;
                    }
                }
            }
        }
        else if(m_direction[index -1] == CarDirection.RighttoLeft)
        {
            if ((m_y[index] - dst_left) > 0.001) {
                m_y[index] -= 2.5;
            }
            else
            {
                m_direction[index -1] = CarDirection.LefttoRight;
                m_state[index-1] = CarState.Stop;
                m_sound.playsound("leave");

                //for test
                if(!isSameDirection) {
                    m_crossIndex++;
                    if (m_crossIndex < 4) {
                        SetMoving(crossQ[m_crossIndex]);
                    }
                }
                else
                {
                    m_crossIndex-=2;
                    if(m_crossIndex >= 0)
                    {
                        SetMoving(1);
                        SetMoving(2);
                        m_crossIndex -=2;
                    }
                }
            }


        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        DrawBridge(canvas);
        DrawCar(canvas);
    }

    public void DrawBridge(Canvas canvas)
    {
        if(mLayers.size() < 5) return;

        Bitmap b = mLayers.get(0);
        if(alteredBridge == null)
        {
            alteredBridge = Bitmap.createBitmap(b.getWidth(), b
                    .getHeight(), b.getConfig());
        }

        Paint paint = new Paint();
        canvas.drawBitmap(b, m_x[0],m_y[0],paint);
        setImageBitmap(alteredBridge);
    }

    public void DrawCar(Canvas canvas)
    {
        if(mLayers.size() < 5) return;
        for(int i=1;i<5;i++) {
            Bitmap b = mLayers.get(i);
            Move(i);
            Paint paint = new Paint();
            canvas.drawBitmap(b, m_x[i], m_y[i], paint);
        }
    }
    public void addLayer(int idx, Bitmap b) {
        mLayers.add(idx, b);
        invalidate();
    }

    public void SetMoving(int index)
    {
        if(index > 0 && index <5)
        {
            m_state[index-1] = CarState.Moving;
            m_sound.playsound("enter");
        }
    }

}
