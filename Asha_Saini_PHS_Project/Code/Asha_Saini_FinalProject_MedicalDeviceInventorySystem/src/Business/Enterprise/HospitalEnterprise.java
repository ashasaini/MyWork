/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Enterprise;

import Business.CareTeam.CareTeam;
import Business.CareTeam.CareTeamDirectory;
import Business.Doctor.DoctorDirectory;
import Business.OperationRoom.OperationRoomDirectory;
import Business.Patient.PatientDirectory;
import Business.Role.Role;
import Business.ScheduedOperation.ScheduledOperationDirectory;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class HospitalEnterprise extends Enterprise {

    PatientDirectory patientDirectory;
    DoctorDirectory doctorDirectory;
    CareTeamDirectory careTeamDirectory;
    OperationRoomDirectory operationRoomDirectory;
    ScheduledOperationDirectory scheduledOperationDirectory;

    public HospitalEnterprise(String name) {
        super(name, Enterprise.EnterpriseType.Hospital);
        patientDirectory = new PatientDirectory();
        doctorDirectory = new DoctorDirectory();
        careTeamDirectory = new CareTeamDirectory();
        operationRoomDirectory = new OperationRoomDirectory();
        scheduledOperationDirectory = new ScheduledOperationDirectory();

        InitializeHospitalEnterprise();
    }

    private void InitializeHospitalEnterprise() {
        InitializePatientDirectory();
        InitializeDoctorDirectory();
        InitializeCareTeamDirectory();
        InitializeOperationRoomDirectory();
    }

    public PatientDirectory getPatientDirectory() {
        return patientDirectory;
    }

    public void setPatientDirectory(PatientDirectory patientDirectory) {
        this.patientDirectory = patientDirectory;
    }

    public DoctorDirectory getDoctorDirectory() {
        return doctorDirectory;
    }

    public void setDoctorDirectory(DoctorDirectory doctorDirectory) {
        this.doctorDirectory = doctorDirectory;
    }

    public CareTeamDirectory getCareTeamDirectory() {
        return careTeamDirectory;
    }

    public void setCareTeamDirectory(CareTeamDirectory careTeamDirectory) {
        this.careTeamDirectory = careTeamDirectory;
    }

    public OperationRoomDirectory getOperationRoomDirectory() {
        return operationRoomDirectory;
    }

    public void setOperationRoomDirectory(OperationRoomDirectory operationRoomDirectory) {
        this.operationRoomDirectory = operationRoomDirectory;
    }   

    @Override
    public ArrayList<Role> getSupportedRole() {
        return null;
    }

    private void InitializePatientDirectory() {
        for (int i = 0; i < 10; i++) {
            String name = "";
            String docName = "";
            String pharmacyName = "";
            int age = 0;

            switch (i + 1) {
                case 1:
                    name = "John Smith";
                    docName = "Dr. Mathew";
                    pharmacyName = "CVS Pharmacy";
                    age = 25;
                    break;
                case 2:
                    name = "Peter";
                    docName = "Dr. Lee";
                    pharmacyName = "Walgreens";
                    age = 30;
                    break;
                case 3:
                    name = "Micheal";
                    docName = "Dr. Nelson";
                    pharmacyName = "Safe Way";
                    age = 35;
                    break;
                case 4:
                    name = "Racheal";
                    docName = "Dr. Jennifer";
                    pharmacyName = "Accurate";
                    age = 40;
                    break;
                case 5:
                    name = "Cathie";
                    docName = "Dr. Sharon";
                    pharmacyName = "Pharma Save";
                    age = 45;
                    break;
                case 6:
                    name = "Christine";
                    docName = "Dr. Maria";
                    pharmacyName = "Rite Aid Corporation";
                    age = 20;
                    break;
                case 7:
                    name = "Daniel";
                    docName = "Dr. Kevin";
                    pharmacyName = "Publix Pharmacy";
                    age = 19;
                    break;
                case 8:
                    name = "Christopher";
                    docName = "Dr. Anthony";
                    pharmacyName = "Costco Pharmacy";
                    age = 32;
                    break;
                case 9:
                    name = "Nancy";
                    docName = "Dr. William";
                    pharmacyName = "Fredmeyer Pharmacy";
                    age = 25;
                    break;
                case 10:
                    name = "Joseph";
                    docName = "Dr. Richard";
                    pharmacyName = "Target Corp.";
                    age = 28;
                    break;
            }
            patientDirectory.addPatient(name, age, docName, pharmacyName);
        }
    }

    private void InitializeCareTeamDirectory() {

        for (int i = 0; i < 10; i++) {
            String teamName = "";
            String teamLeadName = "";
            int age = 0;
            switch (i + 1) {
                case 1:
                    teamName = "Team A";
                    teamLeadName = "Team Lead A";
                    age = 32;
                    break;
                case 2:
                    teamName = "Team B";
                    teamLeadName = "Team Lead B";
                    age = 32;
                    break;
                case 3:
                    teamName = "Team C";
                    teamLeadName = "Team Lead C";
                    age = 32;
                    break;
                case 4:
                    teamName = "Team D";
                    teamLeadName = "Team Lead D";
                    age = 32;
                    break;
                case 5:
                    teamName = "Team E";
                    teamLeadName = "Team Lead E";
                    age = 32;
                    break;
                case 6:
                    teamName = "Team F";
                    teamLeadName = "Team Lead F";
                    age = 32;
                    break;
                case 7:
                    teamName = "Team G";
                    teamLeadName = "Team Lead G";
                    age = 32;
                    break;
                case 8:
                    teamName = "Team H";
                    teamLeadName = "Team Lead H";
                    age = 32;
                    break;
                case 9:
                    teamName = "Team I";
                    teamLeadName = "Team Lead I";
                    age = 32;
                    break;
                case 10:
                    teamName = "Team J";
                    teamLeadName = "Team Lead J";
                    age = 32;
                    break;
            }
            CareTeam ct = careTeamDirectory.addCareTeam(teamName, teamLeadName, age);
            for (int j = 0; j < 10; j++) {
                String name = "";

                switch (j + 1) {
                    case 1:
                        name = "Wright";
                        age = 28;
                        break;
                    case 2:
                        name = "Lopez";
                        age = 28;
                        break;
                    case 3:
                        name = "Hill";
                        age = 28;
                        break;
                    case 4:
                        name = "Greem";
                        age = 28;
                        break;
                    case 5:
                        name = "Adam";
                        age = 28;
                        break;
                    case 6:
                        name = "Davis";
                        age = 28;
                        break;
                    case 7:
                        name = "Robert";
                        age = 28;
                        break;
                    case 8:
                        name = "Collin";
                        age = 28;
                        break;
                    case 9:
                        name = "Evans";
                        age = 45;
                        break;
                    case 10:
                        name = "Miller";
                        age = 28;
                        break;
                }
                ct.getTeamMember().addEmployee(name, age);
            }

        }

    }

    private void InitializeOperationRoomDirectory() {
        for (int i = 0; i < 10; i++) {
            String roomName = "";
            int roomNumber = 0;
            int level = 0;
            String buildingName = "";

            switch (i + 1) {
                case 1:
                    roomName = "Operation Room 1";
                    roomNumber = 1;
                    level = 1;
                    buildingName = "Building 1";
                    break;
                case 2:
                    roomName = "Operation Room 2";
                    roomNumber = 2;
                    level = 1;
                    buildingName = "Building 1";
                    break;
                case 3:
                    roomName = "Operation Room 3";
                    roomNumber = 3;
                    level = 1;
                    buildingName = "Building 1";
                    break;
                case 4:
                    roomName = "Operation Room A";
                    roomNumber = 1;
                    level = 1;
                    buildingName = "Building A";
                    break;
                case 5:
                    roomName = "Operation Room B";
                    roomNumber = 2;
                    level = 1;
                    buildingName = "Building A";
                    break;
                case 6:
                    roomName = "Operation Room C";
                    roomNumber = 3;
                    level = 1;
                    buildingName = "Building A";
                    break;
                case 7:
                    roomName = "Operation Room P";
                    roomNumber = 1;
                    level = 2;
                    buildingName = "Building X";
                    break;
                case 8:
                    roomName = "Operation Room Q";
                    roomNumber = 2;
                    level = 2;
                    buildingName = "Building X";
                    break;
                case 9:
                    roomName = "Operation Room R";
                    roomNumber = 3;
                    level = 2;
                    buildingName = "Building X";
                    break;
                case 10:
                    roomName = "Operation Room Z";
                    roomNumber = 1;
                    level = 1;
                    buildingName = "Building Z";
                    break;
            }
            operationRoomDirectory.addOperationRoom(roomName, roomNumber, level, buildingName);
        }
    }

    private void InitializeDoctorDirectory() {
        for (int i = 0; i < 10; i++) {
            String name = "";
            int age = 0;

            switch (i + 1) {
                case 1:
                    name = "Dr. Mathew";
                    age = 42;
                    break;
                case 2:
                    name = "Dr. Lee";
                    age = 38;
                    break;
                case 3:
                    name = "Dr. Nelson";
                    age = 35;
                    break;
                case 4:
                    name = "Dr. Jennifer";
                    age = 40;
                    break;
                case 5:
                    name = "Dr. Sharon";
                    age = 45;
                    break;
                case 6:
                    name = "Dr. Maria";
                    age = 40;
                    break;
                case 7:
                    name = "Dr. Kevin";
                    age = 49;
                    break;
                case 8:
                    name = "Dr. Anthony";
                    age = 42;
                    break;
                case 9:
                    name = "Dr. William";
                    age = 45;
                    break;
                case 10:
                    name = "Dr. Richard";
                    age = 48;
                    break;
            }
            doctorDirectory.addDoctor(name, age);
        }
    }

    public ScheduledOperationDirectory getScheduledOperationDirectory() {
        return scheduledOperationDirectory;
    }

    public void setScheduledOperationDirectory(ScheduledOperationDirectory scheduledOperationDirectory) {
        this.scheduledOperationDirectory = scheduledOperationDirectory;
    }
    
    
}
