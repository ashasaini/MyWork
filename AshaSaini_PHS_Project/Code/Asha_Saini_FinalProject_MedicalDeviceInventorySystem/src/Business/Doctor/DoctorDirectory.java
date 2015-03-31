/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Doctor;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class DoctorDirectory {

    ArrayList<Doctor> doctorList;

    public DoctorDirectory() {
        doctorList = new ArrayList<>();
    }

    public ArrayList<Doctor> getDoctorList() {
        return doctorList;
    }

    public void setDoctorList(ArrayList<Doctor> doctorList) {
        this.doctorList = doctorList;
    }

    public Doctor addDoctor(String name, int age) {
        Doctor p = new Doctor(name, age);
        doctorList.add(p);
        return p;
    }

    public Doctor searchDoctorbyId(int id) {
        for (Doctor p : doctorList) {
            if (id == p.getEmployeeId()) {
                return p;
            }
        }
        return null;
    }

    public ArrayList<Doctor> searchDoctorbyName(String name) {
        ArrayList<Doctor> serachResults = new ArrayList<>();
        for (Doctor p : doctorList) {
            if (p.getEmployeeName().contains(name)) {
                serachResults.add(p);
            }
        }
        return serachResults;
    }

    public void deleteDoctor(Doctor p) {
        doctorList.remove(p);
    }
}
