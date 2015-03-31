/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.OperationRoom;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class OperationRoomDirectory {

    ArrayList<OperationRoom> opertionRoomList;

    public OperationRoomDirectory() {
        opertionRoomList = new ArrayList<>();
    }

    public ArrayList<OperationRoom> getOpertionRoomList() {
        return opertionRoomList;
    }

    public void setOpertionRoomList(ArrayList<OperationRoom> opertionRoomList) {
        this.opertionRoomList = opertionRoomList;
    }

    public OperationRoom addOperationRoom(String roomName, int roomNumber, int level, String buildingName) {
        OperationRoom p = new OperationRoom(roomName, roomNumber, level, buildingName);
        opertionRoomList.add(p);
        return p;
    }

    public OperationRoom searchOperationRoombyNumber(int id) {
        for (OperationRoom p : opertionRoomList) {
            if (id == p.getRoomNumber()) {
                return p;
            }
        }
        return null;
    }

    public ArrayList<OperationRoom> searchOperationRoombyName(String name) {
        ArrayList<OperationRoom> serachResults = new ArrayList<>();
        for (OperationRoom p : opertionRoomList) {
            if (p.getRoomName().contains(name)) {
                serachResults.add(p);
            }
        }
        return serachResults;
    }

    public void deleteOperationRoom(OperationRoom p) {
        opertionRoomList.remove(p);
    }

}
