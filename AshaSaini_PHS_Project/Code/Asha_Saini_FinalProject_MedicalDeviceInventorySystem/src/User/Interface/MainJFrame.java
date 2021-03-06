/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package User.Interface;

import Bsuiness.EcoSystem;
import Business.DB4OUtil.DB4OUtil;
import Business.Enterprise.Enterprise;
import Business.Enterprise.InventoryEnterprise;
import Business.Enterprise.ProcurementEnterprise;
import Business.Enterprise.SupplierEnterprise;
import Business.Network.Network;
import Business.Organization.InventoryAdminOrganization;
import Business.Organization.Organization;
import Business.Organization.ProcurementAdminOrganization;
import Business.Organization.SupplierAdminOrganization;
import Business.Organization.SupplierShippingOrganization;
import Business.UserAccount.UserAccount;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Image;
import javax.swing.ImageIcon;
import javax.swing.JOptionPane;

/**
 *
 * @author Asha
 */
public class MainJFrame extends javax.swing.JFrame {

    /**
     * Creates new form MainJFrame
     */
    private EcoSystem system;
    private DB4OUtil dB4OUtil = DB4OUtil.getInstance();

    public MainJFrame() {
        initComponents();
        system = dB4OUtil.retrieveSystem();
        // this.system = ConfigureASystem.configure();
        Color c = new Color(0, 85, 119);
        container.setBackground(c);
        jPanel1.setBackground(c);
        headerjLabel.setBackground(c);
        headerjPanel.setBackground(c);        
        jSplitPane1.setBackground(c);
        jSplitPane2.setBackground(c);
      //  jPanel3.setBackground(c);
        

        this.setTitle("Partners Healthcare");
        ImageIcon img = new ImageIcon(".\\PHS_icon.png");


this.setIconImage(img.getImage());

    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jSplitPane2 = new javax.swing.JSplitPane();
        headerjPanel = new javax.swing.JPanel();
        headerjLabel = new javax.swing.JLabel();
        jPanel3 = new javax.swing.JPanel();
        jSplitPane1 = new javax.swing.JSplitPane();
        jPanel1 = new javax.swing.JPanel();
        loginJButton = new javax.swing.JButton();
        userNameJTextField = new javax.swing.JTextField();
        passwordField = new javax.swing.JPasswordField();
        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();
        loginJLabel = new javax.swing.JLabel();
        logoutJButton = new javax.swing.JButton();
        membersLoginjLabel = new javax.swing.JLabel();
        alreadyMemberjLabel = new javax.swing.JLabel();
        supplierRegisterjLabel = new javax.swing.JLabel();
        registerSupplierjButton = new javax.swing.JButton();
        container = new javax.swing.JPanel();
        jLabel3 = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jSplitPane2.setDividerLocation(128);
        jSplitPane2.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);
        jSplitPane2.setAutoscrolls(true);
        jSplitPane2.setCursor(new java.awt.Cursor(java.awt.Cursor.HAND_CURSOR));
        jSplitPane2.setMinimumSize(new java.awt.Dimension(1024, 728));
        jSplitPane2.setPreferredSize(new java.awt.Dimension(1024, 728));

        headerjPanel.setMinimumSize(new java.awt.Dimension(1024, 128));

        headerjLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        headerjLabel.setIcon(new javax.swing.ImageIcon(getClass().getResource("/User/Interface/PHS_header.JPG"))); // NOI18N

        javax.swing.GroupLayout headerjPanelLayout = new javax.swing.GroupLayout(headerjPanel);
        headerjPanel.setLayout(headerjPanelLayout);
        headerjPanelLayout.setHorizontalGroup(
            headerjPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(headerjLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 1127, Short.MAX_VALUE)
        );
        headerjPanelLayout.setVerticalGroup(
            headerjPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(headerjLabel, javax.swing.GroupLayout.DEFAULT_SIZE, 128, Short.MAX_VALUE)
        );

        jSplitPane2.setTopComponent(headerjPanel);

        jSplitPane1.setDividerLocation(170);
        jSplitPane1.setAutoscrolls(true);

        jPanel1.setBackground(new java.awt.Color(0, 85, 119));

        loginJButton.setBackground(new java.awt.Color(30, 30, 30));
        loginJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        loginJButton.setForeground(new java.awt.Color(255, 255, 255));
        loginJButton.setText("Login");
        loginJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                loginJButtonActionPerformed(evt);
            }
        });

        userNameJTextField.setBackground(new java.awt.Color(120, 120, 120));
        userNameJTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        userNameJTextField.setForeground(new java.awt.Color(255, 255, 255));

        passwordField.setBackground(new java.awt.Color(120, 120, 120));
        passwordField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        passwordField.setForeground(new java.awt.Color(255, 255, 255));

        jLabel1.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel1.setForeground(new java.awt.Color(255, 255, 255));
        jLabel1.setText("User Name");

        jLabel2.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel2.setForeground(new java.awt.Color(255, 255, 255));
        jLabel2.setText("Password");

        logoutJButton.setBackground(new java.awt.Color(30, 30, 30));
        logoutJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        logoutJButton.setForeground(new java.awt.Color(255, 255, 255));
        logoutJButton.setText("Logout");
        logoutJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                logoutJButtonActionPerformed(evt);
            }
        });

        membersLoginjLabel.setFont(new java.awt.Font("Tahoma", 1, 17)); // NOI18N
        membersLoginjLabel.setForeground(new java.awt.Color(255, 255, 255));
        membersLoginjLabel.setText("Welcome!");

        alreadyMemberjLabel.setFont(new java.awt.Font("Tahoma", 1, 14)); // NOI18N
        alreadyMemberjLabel.setForeground(new java.awt.Color(255, 255, 255));
        alreadyMemberjLabel.setText("Members Login");

        supplierRegisterjLabel.setFont(new java.awt.Font("Tahoma", 1, 14)); // NOI18N
        supplierRegisterjLabel.setForeground(new java.awt.Color(255, 255, 255));
        supplierRegisterjLabel.setText("Supplier Registration");

        registerSupplierjButton.setBackground(new java.awt.Color(30, 30, 30));
        registerSupplierjButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        registerSupplierjButton.setForeground(new java.awt.Color(255, 255, 255));
        registerSupplierjButton.setText("Register");
        registerSupplierjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                registerSupplierjButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addComponent(supplierRegisterjLabel))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(30, 30, 30)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addComponent(logoutJButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(loginJLabel))
                            .addComponent(alreadyMemberjLabel)
                            .addComponent(membersLoginjLabel)
                            .addComponent(jLabel1)
                            .addComponent(userNameJTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel2)
                            .addComponent(passwordField, javax.swing.GroupLayout.PREFERRED_SIZE, 69, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(loginJButton, javax.swing.GroupLayout.DEFAULT_SIZE, 82, Short.MAX_VALUE)))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(28, 28, 28)
                        .addComponent(registerSupplierjButton, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel1Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {loginJButton, logoutJButton, passwordField, userNameJTextField});

        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(52, 52, 52)
                .addComponent(membersLoginjLabel)
                .addGap(18, 18, 18)
                .addComponent(alreadyMemberjLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jLabel1)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(userNameJTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jLabel2)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(passwordField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(loginJButton)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(34, 34, 34)
                        .addComponent(loginJLabel))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(18, 18, 18)
                        .addComponent(logoutJButton)))
                .addGap(71, 71, 71)
                .addComponent(supplierRegisterjLabel)
                .addGap(18, 18, 18)
                .addComponent(registerSupplierjButton)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel1Layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {loginJButton, logoutJButton, passwordField, userNameJTextField});

        jSplitPane1.setLeftComponent(jPanel1);

        container.setLayout(new java.awt.CardLayout());

        jLabel3.setBackground(new java.awt.Color(0, 85, 119));
        jLabel3.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/User/Interface/PHS_home.jpg"))); // NOI18N
        container.add(jLabel3, "card2");

        jSplitPane1.setRightComponent(container);

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addComponent(jSplitPane1)
                .addContainerGap())
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jSplitPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 509, Short.MAX_VALUE)
        );

        jSplitPane2.setRightComponent(jPanel3);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jSplitPane2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jSplitPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 643, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void loginJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_loginJButtonActionPerformed
        //read the username

        String userName = userNameJTextField.getText();

        //read the password
        char[] passwordCharArray = passwordField.getPassword();
        String password = String.valueOf(passwordCharArray);

        //validate user's input
        if ((null == userName)
                || (null == password)
                || (true == userName.isEmpty())
                || (true == password.isEmpty())) {
            JOptionPane.showMessageDialog(this, "Please enter a valid user name and password! ", "Error", 0, null);
            return;
        }

        Enterprise inEnterprise = null;
        Organization inOrganization = null;
        SupplierEnterprise inSupplierEnterprise = null;
        SupplierAdminOrganization inSupplierAdminOrganization = null;
        SupplierShippingOrganization inSupplierShippingOrganization = null;
        ProcurementEnterprise inProcurementEnterprise = system.getProcurementEnterprise();
        ProcurementAdminOrganization inProcurementAdminOrganization = null;
        InventoryEnterprise inInventoryEnterprise = system.getInventoryEnterprise();
        InventoryAdminOrganization inInventoryAdminOrganization = null;
        //Organization inSEOrganization = null;

        //find which organization user account belongs to
        //check if user account is a sys admin
        UserAccount userAccount = system.getUserAccountDirectory().authenticateUser(userName, password);
        if (null == userAccount) {
            //not a sys admin

            //check if it is a registered supplier
            userAccount = system.serachUserAccountInRegisteredSupplier(userName, password);
            if (null != userAccount) {
                if (true != userAccount.isIsApproved()) {
                    DisplayInfoJPanel panel = new DisplayInfoJPanel();
                    container.add("DisplayInfoJPanel", panel);
                    CardLayout layout = (CardLayout) container.getLayout();
                    layout.next(container);
                    return;
                }
            }

            //check in networks            
            for (Network network : system.getNetworkList()) {
                //check in the hospitals
                for (Enterprise enterprise : network.getEnterpriseDirectory().getEnterpriseList()) {
                    userAccount = enterprise.getUserAccountDirectory().authenticateUser(userName, password);
                    if (null == userAccount) {    //not an neterprise admin
                        //check if it is a organization user account
                        for (Organization organization : enterprise.getOrganizationDirectory().getOrganizationList()) {
                            userAccount = organization.getUserAccountDirectory().authenticateUser(userName, password);
                            if (null != userAccount) {
                                //user account not found
                                inEnterprise = enterprise;
                                inOrganization = organization;
                                break;
                            }
                        }//end of org for loop
                    } else {  //found the account in enterprise
                        inEnterprise = enterprise;
                        break;
                    }
                    if (null != inEnterprise) {
                        break;
                    }
                }//end of enterprise loop
                if (null != inEnterprise) {
                    break;
                }
            }//end of network for loop

            //check in supplier enterprise list  
            if (null == userAccount) {
                for (SupplierEnterprise se : system.getSupplierEnterpriseList()) {
                    userAccount = se.getUserAccountDirectory().authenticateUser(userName, password);
                    if (null != userAccount) {
                        inSupplierEnterprise = se;
                        //JOptionPane.showMessageDialog(this, "found in supplier enterprise");
                        break;
                    }
                    //if not found then check in various org inside supplier enterprise               
                    for (Organization org : se.getOrganizationDirectory().getOrganizationList()) {

                        if (org.getType().getValue().equals(Organization.Type.SupplierAdminOrganization.getValue())) {   //if org is supplier admin org
                            userAccount = org.getUserAccountDirectory().authenticateUser(userName, password);
                            if (null != userAccount) {
                                //JOptionPane.showMessageDialog(this, "found in supplier admin org");
                                inSupplierAdminOrganization = (SupplierAdminOrganization) org;
                                break;
                            }
                        } else if (org.getType().getValue().equals(Organization.Type.SupplierShippingOrganization.getValue())) { //if org is supplier shipping org
                            userAccount = org.getUserAccountDirectory().authenticateUser(userName, password);
                            if (null != userAccount) {
                                //JOptionPane.showMessageDialog(this, "found in supplier shippingorg");
                                inSupplierShippingOrganization = (SupplierShippingOrganization) org;
                                break;
                            }
                        }
                    }   //end of supplier enterprise organization for
                    if (null != inSupplierAdminOrganization || null != inSupplierShippingOrganization) {
                        inSupplierEnterprise = se;
                        break;
                    }
                }//end of Supplier enterprise for
            }

            if (null == userAccount) {
                //check in procurement enterprise
                ProcurementEnterprise pe = system.getProcurementEnterprise();
                userAccount = pe.getUserAccountDirectory().authenticateUser(userName, password);
                if (null != userAccount) {
                    //JOptionPane.showMessageDialog(this, "found in procurement enterprise");
                    inProcurementEnterprise = pe;
                } else {
                    for (Organization org : pe.getOrganizationDirectory().getOrganizationList()) {
                        userAccount = org.getUserAccountDirectory().authenticateUser(userName, password);
                        if (null != userAccount) {
                            //JOptionPane.showMessageDialog(this, "found in procurement org");
                            inProcurementEnterprise = pe;
                            inProcurementAdminOrganization = (ProcurementAdminOrganization) org;
                            break;
                        }
                    }
                }
            }

            if (null == userAccount) {
                //check in inventory enterprises
                InventoryEnterprise ie = system.getInventoryEnterprise();
                userAccount = ie.getUserAccountDirectory().authenticateUser(userName, password);
                if (null != userAccount) {
                    //JOptionPane.showMessageDialog(this, "found in inventory enterprise");
                    inInventoryEnterprise = ie;
                } else {
                    for (Organization org : ie.getOrganizationDirectory().getOrganizationList()) {
                        userAccount = org.getUserAccountDirectory().authenticateUser(userName, password);
                        if (null != userAccount) {
                            //JOptionPane.showMessageDialog(this, "found in inventory org");
                            inInventoryEnterprise = ie;
                            inInventoryAdminOrganization = (InventoryAdminOrganization) org;
                            break;
                        }
                    }
                }
            }
        }

        if (null == userAccount) {
            //user account not found anywhere
            JOptionPane.showMessageDialog(this, "User Account not found");
        } else {
            CardLayout layout = (CardLayout) container.getLayout();
            container.add("workArea", userAccount.getRole().createWorkArea(container,
                    userAccount,
                    inOrganization,
                    inEnterprise,
                    system,
                    inSupplierEnterprise,
                    inSupplierAdminOrganization,
                    inSupplierShippingOrganization,
                    inProcurementEnterprise,
                    inProcurementAdminOrganization,
                    inInventoryEnterprise,
                    inInventoryAdminOrganization));
            layout.next(container);
        }
    }//GEN-LAST:event_loginJButtonActionPerformed

    private void logoutJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_logoutJButtonActionPerformed

        container.removeAll();
        userNameJTextField.setText(null);
        passwordField.setText(null);

        dB4OUtil.storeSystem(system);

    }//GEN-LAST:event_logoutJButtonActionPerformed

    private void registerSupplierjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_registerSupplierjButtonActionPerformed
        // TODO add your handling code here:
        SupplierRegisterationJPanel sr = new SupplierRegisterationJPanel(container, system);
        container.add("SupplierRegisterationJPanel", sr);
        CardLayout layout = (CardLayout) container.getLayout();
        layout.next(container);
    }//GEN-LAST:event_registerSupplierjButtonActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(MainJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(MainJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(MainJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(MainJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new MainJFrame().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel alreadyMemberjLabel;
    private javax.swing.JPanel container;
    private javax.swing.JLabel headerjLabel;
    private javax.swing.JPanel headerjPanel;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JSplitPane jSplitPane1;
    private javax.swing.JSplitPane jSplitPane2;
    private javax.swing.JButton loginJButton;
    private javax.swing.JLabel loginJLabel;
    private javax.swing.JButton logoutJButton;
    private javax.swing.JLabel membersLoginjLabel;
    private javax.swing.JPasswordField passwordField;
    private javax.swing.JButton registerSupplierjButton;
    private javax.swing.JLabel supplierRegisterjLabel;
    private javax.swing.JTextField userNameJTextField;
    // End of variables declaration//GEN-END:variables
}
