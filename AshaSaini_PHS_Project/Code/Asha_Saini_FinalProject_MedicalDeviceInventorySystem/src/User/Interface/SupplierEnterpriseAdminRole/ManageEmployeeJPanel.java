/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package User.Interface.SupplierEnterpriseAdminRole;

import Business.Employee.Employee;
import Business.Enterprise.SupplierEnterprise;
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
public class ManageEmployeeJPanel extends javax.swing.JPanel {

    /**
     * Creates new form ManageEmployeeJPanel
     */
    private JPanel userProcessContainer;
    private SupplierEnterprise supplierEnterprise;
    public ManageEmployeeJPanel(JPanel userProcessContainer,SupplierEnterprise supplierEnterprise) {
        initComponents();
        this.userProcessContainer = userProcessContainer;
        this.supplierEnterprise = supplierEnterprise;
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

        titlejLabel = new javax.swing.JLabel();
        refreshjButton = new javax.swing.JButton();
        backjButton = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        employeejTable = new javax.swing.JTable();
        jLabel2 = new javax.swing.JLabel();
        organizationjComboBox = new javax.swing.JComboBox();
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

        setBackground(new java.awt.Color(0, 85, 119));
        setForeground(new java.awt.Color(255, 255, 255));

        titlejLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel.setForeground(new java.awt.Color(255, 255, 255));
        titlejLabel.setText("Supplier Admin - Manage Employee");

        refreshjButton.setBackground(new java.awt.Color(30, 30, 30));
        refreshjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        refreshjButton.setForeground(new java.awt.Color(255, 255, 255));
        refreshjButton.setText("Refresh");
        refreshjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                refreshjButtonActionPerformed(evt);
            }
        });

        backjButton.setBackground(new java.awt.Color(30, 30, 30));
        backjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        backjButton.setForeground(new java.awt.Color(255, 255, 255));
        backjButton.setText("<< Back");
        backjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                backjButtonActionPerformed(evt);
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

        jLabel2.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel2.setForeground(new java.awt.Color(255, 255, 255));
        jLabel2.setText("Organization");

        organizationjComboBox.setBackground(new java.awt.Color(120, 120, 120));
        organizationjComboBox.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        organizationjComboBox.setForeground(new java.awt.Color(255, 255, 255));
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

        jTabbedPane1.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        jTabbedPane1.setTabPlacement(javax.swing.JTabbedPane.LEFT);
        jTabbedPane1.setToolTipText("");

        addEmployeejPanel.setBackground(new java.awt.Color(0, 82, 119));

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

        addEmployeerjButton.setBackground(new java.awt.Color(30, 30, 30));
        addEmployeerjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        addEmployeerjButton.setForeground(new java.awt.Color(255, 255, 255));
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
                .addGroup(addEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, addEmployeejPanelLayout.createSequentialGroup()
                        .addGap(65, 65, 65)
                        .addComponent(jLabel3)
                        .addGap(18, 18, 18)
                        .addComponent(addEmployeeOrganizationjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 140, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(addEmployeejPanelLayout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(networkNamejLabel)
                        .addGap(28, 28, 28)
                        .addGroup(addEmployeejPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(addEmployeerjButton)
                            .addComponent(empNameJTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 138, javax.swing.GroupLayout.PREFERRED_SIZE))))
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

        addEmployeejPanelLayout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {addEmployeeOrganizationjComboBox, empNameJTextField});

        jTabbedPane1.addTab("Add Employee", addEmployeejPanel);

        removeEmployeejPanel.setBackground(new java.awt.Color(0, 82, 119));

        removeEmployeejButton.setBackground(new java.awt.Color(30, 30, 30));
        removeEmployeejButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        removeEmployeejButton.setForeground(new java.awt.Color(255, 255, 255));
        removeEmployeejButton.setText("Remove Employee");
        removeEmployeejButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                removeEmployeejButtonActionPerformed(evt);
            }
        });

        jLabel1.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel1.setForeground(new java.awt.Color(255, 255, 255));
        jLabel1.setText("Select an employee form the list and");
        jLabel1.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));

        jLabel4.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel4.setForeground(new java.awt.Color(255, 255, 255));
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
                .addContainerGap(41, Short.MAX_VALUE))
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
                .addContainerGap(55, Short.MAX_VALUE))
        );

        jTabbedPane1.addTab("Remove Employee", removeEmployeejPanel);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(147, 147, 147)
                        .addComponent(titlejLabel))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(450, 450, 450)
                        .addComponent(refreshjButton))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(80, 80, 80)
                        .addComponent(jTabbedPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 452, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(92, 92, 92)
                        .addComponent(backjButton)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addGap(79, 79, 79)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                            .addComponent(jLabel2)
                            .addGap(41, 41, 41)
                            .addComponent(organizationjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 158, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 443, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addContainerGap(81, Short.MAX_VALUE)))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(33, 33, 33)
                .addComponent(titlejLabel)
                .addGap(215, 215, 215)
                .addComponent(refreshjButton)
                .addGap(18, 18, 18)
                .addComponent(jTabbedPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 177, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(29, 29, 29)
                .addComponent(backjButton)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addGap(92, 92, 92)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabel2)
                        .addComponent(organizationjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(18, 18, 18)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 118, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addContainerGap(302, Short.MAX_VALUE)))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void refreshjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_refreshjButtonActionPerformed
        // TODO add your handling code here:
        Organization org = (Organization)organizationjComboBox.getSelectedItem();
        if(null != org){
            populateEmployeeTable(org);
        }
    }//GEN-LAST:event_refreshjButtonActionPerformed

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

    private void addEmployeeOrganizationjComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_addEmployeeOrganizationjComboBoxActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_addEmployeeOrganizationjComboBoxActionPerformed

    private void addEmployeeOrganizationjComboBoxPropertyChange(java.beans.PropertyChangeEvent evt) {//GEN-FIRST:event_addEmployeeOrganizationjComboBoxPropertyChange
        // TODO add your handling code here:
    }//GEN-LAST:event_addEmployeeOrganizationjComboBoxPropertyChange


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
    private javax.swing.JLabel titlejLabel;
    // End of variables declaration//GEN-END:variables

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
        
        for (Organization organization : supplierEnterprise.getOrganizationDirectory().getOrganizationList()){
            organizationjComboBox.addItem(organization);
        }
    }

    private void populateAddEmplyeeOrgComboBox() {
        addEmployeeOrganizationjComboBox.removeAllItems();
        
        for (Organization organization : supplierEnterprise.getOrganizationDirectory().getOrganizationList()){
            addEmployeeOrganizationjComboBox.addItem(organization);
        }
    }
}