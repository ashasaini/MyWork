/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Patient;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class PatientDirectory {

    private ArrayList<Patient> patientList;

    public PatientDirectory() {
        this.patientList = new ArrayList<>();
    }

    public ArrayList<Patient> getPatientList() {
        return patientList;
    }

    public void setPatientList(ArrayList<Patient> patientList) {
        this.patientList = patientList;
    }

    public Patient addPatient(String patientName, int age, String docName, String pharName) {
        Patient p = new Patient(patientName, age, docName, pharName);        
        patientList.add(p);
        return p;
    }

    public Patient searchPatientbyId(int id) {
        for (Patient p : patientList) {
            if (id == p.getPatientId()) {
                return p;
            }
        }
        return null;
    }

    public ArrayList<Patient> searchPatientbyName(String name) {
        ArrayList<Patient> serachResults = new ArrayList<>();
        for (Patient p : patientList) {
            if (p.getPatientName().contains(name)) {
                serachResults.add(p);
            }
        }
        return serachResults;
    }
    
    public void deletePatient(Patient p){
        patientList.remove(p);
    }
}
