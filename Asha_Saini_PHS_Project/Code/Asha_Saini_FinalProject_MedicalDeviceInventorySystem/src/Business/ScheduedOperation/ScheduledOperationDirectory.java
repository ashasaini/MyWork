/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.ScheduedOperation;

//import Business.CareTeam.CareTeam;
//import Business.Doctor.Doctor;
//import Business.MedicalDevice.MedicalDevice;
//import Business.OperationRoom.OperationRoom;
//import Business.Patient.Patient;
//import Business.Schedule.Reservation;
//import Business.Schedule.Schedule;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class ScheduledOperationDirectory {

    ArrayList<ScheduledOperation> scheduledOperationList;

    public ScheduledOperationDirectory() {
        scheduledOperationList = new ArrayList<ScheduledOperation>();
    }

    public ArrayList<ScheduledOperation> getScheduledOperationList() {
        return scheduledOperationList;
    }

    public void setScheduledOperationList(ArrayList<ScheduledOperation> scheduledOperationList) {
        this.scheduledOperationList = scheduledOperationList;
    }

    public ScheduledOperation addScheduledOperation(
            //            String operationName,
            //            Patient patient, //Schedule schedule,
            //            Reservation r, OperationRoom or,
            //            MedicalDevice md, Doctor doc, CareTeam ct
            ScheduledOperation scheduledOperation) {
       // ScheduledOperation p = new ScheduledOperation();
//        p.setOperationName(operationName);
//        p.setPatient(patient);
//        //p.setSchedule(schedule);
//        p.setResevation(r);
//        p.setOperationRoom(or);
//        p.setMedicalDevice(md);
//        p.setDoctor(doc);
//        p.setCareTeam(ct);
        scheduledOperationList.add(scheduledOperation);
        return scheduledOperation;
    }

    public ScheduledOperation searchScheduledOperationbyId(int id) {
        for (ScheduledOperation p : scheduledOperationList) {
            if (id == p.getOperationID()) {
                return p;
            }
        }
        return null;
    }

    public ArrayList<ScheduledOperation> searchScheduledOperationbyName(String name) {
        ArrayList<ScheduledOperation> serachResults = new ArrayList<>();
        for (ScheduledOperation p : scheduledOperationList) {
            if (p.getOperationName().contains(name)) {
                serachResults.add(p);
            }
        }
        return serachResults;
    }

    public void deleteScheduledOperation(ScheduledOperation p) {
        scheduledOperationList.remove(p);
    }
}
