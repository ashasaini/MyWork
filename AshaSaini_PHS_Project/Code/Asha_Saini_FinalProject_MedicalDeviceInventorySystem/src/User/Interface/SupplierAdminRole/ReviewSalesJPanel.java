/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package User.Interface.SupplierAdminRole;

import Business.MedicalDevice.MedicalDevice;
import Business.Organization.SupplierAdminOrganization;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Point;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.table.DefaultTableModel;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartFrame;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.DefaultCategoryDataset;

/**
 *
 * @author Asha
 */
public class ReviewSalesJPanel extends javax.swing.JPanel {

    /**
     * Creates new form ReviewSalesJPanel
     */
    private JPanel userProcessContainer;
    private SupplierAdminOrganization supplierAdminOrganization;
    
    public ReviewSalesJPanel(JPanel userProcessContainer, SupplierAdminOrganization supplierAdminOrganization) {
        initComponents();
        this.userProcessContainer = userProcessContainer;
        this.supplierAdminOrganization = supplierAdminOrganization;

        displayData();
    }

    private void displayData() {
        try {

            //check if supplier has products
            if (supplierAdminOrganization.getMedicalDeviceCatalog().getMedicalDeviceList().size() < 1) {
                JOptionPane.showMessageDialog(this, "No products avaiailable from supplier");
                return;
            }

            int rowCount = productjTable.getRowCount();
            DefaultTableModel model = (DefaultTableModel) productjTable.getModel();
            for (int i = rowCount - 1; i >= 0; i--) {
                model.removeRow(i);
            }

            double totalSaleAmt = 0;
            int topSoldProductID = 1;

            for (MedicalDevice md : supplierAdminOrganization.getMedicalDeviceCatalog().getMedicalDeviceList()) {
                Object row[] = new Object[4];
                row[0] = md;
                row[1] = md.getDeviceId();
                row[2] = md.getSoldQuantity();
                double totalProductSale = (md.getPrice() * md.getSoldQuantity());
                row[3] = totalProductSale;
                model.addRow(row);

                totalSaleAmt = totalSaleAmt + totalProductSale;
                if (md.getSoldQuantity() > supplierAdminOrganization.getMedicalDeviceCatalog().getMedicalDeviceList().get(topSoldProductID - 1).getSoldQuantity()) {
                    topSoldProductID = md.getDeviceId();
                }
            }

            if (totalSaleAmt > 0) {
                topSoldjTextField.setText(supplierAdminOrganization.getMedicalDeviceCatalog().getMedicalDeviceList().get(topSoldProductID - 1).getDeviceName());
                totalSalejTextField.setText(String.valueOf(totalSaleAmt));
            } else {
                topSoldjTextField.setText("None");
                totalSalejTextField.setText("0");
            }

        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "An error occurred");
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

        jLabel1 = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        productjTable = new javax.swing.JTable();
        jLabel3 = new javax.swing.JLabel();
        totalSalejTextField = new javax.swing.JTextField();
        jLabel4 = new javax.swing.JLabel();
        topSoldjTextField = new javax.swing.JTextField();
        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();

        setBackground(new java.awt.Color(0, 82, 119));
        setForeground(new java.awt.Color(255, 255, 255));

        jLabel1.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        jLabel1.setForeground(new java.awt.Color(255, 255, 255));
        jLabel1.setText("Sales Performance Overview");

        productjTable.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        productjTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Device Name", "Device ID", "Items sold", "Total sale"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }
        });
        productjTable.setEnabled(false);
        productjTable.getTableHeader().setReorderingAllowed(false);
        jScrollPane1.setViewportView(productjTable);

        jLabel3.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel3.setForeground(new java.awt.Color(255, 255, 255));
        jLabel3.setText("Total sale amount:");

        totalSalejTextField.setEditable(false);
        totalSalejTextField.setBackground(new java.awt.Color(120, 120, 120));
        totalSalejTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        totalSalejTextField.setForeground(new java.awt.Color(255, 255, 255));

        jLabel4.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel4.setForeground(new java.awt.Color(255, 255, 255));
        jLabel4.setText("Top sold product:");

        topSoldjTextField.setEditable(false);
        topSoldjTextField.setBackground(new java.awt.Color(120, 120, 120));
        topSoldjTextField.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        topSoldjTextField.setForeground(new java.awt.Color(255, 255, 255));

        jButton1.setBackground(new java.awt.Color(30, 30, 30));
        jButton1.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jButton1.setForeground(new java.awt.Color(255, 255, 255));
        jButton1.setText("View Performance Report");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jButton2.setBackground(new java.awt.Color(30, 30, 30));
        jButton2.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jButton2.setForeground(new java.awt.Color(255, 255, 255));
        jButton2.setText("<< Back");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(45, 45, 45)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 520, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 64, Short.MAX_VALUE))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel4)
                                .addGap(18, 18, 18)
                                .addComponent(topSoldjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 94, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(jButton2)
                                    .addComponent(jLabel3))
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addGap(18, 18, 18)
                                        .addComponent(totalSalejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 94, javax.swing.GroupLayout.PREFERRED_SIZE))
                                    .addGroup(layout.createSequentialGroup()
                                        .addGap(118, 118, 118)
                                        .addComponent(jButton1))))
                            .addGroup(layout.createSequentialGroup()
                                .addGap(153, 153, 153)
                                .addComponent(jLabel1)))
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
        );

        layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {topSoldjTextField, totalSalejTextField});

        layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {jLabel3, jLabel4});

        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(jLabel1)
                .addGap(43, 43, 43)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 164, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(93, 93, 93)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel4)
                    .addComponent(topSoldjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(totalSalejTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(40, 40, 40)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jButton1)
                    .addComponent(jButton2))
                .addContainerGap(81, Short.MAX_VALUE))
        );

        layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {topSoldjTextField, totalSalejTextField});

        layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {jLabel3, jLabel4});

    }// </editor-fold>//GEN-END:initComponents

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        // TODO add your handling code here:
        displayBarCharts();
    }//GEN-LAST:event_jButton1ActionPerformed

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        // TODO add your handling code here:
        userProcessContainer.remove(this);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.previous(userProcessContainer);
    }//GEN-LAST:event_jButton2ActionPerformed

    private void displayBarCharts() {

        //check if the supplier has products in the product list
        if (supplierAdminOrganization.getMedicalDeviceCatalog().getMedicalDeviceList().size() < 0) {
            JOptionPane.showMessageDialog(this, "No products found from selected supplier.", "Warning", JOptionPane.WARNING_MESSAGE);
            return;
        }

        MedicalDevice[] product;
        product = new MedicalDevice[100];

        int numberOfProducts = supplierAdminOrganization.getMedicalDeviceCatalog().getMedicalDeviceList().size();
        DefaultCategoryDataset dataSetProduct = new DefaultCategoryDataset();

        for (int i = 0; i < numberOfProducts; i++) {
            product[i] = supplierAdminOrganization.getMedicalDeviceCatalog().getMedicalDeviceList().get(i);
            int soldQuantity = 0;
            soldQuantity = product[i].getSoldQuantity();
            String prodName = product[i].getDeviceName();
            dataSetProduct.setValue(soldQuantity, "Medical Device", prodName);
        }

        JFreeChart chartProduct = ChartFactory.createBarChart("Supplier Performance Report", "Products", "Number of Products Sold", dataSetProduct, PlotOrientation.VERTICAL, false, true, false);

        CategoryPlot p1 = chartProduct.getCategoryPlot();
        p1.setRangeGridlinePaint(Color.black);
        ChartFrame frame1 = new ChartFrame("Supplier Performance Report", chartProduct);
        frame1.setVisible(true);
        frame1.setSize(400, 400);
        Point pt1 = new Point(0, 0);
        frame1.setLocation(pt1);
    }


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTable productjTable;
    private javax.swing.JTextField topSoldjTextField;
    private javax.swing.JTextField totalSalejTextField;
    // End of variables declaration//GEN-END:variables
}
