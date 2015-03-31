/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package User.Interface.HospitalStaffWorkArea;

import Bsuiness.EcoSystem;
import Business.CareTeam.CareTeam;
import Business.Doctor.Doctor;
import Business.Enterprise.HospitalEnterprise;
import Business.MedicalDevice.MedicalDevice;
import Business.OperationRoom.OperationRoom;
import Business.Patient.Patient;
import Business.ScheduedOperation.ScheduledOperation;
import Business.Schedule.Reservation;
import Business.Schedule.Schedule;
import java.awt.Color;
import java.util.Calendar;
import java.util.Date;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author Asha
 */
public class HospitalStaffWorkAreaJPanel extends javax.swing.JPanel {

    /**
     * Creates new form HospitalStaffWorkAreaPanel
     */
    private JPanel userProcessContainer;
    private EcoSystem system;
    private HospitalEnterprise hospitalEnterprise;
    private ScheduledOperation scheduledOperation;

    public HospitalStaffWorkAreaJPanel(JPanel userProcessContainer, EcoSystem system, HospitalEnterprise hospitalEnterprise) {
        initComponents();
        Color c = new Color(0, 85, 119);
        this.setBackground(c);

        this.userProcessContainer = userProcessContainer;
        this.system = system;
        this.hospitalEnterprise = hospitalEnterprise;
        this.operationRoomavailabilityjLabel.setText("");
        this.reserveOperationRoomjButton.setEnabled(false);
        this.medicalDeviceavlbljLabel.setText("");
        this.reserveMedicalDevicejButton.setEnabled(false);
        this.doctoravlbljLabel.setText("");
        this.reserveDoctorjButton.setEnabled(false);
        this.careTeamavlbljLabel.setText("");
        this.reserveCareTeamjButton.setEnabled(false);

        populatePatientTable();
        populateOperationRoomTable();
        populateMedicalDeviceTable();
        populateDoctorTable();
        populateCareTeamTable();
        this.scheduledOperation = new ScheduledOperation();
    }

    private void populatePatientTable() {
        DefaultTableModel model = (DefaultTableModel) patientjTable.getModel();

        int rowCount = patientjTable.getRowCount();
        for (int i = rowCount - 1; i >= 0; i--) {
            model.removeRow(i);
        }

        model.setRowCount(0);
        for (Patient p : hospitalEnterprise.getPatientDirectory().getPatientList()) {
            Object row[] = new Object[4];
            row[0] = p;
            row[1] = p.getPatientName();
            row[2] = p.getAge();
            row[3] = p.getDocName();
            model.addRow(row);
        }
    }

    private void populateOperationRoomTable() {
        DefaultTableModel model = (DefaultTableModel) operationRoomjTable.getModel();

        int rowCount = operationRoomjTable.getRowCount();
        for (int i = rowCount - 1; i >= 0; i--) {
            model.removeRow(i);
        }

        model.setRowCount(0);
        for (OperationRoom op : hospitalEnterprise.getOperationRoomDirectory().getOpertionRoomList()) {
            Object row[] = new Object[4];
            row[0] = op;
            row[1] = op.getRoomNumber();
            row[2] = op.getLevel();
            row[3] = op.getBuildingName();
            model.addRow(row);
        }
    }

    private void populateMedicalDeviceTable() {
        DefaultTableModel model = (DefaultTableModel) medicalDevicejTable.getModel();

        int rowCount = medicalDevicejTable.getRowCount();
        for (int i = rowCount - 1; i >= 0; i--) {
            model.removeRow(i);
        }

        model.setRowCount(0);
        for (MedicalDevice md : system.
                getInventoryEnterprise().
                getWarehouse().
                getMdeciDeviceCatalog().
                getMedicalDeviceList()) {
            Object row[] = new Object[4];
            row[0] = md;
            row[1] = md.getDeviceId();
            row[2] = md.getDeviceCatagory();
            row[3] = md.getAvailableQuantity();
            model.addRow(row);
        }
    }

    private void populateDoctorTable() {
        DefaultTableModel model = (DefaultTableModel) doctorjTable.getModel();

        int rowCount = doctorjTable.getRowCount();
        for (int i = rowCount - 1; i >= 0; i--) {
            model.removeRow(i);
        }

        model.setRowCount(0);
        for (Doctor d : hospitalEnterprise.getDoctorDirectory().getDoctorList()) {
            Object row[] = new Object[4];
            row[0] = d;
            row[1] = d.getAge();
            model.addRow(row);
        }
    }

    private void populateCareTeamTable() {
        DefaultTableModel model = (DefaultTableModel) careTeamjTable.getModel();

        int rowCount = careTeamjTable.getRowCount();
        for (int i = rowCount - 1; i >= 0; i--) {
            model.removeRow(i);
        }

        model.setRowCount(0);
        for (CareTeam ct : hospitalEnterprise.getCareTeamDirectory().getCareTeamList()) {
            Object row[] = new Object[4];
            row[0] = ct;
            row[1] = ct.getId();
            row[2] = ct.getTeamLeader().getEmployeeName();
            model.addRow(row);
        }
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane4 = new javax.swing.JScrollPane();
        deletePatientsjTable = new javax.swing.JTable();
        jButton1 = new javax.swing.JButton();
        titlejLabel = new javax.swing.JLabel();
        mainTabjTabbedPane = new javax.swing.JTabbedPane();
        jPanel1 = new javax.swing.JPanel();
        innerTabjTabbedPane = new javax.swing.JTabbedPane();
        jPanel2 = new javax.swing.JPanel();
        jScrollPane5 = new javax.swing.JScrollPane();
        patientjTable = new javax.swing.JTable();
        selectPatientjButton = new javax.swing.JButton();
        jLabel6 = new javax.swing.JLabel();
        jPanel6 = new javax.swing.JPanel();
        jLabel1 = new javax.swing.JLabel();
        startDateTimePicker = new com.lavantech.gui.comp.DateTimePicker();
        jLabel5 = new javax.swing.JLabel();
        endDateTimePicker = new com.lavantech.gui.comp.DateTimePicker();
        jLabel7 = new javax.swing.JLabel();
        nextjButton = new javax.swing.JButton();
        jPanel3 = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        operationRoomjTable = new javax.swing.JTable();
        checkOperationRoomAvalbljButton = new javax.swing.JButton();
        reserveOperationRoomjButton = new javax.swing.JButton();
        jLabel9 = new javax.swing.JLabel();
        operationRoomavailabilityjLabel = new javax.swing.JLabel();
        jPanel4 = new javax.swing.JPanel();
        jScrollPane6 = new javax.swing.JScrollPane();
        medicalDevicejTable = new javax.swing.JTable();
        checkMedicalDeviceAvlbljButton = new javax.swing.JButton();
        reserveMedicalDevicejButton = new javax.swing.JButton();
        jLabel10 = new javax.swing.JLabel();
        medicalDeviceavlbljLabel = new javax.swing.JLabel();
        jPanel5 = new javax.swing.JPanel();
        jScrollPane7 = new javax.swing.JScrollPane();
        doctorjTable = new javax.swing.JTable();
        doctoravlbljButton = new javax.swing.JButton();
        reserveDoctorjButton = new javax.swing.JButton();
        jLabel11 = new javax.swing.JLabel();
        doctoravlbljLabel = new javax.swing.JLabel();
        jPanel7 = new javax.swing.JPanel();
        jScrollPane8 = new javax.swing.JScrollPane();
        careTeamjTable = new javax.swing.JTable();
        careTeamavlbljButton = new javax.swing.JButton();
        reserveCareTeamjButton = new javax.swing.JButton();
        jLabel12 = new javax.swing.JLabel();
        careTeamavlbljLabel = new javax.swing.JLabel();
        jPanel8 = new javax.swing.JPanel();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        jLabel8 = new javax.swing.JLabel();
        jLabel13 = new javax.swing.JLabel();
        jLabel14 = new javax.swing.JLabel();
        jLabel15 = new javax.swing.JLabel();
        jLabel16 = new javax.swing.JLabel();
        patientNamejTextField = new javax.swing.JTextField();
        operationNamejTextField = new javax.swing.JTextField();
        startDatejTextField = new javax.swing.JTextField();
        endDatejTextField = new javax.swing.JTextField();
        operationRoomjTextField = new javax.swing.JTextField();
        medicalDevicejTextField = new javax.swing.JTextField();
        doctorNamejTextField = new javax.swing.JTextField();
        careTeamjTextField = new javax.swing.JTextField();
        scheduleOperationjButton = new javax.swing.JButton();

        deletePatientsjTable.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        deletePatientsjTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Patient Id", "Patient Name", "Patient Age", "Doc Name"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false, false, false, false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jScrollPane4.setViewportView(deletePatientsjTable);

        jButton1.setText("Delete Patient");

        setBackground(new java.awt.Color(0, 85, 119));
        setAutoscrolls(true);
        setPreferredSize(new java.awt.Dimension(1280, 768));

        titlejLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel.setText("Hospital Staff Work Area");

        mainTabjTabbedPane.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        innerTabjTabbedPane.setBackground(new java.awt.Color(0, 102, 153));
        innerTabjTabbedPane.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        innerTabjTabbedPane.setFocusable(false);

        jPanel2.setBackground(new java.awt.Color(0, 85, 119));

        patientjTable.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        patientjTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Patient Id", "Patient Name", "Patient Age", "Doc Name"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false, false, false, false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jScrollPane5.setViewportView(patientjTable);

        selectPatientjButton.setBackground(new java.awt.Color(30, 30, 30));
        selectPatientjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        selectPatientjButton.setForeground(new java.awt.Color(255, 255, 255));
        selectPatientjButton.setText("Select Patient & Next >>");
        selectPatientjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                selectPatientjButtonActionPerformed(evt);
            }
        });

        jLabel6.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel6.setForeground(new java.awt.Color(255, 255, 255));
        jLabel6.setText("Select a patient from the list");

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(67, 67, 67)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(selectPatientjButton)
                    .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jScrollPane5, javax.swing.GroupLayout.PREFERRED_SIZE, 769, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jLabel6)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(27, 27, 27)
                .addComponent(jLabel6)
                .addGap(18, 18, 18)
                .addComponent(jScrollPane5, javax.swing.GroupLayout.PREFERRED_SIZE, 250, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(selectPatientjButton)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        innerTabjTabbedPane.addTab("Select Patient", jPanel2);

        jPanel6.setBackground(new java.awt.Color(0, 85, 119));
        jPanel6.setForeground(new java.awt.Color(255, 255, 255));

        jLabel1.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel1.setForeground(new java.awt.Color(255, 255, 255));
        jLabel1.setText("Select Start  Date and Time");

        startDateTimePicker.setBackground(new java.awt.Color(120, 120, 120));
        startDateTimePicker.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                startDateTimePickerActionPerformed(evt);
            }
        });

        jLabel5.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel5.setForeground(new java.awt.Color(255, 255, 255));
        jLabel5.setText("Select End  Date and Time");

        endDateTimePicker.setBackground(new java.awt.Color(120, 120, 120));

        jLabel7.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel7.setForeground(new java.awt.Color(255, 255, 255));
        jLabel7.setText("Select start, end date and time for operation");

        nextjButton.setBackground(new java.awt.Color(30, 30, 30));
        nextjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        nextjButton.setForeground(new java.awt.Color(255, 255, 255));
        nextjButton.setText("Next >>");
        nextjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                nextjButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel6Layout = new javax.swing.GroupLayout(jPanel6);
        jPanel6.setLayout(jPanel6Layout);
        jPanel6Layout.setHorizontalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addGap(80, 80, 80)
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel7)
                    .addGroup(jPanel6Layout.createSequentialGroup()
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(jLabel5)
                            .addComponent(jLabel1))
                        .addGap(61, 61, 61)
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(startDateTimePicker, javax.swing.GroupLayout.PREFERRED_SIZE, 177, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(endDateTimePicker, javax.swing.GroupLayout.PREFERRED_SIZE, 177, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(nextjButton, javax.swing.GroupLayout.PREFERRED_SIZE, 120, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel6Layout.setVerticalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addGap(34, 34, 34)
                .addComponent(jLabel7)
                .addGap(36, 36, 36)
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel1)
                    .addComponent(startDateTimePicker, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(64, 64, 64)
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jLabel5)
                    .addComponent(endDateTimePicker, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(65, 65, 65)
                .addComponent(nextjButton)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        innerTabjTabbedPane.addTab("Select Date and Time", jPanel6);

        jPanel3.setBackground(new java.awt.Color(0, 85, 119));

        operationRoomjTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Operation Room Name", "Room #", "Level", "Building Name"
            }
        ));
        operationRoomjTable.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                operationRoomjTableKeyReleased(evt);
            }
        });
        jScrollPane1.setViewportView(operationRoomjTable);

        checkOperationRoomAvalbljButton.setBackground(new java.awt.Color(30, 30, 30));
        checkOperationRoomAvalbljButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        checkOperationRoomAvalbljButton.setForeground(new java.awt.Color(255, 255, 255));
        checkOperationRoomAvalbljButton.setText("Check Availability");
        checkOperationRoomAvalbljButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                checkOperationRoomAvalbljButtonActionPerformed(evt);
            }
        });

        reserveOperationRoomjButton.setBackground(new java.awt.Color(30, 30, 30));
        reserveOperationRoomjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        reserveOperationRoomjButton.setForeground(new java.awt.Color(255, 255, 255));
        reserveOperationRoomjButton.setText("Reserve Room & Next >>");
        reserveOperationRoomjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                reserveOperationRoomjButtonActionPerformed(evt);
            }
        });

        jLabel9.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel9.setForeground(new java.awt.Color(255, 255, 255));
        jLabel9.setText("Select operation room from the list");

        operationRoomavailabilityjLabel.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        operationRoomavailabilityjLabel.setForeground(new java.awt.Color(255, 255, 255));
        operationRoomavailabilityjLabel.setText("jLabel2");

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, jPanel3Layout.createSequentialGroup()
                        .addGap(46, 46, 46)
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addComponent(jLabel9)
                                .addGap(583, 583, 583))
                            .addComponent(jScrollPane1)))
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(operationRoomavailabilityjLabel)
                        .addGap(93, 93, 93)
                        .addComponent(checkOperationRoomAvalbljButton)
                        .addGap(18, 18, 18)
                        .addComponent(reserveOperationRoomjButton)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel3Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {checkOperationRoomAvalbljButton, reserveOperationRoomjButton});

        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGap(20, 20, 20)
                .addComponent(jLabel9)
                .addGap(18, 18, 18)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 256, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(reserveOperationRoomjButton)
                    .addComponent(checkOperationRoomAvalbljButton)
                    .addComponent(operationRoomavailabilityjLabel))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel3Layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {checkOperationRoomAvalbljButton, reserveOperationRoomjButton});

        innerTabjTabbedPane.addTab("Reserve Operation Room", jPanel3);

        jPanel4.setBackground(new java.awt.Color(0, 85, 119));

        medicalDevicejTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Device Name", "Device ID", "Catagory", "Availability (No.)"
            }
        ));
        medicalDevicejTable.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                medicalDevicejTableKeyReleased(evt);
            }
        });
        jScrollPane6.setViewportView(medicalDevicejTable);

        checkMedicalDeviceAvlbljButton.setBackground(new java.awt.Color(30, 30, 30));
        checkMedicalDeviceAvlbljButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        checkMedicalDeviceAvlbljButton.setForeground(new java.awt.Color(255, 255, 255));
        checkMedicalDeviceAvlbljButton.setText("Check Availability");
        checkMedicalDeviceAvlbljButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                checkMedicalDeviceAvlbljButtonActionPerformed(evt);
            }
        });

        reserveMedicalDevicejButton.setBackground(new java.awt.Color(30, 30, 30));
        reserveMedicalDevicejButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        reserveMedicalDevicejButton.setForeground(new java.awt.Color(255, 255, 255));
        reserveMedicalDevicejButton.setText("Reserve Device & Next >>");
        reserveMedicalDevicejButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                reserveMedicalDevicejButtonActionPerformed(evt);
            }
        });

        jLabel10.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel10.setForeground(new java.awt.Color(255, 255, 255));
        jLabel10.setText("Select a medical device from the list");

        medicalDeviceavlbljLabel.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        medicalDeviceavlbljLabel.setForeground(new java.awt.Color(255, 255, 255));
        medicalDeviceavlbljLabel.setText("jLabel2");

        javax.swing.GroupLayout jPanel4Layout = new javax.swing.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, jPanel4Layout.createSequentialGroup()
                        .addGap(56, 56, 56)
                        .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jScrollPane6, javax.swing.GroupLayout.PREFERRED_SIZE, 780, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel10)))
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(medicalDeviceavlbljLabel)
                        .addGap(102, 102, 102)
                        .addComponent(checkMedicalDeviceAvlbljButton)
                        .addGap(18, 18, 18)
                        .addComponent(reserveMedicalDevicejButton)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addGap(24, 24, 24)
                .addComponent(jLabel10)
                .addGap(18, 18, 18)
                .addComponent(jScrollPane6, javax.swing.GroupLayout.PREFERRED_SIZE, 250, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(reserveMedicalDevicejButton)
                    .addComponent(checkMedicalDeviceAvlbljButton)
                    .addComponent(medicalDeviceavlbljLabel))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        innerTabjTabbedPane.addTab("Reserve Mdecial Device", jPanel4);

        jPanel5.setBackground(new java.awt.Color(0, 85, 119));
        jPanel5.setForeground(new java.awt.Color(255, 255, 255));

        doctorjTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Doctor Name", "Age"
            }
        ));
        jScrollPane7.setViewportView(doctorjTable);

        doctoravlbljButton.setBackground(new java.awt.Color(30, 30, 30));
        doctoravlbljButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        doctoravlbljButton.setForeground(new java.awt.Color(255, 255, 255));
        doctoravlbljButton.setText("Check Availability");
        doctoravlbljButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                doctoravlbljButtonActionPerformed(evt);
            }
        });

        reserveDoctorjButton.setBackground(new java.awt.Color(30, 30, 30));
        reserveDoctorjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        reserveDoctorjButton.setForeground(new java.awt.Color(255, 255, 255));
        reserveDoctorjButton.setText("Schedule for Operation & Next >>");
        reserveDoctorjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                reserveDoctorjButtonActionPerformed(evt);
            }
        });

        jLabel11.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel11.setForeground(new java.awt.Color(255, 255, 255));
        jLabel11.setText("Select a doctor from the list");

        doctoravlbljLabel.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        doctoravlbljLabel.setForeground(new java.awt.Color(255, 255, 255));
        doctoravlbljLabel.setText("jLabel2");

        javax.swing.GroupLayout jPanel5Layout = new javax.swing.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addGap(82, 82, 82)
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel11)
                    .addGroup(jPanel5Layout.createSequentialGroup()
                        .addComponent(jScrollPane7, javax.swing.GroupLayout.PREFERRED_SIZE, 363, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(30, 30, 30)
                        .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(doctoravlbljButton, javax.swing.GroupLayout.PREFERRED_SIZE, 143, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(reserveDoctorjButton)
                            .addComponent(doctoravlbljLabel))))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addGap(37, 37, 37)
                .addComponent(jLabel11)
                .addGap(18, 18, 18)
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane7, javax.swing.GroupLayout.PREFERRED_SIZE, 250, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel5Layout.createSequentialGroup()
                        .addComponent(doctoravlbljLabel)
                        .addGap(18, 18, 18)
                        .addComponent(doctoravlbljButton)
                        .addGap(18, 18, 18)
                        .addComponent(reserveDoctorjButton)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        innerTabjTabbedPane.addTab("Select Doctor", jPanel5);

        jPanel7.setBackground(new java.awt.Color(0, 85, 119));
        jPanel7.setForeground(new java.awt.Color(255, 255, 255));

        careTeamjTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Team Name", "Team ID", "Team Lead"
            }
        ));
        jScrollPane8.setViewportView(careTeamjTable);

        careTeamavlbljButton.setBackground(new java.awt.Color(30, 30, 30));
        careTeamavlbljButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        careTeamavlbljButton.setForeground(new java.awt.Color(255, 255, 255));
        careTeamavlbljButton.setText("Check Availability");
        careTeamavlbljButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                careTeamavlbljButtonActionPerformed(evt);
            }
        });

        reserveCareTeamjButton.setBackground(new java.awt.Color(30, 30, 30));
        reserveCareTeamjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        reserveCareTeamjButton.setForeground(new java.awt.Color(255, 255, 255));
        reserveCareTeamjButton.setText("Schedule for Operation & Next >>");
        reserveCareTeamjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                reserveCareTeamjButtonActionPerformed(evt);
            }
        });

        jLabel12.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel12.setForeground(new java.awt.Color(255, 255, 255));
        jLabel12.setText("Select a care team from the list");

        careTeamavlbljLabel.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        careTeamavlbljLabel.setForeground(new java.awt.Color(255, 255, 255));
        careTeamavlbljLabel.setText("jLabel2");

        javax.swing.GroupLayout jPanel7Layout = new javax.swing.GroupLayout(jPanel7);
        jPanel7.setLayout(jPanel7Layout);
        jPanel7Layout.setHorizontalGroup(
            jPanel7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel7Layout.createSequentialGroup()
                .addGap(82, 82, 82)
                .addGroup(jPanel7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel12)
                    .addGroup(jPanel7Layout.createSequentialGroup()
                        .addComponent(jScrollPane8, javax.swing.GroupLayout.PREFERRED_SIZE, 556, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addGroup(jPanel7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(reserveCareTeamjButton)
                            .addComponent(careTeamavlbljButton, javax.swing.GroupLayout.PREFERRED_SIZE, 143, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(careTeamavlbljLabel))))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel7Layout.setVerticalGroup(
            jPanel7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel7Layout.createSequentialGroup()
                .addGap(38, 38, 38)
                .addComponent(jLabel12)
                .addGap(18, 18, 18)
                .addGroup(jPanel7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jScrollPane8, javax.swing.GroupLayout.PREFERRED_SIZE, 250, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanel7Layout.createSequentialGroup()
                        .addComponent(careTeamavlbljLabel)
                        .addGap(18, 18, 18)
                        .addComponent(careTeamavlbljButton)
                        .addGap(18, 18, 18)
                        .addComponent(reserveCareTeamjButton)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        innerTabjTabbedPane.addTab("Reserve Care Team", jPanel7);

        jPanel8.setBackground(new java.awt.Color(0, 85, 119));
        jPanel8.setForeground(new java.awt.Color(255, 255, 255));

        jLabel2.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel2.setForeground(new java.awt.Color(255, 255, 255));
        jLabel2.setText("Patient Name");

        jLabel3.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel3.setForeground(new java.awt.Color(255, 255, 255));
        jLabel3.setText("Operation name");

        jLabel4.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel4.setForeground(new java.awt.Color(255, 255, 255));
        jLabel4.setText("Start date and time");

        jLabel8.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel8.setForeground(new java.awt.Color(255, 255, 255));
        jLabel8.setText("Operation room");

        jLabel13.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel13.setForeground(new java.awt.Color(255, 255, 255));
        jLabel13.setText("Medical device");

        jLabel14.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel14.setForeground(new java.awt.Color(255, 255, 255));
        jLabel14.setText("Doctor name");

        jLabel15.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel15.setForeground(new java.awt.Color(255, 255, 255));
        jLabel15.setText("Care Team name");

        jLabel16.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel16.setForeground(new java.awt.Color(255, 255, 255));
        jLabel16.setText("End date and time");

        patientNamejTextField.setBackground(new java.awt.Color(120, 120, 120));
        patientNamejTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        patientNamejTextField.setForeground(new java.awt.Color(255, 255, 255));

        operationNamejTextField.setBackground(new java.awt.Color(120, 120, 120));
        operationNamejTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        operationNamejTextField.setForeground(new java.awt.Color(255, 255, 255));
        operationNamejTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                operationNamejTextFieldActionPerformed(evt);
            }
        });

        startDatejTextField.setBackground(new java.awt.Color(120, 120, 120));
        startDatejTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        startDatejTextField.setForeground(new java.awt.Color(255, 255, 255));

        endDatejTextField.setBackground(new java.awt.Color(120, 120, 120));
        endDatejTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        endDatejTextField.setForeground(new java.awt.Color(255, 255, 255));

        operationRoomjTextField.setBackground(new java.awt.Color(120, 120, 120));
        operationRoomjTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        operationRoomjTextField.setForeground(new java.awt.Color(255, 255, 255));

        medicalDevicejTextField.setBackground(new java.awt.Color(120, 120, 120));
        medicalDevicejTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        medicalDevicejTextField.setForeground(new java.awt.Color(255, 255, 255));

        doctorNamejTextField.setBackground(new java.awt.Color(120, 120, 120));
        doctorNamejTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        doctorNamejTextField.setForeground(new java.awt.Color(255, 255, 255));

        careTeamjTextField.setBackground(new java.awt.Color(120, 120, 120));
        careTeamjTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        careTeamjTextField.setForeground(new java.awt.Color(255, 255, 255));

        scheduleOperationjButton.setBackground(new java.awt.Color(30, 30, 30));
        scheduleOperationjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        scheduleOperationjButton.setForeground(new java.awt.Color(255, 255, 255));
        scheduleOperationjButton.setText("Schedule Operation");
        scheduleOperationjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                scheduleOperationjButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel8Layout = new javax.swing.GroupLayout(jPanel8);
        jPanel8.setLayout(jPanel8Layout);
        jPanel8Layout.setHorizontalGroup(
            jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel8Layout.createSequentialGroup()
                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel8Layout.createSequentialGroup()
                        .addGap(90, 90, 90)
                        .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel8Layout.createSequentialGroup()
                                .addComponent(jLabel2)
                                .addGap(18, 18, 18)
                                .addComponent(patientNamejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 170, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(57, 57, 57)
                                .addComponent(jLabel14)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(doctorNamejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(jPanel8Layout.createSequentialGroup()
                                .addComponent(jLabel3)
                                .addGap(18, 18, 18)
                                .addComponent(operationNamejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(jPanel8Layout.createSequentialGroup()
                                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(jPanel8Layout.createSequentialGroup()
                                        .addComponent(jLabel4)
                                        .addGap(18, 18, 18)
                                        .addComponent(startDatejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                    .addGroup(jPanel8Layout.createSequentialGroup()
                                        .addComponent(jLabel8)
                                        .addGap(18, 18, 18)
                                        .addComponent(operationRoomjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                .addGap(47, 47, 47)
                                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(jPanel8Layout.createSequentialGroup()
                                        .addComponent(jLabel13)
                                        .addGap(18, 18, 18)
                                        .addComponent(medicalDevicejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                    .addGroup(jPanel8Layout.createSequentialGroup()
                                        .addComponent(jLabel16)
                                        .addGap(18, 18, 18)
                                        .addComponent(endDatejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                            .addGroup(jPanel8Layout.createSequentialGroup()
                                .addComponent(jLabel15)
                                .addGap(18, 18, 18)
                                .addComponent(careTeamjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                    .addGroup(jPanel8Layout.createSequentialGroup()
                        .addGap(329, 329, 329)
                        .addComponent(scheduleOperationjButton, javax.swing.GroupLayout.PREFERRED_SIZE, 170, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(152, Short.MAX_VALUE))
        );

        jPanel8Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {jLabel13, jLabel14, jLabel15, jLabel16, jLabel2, jLabel3, jLabel4, jLabel8});

        jPanel8Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {careTeamjTextField, doctorNamejTextField, endDatejTextField, medicalDevicejTextField, operationNamejTextField, operationRoomjTextField, patientNamejTextField, startDatejTextField});

        jPanel8Layout.setVerticalGroup(
            jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel8Layout.createSequentialGroup()
                .addGap(40, 40, 40)
                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel2)
                    .addComponent(patientNamejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel14)
                    .addComponent(doctorNamejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(operationNamejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel4)
                    .addComponent(startDatejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel16)
                    .addComponent(endDatejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel8)
                    .addComponent(operationRoomjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel13)
                    .addComponent(medicalDevicejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel15)
                    .addComponent(careTeamjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(29, 29, 29)
                .addComponent(scheduleOperationjButton)
                .addContainerGap(115, Short.MAX_VALUE))
        );

        jPanel8Layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {jLabel13, jLabel14, jLabel15, jLabel16, jLabel2, jLabel3, jLabel4, jLabel8});

        jPanel8Layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {careTeamjTextField, doctorNamejTextField, endDatejTextField, medicalDevicejTextField, operationNamejTextField, operationRoomjTextField, patientNamejTextField, startDatejTextField});

        innerTabjTabbedPane.addTab("Preview Operation Schedule", jPanel8);

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(innerTabjTabbedPane)
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addComponent(innerTabjTabbedPane, javax.swing.GroupLayout.PREFERRED_SIZE, 416, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
        );

        mainTabjTabbedPane.addTab("Schedule Operation", jPanel1);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(390, 390, 390)
                        .addComponent(titlejLabel))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(72, 72, 72)
                        .addComponent(mainTabjTabbedPane, javax.swing.GroupLayout.PREFERRED_SIZE, 908, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(300, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(25, 25, 25)
                .addComponent(titlejLabel)
                .addGap(18, 18, 18)
                .addComponent(mainTabjTabbedPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(23, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void doctoravlbljButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_doctoravlbljButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = doctorjTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a doctor from the list");
            return;
        }

        //get the selected product
        Doctor d = (Doctor) doctorjTable.getValueAt(selectedRow, 0);
        if (null == d) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected doctor's information");
            return;
        }

        //check availability
        boolean result = checkDoctorAvailability(d);
        if (false == result) {
            //set label to un-available
            doctoravlbljLabel.setText("Un-available");
            return;
        }
        //set label to available
        doctoravlbljLabel.setText("Available");
        reserveDoctorjButton.setEnabled(true);
    }//GEN-LAST:event_doctoravlbljButtonActionPerformed

    private void startDateTimePickerActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_startDateTimePickerActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_startDateTimePickerActionPerformed

    private void reserveMedicalDevicejButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_reserveMedicalDevicejButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = medicalDevicejTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a medical device from the list");
            return;
        }

        //get the selected product
        MedicalDevice md = (MedicalDevice) medicalDevicejTable.getValueAt(selectedRow, 0);
        if (null == md) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected medical device's information");
            return;
        }

        //set operation room in the scheduled operation
        scheduledOperation.setMedicalDevice(md);

        int selectedIndex = innerTabjTabbedPane.getSelectedIndex();
        innerTabjTabbedPane.setSelectedIndex(selectedIndex + 1);
    }//GEN-LAST:event_reserveMedicalDevicejButtonActionPerformed

    private void selectPatientjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_selectPatientjButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = patientjTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a patient from the list");
            return;
        }

        //get the selected product
        Patient p = (Patient) patientjTable.getValueAt(selectedRow, 0);
        if (null == p) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected patient's information");
            return;
        }

        //set patient in scheduled opetaion
        scheduledOperation.setPatient(p);

        int selectedIndex = innerTabjTabbedPane.getSelectedIndex();
        innerTabjTabbedPane.setSelectedIndex(selectedIndex + 1);
    }//GEN-LAST:event_selectPatientjButtonActionPerformed

    private void checkMedicalDeviceAvlbljButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_checkMedicalDeviceAvlbljButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = medicalDevicejTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a device from the list");
            return;
        }

        //get the selected product
        MedicalDevice md = (MedicalDevice) medicalDevicejTable.getValueAt(selectedRow, 0);
        if (null == md) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected device's information");
            return;
        }

        //check availability
        boolean result = checkMedicalAvailability(md);
        if (false == result) {
            //set label to un-available
            medicalDeviceavlbljLabel.setText("Un-available");
            return;
        }
        //set label to available
        medicalDeviceavlbljLabel.setText("Available");
        reserveMedicalDevicejButton.setEnabled(true);
    }//GEN-LAST:event_checkMedicalDeviceAvlbljButtonActionPerformed

    private void nextjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_nextjButtonActionPerformed
        // TODO add your handling code here:
        //get start, end date and time
        Date startDateTime = startDateTimePicker.getDate();
        Date endDateTime = endDateTimePicker.getDate();

        Date currentDate = new Date();
        Date futureDate = new Date();

        Calendar c = Calendar.getInstance();
        c.add(Calendar.DAY_OF_MONTH, (31 * 3));
        futureDate = c.getTime();

        //.check if start date is before current date
        if (false == startDateTime.before(currentDate)) {
            JOptionPane.showMessageDialog(this, "Please select a valid date");
            return;
        }

        if (false == startDateTime.before(futureDate)) {
            JOptionPane.showMessageDialog(this, "Please select a date within 3 months");
            return;
        }

        //create schedule object
        Reservation r = new Reservation(startDateTime, endDateTime);
        //set Reservation in scheduled operation
        scheduledOperation.setResevation(r);

        int selectedIndex = innerTabjTabbedPane.getSelectedIndex();
        innerTabjTabbedPane.setSelectedIndex(selectedIndex + 1);
    }//GEN-LAST:event_nextjButtonActionPerformed

    private void checkOperationRoomAvalbljButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_checkOperationRoomAvalbljButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = operationRoomjTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select an operation room from the list");
            return;
        }

        //get the selected product
        OperationRoom or = (OperationRoom) operationRoomjTable.getValueAt(selectedRow, 0);
        if (null == or) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected operation room's information");
            return;
        }

        boolean result = checkOperationRoomAvailability(or);
        if (false == result) {
            //set label to un-available
            operationRoomavailabilityjLabel.setText("Un-available");
            return;
        }
        //set label to available
        operationRoomavailabilityjLabel.setText("Available");
        reserveOperationRoomjButton.setEnabled(true);
    }//GEN-LAST:event_checkOperationRoomAvalbljButtonActionPerformed

    private void careTeamavlbljButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_careTeamavlbljButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = careTeamjTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select an care team from the list");
            return;
        }

        //get the selected product
        CareTeam ct = (CareTeam) careTeamjTable.getValueAt(selectedRow, 0);
        if (null == ct) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected care team's information");
            return;
        }

        //check availability
        boolean result = checkCareTeamAvailability(ct);
        if (false == result) {
            //set label to un-available
            careTeamavlbljLabel.setText("Un-available");
            return;
        }
        //set label to available
        careTeamavlbljLabel.setText("Available");
        reserveCareTeamjButton.setEnabled(true);
    }//GEN-LAST:event_careTeamavlbljButtonActionPerformed

    private void operationRoomjTableKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_operationRoomjTableKeyReleased
        // TODO add your handling code here:
        operationRoomavailabilityjLabel.setText("");
        reserveOperationRoomjButton.setEnabled(false);
    }//GEN-LAST:event_operationRoomjTableKeyReleased

    private void reserveOperationRoomjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_reserveOperationRoomjButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = operationRoomjTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select an operation room from the list");
            return;
        }

        //get the selected product
        OperationRoom or = (OperationRoom) operationRoomjTable.getValueAt(selectedRow, 0);
        if (null == or) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected operation room's information");
            return;
        }

        //set operation room in the scheduled operation
        scheduledOperation.setOperationRoom(or);

        int selectedIndex = innerTabjTabbedPane.getSelectedIndex();
        innerTabjTabbedPane.setSelectedIndex(selectedIndex + 1);
    }//GEN-LAST:event_reserveOperationRoomjButtonActionPerformed

    private void medicalDevicejTableKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_medicalDevicejTableKeyReleased
        // TODO add your handling code here:
        medicalDeviceavlbljLabel.setText("");
        reserveMedicalDevicejButton.setEnabled(false);
    }//GEN-LAST:event_medicalDevicejTableKeyReleased

    private void reserveDoctorjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_reserveDoctorjButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = doctorjTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a doctor room from the list");
            return;
        }

        //get the selected product
        Doctor d = (Doctor) doctorjTable.getValueAt(selectedRow, 0);
        if (null == d) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected doctor's information");
            return;
        }

        //set operation room in the scheduled operation
        scheduledOperation.setDoctor(d);
        int selectedIndex = innerTabjTabbedPane.getSelectedIndex();
        innerTabjTabbedPane.setSelectedIndex(selectedIndex + 1);
    }//GEN-LAST:event_reserveDoctorjButtonActionPerformed

    private void reserveCareTeamjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_reserveCareTeamjButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = careTeamjTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a care team room from the list");
            return;
        }

        //get the selected product
        CareTeam ct = (CareTeam) careTeamjTable.getValueAt(selectedRow, 0);
        if (null == ct) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve care team's information");
            return;
        }

        //set operation room in the scheduled operation
        scheduledOperation.setCareTeam(ct);

        int selectedIndex = innerTabjTabbedPane.getSelectedIndex();
        innerTabjTabbedPane.setSelectedIndex(selectedIndex + 1);

        populatePreviewTab();
    }//GEN-LAST:event_reserveCareTeamjButtonActionPerformed

    private void operationNamejTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_operationNamejTextFieldActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_operationNamejTextFieldActionPerformed

    private void scheduleOperationjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_scheduleOperationjButtonActionPerformed
        // TODO add your handling code here:
        String operationName = operationNamejTextField.getText();
        if (null == operationName || true == operationName.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Please provide an operation name");
            return;
        }

        scheduledOperation.setOperationName(operationName);
        hospitalEnterprise.getScheduledOperationDirectory().addScheduledOperation(scheduledOperation);

        //set the same in hospital enterprise directory
        //patient
        //get the same patient
        for (Patient p : hospitalEnterprise.getPatientDirectory().getPatientList()) {
            if (p.equals(scheduledOperation.getPatient())) {
                p.getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                        scheduledOperation.getResevation().getEndDate());
                break;
            }
        }

        //operation room
        //get the same operation room
        for (OperationRoom or : hospitalEnterprise.getOperationRoomDirectory().getOpertionRoomList()) {
            if (or.equals(scheduledOperation.getOperationRoom())) {
                or.getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                        scheduledOperation.getResevation().getEndDate());
                break;
            }
        }

        //medical device
        //get the same device
        for (MedicalDevice md : system.getInventoryEnterprise().getWarehouse().getMdeciDeviceCatalog().getMedicalDeviceList()) {
            if (md.equals(scheduledOperation.getMedicalDevice())) {
                md.getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                        scheduledOperation.getResevation().getEndDate());
                break;
            }
        }

        //doctor
        //get the same doctor
        for (Doctor d : hospitalEnterprise.getDoctorDirectory().getDoctorList()) {
            if (d.equals(scheduledOperation.getDoctor())) {
                d.getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                        scheduledOperation.getResevation().getEndDate());
                break;
            }
        }

        //care team       
        //get the same care team
        for (CareTeam cr : hospitalEnterprise.getCareTeamDirectory().getCareTeamList()) {
            if (cr.equals(scheduledOperation.getCareTeam())) {
                cr.getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                        scheduledOperation.getResevation().getEndDate());
                break;
            }
        }

        //set the reservation in all objects
        scheduledOperation.getPatient().getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                scheduledOperation.getResevation().getEndDate());
        scheduledOperation.getOperationRoom().getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                scheduledOperation.getResevation().getEndDate());
        scheduledOperation.getMedicalDevice().getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                scheduledOperation.getResevation().getEndDate());
        scheduledOperation.getDoctor().getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                scheduledOperation.getResevation().getEndDate());
        scheduledOperation.getCareTeam().getSchedule().addReservation(scheduledOperation.getResevation().getStartDate(),
                scheduledOperation.getResevation().getEndDate());
    }//GEN-LAST:event_scheduleOperationjButtonActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton careTeamavlbljButton;
    private javax.swing.JLabel careTeamavlbljLabel;
    private javax.swing.JTable careTeamjTable;
    private javax.swing.JTextField careTeamjTextField;
    private javax.swing.JButton checkMedicalDeviceAvlbljButton;
    private javax.swing.JButton checkOperationRoomAvalbljButton;
    private javax.swing.JTable deletePatientsjTable;
    private javax.swing.JTextField doctorNamejTextField;
    private javax.swing.JButton doctoravlbljButton;
    private javax.swing.JLabel doctoravlbljLabel;
    private javax.swing.JTable doctorjTable;
    private com.lavantech.gui.comp.DateTimePicker endDateTimePicker;
    private javax.swing.JTextField endDatejTextField;
    private javax.swing.JTabbedPane innerTabjTabbedPane;
    private javax.swing.JButton jButton1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel10;
    private javax.swing.JLabel jLabel11;
    private javax.swing.JLabel jLabel12;
    private javax.swing.JLabel jLabel13;
    private javax.swing.JLabel jLabel14;
    private javax.swing.JLabel jLabel15;
    private javax.swing.JLabel jLabel16;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JPanel jPanel7;
    private javax.swing.JPanel jPanel8;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane4;
    private javax.swing.JScrollPane jScrollPane5;
    private javax.swing.JScrollPane jScrollPane6;
    private javax.swing.JScrollPane jScrollPane7;
    private javax.swing.JScrollPane jScrollPane8;
    private javax.swing.JTabbedPane mainTabjTabbedPane;
    private javax.swing.JLabel medicalDeviceavlbljLabel;
    private javax.swing.JTable medicalDevicejTable;
    private javax.swing.JTextField medicalDevicejTextField;
    private javax.swing.JButton nextjButton;
    private javax.swing.JTextField operationNamejTextField;
    private javax.swing.JLabel operationRoomavailabilityjLabel;
    private javax.swing.JTable operationRoomjTable;
    private javax.swing.JTextField operationRoomjTextField;
    private javax.swing.JTextField patientNamejTextField;
    private javax.swing.JTable patientjTable;
    private javax.swing.JButton reserveCareTeamjButton;
    private javax.swing.JButton reserveDoctorjButton;
    private javax.swing.JButton reserveMedicalDevicejButton;
    private javax.swing.JButton reserveOperationRoomjButton;
    private javax.swing.JButton scheduleOperationjButton;
    private javax.swing.JButton selectPatientjButton;
    private com.lavantech.gui.comp.DateTimePicker startDateTimePicker;
    private javax.swing.JTextField startDatejTextField;
    private javax.swing.JLabel titlejLabel;
    // End of variables declaration//GEN-END:variables

    private boolean checkOperationRoomAvailability(OperationRoom or) {
        //get start and end date of current operation schedule
        Date currentStartDate = scheduledOperation.getResevation().getStartDate();
        Date currentEndDate = scheduledOperation.getResevation().getEndDate();

        //get schedule of selected operation room
        Schedule schedule = or.getSchedule();

        for (Reservation r : schedule.getReservationList()) {
            Date orStartDate = r.getStartDate();
            Date orEndDate = r.getEndDate();

            //check for dates
            if (currentStartDate.equals(r.getStartDate()) || currentEndDate.equals(r.getEndDate())) {
                JOptionPane.showMessageDialog(this, "Selected Operation room is un-available for the date and time you selected."
                        + " Please select another operation room or modify the start and end date of the operation.");
                return false;
            }
        }
        return true;
    }

    private boolean checkMedicalAvailability(MedicalDevice md) {
        Date currentStartDate = scheduledOperation.getResevation().getStartDate();
        Date currentEndDate = scheduledOperation.getResevation().getEndDate();

        //get schedule of selected operation room
        Schedule schedule = md.getSchedule();

        for (Reservation r : schedule.getReservationList()) {
            Date orStartDate = r.getStartDate();
            Date orEndDate = r.getEndDate();

            //check for dates
            if (currentStartDate.equals(r.getStartDate()) || currentEndDate.equals(r.getEndDate())) {
                JOptionPane.showMessageDialog(this, "Selected medical device is un-available for the date and time you selected."
                        + " Please select another operation room or modify the start and end date of the operation.");
                return false;
            }
        }
        return true;
    }

    private boolean checkDoctorAvailability(Doctor d) {
        Date currentStartDate = scheduledOperation.getResevation().getStartDate();
        Date currentEndDate = scheduledOperation.getResevation().getEndDate();

        //get schedule of selected operation room
        Schedule schedule = d.getSchedule();

        for (Reservation r : schedule.getReservationList()) {
            Date orStartDate = r.getStartDate();
            Date orEndDate = r.getEndDate();

            //check for dates
            if (currentStartDate.equals(r.getStartDate()) || currentEndDate.equals(r.getEndDate())) {
                JOptionPane.showMessageDialog(this, "Selected doctor is un-available for the date and time you selected."
                        + " Please select another doctor or modify the start and end date of the operation.");
                return false;
            }
        }
        return true;
    }

    private boolean checkCareTeamAvailability(CareTeam ct) {
        Date currentStartDate = scheduledOperation.getResevation().getStartDate();
        Date currentEndDate = scheduledOperation.getResevation().getEndDate();

        //get schedule of selected operation room
        Schedule schedule = ct.getSchedule();

        for (Reservation r : schedule.getReservationList()) {
            Date orStartDate = r.getStartDate();
            Date orEndDate = r.getEndDate();

            //check for dates
            if (currentStartDate.equals(r.getStartDate()) || currentEndDate.equals(r.getEndDate())) {
                JOptionPane.showMessageDialog(this, "Selected care team is un-available for the date and time you selected."
                        + " Please select another care team or modify the start and end date of the operation.");
                return false;
            }
        }
        return true;
    }

    private void populatePreviewTab() {
        patientNamejTextField.setText(scheduledOperation.getPatient().getPatientName());
        startDatejTextField.setText(scheduledOperation.getResevation().getStartDate().toString());
        endDatejTextField.setText(scheduledOperation.getResevation().getEndDate().toString());
        operationRoomjTextField.setText(scheduledOperation.getOperationRoom().getRoomName());
        careTeamjTextField.setText(scheduledOperation.getCareTeam().getTeamName());
        doctorNamejTextField.setText(scheduledOperation.getDoctor().getEmployeeName());
        medicalDevicejTextField.setText(scheduledOperation.getMedicalDevice().getDeviceName());
    }
}
