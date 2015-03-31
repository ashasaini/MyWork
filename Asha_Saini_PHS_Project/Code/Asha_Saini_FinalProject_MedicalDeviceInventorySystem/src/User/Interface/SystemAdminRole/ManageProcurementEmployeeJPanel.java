/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package User.Interface.SystemAdminRole;

import Bsuiness.EcoSystem;
import Business.Employee.Employee;
import Business.Organization.Organization;
import java.awt.CardLayout;
import java.awt.Color;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author Asha
 */
public class ManageProcurementEmployeeJPanel extends javax.swing.JPanel {

    /**
     * Creates new form ManageProcurementEmployeeJPanel
     */
    private JPanel userProcessContainer;
    private EcoSystem system;
    public ManageProcurementEmployeeJPanel(JPanel userProcessContainer, EcoSystem system) {
        initComponents();

        this.userProcessContainer = userProcessContainer;
        this.system = system;  
         Color c = new Color(0, 85, 119);
        this.setBackground(c);
        populateOrganizationComboBox();
        populateAddEmplyeeOrgComboBox();
        
        Organization org = (Organization)organizationjComboBox.getItemAt(0);
        if(null != org){
            populateEmployeeTable(org);
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

        titlejLabel1 = new javax.swing.JLabel();
        organizationjComboBox = new javax.swing.JComboBox();
        jLabel2 = new javax.swing.JLabel();
        refreshjButton = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        employeejTable = new javax.swing.JTable();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        addEmployeejPanel = new javax.swing.JPanel();
        jLabel3 = new javax.swing.JLabel();
        addEmployeeOrganizationjComboBox = new javax.swing.JComboBox();
        networkNamejLabel = new javax.swing.JLabel();
        empNameJTextField = new javax.swing.JTextField();
        addEmployeerjButton = new javax.swing.JButton();
        removeEmployeejPanel = new javax.swing.JPanel();
        removeEmployeejButton = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        backjButton = new javax.swing.JButton();

        setBackground(new java.awt.Color(0, 85, 119));
        setForeground(new java.awt.Color(255, 255, 255));

        titlejLabel1.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel1.setText("System Admin - Manage Procurement Enterprise -> Manage Employee");

        organizationjComboBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                organizationjComboBoxActionPerformed(evt);
            }
        });
        organizationjComboBox.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                organizationjComboBoxPropertyChange(evt);
            }
        });

        jLabel2.setText("Organization");

        refreshjButton.setText("Refresh");
        refreshjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                refreshjButtonActionPerformed(evt);
            }
        });

        employeejTable.setFont(new java.awt.Font("Tahoma", 0, 14)); // NOI18N
        employeejTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Employee Id", "Employee name"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.Integer.class, java.lang.String.class
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }
        });
        employeejTable.getTableHeader().setReorderingAllowed(false);
        jScrollPane1.setViewportView(employeejTable);

        jTabbedPane1.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        jTabbedPane1.setTabPlacement(javax.swing.JTabbedPane.LEFT);
        jTabbedPane1.setToolTipText("");

        jLabel3.setText("Organization");

        addEmployeeOrganizationjComboBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                addEmployeeOrganizationjComboBoxActionPerformed(evt);
            }
        });
        addEmployeeOrganizationjComboBox.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                addEmployeeOrganizationjComboBoxPropertyChange(evt);
            }
        });

        networkNamejLabel.setText("Employee name");

        addEmployeerjButton.setText("Add Employee");
        addEmployeerjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                addEmployeerjButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout addEmployeejPanelLayout = new javax.swing.GroupLayout(addEmployeejPanel);
        addEmployeejPanel.setLayout(addEmployeejPanelLayout);
        addEmployeejPanelLayout.setHorizontalGroup(
            addEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(addEmployeejPanelLayout.createSequentialGroup()
                .addGroup(addEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(addEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                        .addGroup(addEmployeejPanelLayout.createSequentialGroup()
                            .addContainerGap()
                            .addComponent(networkNamejLabel)
                            .addGap(28, 28, 28)
                            .addComponent(empNameJTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 138, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGroup(javax.swing.GroupLayout.Alignment.LEADING, addEmployeejPanelLayout.createSequentialGroup()
                            .addGap(65, 65, 65)
                            .addComponent(jLabel3)
                            .addGap(18, 18, 18)
                            .addComponent(addEmployeeOrganizationjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 140, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(addEmployeejPanelLayout.createSequentialGroup()
                        .addGap(120, 120, 120)
                        .addComponent(addEmployeerjButton)))
                .addContainerGap(55, Short.MAX_VALUE))
        );
        addEmployeejPanelLayout.setVerticalGroup(
            addEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(addEmployeejPanelLayout.createSequentialGroup()
                .addGap(39, 39, 39)
                .addGroup(addEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(addEmployeeOrganizationjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(addEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(empNameJTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(networkNamejLabel))
                .addGap(18, 18, 18)
                .addComponent(addEmployeerjButton)
                .addContainerGap())
        );

        jTabbedPane1.addTab("Add Employee", addEmployeejPanel);

        removeEmployeejButton.setText("Remove Employee");
        removeEmployeejButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                removeEmployeejButtonActionPerformed(evt);
            }
        });

        jLabel1.setText("Select an employee form the list and");
        jLabel1.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));

        jLabel4.setText(" click 'Remove Employee' button");

        javax.swing.GroupLayout removeEmployeejPanelLayout = new javax.swing.GroupLayout(removeEmployeejPanel);
        removeEmployeejPanel.setLayout(removeEmployeejPanelLayout);
        removeEmployeejPanelLayout.setHorizontalGroup(
            removeEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(removeEmployeejPanelLayout.createSequentialGroup()
                .addGap(73, 73, 73)
                .addGroup(removeEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.CENTER)
                    .addComponent(jLabel1)
                    .addComponent(jLabel4)
                    .addComponent(removeEmployeejButton))
                .addContainerGap(92, Short.MAX_VALUE))
        );
        removeEmployeejPanelLayout.setVerticalGroup(
            removeEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(removeEmployeejPanelLayout.createSequentialGroup()
                .addGap(20, 20, 20)
                .addComponent(jLabel1)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jLabel4)
                .addGap(34, 34, 34)
                .addComponent(removeEmployeejButton)
                .addContainerGap(57, Short.MAX_VALUE))
        );

        jTabbedPane1.addTab("Remove Employee", removeEmployeejPanel);

        backjButton.setText("<< Back");
        backjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                backjButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(24, 24, 24)
                        .addComponent(titlejLabel1))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(88, 88, 88)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(refreshjButton)
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                .addGroup(layout.createSequentialGroup()
                                    .addComponent(jLabel2)
                                    .addGap(41, 41, 41)
                                    .addComponent(organizationjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 158, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addComponent(jTabbedPane1)
                                .addComponent(backjButton)
                                .addComponent(jScrollPane1)))))
                .addContainerGap(29, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(25, 25, 25)
                .addComponent(titlejLabel1)
                .addGap(33, 33, 33)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel2)
                    .addComponent(organizationjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 118, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(refreshjButton)
                .addGap(18, 18, 18)
                .addComponent(jTabbedPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 177, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(backjButton)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void organizationjComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_organizationjComboBoxActionPerformed
        // TODO add your handling code here:
        Organization org = (Organization)organizationjComboBox.getSelectedItem();
        if(null != org){
            populateEmployeeTable(org);
        }
    }//GEN-LAST:event_organizationjComboBoxActionPerformed

    private void organizationjComboBoxPropertyChange(java.beans.PropertyChangeEvent evt) {//GEN-FIRST:event_organizationjComboBoxPropertyChange
        // TODO add your handling code here:
        //         Organization org = (Organization)organizationjComboBox.getSelectedItem();
        //        if(null != org){
            //            refreshTable(org);
            //        }
    }//GEN-LAST:event_organizationjComboBoxPropertyChange

    private void refreshjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_refreshjButtonActionPerformed
        // TODO add your handling code here:
        Organization org = (Organization)organizationjComboBox.getSelectedItem();
        if(null != org){
            populateEmployeeTable(org);
        }
    }//GEN-LAST:event_refreshjButtonActionPerformed

    private void addEmployeeOrganizationjComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_addEmployeeOrganizationjComboBoxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_addEmployeeOrganizationjComboBoxActionPerformed

    private void addEmployeeOrganizationjComboBoxPropertyChange(java.beans.PropertyChangeEvent evt) {//GEN-FIRST:event_addEmployeeOrganizationjComboBoxPropertyChange
        // TODO add your handling code here:
    }//GEN-LAST:event_addEmployeeOrganizationjComboBoxPropertyChange

    private void addEmployeerjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_addEmployeerjButtonActionPerformed
        // TODO add your handling code here:
        //get the selected organization
        Organization org = (Organization)addEmployeeOrganizationjComboBox.getSelectedItem();
        if(null != org){
            //get the employee name
            String empName = empNameJTextField.getText();
            if((null == empName) || (true == empName.isEmpty())){
                JOptionPane.showMessageDialog(this, "Please enter a valid employee name", "Warning", JOptionPane.WARNING_MESSAGE);
                return;
            }
            org.getEmployeeDirectory().createEmployee(empName, 26);
        }
        populateEmployeeTable(org);
    }//GEN-LAST:event_addEmployeerjButtonActionPerformed

    private void removeEmployeejButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_removeEmployeejButtonActionPerformed
        // TODO add your handling code here:
        Organization org = (Organization)organizationjComboBox.getSelectedItem();
        if(null == org){
            JOptionPane.showMessageDialog(this, "An error occurred.", "Warning", JOptionPane.WARNING_MESSAGE);
            return;
        }

        int selectedRow = employeejTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select an employee from the list.", "Warning", JOptionPane.WARNING_MESSAGE);
            return;
        }

        //pending
        //        String str = org.getName();
        //        if(true == str.equalsIgnoreCase("Admin")){
            //            AdminOrganization aOrg = (AdminOrganization)org;
            //            Employee e = (Employee)employeejTable.getValueAt(selectedRow, 0);
            //            aOrg.getEmployeeDirectory().removeEmployee(e);
            //            refreshTable(aOrg);
            //        }else if(true == str.equalsIgnoreCase("Sales")){
            //            SalesOrganization sOrg = (SalesOrganization)org;
            //            Employee e = (Employee)employeejTable.getValueAt(selectedRow, 0);
            //            sOrg.getEmployeeDirectory().removeEmployee(e);
            //            refreshTable(sOrg);
            //        }else if(true == str.equalsIgnoreCase("Shipping")){
            //            ShippingOrganization sOrg = (ShippingOrganization)org;
            //            Employee e = (Employee)employeejTable.getValueAt(selectedRow, 0);
            //            sOrg.getEmployeeDirectory().removeEmployee(e);
            //            refreshTable(sOrg);
            //        }else{
            //            //do nothing
            //        }
    }//GEN-LAST:event_removeEmployeejButtonActionPerformed

    private void backjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_backjButtonActionPerformed
        // TODO add your handling code here:
        userProcessContainer.remove(this);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.previous(userProcessContainer);
    }//GEN-LAST:event_backjButtonActionPerformed

    private void populateEmployeeTable(Organization org) {
        DefaultTableModel model = (DefaultTableModel) employeejTable.getModel();
        
        model.setRowCount(0);
        
        for (Employee employee : org.getEmployeeDirectory().getEmployeeList()){
            Object[] row = new Object[2];
            row[0] = employee.getEmployeeId();
            row[1] = employee.getEmployeeName();
            model.addRow(row);
        }
    }

    private void populateOrganizationComboBox() {        
        organizationjComboBox.removeAllItems();
        
        for (Organization organization : system.getProcurementEnterprise().getOrganizationDirectory().getOrganizationList()){
            organizationjComboBox.addItem(organization);
        }
    }

    private void populateAddEmplyeeOrgComboBox() {
        addEmployeeOrganizationjComboBox.removeAllItems();
        
        for (Organization organization : system.getProcurementEnterprise().getOrganizationDirectory().getOrganizationList()){
            addEmployeeOrganizationjComboBox.addItem(organization);
        }
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JComboBox addEmployeeOrganizationjComboBox;
    private javax.swing.JPanel addEmployeejPanel;
    private javax.swing.JButton addEmployeerjButton;
    private javax.swing.JButton backjButton;
    private javax.swing.JTextField empNameJTextField;
    private javax.swing.JTable employeejTable;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTabbedPane jTabbedPane1;
    private javax.swing.JLabel networkNamejLabel;
    private javax.swing.JComboBox organizationjComboBox;
    private javax.swing.JButton refreshjButton;
    private javax.swing.JButton removeEmployeejButton;
    private javax.swing.JPanel removeEmployeejPanel;
    private javax.swing.JLabel titlejLabel1;
    // End of variables declaration//GEN-END:variables
}