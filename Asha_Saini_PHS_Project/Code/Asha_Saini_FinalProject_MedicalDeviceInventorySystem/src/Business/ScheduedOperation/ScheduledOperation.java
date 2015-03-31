/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.ScheduedOperation;

import Business.CareTeam.CareTeam;
import Business.Doctor.Doctor;
import Business.MedicalDevice.MedicalDevice;
import Business.OperationRoom.OperationRoom;
import Business.Patient.Patient;
import Business.Schedule.Reservation;
import Business.Schedule.Schedule;
import java.util.Date;

/**
 *
 * @author Asha
 */
public class ScheduledOperation {

    String operationName;
    int operationID;
    Patient patient;
    //Schedule schedule;
    Reservation resevation;
    OperationRoom operationRoom;
    MedicalDevice medicalDevice;
    Doctor doctor;
    CareTeam careTeam;
    private static int count = 0;

    public ScheduledOperation() {
        patient = new Patient();
        //schedule = new Schedule();
        resevation = new Reservation(new Date(), new Date());
        operationRoom = new OperationRoom();
        medicalDevice = new MedicalDevice();
        doctor = new Doctor("", 0);
        careTeam = new CareTeam();
    }

    public String getOperationName() {
        return operationName;
    }

    public void setOperationName(String operationName) {
        this.operationName = operationName;
    }

    public int getOperationID() {
        return operationID;
    }

    public void setOperationID(int operationID) {
        this.operationID = operationID;
    }

    public Patient getPatient() {
        return patient;
    }

    public void setPatient(Patient patient) {
        this.patient = patient;
    }

//    public Schedule getSchedule() {
//        return schedule;
//    }
//
//    public void setSchedule(Schedule schedule) {
//        this.schedule = schedule;
//    }
    public Reservation getResevation() {
        return resevation;
    }

    public void setResevation(Reservation resevation) {
        this.resevation = resevation;
    }
    
    public OperationRoom getOperationRoom() {
        return operationRoom;
    }

    public void setOperationRoom(OperationRoom operationRoom) {
        this.operationRoom = operationRoom;
    }

    public MedicalDevice getMedicalDevice() {
        return medicalDevice;
    }

    public void setMedicalDevice(MedicalDevice medicalDevice) {
        this.medicalDevice = medicalDevice;
    }

    public Doctor getDoctor() {
        return doctor;
    }

    public void setDoctor(Doctor doctor) {
        this.doctor = doctor;
    }

    public CareTeam getCareTeam() {
        return careTeam;
    }

    public void setCareTeam(CareTeam careTeam) {
        this.careTeam = careTeam;
    }

    @Override
    public String toString() {
        return operationName;
    }    
}
