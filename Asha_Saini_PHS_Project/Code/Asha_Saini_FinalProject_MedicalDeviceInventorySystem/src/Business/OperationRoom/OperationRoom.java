/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.OperationRoom;

import Business.Schedule.Schedule;

/**
 *
 * @author Asha
 */
public class OperationRoom {
    private String roomName;
    private int roomNumber;
    private int level;
    private String buildingName;    
    Schedule schedule;

    public OperationRoom() {
    }
    
    public OperationRoom(String roomName, int roomNumber, int level, String buildingName) {
        this.roomName = roomName;
        this.roomNumber = roomNumber;
        this.level = level;
        this.buildingName = buildingName;
        this.schedule = new Schedule();
    }
    
    public String getRoomName() {
        return roomName;
    }

    public void setRoomName(String roomName) {
        this.roomName = roomName;
    }

    public int getRoomNumber() {
        return roomNumber;
    }

    public void setRoomNumber(int roomNumber) {
        this.roomNumber = roomNumber;
    }

    public int getLevel() {
        return level;
    }

    public void setLevel(int level) {
        this.level = level;
    }

    public String getBuildingName() {
        return buildingName;
    }

    public void setBuildingName(String buildingName) {
        this.buildingName = buildingName;
    }

    public Schedule getSchedule() {
        return schedule;
    }

    public void setSchedule(Schedule schedule) {
        this.schedule = schedule;
    }

    @Override
    public String toString() {
        return roomName;
    }   
}
