package org.town.android.simple_collector;

import android.app.IntentService;
import android.app.Notification;
import android.content.Intent;
import android.content.Context;
import android.os.Environment;
import android.os.PowerManager;
import android.util.Log;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */
public class Write_Service extends IntentService {
    // TODO: Rename actions, choose action names that describe tasks that this
    // IntentService can perform, e.g. ACTION_FETCH_NEW_ITEMS
    private static final String ACTION_FOO = "org.town.android.simple_collector.action.FOO";
    private static final String ACTION_BAZ = "org.town.android.simple_collector.action.BAZ";

    // TODO: Rename parameters
    private static final String EXTRA_PARAM1 = "org.town.android.simple_collector.extra.PARAM1";
    private static final String EXTRA_PARAM2 = "org.town.android.simple_collector.extra.PARAM2";
    PrintWriter myFile=null;
    long last=0;
    long mytime=0;
    PowerManager pm= null;
    PowerManager.WakeLock wl=null;
    boolean cont=true;

    public Write_Service() {
        super("Write_Service");
    }

    /**
     * Starts this service to perform action Foo with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    // TODO: Customize helper method
    public static void startActionFoo(Context context, String param1, String param2) {
        Intent intent = new Intent(context, Write_Service.class);
        intent.setAction(ACTION_FOO);
        intent.putExtra(EXTRA_PARAM1, param1);
        intent.putExtra(EXTRA_PARAM2, param2);
        context.startService(intent);
    }

    /**
     * Starts this service to perform action Baz with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    // TODO: Customize helper method
    public static void startActionBaz(Context context, String param1, String param2) {
        Intent intent = new Intent(context, Write_Service.class);
        intent.setAction(ACTION_BAZ);
        intent.putExtra(EXTRA_PARAM1, param1);
        intent.putExtra(EXTRA_PARAM2, param2);
        context.startService(intent);
    }

    @Override
    public void onDestroy() {
        cont=false;
        while(!SensorQueue.is_empty()){
            long now;
            SensorValue temp= SensorQueue.pop();
            myFile.printf("%f %f %f",temp.getX(),temp.getY(),temp.getZ());
            if(mytime==0){
                mytime=temp.getTime();
            }
            now=(temp.getTime()-mytime)/1000000;
            myFile.println(" "+now+" "+(now-last));
            // Log.d("Test","time "+now+" "+(now-last));
            last=now;
        }
        myFile.close();
        myFile=null;
        Log.d("test","close complete");
        wl.release();
        stopForeground(STOP_FOREGROUND_REMOVE);
        super.onDestroy();
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            final String action = intent.getAction();
            if (ACTION_FOO.equals(action)) {
                final String param1 = intent.getStringExtra(EXTRA_PARAM1);
                final String param2 = intent.getStringExtra(EXTRA_PARAM2);
                handleActionFoo(param1, param2);
            } else if (ACTION_BAZ.equals(action)) {
                final String param1 = intent.getStringExtra(EXTRA_PARAM1);
                final String param2 = intent.getStringExtra(EXTRA_PARAM2);
                handleActionBaz(param1, param2);
            }
        }

        if(myFile==null){
            openFile(intent);
            startForeground(1, new Notification());
            pm=(PowerManager)getSystemService(Context.POWER_SERVICE);
            wl = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,"tag");
            wl.acquire();
        }

        while(cont){
            if(!SensorQueue.is_empty()){
                long now;

                SensorValue temp= SensorQueue.pop();
                myFile.printf("%f %f %f",temp.getX(),temp.getY(),temp.getZ());
                if(mytime==0){
                    mytime=temp.getTime();
                }
                now=(temp.getTime()-mytime)/1000000;
                myFile.println(" "+now+" "+(now-last));
                // Log.d("Test","time "+now+" "+(now-last));
                last=now;
            }
            try {
                Thread.sleep(20);
            }
            catch (Exception e)
            {}

        }
    }
    public void openFile(Intent intent){//사용안함
        String filename= intent.getStringExtra("file");
        String sdcardFolder = Environment.getExternalStorageDirectory().getAbsolutePath();
        String filepath = sdcardFolder+ File.separator+filename;
        try{
            myFile= new PrintWriter(new FileOutputStream(filepath+"-20ms.txt"));
            // myFile20= new PrintWriter(new FileOutputStream(filepath+"-40ms.txt"));
            //  myFile40= new PrintWriter(new FileOutputStream(filepath+"-80ms.txt"));
        }
        catch (FileNotFoundException e){
            //Toast.makeText(this, "저장할 파일을 생성 할 수 없습니다..", Toast.LENGTH_SHORT).show();
        }
        myFile.printf("x y z time\n");

        // myFile20.printf("x y z time\n");
        // myFile40.printf("x y z time\n");
        //Toast.makeText(this, "파일 오픈 완료", Toast.LENGTH_SHORT).show();
    }



    /**
     * Handle action Foo in the provided background thread with the provided
     * parameters.
     */
    private void handleActionFoo(String param1, String param2) {
        // TODO: Handle action Foo
        throw new UnsupportedOperationException("Not yet implemented");
    }

    /**
     * Handle action Baz in the provided background thread with the provided
     * parameters.
     */
    private void handleActionBaz(String param1, String param2) {
        // TODO: Handle action Baz
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
