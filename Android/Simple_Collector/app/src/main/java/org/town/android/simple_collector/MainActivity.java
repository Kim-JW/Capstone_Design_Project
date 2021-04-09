package org.town.android.simple_collector;

import android.Manifest;
import android.app.Notification;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;

public class MainActivity extends AppCompatActivity {
    Intent intent_collect = null;
    Intent intent_write= null;
    EditText editText = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editText = (EditText)findViewById(R.id.editText);
        SensorQueue.Setup();
    }

    public void button1Clicked(View v){
        String filename = editText.getText().toString();
        if(intent_collect!=null){
            Toast.makeText(getApplicationContext(),"측정이 이미 진행중 입니다.",Toast.LENGTH_LONG).show();
            return ;
        }
        else if(filename.length()>0){
            intent_collect = new Intent(this,Collect_Service.class);
            intent_write = new Intent(this,Write_Service.class);
            intent_write.putExtra("file",filename);
            Toast.makeText(getApplicationContext(),"측정 시작",Toast.LENGTH_LONG).show();
            startService(intent_collect);
            startService(intent_write);
        }
    }
    public void button2Clicked(View v) {
        if(intent_collect==null){
            Toast.makeText(getApplicationContext(),"측정진행중이 아닙니다.",Toast.LENGTH_LONG).show();
        }
        else {
            stopService(intent_collect);
            stopService(intent_write);
            Toast.makeText(this, "측정 종료,,,..", Toast.LENGTH_SHORT).show();
            intent_collect=null;
            intent_write=null;

        }
        //end_record();
    }
    /*public  void onAccuracyChanged(Sensor sensor,int accuracy){

    }

    private long timeDiff = 0l;
    public void onSensorChanged(SensorEvent event){
        if (i == 0) {
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
        temp = set1;
        temp2 = set2;
        time[i]=set1;
        now_time[i]=set2;
        i++;
        if(i==180000){
            end_record();
        }
    }
    public void end_record(){
        if(myFile!=null) {
            Toast.makeText(this, "측정종료..", Toast.LENGTH_LONG).show();

            manager.unregisterListener(this);
            myFile.printf("x y z differ time2 system_time\n");
            myFile.printf("%f %f %f %d\n",x[0],y[0],z[0],time[0]);
            long last=time[0];
            long last2=now_time[0];
            for(int j=1;j<i;j++){
                myFile.printf("%f %f %f %d %d %d\n",x[j],y[j],z[j],time[j],time[j]-last,(now_time[j]-last2));
                last=time[j];
                last2=now_time[j];
            }
            myFile.close();
        }
        else{
            Toast.makeText(this, "측정이 진행중이 아닙니다...", Toast.LENGTH_SHORT).show();

        }
        myFile=null;
    }*/

}
