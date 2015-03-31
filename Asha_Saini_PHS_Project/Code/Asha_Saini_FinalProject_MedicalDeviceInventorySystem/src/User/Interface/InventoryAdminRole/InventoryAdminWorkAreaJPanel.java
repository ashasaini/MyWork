/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package User.Interface.InventoryAdminRole;

import Business.Enterprise.InventoryEnterprise;
import Business.MedicalDevice.MedicalDevice;
import Business.Order.Order;
import Business.Order.OrderItem;
import java.awt.Color;
import java.awt.Point;
import java.util.Date;
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
public class InventoryAdminWorkAreaJPanel extends javax.swing.JPanel {

    /**
     * Creates new form InventoryAdminWorkAreaJPanel
     */
    private JPanel userProcessContainer;
    private InventoryEnterprise inventoryEnterprise;

    public InventoryAdminWorkAreaJPanel(JPanel userProcessContainer, InventoryEnterprise inventoryEnterprise) {
        initComponents();
        this.userProcessContainer = userProcessContainer;
        this.inventoryEnterprise = inventoryEnterprise;
        Color c = new Color(0, 85, 119);
        this.setBackground(c);
        populateDeliveryTable();
        populateWarehouseDevicesTable();
        populateTab3Table();
        populateTab4Table();
    }

    private void populateTab3Table() {
        int rowCount = tab3jTable.getRowCount();
        DefaultTableModel dtm = (DefaultTableModel) tab3jTable.getModel();

        //delete all the previous rows
        for (int i = rowCount - 1; i >= 0; i--) {
            dtm.removeRow(i);
        }

        for (MedicalDevice md : inventoryEnterprise.getWarehouse().getMdeciDeviceCatalog().getMedicalDeviceList()) {
            Object[] row = new Object[4];
            row[0] = md;
            row[1] = md.getDeviceCatagory();
            row[2] = md.getAvailableQuantity();
            row[3] = md.getServiceInterval();
            dtm.addRow(row);
        }

    }

    private void populateTab4Table() {
        int rowCount = tab4jTable.getRowCount();
        DefaultTableModel dtm = (DefaultTableModel) tab4jTable.getModel();

        //delete all the previous rows
        for (int i = rowCount - 1; i >= 0; i--) {
            dtm.removeRow(i);
        }

        for (MedicalDevice md : inventoryEnterprise.getWarehouse().getMdeciDeviceCatalog().getMedicalDeviceList()) {
            Object[] row = new Object[4];
            row[0] = md;
            row[1] = md.getDeviceCatagory();
            row[2] = md.getAvailableQuantity();
            row[3] = md.getServiceInterval();
            dtm.addRow(row);
        }

    }

    private void populateDeliveryTable() {
        int rowCount = deliveryJTable.getRowCount();
        DefaultTableModel dtm = (DefaultTableModel) deliveryJTable.getModel();

        //delete all the previous rows
        for (int i = rowCount - 1; i >= 0; i--) {
            dtm.removeRow(i);
        }

        for (Order o : inventoryEnterprise.getOrderDelivery().getOrderList()) {
            Object[] row = new Object[1];
            row[0] = o;
            dtm.addRow(row);
        }
    }

    private void populateWarehouseDevicesTable() {
        int rowCount = warehouseDevicesjTable.getRowCount();
        DefaultTableModel dtm = (DefaultTableModel) warehouseDevicesjTable.getModel();

        //delete all the previous rows
        for (int i = rowCount - 1; i >= 0; i--) {
            dtm.removeRow(i);
        }

        for (MedicalDevice md : inventoryEnterprise.getWarehouse().getMdeciDeviceCatalog().getMedicalDeviceList()) {
            Object[] row = new Object[4];
            row[0] = md;
            row[1] = md.getDeviceCatagory();
            row[2] = md.getAvailableQuantity();
            row[3] = md.getServiceInterval();
            dtm.addRow(row);
        }
    }

    private void populateDetailsOrderTable() {

        //get the selected order
        int selectedRow = deliveryJTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select an order from the list");
            return;
        }

        Order o = (Order) deliveryJTable.getValueAt(selectedRow, 0);
        if (null == o) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected order's information");
            return;
        }

        int rowCount = detailsorderJTable.getRowCount();
        DefaultTableModel dtm = (DefaultTableModel) detailsorderJTable.getModel();

        //delete all the previous rows
        for (int i = rowCount - 1; i >= 0; i--) {
            dtm.removeRow(i);
        }

        for (OrderItem oi : o.getOrderItemList()) {
            Object[] row = new Object[4];
            row[0] = oi;
            row[1] = oi.getMedicalDevice().getPrice();
            row[2] = oi.getQuantity();
            row[3] = (oi.getMedicalDevice().getPrice() * oi.getQuantity());
            dtm.addRow(row);
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
        jTabbedPane1 = new javax.swing.JTabbedPane();
        jPanel1 = new javax.swing.JPanel();
        jScrollPane3 = new javax.swing.JScrollPane();
        deliveryJTable = new javax.swing.JTable();
        jScrollPane2 = new javax.swing.JScrollPane();
        detailsorderJTable = new javax.swing.JTable();
        processOrderjButton = new javax.swing.JButton();
        jPanel2 = new javax.swing.JPanel();
        jScrollPane4 = new javax.swing.JScrollPane();
        warehouseDevicesjTable = new javax.swing.JTable();
        jPanel3 = new javax.swing.JPanel();
        jScrollPane5 = new javax.swing.JScrollPane();
        tab3jTable = new javax.swing.JTable();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        lastServicedOnjTextField = new javax.swing.JTextField();
        maintenanceDueOnjTextField = new javax.swing.JTextField();
        jLabel1 = new javax.swing.JLabel();
        probjTextField = new javax.swing.JTextField();
        jPanel4 = new javax.swing.JPanel();
        jLabel5 = new javax.swing.JLabel();
        jScrollPane6 = new javax.swing.JScrollPane();
        tab4jTable = new javax.swing.JTable();
        jButton1 = new javax.swing.JButton();
        jPanel5 = new javax.swing.JPanel();

        setBackground(new java.awt.Color(0, 85, 119));

        titlejLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel.setForeground(new java.awt.Color(255, 255, 255));
        titlejLabel.setText("Inventory Admin Work Area");

        jPanel1.setBackground(new java.awt.Color(0, 82, 119));

        deliveryJTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Order ID"
            }
        ));
        deliveryJTable.getTableHeader().setReorderingAllowed(false);
        deliveryJTable.addFocusListener(new java.awt.event.FocusAdapter() {
            public void focusGained(java.awt.event.FocusEvent evt) {
                deliveryJTableFocusGained(evt);
            }
        });
        jScrollPane3.setViewportView(deliveryJTable);

        detailsorderJTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Device Name", "Price", "Quantity", "Total Amount"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class, java.lang.Integer.class
            };
            boolean[] canEdit = new boolean [] {
                false, false, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        detailsorderJTable.getTableHeader().setReorderingAllowed(false);
        jScrollPane2.setViewportView(detailsorderJTable);

        processOrderjButton.setText("Store to Warehouse");
        processOrderjButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                processOrderjButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(41, 41, 41)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(processOrderjButton)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 569, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 569, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(41, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(38, 38, 38)
                .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 132, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(48, 48, 48)
                .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 144, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(processOrderjButton)
                .addContainerGap(28, Short.MAX_VALUE))
        );

        jTabbedPane1.addTab("View Deliveries", jPanel1);

        jPanel2.setBackground(new java.awt.Color(0, 82, 119));

        warehouseDevicesjTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Device Name", "Catagory", "Quantity", "Maintenance due in (months)"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class, java.lang.Integer.class
            };
            boolean[] canEdit = new boolean [] {
                false, false, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        warehouseDevicesjTable.getTableHeader().setReorderingAllowed(false);
        jScrollPane4.setViewportView(warehouseDevicesjTable);

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(19, 19, 19)
                .addComponent(jScrollPane4, javax.swing.GroupLayout.PREFERRED_SIZE, 622, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(jScrollPane4, javax.swing.GroupLayout.PREFERRED_SIZE, 144, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(266, Short.MAX_VALUE))
        );

        jTabbedPane1.addTab("View Warehouse Devices", jPanel2);

        jPanel3.setBackground(new java.awt.Color(0, 82, 119));

        tab3jTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Device Name", "Catagory", "Quantity", "Maintenance due in (months)"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class, java.lang.Integer.class
            };
            boolean[] canEdit = new boolean [] {
                false, false, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        tab3jTable.getTableHeader().setReorderingAllowed(false);
        tab3jTable.addFocusListener(new java.awt.event.FocusAdapter() {
            public void focusGained(java.awt.event.FocusEvent evt) {
                tab3jTableFocusGained(evt);
            }
        });
        jScrollPane5.setViewportView(tab3jTable);

        jLabel2.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel2.setForeground(new java.awt.Color(255, 255, 255));
        jLabel2.setText("Medical Devices in Inventory");

        jLabel3.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel3.setForeground(new java.awt.Color(255, 255, 255));
        jLabel3.setText("Last serviced on");

        jLabel4.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel4.setForeground(new java.awt.Color(255, 255, 255));
        jLabel4.setText("Maintenance due on");

        jLabel1.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel1.setForeground(new java.awt.Color(255, 255, 255));
        jLabel1.setText("Device Failre Probability");

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addGap(19, 19, 19)
                        .addComponent(jScrollPane5, javax.swing.GroupLayout.PREFERRED_SIZE, 622, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addGap(27, 27, 27)
                        .addComponent(jLabel2))
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addGap(109, 109, 109)
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addComponent(jLabel1)
                                .addGap(18, 18, 18)
                                .addComponent(probjTextField))
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel3Layout.createSequentialGroup()
                                .addComponent(jLabel3)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(lastServicedOnjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 169, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addComponent(jLabel4)
                                .addGap(37, 37, 37)
                                .addComponent(maintenanceDueOnjTextField)))))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel3Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {jLabel3, jLabel4});

        jPanel3Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {lastServicedOnjTextField, maintenanceDueOnjTextField});

        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGap(14, 14, 14)
                .addComponent(jLabel2)
                .addGap(18, 18, 18)
                .addComponent(jScrollPane5, javax.swing.GroupLayout.PREFERRED_SIZE, 144, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(33, 33, 33)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(lastServicedOnjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel4)
                    .addComponent(maintenanceDueOnjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(probjTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(111, Short.MAX_VALUE))
        );

        jPanel3Layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {jLabel3, jLabel4});

        jPanel3Layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {lastServicedOnjTextField, maintenanceDueOnjTextField});

        jTabbedPane1.addTab("View Failure Forecasting", jPanel3);

        jPanel4.setBackground(new java.awt.Color(0, 82, 119));

        jLabel5.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel5.setForeground(new java.awt.Color(255, 255, 255));
        jLabel5.setText("Medical Devices in Inventory");

        tab4jTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Device Name", "Catagory", "Quantity", "Maintenance due in (months)"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class, java.lang.Integer.class
            };
            boolean[] canEdit = new boolean [] {
                false, false, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        tab4jTable.getTableHeader().setReorderingAllowed(false);
        tab4jTable.addFocusListener(new java.awt.event.FocusAdapter() {
            public void focusGained(java.awt.event.FocusEvent evt) {
                tab4jTableFocusGained(evt);
            }
        });
        jScrollPane6.setViewportView(tab4jTable);

        jButton1.setText("View Usage");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel4Layout = new javax.swing.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jButton1)
                    .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jScrollPane6, javax.swing.GroupLayout.PREFERRED_SIZE, 622, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGroup(jPanel4Layout.createSequentialGroup()
                            .addGap(8, 8, 8)
                            .addComponent(jLabel5))))
                .addContainerGap(19, Short.MAX_VALUE))
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addGap(19, 19, 19)
                .addComponent(jLabel5)
                .addGap(18, 18, 18)
                .addComponent(jScrollPane6, javax.swing.GroupLayout.PREFERRED_SIZE, 144, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(30, 30, 30)
                .addComponent(jButton1)
                .addContainerGap(182, Short.MAX_VALUE))
        );

        jTabbedPane1.addTab("View Usage Trends", jPanel4);

        jPanel5.setBackground(new java.awt.Color(0, 82, 119));

        javax.swing.GroupLayout jPanel5Layout = new javax.swing.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 651, Short.MAX_VALUE)
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 431, Short.MAX_VALUE)
        );

        jTabbedPane1.addTab("View Safety Guidelines", jPanel5);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(26, 26, 26)
                        .addComponent(jTabbedPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(237, 237, 237)
                        .addComponent(titlejLabel)))
                .addContainerGap(36, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(41, 41, 41)
                .addComponent(titlejLabel)
                .addGap(18, 18, 18)
                .addComponent(jTabbedPane1)
                .addContainerGap())
        );
    }// </editor-fold>//GEN-END:initComponents

    private void deliveryJTableFocusGained(java.awt.event.FocusEvent evt) {//GEN-FIRST:event_deliveryJTableFocusGained
        // TODO add your handling code here:
        populateDetailsOrderTable();
    }//GEN-LAST:event_deliveryJTableFocusGained

    private void processOrderjButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_processOrderjButtonActionPerformed
        // TODO add your handling code here:

        //get the selected order
        int selectedRow = deliveryJTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select an order from the list");
            return;
        }

        Order o = (Order) deliveryJTable.getValueAt(selectedRow, 0);
        if (null == o) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected order's information");
            return;
        }

        //set the order in inventory deliveries list
        for (OrderItem oi : o.getOrderItemList()) {
            inventoryEnterprise.getWarehouse().getMdeciDeviceCatalog().getMedicalDeviceList().add(oi.getMedicalDevice());
        }
        populateWarehouseDevicesTable();
    }//GEN-LAST:event_processOrderjButtonActionPerformed

    private void tab3jTableFocusGained(java.awt.event.FocusEvent evt) {//GEN-FIRST:event_tab3jTableFocusGained
        // TODO add your handling code here:
        //get the selected device        
        int selectedRow = tab3jTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a medical device from the list");
            return;
        }

        MedicalDevice o = (MedicalDevice) tab3jTable.getValueAt(selectedRow, 0);
        if (null == o) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected medical device's information");
            return;
        }

        populateGUI(o);
    }//GEN-LAST:event_tab3jTableFocusGained

    private void tab4jTableFocusGained(java.awt.event.FocusEvent evt) {//GEN-FIRST:event_tab4jTableFocusGained
        // TODO add your handling code here:
    }//GEN-LAST:event_tab4jTableFocusGained

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        // TODO add your handling code here:
        //check if the supplier has products in the product list
        if (inventoryEnterprise.getWarehouse().getMdeciDeviceCatalog().getMedicalDeviceList().size() < 0) {
            JOptionPane.showMessageDialog(this, "No devices found in the inventory.", "Warning", JOptionPane.WARNING_MESSAGE);
            return;
        }

        MedicalDevice[] product;
        product = new MedicalDevice[100];

        int numberOfProducts = inventoryEnterprise.getWarehouse().getMdeciDeviceCatalog().getMedicalDeviceList().size();
        DefaultCategoryDataset dataSetProduct = new DefaultCategoryDataset();

        for (int i = 0; i < numberOfProducts; i++) {
            product[i] = inventoryEnterprise.getWarehouse().getMdeciDeviceCatalog().getMedicalDeviceList().get(i);
            int soldQuantity = 0;
            soldQuantity = product[i].getNumUses();
            String prodName = product[i].getDeviceName();
            dataSetProduct.setValue(soldQuantity, "Medical Device", prodName);
        }

        JFreeChart chartProduct = ChartFactory.createBarChart("Device Usage Report", "Device", "Number of times usdr", dataSetProduct, PlotOrientation.VERTICAL, false, true, false);

        CategoryPlot p1 = chartProduct.getCategoryPlot();
        p1.setRangeGridlinePaint(Color.black);
        ChartFrame frame1 = new ChartFrame("Device Usage Report", chartProduct);
        frame1.setVisible(true);
        frame1.setSize(400, 400);
        Point pt1 = new Point(0, 0);
        frame1.setLocation(pt1);
    }//GEN-LAST:event_jButton1ActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JTable deliveryJTable;
    private javax.swing.JTable detailsorderJTable;
    private javax.swing.JButton jButton1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JScrollPane jScrollPane4;
    private javax.swing.JScrollPane jScrollPane5;
    private javax.swing.JScrollPane jScrollPane6;
    private javax.swing.JTabbedPane jTabbedPane1;
    private javax.swing.JTextField lastServicedOnjTextField;
    private javax.swing.JTextField maintenanceDueOnjTextField;
    private javax.swing.JTextField probjTextField;
    private javax.swing.JButton processOrderjButton;
    private javax.swing.JTable tab3jTable;
    private javax.swing.JTable tab4jTable;
    private javax.swing.JLabel titlejLabel;
    private javax.swing.JTable warehouseDevicesjTable;
    // End of variables declaration//GEN-END:variables

    private void populateGUI(MedicalDevice md) {

        lastServicedOnjTextField.setText(md.getMaintenanceSchedule().getLastMaintenanceDate().toString());
        maintenanceDueOnjTextField.setText(md.getMaintenanceSchedule().getNextMaintenanceDate().toString());

        double prob = calculateProbability(md);

        probjTextField.setText(String.valueOf(prob));
    }

    private double calculateProbability(MedicalDevice md) {

        double result = 0;

        //get current date
        Date currDate = new Date();

        //get last maintenance date
        Date lastMain = md.getMaintenanceSchedule().getLastMaintenanceDate();

        //get next maintenance date
        Date nextMain = md.getMaintenanceSchedule().getNextMaintenanceDate();

        //daysLeftToService = lastMain.
        int daysLeftToService = daysBetween(currDate, nextMain);

        int timeForMain = daysBetween(lastMain, nextMain);

        //calculate days left for maintenance
        //calculate prob
        result = (daysLeftToService / timeForMain) * 100;

        return result;
    }

    private int daysBetween(Date d1, Date d2) {
        return (int) ((d2.getTime() - d1.getTime()) / (1000 * 60 * 60 * 24));
    }
}
