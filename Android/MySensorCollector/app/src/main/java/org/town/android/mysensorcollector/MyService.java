package org.town.android.mysensorcollector;

import android.app.Service;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Environment;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;

public class MyService extends Service implements SensorEventListener{
    SensorManager manager = null;
    Sensor a_sensor = null;
    PrintWriter myFile=null;
    PrintWriter myFile20=null;
    PrintWriter myFile40=null;//파일 3개 생성
    long myTime = 0;
    int term=1;

    private final  static long timezone = 999999999;
    public MyService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();

        manager = (SensorManager)getSystemService(SENSOR_SERVICE);
        a_sensor= manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(intent==null){
            return Service.START_STICKY;
        }
       /* else if(myFile==null){
            openFile(intent);

        }
        else{
            Toast.makeText(this, "이미 진행중 입니다..,,,..", Toast.LENGTH_SHORT).show();
        }*/
        manager.registerListener(this,a_sensor, 20000);


        return super.onStartCommand(intent, flags, startId);

    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        /*myFile.close();
        myFile=null;
        myFile20.close();
        myFile20=null;
        myFile40.close();
        myFile40=null;*/
        manager.unregisterListener(this);
        Toast.makeText(this, "측정 종료,,,..", Toast.LENGTH_SHORT).show();
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    /*public void openFile(Intent intent){//사용안함
        String filename= intent.getStringExtra("file")+"-"+intent.getStringExtra("type");
        String sdcardFolder = Environment.getExternalStorageDirectory().getAbsolutePath();
        String filepath = sdcardFolder+ File.separator+filename;
        try{
            myFile= new PrintWriter(new FileOutputStream(filepath+"-20ms.txt"));
            myFile20= new PrintWriter(new FileOutputStream(filepath+"-40ms.txt"));
            myFile40= new PrintWriter(new FileOutputStream(filepath+"-80ms.txt"));
        }
        catch (FileNotFoundException e){
            Toast.makeText(this, "저장할 파일을 생성 할 수 없습니다..", Toast.LENGTH_SHORT).show();
        }
        myFile.printf("x y z time\n");
        myFile20.printf("x y z time\n");
        myFile40.printf("x y z time\n");
        Toast.makeText(this, "파일 오픈 완료", Toast.LENGTH_SHORT).show();
    }*/
    public  void onAccuracyChanged(Sensor sensor,int accuracy){

    }
    public void onSensorChanged(SensorEvent event){
        if(myTime==0){
            myTime=event.timestamp;
            //long relative = (event.timestamp-myTime)/1000000;
            /*myFile.printf("%f %f %f",event.values[0],event.values[1],event.values[2]);
            myFile.println(" "+relative);
            myFile.flush();
            myFile20.printf("%f %f %f", event.values[0], event.values[1], event.values[2]);
            myFile20.println(" "+relative);
            myFile20.flush();
            myFile40.printf("%f %f %f", event.values[0], event.values[1], event.values[2]);
            myFile40.println(" "+relative);
            myFile40.flush();*/
            return ;
        }
        long relative = (event.timestamp-myTime)/1000000;
        SensorQueue.push(new SensorValue(relative,event.values[0],event.values[1],event.values[2]));

       /* myFile.printf("%f %f %f",event.values[0],event.values[1],event.values[2]);
        myFile.println(" "+relative);
        myFile.flush();
        if(term==2) {
            myFile20.printf("%f %f %f", event.values[0], event.values[1], event.values[2]);
            myFile20.println(" "+relative);
            myFile20.flush();
            term++;
        }
        else if(term==4){
            myFile20.printf("%f %f %f", event.values[0], event.values[1], event.values[2]);
            myFile20.println(" "+relative);
            myFile20.flush();
            myFile40.printf("%f %f %f", event.values[0], event.values[1], event.values[2]);
            myFile40.println(" "+relative);
            myFile40.flush();
            term=1;
        }
        else{
            term++;
        }*/
    }
}
