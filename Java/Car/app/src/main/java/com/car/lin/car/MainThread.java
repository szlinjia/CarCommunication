/*Class name: MainThread
 *Author: Li Lin & Shaomin Zhang
 *Date: 07/06/2014
 *Describe: Toooooooo tired to write detail. I will optimize the drawing algorithm later
 *after Summer1. The spiders run not very smoothly. And there are some small bugs in 
 *program when it runs a long time. 
 *
 */

package com.car.lin.car;

import java.util.List;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.TextView;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;

public class MainThread extends Thread 
{
    private boolean isRunning = false;
    private static final Object lock = new Object();
    private Handler m_handler = null;

    public MainThread ()
	{
	}

	public void setRunning(boolean b)
	{
		isRunning = b;
	}

    public void setControls(MainCarActivity.MyHandler _handler)
    {
        m_handler = _handler;
    }
	
	@Override
	public void run() 
	{
        Log.i("Car", "Thread starts to run.");
        try {
                DatagramSocket clientsocket=new DatagramSocket(9876);
                byte[] receivedata = new byte[1024];
                while(isRunning)
                {
                    DatagramPacket recv_packet = new DatagramPacket(receivedata, receivedata.length);
                    //Log.i("Car", "Wait for reciving data.");
                    clientsocket.receive(recv_packet);
                    Message msg = Message.obtain();

                    String rec_str = new String(recv_packet.getData());
                    String data = rec_str.substring(0,5);
                    String params[] = data.split(" ");
                    //Log.i("Car", "Recive data:" + data);
                    InetAddress ipaddress = recv_packet.getAddress();
                    int port = recv_packet.getPort();
                    //Log.i("Car","IPAddress : "+ipaddress.toString());
                    //Log.i("Car", " Port : " + Integer.toString(port));
                    //Log.i("Car", "Received_from(" + ipaddress.toString() + ":" +
                    //        port + "): " + data);

                    msg.what = Integer.parseInt(params[0])+1000;
                    msg.arg1 = Integer.parseInt(params[1]);
                    msg.arg2 = Integer.parseInt(params[2]);

                    //Log.i("Car","start to send:"+msg.what);

                    if(m_handler != null)
                    {
                        //Log.i("Car","send msg to main activity.");
                        m_handler.sendMessage(msg);

                    }
                    sleep(1000);
                }
        } catch (Exception e) {
            Log.e("Car", "S: Error", e);
        }
	}
}
