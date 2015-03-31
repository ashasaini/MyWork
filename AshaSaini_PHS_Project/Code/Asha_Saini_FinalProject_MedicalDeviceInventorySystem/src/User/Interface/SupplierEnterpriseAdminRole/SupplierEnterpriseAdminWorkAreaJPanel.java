/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package User.Interface.SupplierEnterpriseAdminRole;

import User.Interface.SupplierAdminRole.ManageMedicalDeviceCatalogJPanel;
import Business.Enterprise.SupplierEnterprise;
import java.awt.CardLayout;
import java.awt.Color;
import static java.time.Clock.system;
import javax.swing.JPanel;

/**
 *
 * @author Asha
 */
public class SupplierEnterpriseAdminWorkAreaJPanel extends javax.swing.JPanel {

    /**
     * Creates new form SupplierAdminWorkAreaJPanel
     */
    private JPanel userProcessContainer;
    private SupplierEnterprise supplierEnterprise;
    public SupplierEnterpriseAdminWorkAreaJPanel(JPanel userProcessContainer,SupplierEnterprise supplierEnterprise) {
        initComponents();
        this.userProcessContainer = userProcessContainer;
        this.supplierEnterprise = supplierEnterprise;
         Color c = new Color(0, 85, 119);
        this.setBackground(c);
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
        manageEmployeesjButton = new javax.swing.JButton();
        manageUserAccountsjButton = new javax.swing.JButton();
        manageOrganizationjButton = new javax.swing.JButton();

        setBackground(new java.awt.Color(0, 85, 119));
        setForeground(new java.awt.Color(255, 255, 255));

        titlejLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel.setForeground(new java.awt.Color(255, 255, 255));
        titlejLabel.setText("Supplier Admin Work Area");

        manageEmployeesjButton.setBackground(new java.awt.Color(30, 30, 30));
        manageEmployeesjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        manageEmployeesjButton.setForeground(new java.awt.Color(255, 255, 255));
        manageEmployeesjButton.setText("Manage Employees");
        manageEmployeesjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                manageEmployeesjButtonActionPerformed(evt);
            }
        });

        manageUserAccountsjButton.setBackground(new java.awt.Color(30, 30, 30));
        manageUserAccountsjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        manageUserAccountsjButton.setForeground(new java.awt.Color(255, 255, 255));
        manageUserAccountsjButton.setText("Manage User Accounts");
        manageUserAccountsjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                manageUserAccountsjButtonActionPerformed(evt);
            }
        });

        manageOrganizationjButton.setBackground(new java.awt.Color(30, 30, 30));
        manageOrganizationjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        manageOrganizationjButton.setForeground(new java.awt.Color(255, 255, 255));
        manageOrganizationjButton.setText("Manage Organization");
        manageOrganizationjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                manageOrganizationjButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(85, 85, 85)
                        .addComponent(titlejLabel))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(116, 116, 116)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(manageUserAccountsjButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(manageEmployeesjButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(manageOrganizationjButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
                .addContainerGap(78, Short.MAX_VALUE))
        );

        layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {manageEmployeesjButton, manageUserAccountsjButton});

        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(23, 23, 23)
                .addComponent(titlejLabel)
                .addGap(28, 28, 28)
                .addComponent(manageOrganizationjButton)
                .addGap(18, 18, 18)
                .addComponent(manageEmployeesjButton)
                .addGap(18, 18, 18)
                .addComponent(manageUserAccountsjButton)
                .addContainerGap(122, Short.MAX_VALUE))
        );

        layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {manageEmployeesjButton, manageUserAccountsjButton});

    }// </editor-fold>//GEN-END:initComponents

    private void manageEmployeesjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_manageEmployeesjButtonActionPerformed
        // TODO add your handling code here:
        ManageEmployeeJPanel panel = new ManageEmployeeJPanel(userProcessContainer, supplierEnterprise);
        userProcessContainer.add("ManageEmployeeJPanel", panel);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.next(userProcessContainer);
    }//GEN-LAST:event_manageEmployeesjButtonActionPerformed

    private void manageUserAccountsjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_manageUserAccountsjButtonActionPerformed
        // TODO add your handling code here:
        ManageUserAccountJPanel panel = new ManageUserAccountJPanel(userProcessContainer, supplierEnterprise);
        userProcessContainer.add("ManageUserAccountJPanel", panel);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.next(userProcessContainer);
    }//GEN-LAST:event_manageUserAccountsjButtonActionPerformed

    private void manageOrganizationjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_manageOrganizationjButtonActionPerformed
        // TODO add your handling code here:
        ManageOrganizationJPanel panel = new ManageOrganizationJPanel(userProcessContainer, supplierEnterprise);
        userProcessContainer.add("ManageOrganizationJPanel", panel);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.next(userProcessContainer);
    }//GEN-LAST:event_manageOrganizationjButtonActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton manageEmployeesjButton;
    private javax.swing.JButton manageOrganizationjButton;
    private javax.swing.JButton manageUserAccountsjButton;
    private javax.swing.JLabel titlejLabel;
    // End of variables declaration//GEN-END:variables
}