package org.town.android.simple_collector;

import android.annotation.TargetApi;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.Environment;
import android.os.IBinder;
import android.os.PowerManager;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;

public class Collect_Service extends Service implements SensorEventListener {
    SensorManager manager = null;
    Sensor sensor = null;
    PrintWriter myFile=null;
    PrintWriter myFile2=null;
    double [] x;
    double [] y;
    double [] z;
    long[] time;
    long[] now_time;
    int i;
    long temp=0;
    long temp2=0;
    PowerManager pm= null;
    PowerManager.WakeLock wl=null;
    public Collect_Service() {
    }

    @Override
    public void onCreate() {
        super.onCreate();


    }


    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(intent==null){
            return Service.START_STICKY;
        }
        else if(myFile==null){/*
            x = new double[180000];
            y = new double[180000];
            z = new double[180000];
            time = new long[180000];
            now_time = new long[180000];

            i = 0;
            String filename= intent.getStringExtra("file");
            String sdcardFolder = Environment.getExternalStorageDirectory().getAbsolutePath();
            String filepath = sdcardFolder + File.separator + filename;
            if (myFile == null) {
                try {
                    myFile = new PrintWriter(new FileOutputStream(filepath + ".txt"));
                    // myFile2 = new PrintWriter(new FileOutputStream(filepath + "22.txt"));
                } catch (FileNotFoundException e) {
                    Toast.makeText(this, "저장할 파일을 생성 할 수 없습니다..", Toast.LENGTH_SHORT).show();
                }
            }*/

            startForeground(1, new Notification());
            pm=(PowerManager)getSystemService(Context.POWER_SERVICE);
            wl = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,"tag");
            wl.acquire();
            manager = (SensorManager) getSystemService(SENSOR_SERVICE);
            sensor = manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
            manager.registerListener(this, sensor,SensorManager.SENSOR_DELAY_GAME);
            //myFile2.println("index gap1 gap2");
        }
        else{
            Toast.makeText(this, "측정이 이미 진행중입니다...", Toast.LENGTH_SHORT).show();
        }

        return super.onStartCommand(intent, flags, startId);
    }

    @TargetApi(Build.VERSION_CODES.N)
    @Override
    public void onDestroy() {

        if(myFile!=null) {
            manager.unregisterListener(this);
            /*myFile.printf("x y z time time2 system_time\n");
            myFile.printf("%f %f %f %d\n",x[0],y[0],z[0],time[0]);
            long last=time[0];
            long last2=now_time[0];
            for(int j=1;j<i;j++){
                myFile.printf("%f %f %f %d %d %d\n",x[j],y[j],z[j],time[j],time[j]-last,(now_time[j]-last2));
                last=time[j];
                last2=now_time[j];
            }
            myFile.close();*/
            wl.release();
            stopForeground(STOP_FOREGROUND_REMOVE);
        }
        //myFile=null;
        //myFile2.close();
        //myFile2=null;


        super.onDestroy();
    }
    public  void onAccuracyChanged(Sensor sensor,int accuracy){

    }

    private long timeDiff = 0l;
    public void onSensorChanged(SensorEvent event){
        SensorQueue.push(new SensorValue(event.timestamp,event.values[0],event.values[1],event.values[2]));
     /*   if (i == 0) {
            long miliTime = System.currentTimeMillis();

            long nanoTime = event.timestamp;

            timeDiff = miliTime - nanoTime / 1000000;
            Log.d("test","Synchornizing sensor clock. Current time= " + miliTime
                    + ", difference between clocks = " + timeDiff);
        }

        x[i]=event.values[0];
        y[i]=event.values[1];
        z[i]=event.values[2];
        long set1=event.timestamp / 1000000 + timeDiff;
        long set2=System.currentTimeMillis();


        //long timeGap = (set1 - temp)/1000000;
        //long timeGap2 = set2 - temp2;

        ///
        /// print timeGap and timeGap2
        ///
      /*  boolean check = false;
        if(Math.abs(timeGap) > 10000) {
            //Log.d(String.valueOf(Log.DEBUG),"TimeGap > 1000" + timeGap);
            myFile2.printf("%d %d %d\n",i,timeGap,timeGap2);
            check=true;
        }
        if(Math.abs(timeGap2) > 10000&&check==false) {
            myFile2.printf("%d %d %d\n",i,timeGap,timeGap2);

        }
        temp = set1;
        temp2 = set2;
        time[i]=set1;
        now_time[i]=set2;
        i++;
        if(i==180000){
            onDestroy();
        }*/
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
