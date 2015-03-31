/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package User.Interface.SystemAdminRole;

import Bsuiness.EcoSystem;
import Business.Enterprise.Enterprise;
import Business.Enterprise.SupplierEnterprise;
import Business.UserAccount.UserAccount;
import java.awt.Color;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

/**
 *
 * @author Asha
 */
public class CreateSupplierAccountJPanel extends javax.swing.JPanel {

    /**
     * Creates new form CreateSupplierAccountJPanel
     */
    private JPanel userProcessContainer;
    private EcoSystem system;
    private UserAccount userAccount;

    /**
     *
     * Creates new form ManageNetworkJPanel
     */
    public CreateSupplierAccountJPanel(JPanel userProcessContainer, EcoSystem system, UserAccount ua) {
        initComponents();
 Color c = new Color(0, 85, 119);
        this.setBackground(c);
        this.userProcessContainer = userProcessContainer;
        this.system = system;
        this.userAccount = ua;                
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
        userNamejLabel = new javax.swing.JLabel();
        jLabel1 = new javax.swing.JLabel();
        userNamejTextField = new javax.swing.JTextField();
        passwordjPasswordField = new javax.swing.JPasswordField();
        DonejButton = new javax.swing.JButton();

        setBackground(new java.awt.Color(0, 85, 119));
        setForeground(new java.awt.Color(255, 255, 255));

        titlejLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel.setText("System Admin - Add Supplier Account");

        userNamejLabel.setText("User name");

        jLabel1.setText("Password");

        DonejButton.setText("Done");
        DonejButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                DonejButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(45, 45, 45)
                        .addComponent(titlejLabel))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(121, 121, 121)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(userNamejLabel)
                            .addComponent(jLabel1))
                        .addGap(18, 18, 18)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(userNamejTextField)
                            .addComponent(passwordjPasswordField, javax.swing.GroupLayout.PREFERRED_SIZE, 107, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(180, 180, 180)
                        .addComponent(DonejButton)))
                .addContainerGap(55, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(31, 31, 31)
                .addComponent(titlejLabel)
                .addGap(32, 32, 32)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(userNamejLabel)
                    .addComponent(userNamejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(passwordjPasswordField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addComponent(DonejButton)
                .addContainerGap(116, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void DonejButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_DonejButtonActionPerformed
        // TODO add your handling code here:
        //get the user name
        String userName = userNamejTextField.getText();

        //get the password
        String passowrd = String.valueOf(passwordjPasswordField.getPassword());
         boolean isValid = validateUserInput(userName, passowrd);
        if (false == isValid) {
            return;
        }
        
        userAccount.setUserName(userName);
        userAccount.setPassword(passowrd);
        userAccount.setIsApproved(true);
              
        SupplierEnterprise newSupplierEnterprise = system.CreateAndAddSupplierEnterprise(userName);
        newSupplierEnterprise.getUserAccountDirectory().createUserAccount(userName, passowrd, null,
                userAccount.getRole(), "Active", userAccount.getSecurityQuestion(), userAccount.getSecurityAnswer());   
    }//GEN-LAST:event_DonejButtonActionPerformed

     private boolean validateUserInput(String userName, String password) {
        //check if any field is empty
         if (true == userName.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Please enter a valid user name.", "Error", JOptionPane.WARNING_MESSAGE);
            return false; 
        }

        if (true == password.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Please enter a valid password.", "Error", JOptionPane.WARNING_MESSAGE);
            return false; 
        }
        return true;
     }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton DonejButton;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JPasswordField passwordjPasswordField;
    private javax.swing.JLabel titlejLabel;
    private javax.swing.JLabel userNamejLabel;
    private javax.swing.JTextField userNamejTextField;
    // End of variables declaration//GEN-END:variables
}
