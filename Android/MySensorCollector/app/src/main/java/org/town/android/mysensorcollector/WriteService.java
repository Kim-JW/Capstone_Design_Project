package org.town.android.mysensorcollector;

import android.app.IntentService;
import android.content.Intent;
import android.content.Context;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

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
public class WriteService extends IntentService {
    // TODO: Rename actions, choose action names that describe tasks that this
    // IntentService can perform, e.g. ACTION_FETCH_NEW_ITEMS
    private static final String ACTION_FOO = "org.town.android.mysensorcollector.action.FOO";
    private static final String ACTION_BAZ = "org.town.android.mysensorcollector.action.BAZ";

    // TODO: Rename parameters
    private static final String EXTRA_PARAM1 = "org.town.android.mysensorcollector.extra.PARAM1";
    private static final String EXTRA_PARAM2 = "org.town.android.mysensorcollector.extra.PARAM2";

    PrintWriter myFile=null;
    PrintWriter myFile20=null;
    PrintWriter myFile40=null;//파일 3개 생성
    public WriteService() {
        super("WriteService");
    }

    /**
     * Starts this service to perform action Foo with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    // TODO: Customize helper method
    public static void startActionFoo(Context context, String param1, String param2) {
        Intent intent = new Intent(context, WriteService.class);
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
        Intent intent = new Intent(context, WriteService.class);
        intent.setAction(ACTION_BAZ);
        intent.putExtra(EXTRA_PARAM1, param1);
        intent.putExtra(EXTRA_PARAM2, param2);
        context.startService(intent);
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
        }

        while(true){
            if(!SensorQueue.is_empty()){
                SensorValue temp= SensorQueue.pop();
                myFile.printf("%f %f %f",temp.getX(),temp.getY(),temp.getZ());
                myFile.println(" "+temp.getTime());
                Log.d("Test","time "+temp.getTime());
                myFile.flush();
            }
        }

    }
    public void openFile(Intent intent){//사용안함
        String filename= intent.getStringExtra("file")+"-"+intent.getStringExtra("type");
        String sdcardFolder = Environment.getExternalStorageDirectory().getAbsolutePath();
        String filepath = sdcardFolder+ File.separator+filename;
        try{
            myFile= new PrintWriter(new FileOutputStream(filepath+"-20ms.txt"));
           // myFile20= new PrintWriter(new FileOutputStream(filepath+"-40ms.txt"));
          //  myFile40= new PrintWriter(new FileOutputStream(filepath+"-80ms.txt"));
        }
        catch (FileNotFoundException e){
            Toast.makeText(this, "저장할 파일을 생성 할 수 없습니다..", Toast.LENGTH_SHORT).show();
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
