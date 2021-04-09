package org.town.android.simple_collector;

/**
 * Created by Bychoi on 2018-04-11.
 */

public class SensorValue {
    private double x;
    private double y;
    private double z;
    private long time;

    public SensorValue(long time,double x,double y,double z){
        this.time=time;
        this.x= x;
        this.y=y;
        this.z=z;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public double getZ() {
        return z;
    }

    public void setX(double x) {
        this.x = x;
    }

    public void setY(double y) {
        this.y = y;
    }

    public void setZ(double z) {
        this.z = z;
    }

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
    }
}
