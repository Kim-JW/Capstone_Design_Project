package org.town.android.mysensorcollector;

/**
 * Created by Bychoi on 2018-04-11.
 */

public class SensorQueue {
    private static SensorValue[][] queue =null;
    private static int front;
    private static int rear;
    private final static int BIAS = 1000;
    private final static int MAX_SIZE = 5000;
    public  static int count;
    //원형큐에 따라 실행되도록함 이떄 인덱스는 0~4999를 유지하게 하여 %1000와 /1000을 통해 배열 배정
    public static void Setup(){
        queue = new SensorValue[5][1000];
        front=0;
        rear=0;
        count=0;
        //front 가 가리키는 칸은 무조건 비어있음
    }
    public static boolean is_empty(){
        return (front==rear);
    }
    public static boolean is_full(){
        return (rear+1%MAX_SIZE==front);
    }
    public static void push(SensorValue val){
        //더 들어올경우 덮어쓰기 수행
        if(is_full()){
            pop();
            //더들어올경우 제일먼저 들어온것을 하나 버림
        }
        synchronized(SensorQueue.class) {

            rear = (rear + 1) % MAX_SIZE;
            queue[rear / BIAS][rear % BIAS] = val;
            // Log.d("test2","push test "+rear / BIAS+" "+rear % BIAS);
            count++;
        }
    }
    // 푸쉬 팝 정도만 구현
    public static SensorValue pop(){
        //Log.d("tes5","Testttttttt");
        if(is_empty()) {
            return null;//임시
        }
        synchronized(SensorQueue.class) {
            count--;
            front = (front + 1) % MAX_SIZE;
            //Log.d("test3","pop test "+front / BIAS+" "+front % BIAS);
            return queue[front / BIAS][front % BIAS];
        }
    }
}
