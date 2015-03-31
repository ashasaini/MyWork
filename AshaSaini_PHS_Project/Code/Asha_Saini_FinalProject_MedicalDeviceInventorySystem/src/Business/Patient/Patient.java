/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Patient;

import Business.Schedule.Schedule;

/**
 *
 * @author Asha
 */
public class Patient {
    private String patientName;
    private int patientId;
    private int age;
    private VitalSignHistory vitalSignHistory;
    private OperationHistory operationHistory;
    private Schedule schedule;
    private String docName;
    private String pharmacyName;
    private static int  count =1;

    public Patient() {
    }
    
    public Patient( String patientName,
            int age,
            String docName,
            String pharName) {
        
        this.patientId = count++;        
        this.patientName = patientName;
        this.age = age;
        this.pharmacyName = pharName;
        this.docName = docName;
                
        vitalSignHistory = new VitalSignHistory();
        operationHistory = new OperationHistory();
        schedule = new Schedule();
    }

    public String getPatientName() {
        return patientName;
    }

    public void setPatientName(String patientName) {
        this.patientName = patientName;
    }

    public int getPatientId() {
        return patientId;
    }

    public void setPatientId(int patientId) {
        this.patientId = patientId;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public VitalSignHistory getVitalSignHistory() {
        return vitalSignHistory;
    }

    public void setVitalSignHistory(VitalSignHistory vitalSignHistory) {
        this.vitalSignHistory = vitalSignHistory;
    }

    public OperationHistory getOperationHistory() {
        return operationHistory;
    }

    public void setOperationHistory(OperationHistory operationHistory) {
        this.operationHistory = operationHistory;
    }

    public Schedule getSchedule() {
        return schedule;
    }

    public void setSchedule(Schedule schedule) {
        this.schedule = schedule;
    }

    public static int getCount() {
        return count;
    }

    public static void setCount(int count) {
        Patient.count = count;
    }   

    public String getDocName() {
        return docName;
    }

    public void setDocName(String docName) {
        this.docName = docName;
    }

    public String getPharmacyName() {
        return pharmacyName;
    }

    public void setPharmacyName(String pharmacyName) {
        this.pharmacyName = pharmacyName;
    }

    @Override
    public String toString() {
        return String.valueOf(patientId);
    }    
}
