package org.town.android.mysensorcollector;

import android.content.Intent;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;

public class MainActivity extends AppCompatActivity {
    EditText editText = null;
    Button start = null;
    Intent intent_collect = null;
    Intent intent_write = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editText = (EditText)findViewById(R.id.editText);
        start = (Button)findViewById(R.id.button);
        SensorQueue.Setup();
    }

    public void button1Clicked(View v){

        String filename = editText.getText().toString();
        if(intent_collect!=null){
            Toast.makeText(getApplicationContext(),"측정이 이미 진행중 입니다.",Toast.LENGTH_LONG).show();
            return ;
        }
        if(filename.length()>0){

            intent_collect = new Intent(this,MyService.class);
            intent_write = new Intent(this,WriteService.class);
            intent_write.putExtra("file",filename);
            intent_write.putExtra("type","walk");
            startService(intent_collect);
            startService(intent_write);
            Toast.makeText(getApplicationContext(),"측정 시작",Toast.LENGTH_LONG).show();
        }
        else{
            Toast.makeText(getApplicationContext(),"저장할 파일명을 입력하세요",Toast.LENGTH_LONG).show();
        }
    }
    public void button2Clicked(View v){

        String filename = editText.getText().toString();
        if(intent_collect!=null){
            Toast.makeText(getApplicationContext(),"측정이 이미 진행중 입니다.",Toast.LENGTH_LONG).show();
            return ;
        }
        if(filename.length()>0){

            intent_collect = new Intent(this,MyService.class);
            intent_write = new Intent(this,WriteService.class);
            intent_write.putExtra("file",filename);
            intent_write.putExtra("type","run");
            startService(intent_collect);
            startService(intent_write);
        }
        else{
            Toast.makeText(getApplicationContext(),"저장할 파일명을 입력하세요",Toast.LENGTH_LONG).show();
        }
    }
    public void button3Clicked(View v){

        String filename = editText.getText().toString();
        if(intent_collect!=null){
            Toast.makeText(getApplicationContext(),"측정이 이미 진행중 입니다.",Toast.LENGTH_LONG).show();
            return ;
        }
        if(filename.length()>0){

            intent_collect = new Intent(this,MyService.class);
            intent_write = new Intent(this,WriteService.class);
            intent_write.putExtra("file",filename);
            intent_write.putExtra("type","transport");
            startService(intent_collect);
            startService(intent_write);
        }
        else{
            Toast.makeText(getApplicationContext(),"저장할 파일명을 입력하세요",Toast.LENGTH_LONG).show();
        }
    }
    public void button4Clicked(View v){
        if(intent_collect==null){
            Toast.makeText(getApplicationContext(),"측정진행중이 아닙니다.",Toast.LENGTH_LONG).show();
        }
        else {
            stopService(intent_collect);
            stopService(intent_write);
            intent_collect=null;
            intent_write=null;
        }
    }





}
