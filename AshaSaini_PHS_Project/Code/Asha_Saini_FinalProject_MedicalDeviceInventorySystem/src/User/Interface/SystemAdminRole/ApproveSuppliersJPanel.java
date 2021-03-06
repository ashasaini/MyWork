/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package User.Interface.SystemAdminRole;

import Bsuiness.EcoSystem;
import Business.UserAccount.UserAccount;
import java.awt.CardLayout;
import java.awt.Color;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author Asha
 */
public class ApproveSuppliersJPanel extends javax.swing.JPanel {

    /**
     * Creates new form ApproveSuppliersJPanel
     */
    private JPanel userProcessContainer;
    private EcoSystem system;

    /**
     *
     * Creates new form ManageNetworkJPanel
     */
    public ApproveSuppliersJPanel(JPanel userProcessContainer, EcoSystem system) {
        initComponents();

        this.userProcessContainer = userProcessContainer;
        this.system = system;
 Color c = new Color(0, 85, 119);
        this.setBackground(c);
        populateRegisteredSuppliersTable();
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
        jScrollPane1 = new javax.swing.JScrollPane();
        registeredSuppliersjTable = new javax.swing.JTable();
        backJButton = new javax.swing.JButton();
        approveSupplierjButton = new javax.swing.JButton();

        setBackground(new java.awt.Color(0, 85, 119));
        setForeground(new java.awt.Color(255, 255, 255));

        titlejLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel.setText("System Admin - Approve Suppliers");

        registeredSuppliersjTable.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        registeredSuppliersjTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Supplier Name"
            }
        ));
        jScrollPane1.setViewportView(registeredSuppliersjTable);

        backJButton.setBackground(new java.awt.Color(120, 120, 120));
        backJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        backJButton.setForeground(new java.awt.Color(255, 255, 255));
        backJButton.setText("<< Back");
        backJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                backJButtonActionPerformed(evt);
            }
        });

        approveSupplierjButton.setBackground(new java.awt.Color(120, 120, 120));
        approveSupplierjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        approveSupplierjButton.setForeground(new java.awt.Color(255, 255, 255));
        approveSupplierjButton.setText("Approve Supplier");
        approveSupplierjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                approveSupplierjButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(116, 116, 116)
                        .addComponent(titlejLabel))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(43, 43, 43)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(approveSupplierjButton)
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(backJButton)
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 452, javax.swing.GroupLayout.PREFERRED_SIZE)))))
                .addContainerGap(51, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(29, 29, 29)
                .addComponent(titlejLabel)
                .addGap(18, 18, 18)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 154, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(31, 31, 31)
                .addComponent(approveSupplierjButton)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 66, Short.MAX_VALUE)
                .addComponent(backJButton)
                .addGap(73, 73, 73))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void backJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_backJButtonActionPerformed
        // TODO add your handling code here:
        userProcessContainer.remove(this);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.previous(userProcessContainer);
    }//GEN-LAST:event_backJButtonActionPerformed

    private void approveSupplierjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_approveSupplierjButtonActionPerformed
        // TODO add your handling code here:
        //get the selected supplier from the table
        int selectedRow = registeredSuppliersjTable.getSelectedRow();
        if (0 > selectedRow) {
            JOptionPane.showMessageDialog(this, "Please select a supplier from the list.", "Error", JOptionPane.WARNING_MESSAGE);
            return;
        }

        UserAccount ua = (UserAccount) registeredSuppliersjTable.getValueAt(selectedRow, 0);
        if(null == ua){
            //pending status
            return;
        }
        
        CreateSupplierAccountJPanel panel = new CreateSupplierAccountJPanel(userProcessContainer, system,ua);
        userProcessContainer.add("CreateSupplierAccountJPanel", panel);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.next(userProcessContainer);
    }//GEN-LAST:event_approveSupplierjButtonActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton approveSupplierjButton;
    private javax.swing.JButton backJButton;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTable registeredSuppliersjTable;
    private javax.swing.JLabel titlejLabel;
    // End of variables declaration//GEN-END:variables

    private void populateRegisteredSuppliersTable() {
        DefaultTableModel model = (DefaultTableModel) registeredSuppliersjTable.getModel();                
        model.setRowCount(0);
        
        if(0 >= system.getRegisteredSuppliersList().getUserAccountList().size()){
            //pending status
            return;
        }
        
        for (UserAccount ua: system.getRegisteredSuppliersList().getUserAccountList()){
                Object[] row = new Object[1];
                row[0] = ua;                             
                model.addRow(row);            
        }
    }
}
