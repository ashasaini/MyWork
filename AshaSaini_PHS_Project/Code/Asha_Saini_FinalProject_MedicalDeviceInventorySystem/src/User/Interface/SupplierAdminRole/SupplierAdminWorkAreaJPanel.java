/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package User.Interface.SupplierAdminRole;

import Business.Organization.SupplierAdminOrganization;
import java.awt.CardLayout;
import java.awt.Color;
import javax.swing.JPanel;

/**
 *
 * @author Asha
 */
public class SupplierAdminWorkAreaJPanel extends javax.swing.JPanel {

    /**
     * Creates new form SupplierAdminWorkAreaJPanel
     */
    private JPanel userProcessContainer;
    private SupplierAdminOrganization supplierAdminOrganization;
    public SupplierAdminWorkAreaJPanel(JPanel userProcessContainer, SupplierAdminOrganization supplierAdminOrganization) {
        initComponents();
        this.userProcessContainer = userProcessContainer;
        this.supplierAdminOrganization = supplierAdminOrganization;
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
        manageDeviceCatalogjButton = new javax.swing.JButton();
        manageOrdersjButton = new javax.swing.JButton();
        reviewSalesPerformancejButton = new javax.swing.JButton();

        setBackground(new java.awt.Color(0, 85, 119));

        titlejLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel.setForeground(new java.awt.Color(255, 255, 255));
        titlejLabel.setText("Supplier Admin Work Area");

        manageDeviceCatalogjButton.setBackground(new java.awt.Color(30, 30, 30));
        manageDeviceCatalogjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        manageDeviceCatalogjButton.setForeground(new java.awt.Color(255, 255, 255));
        manageDeviceCatalogjButton.setText("Manage Device Catalog");
        manageDeviceCatalogjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                manageDeviceCatalogjButtonActionPerformed(evt);
            }
        });

        manageOrdersjButton.setBackground(new java.awt.Color(30, 30, 30));
        manageOrdersjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        manageOrdersjButton.setForeground(new java.awt.Color(255, 255, 255));
        manageOrdersjButton.setText("Manage Orders");
        manageOrdersjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                manageOrdersjButtonActionPerformed(evt);
            }
        });

        reviewSalesPerformancejButton.setBackground(new java.awt.Color(30, 30, 30));
        reviewSalesPerformancejButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        reviewSalesPerformancejButton.setForeground(new java.awt.Color(255, 255, 255));
        reviewSalesPerformancejButton.setText("Review Sales Performance");
        reviewSalesPerformancejButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                reviewSalesPerformancejButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(90, 90, 90)
                        .addComponent(titlejLabel))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(127, 127, 127)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                            .addComponent(reviewSalesPerformancejButton)
                            .addComponent(manageDeviceCatalogjButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(manageOrdersjButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
                .addContainerGap(99, Short.MAX_VALUE))
        );

        layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {manageDeviceCatalogjButton, manageOrdersjButton, reviewSalesPerformancejButton});

        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(30, 30, 30)
                .addComponent(titlejLabel)
                .addGap(41, 41, 41)
                .addComponent(manageDeviceCatalogjButton)
                .addGap(18, 18, 18)
                .addComponent(manageOrdersjButton)
                .addGap(18, 18, 18)
                .addComponent(reviewSalesPerformancejButton)
                .addContainerGap(102, Short.MAX_VALUE))
        );

        layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {manageDeviceCatalogjButton, manageOrdersjButton, reviewSalesPerformancejButton});

    }// </editor-fold>//GEN-END:initComponents

    private void manageDeviceCatalogjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_manageDeviceCatalogjButtonActionPerformed
        // TODO add your handling code here:
        ManageMedicalDeviceCatalogJPanel panel = new ManageMedicalDeviceCatalogJPanel(userProcessContainer, supplierAdminOrganization);
        userProcessContainer.add("ManageMedicalDeviceCatalogJPanel", panel);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.next(userProcessContainer);
    }//GEN-LAST:event_manageDeviceCatalogjButtonActionPerformed

    private void manageOrdersjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_manageOrdersjButtonActionPerformed
        // TODO add your handling code here:
              
    }//GEN-LAST:event_manageOrdersjButtonActionPerformed

    private void reviewSalesPerformancejButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_reviewSalesPerformancejButtonActionPerformed
        // TODO add your handling code here:
        ReviewSalesJPanel panel = new ReviewSalesJPanel(userProcessContainer, supplierAdminOrganization);
        userProcessContainer.add("ReviewSalesJPanel", panel);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.next(userProcessContainer);
    }//GEN-LAST:event_reviewSalesPerformancejButtonActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton manageDeviceCatalogjButton;
    private javax.swing.JButton manageOrdersjButton;
    private javax.swing.JButton reviewSalesPerformancejButton;
    private javax.swing.JLabel titlejLabel;
    // End of variables declaration//GEN-END:variables
}
