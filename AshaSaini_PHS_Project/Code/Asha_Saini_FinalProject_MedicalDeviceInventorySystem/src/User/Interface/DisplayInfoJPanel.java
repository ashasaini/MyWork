/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package User.Interface;

import java.awt.Color;

/**
 *
 * @author Asha
 */
public class DisplayInfoJPanel extends javax.swing.JPanel {

    /**
     * Creates new form DisplayInfoJPanel
     */
    public DisplayInfoJPanel() {
        initComponents();
        infojLabel.setText("Your account is not yet approved !");
        Color c = new Color(0,85,119);
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

        supplierRegisterFormjLabel = new javax.swing.JLabel();
        infojLabel = new javax.swing.JLabel();

        setBackground(new java.awt.Color(0, 85, 119));

        supplierRegisterFormjLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        supplierRegisterFormjLabel.setForeground(new java.awt.Color(255, 255, 255));
        supplierRegisterFormjLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        supplierRegisterFormjLabel.setText("Information");

        infojLabel.setFont(new java.awt.Font("Tahoma", 1, 14)); // NOI18N
        infojLabel.setForeground(new java.awt.Color(255, 255, 255));
        infojLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        infojLabel.setText("jLabel1");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(325, 325, 325)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.CENTER)
                    .addComponent(supplierRegisterFormjLabel)
                    .addComponent(infojLabel))
                .addContainerGap(313, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(65, 65, 65)
                .addComponent(supplierRegisterFormjLabel)
                .addGap(51, 51, 51)
                .addComponent(infojLabel)
                .addContainerGap(290, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel infojLabel;
    private javax.swing.JLabel supplierRegisterFormjLabel;
    // End of variables declaration//GEN-END:variables
}
