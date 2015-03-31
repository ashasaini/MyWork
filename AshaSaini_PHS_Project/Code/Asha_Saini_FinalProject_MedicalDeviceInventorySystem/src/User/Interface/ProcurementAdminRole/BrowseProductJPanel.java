/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package User.Interface.ProcurementAdminRole;

import Bsuiness.EcoSystem;
import Business.Enterprise.SupplierEnterprise;
import Business.MedicalDevice.MedicalDevice;
import Business.Order.Order;
import Business.Order.OrderItem;
import Business.Organization.Organization;
import Business.Organization.SupplierAdminOrganization;
import java.awt.CardLayout;
import java.awt.Color;
import java.util.ArrayList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author Asha
 */
public class BrowseProductJPanel extends javax.swing.JPanel {

    /**
     * Creates new form BrowseProductJPanel
     */
    private JPanel userProcessContainer;
    private EcoSystem system;
    ArrayList<SupplierEnterprise> supplierEnterpriseList;
    private boolean isCheckedOut = false;
    private Order order;   
    
    public BrowseProductJPanel(JPanel userProcessContainer, EcoSystem system, ArrayList<SupplierEnterprise> supplierEnterpriseList) {
        initComponents();
        this.userProcessContainer = userProcessContainer;
        this.supplierEnterpriseList = supplierEnterpriseList;
        this.system = system;
        Color c = new Color(0, 85, 119);
        this.setBackground(c);

        //create a new order
        if (!isCheckedOut) {
            order = new Order();
        }
        
        populateSupplierComboBox();
        
        SupplierEnterprise se = (SupplierEnterprise) supplierjComboBox.getItemAt(0);
        if (null != se) {
            populateDeviceTable(se);
        }
    }
    
    private void populateSupplierComboBox() {
        supplierjComboBox.removeAllItems();
        for (SupplierEnterprise se : supplierEnterpriseList) {
            supplierjComboBox.addItem(se);
        }
    }
    
    private void populateDeviceTable(SupplierEnterprise se) {
        DefaultTableModel model = (DefaultTableModel) devicetJTable.getModel();
        
        model.setRowCount(0);
        
        for (Organization organization : se.getOrganizationDirectory().getOrganizationList()) {
            if (organization.getType().getValue().equalsIgnoreCase(Organization.Type.SupplierAdminOrganization.getValue())) {
                SupplierAdminOrganization sao = (SupplierAdminOrganization) organization;
                for (MedicalDevice md : sao.getMedicalDeviceCatalog().getMedicalDeviceList()) {
                    Object[] row = new Object[4];
                    row[0] = md;
                    row[1] = md.getDeviceId();
                    row[2] = md.getPrice();
                    row[3] = md.getAvailableQuantity();
                    model.addRow(row);
                }
            }
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

        jLabel5 = new javax.swing.JLabel();
        supplierjComboBox = new javax.swing.JComboBox();
        txtSearchKeyWord = new javax.swing.JTextField();
        searchProductJButton = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        devicetJTable = new javax.swing.JTable();
        viewProductDetailJButton = new javax.swing.JButton();
        jLabel2 = new javax.swing.JLabel();
        quantityJSpinner = new javax.swing.JSpinner();
        addToCartJButton = new javax.swing.JButton();
        jScrollPane2 = new javax.swing.JScrollPane();
        orderJTable = new javax.swing.JTable();
        viewItemJButton = new javax.swing.JButton();
        txtNewQuantity = new javax.swing.JTextField();
        modifyQuantityJButton = new javax.swing.JButton();
        refreshCartJButton = new javax.swing.JButton();
        removeItemJButton = new javax.swing.JButton();
        checkoutJButton = new javax.swing.JButton();
        backJButton = new javax.swing.JButton();
        titlejLabel = new javax.swing.JLabel();
        titlejLabel1 = new javax.swing.JLabel();
        titlejLabel2 = new javax.swing.JLabel();
        totalText = new javax.swing.JTextField();
        jLabel3 = new javax.swing.JLabel();

        setBackground(new java.awt.Color(0, 85, 119));

        jLabel5.setFont(new java.awt.Font("Tahoma", 1, 14)); // NOI18N
        jLabel5.setForeground(new java.awt.Color(255, 255, 255));
        jLabel5.setText("Supplier");

        supplierjComboBox.setBackground(new java.awt.Color(120, 120, 120));
        supplierjComboBox.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        supplierjComboBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                supplierjComboBoxActionPerformed(evt);
            }
        });

        txtSearchKeyWord.setBackground(new java.awt.Color(120, 120, 120));
        txtSearchKeyWord.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        txtSearchKeyWord.setForeground(new java.awt.Color(255, 255, 255));

        searchProductJButton.setBackground(new java.awt.Color(30, 30, 30));
        searchProductJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        searchProductJButton.setForeground(new java.awt.Color(255, 255, 255));
        searchProductJButton.setText("Search Product");
        searchProductJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                searchProductJButtonActionPerformed(evt);
            }
        });

        devicetJTable.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        devicetJTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Device Name", "Device Id", "Price", "Availability"
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
        devicetJTable.getTableHeader().setReorderingAllowed(false);
        jScrollPane1.setViewportView(devicetJTable);

        viewProductDetailJButton.setBackground(new java.awt.Color(30, 30, 30));
        viewProductDetailJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        viewProductDetailJButton.setForeground(new java.awt.Color(255, 255, 255));
        viewProductDetailJButton.setText("View Product Detail");
        viewProductDetailJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                viewProductDetailJButtonActionPerformed(evt);
            }
        });

        jLabel2.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel2.setForeground(new java.awt.Color(255, 255, 255));
        jLabel2.setText("Quantity");

        quantityJSpinner.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N

        addToCartJButton.setBackground(new java.awt.Color(30, 30, 30));
        addToCartJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        addToCartJButton.setForeground(new java.awt.Color(255, 255, 255));
        addToCartJButton.setText("Add To Cart");
        addToCartJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                addToCartJButtonActionPerformed(evt);
            }
        });

        orderJTable.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        orderJTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Device Name", "Price", "Quantity", "Sub Total Amount"
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
        orderJTable.getTableHeader().setReorderingAllowed(false);
        jScrollPane2.setViewportView(orderJTable);

        viewItemJButton.setBackground(new java.awt.Color(30, 30, 30));
        viewItemJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        viewItemJButton.setForeground(new java.awt.Color(255, 255, 255));
        viewItemJButton.setText("View Item");
        viewItemJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                viewItemJButtonActionPerformed(evt);
            }
        });

        txtNewQuantity.setBackground(new java.awt.Color(120, 120, 120));
        txtNewQuantity.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        txtNewQuantity.setForeground(new java.awt.Color(255, 255, 255));
        txtNewQuantity.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                txtNewQuantityActionPerformed(evt);
            }
        });

        modifyQuantityJButton.setBackground(new java.awt.Color(30, 30, 30));
        modifyQuantityJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        modifyQuantityJButton.setForeground(new java.awt.Color(255, 255, 255));
        modifyQuantityJButton.setText("Modify");
        modifyQuantityJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                modifyQuantityJButtonActionPerformed(evt);
            }
        });

        refreshCartJButton.setBackground(new java.awt.Color(30, 30, 30));
        refreshCartJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        refreshCartJButton.setForeground(new java.awt.Color(255, 255, 255));
        refreshCartJButton.setText("Refresh");
        refreshCartJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                refreshCartJButtonActionPerformed(evt);
            }
        });

        removeItemJButton.setBackground(new java.awt.Color(30, 30, 30));
        removeItemJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        removeItemJButton.setForeground(new java.awt.Color(255, 255, 255));
        removeItemJButton.setText("Remove");
        removeItemJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                removeItemJButtonActionPerformed(evt);
            }
        });

        checkoutJButton.setBackground(new java.awt.Color(30, 30, 30));
        checkoutJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        checkoutJButton.setForeground(new java.awt.Color(255, 255, 255));
        checkoutJButton.setText("Checkout");
        checkoutJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                checkoutJButtonActionPerformed(evt);
            }
        });

        backJButton.setBackground(new java.awt.Color(30, 30, 30));
        backJButton.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        backJButton.setForeground(new java.awt.Color(255, 255, 255));
        backJButton.setText("<<Back");
        backJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                backJButtonActionPerformed(evt);
            }
        });

        titlejLabel.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        titlejLabel.setForeground(new java.awt.Color(255, 255, 255));
        titlejLabel.setText("Procurement Admin - Browse Product");

        titlejLabel1.setFont(new java.awt.Font("Tahoma", 1, 14)); // NOI18N
        titlejLabel1.setForeground(new java.awt.Color(255, 255, 255));
        titlejLabel1.setText("Device List");

        titlejLabel2.setFont(new java.awt.Font("Tahoma", 1, 14)); // NOI18N
        titlejLabel2.setForeground(new java.awt.Color(255, 255, 255));
        titlejLabel2.setText("Items in your cart");

        totalText.setBackground(new java.awt.Color(120, 120, 120));
        totalText.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        totalText.setForeground(new java.awt.Color(255, 255, 255));

        jLabel3.setFont(new java.awt.Font("Tahoma", 1, 12)); // NOI18N
        jLabel3.setForeground(new java.awt.Color(255, 255, 255));
        jLabel3.setText("Total");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(51, 51, 51)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(titlejLabel1)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.Alignment.TRAILING)
                        .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                .addComponent(titlejLabel)
                                .addGroup(layout.createSequentialGroup()
                                    .addComponent(jLabel5)
                                    .addGap(18, 18, 18)
                                    .addComponent(supplierjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, 158, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 174, Short.MAX_VALUE)
                                    .addComponent(txtSearchKeyWord, javax.swing.GroupLayout.PREFERRED_SIZE, 100, javax.swing.GroupLayout.PREFERRED_SIZE)))
                            .addGap(18, 18, 18)
                            .addComponent(searchProductJButton))
                        .addComponent(jScrollPane1)
                        .addGroup(layout.createSequentialGroup()
                            .addComponent(viewProductDetailJButton)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(refreshCartJButton)
                            .addGap(18, 18, 18)
                            .addComponent(jLabel2)
                            .addGap(18, 18, 18)
                            .addComponent(quantityJSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGap(18, 18, 18)
                            .addComponent(addToCartJButton))
                        .addGroup(layout.createSequentialGroup()
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addGroup(layout.createSequentialGroup()
                                    .addComponent(viewItemJButton)
                                    .addGap(18, 18, 18)
                                    .addComponent(txtNewQuantity, javax.swing.GroupLayout.PREFERRED_SIZE, 83, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addGap(18, 18, 18)
                                    .addComponent(modifyQuantityJButton)
                                    .addGap(18, 18, 18)
                                    .addComponent(removeItemJButton)
                                    .addGap(58, 58, 58)
                                    .addComponent(jLabel3))
                                .addComponent(backJButton))
                            .addGap(18, 18, 18)
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(checkoutJButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(totalText))))
                    .addComponent(titlejLabel2))
                .addContainerGap(45, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap(28, Short.MAX_VALUE)
                .addComponent(titlejLabel)
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(searchProductJButton, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(txtSearchKeyWord, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(supplierjComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel5))
                .addGap(18, 18, 18)
                .addComponent(titlejLabel1)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 113, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(viewProductDetailJButton)
                    .addComponent(addToCartJButton)
                    .addComponent(quantityJSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel2)
                    .addComponent(refreshCartJButton))
                .addGap(18, 18, 18)
                .addComponent(titlejLabel2)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 113, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(removeItemJButton)
                    .addComponent(modifyQuantityJButton)
                    .addComponent(txtNewQuantity, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(viewItemJButton)
                    .addComponent(totalText, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel3))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(backJButton)
                    .addComponent(checkoutJButton))
                .addContainerGap())
        );
    }// </editor-fold>//GEN-END:initComponents

    private void supplierjComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_supplierjComboBoxActionPerformed
        // TODO add your handling code here:
        SupplierEnterprise se = (SupplierEnterprise) supplierjComboBox.getSelectedItem();
        if (null != se) {
            populateDeviceTable(se);
        }
    }//GEN-LAST:event_supplierjComboBoxActionPerformed

    private void searchProductJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_searchProductJButtonActionPerformed
        // TODO add your handling code here:
        String keyWord = txtSearchKeyWord.getText();
        if ((keyWord.equals("")) || (null == keyWord)) {
            JOptionPane.showMessageDialog(this, "Please enter a valid device name to search");
            return;
        }
        searchProduct(keyWord);
    }//GEN-LAST:event_searchProductJButtonActionPerformed
    
    private void searchProduct(String keyWord) {
        int rowCount = devicetJTable.getRowCount();
        DefaultTableModel dtm = (DefaultTableModel) devicetJTable.getModel();

        //delete all the previous rows
        for (int i = rowCount - 1; i >= 0; i--) {
            dtm.removeRow(i);
        }

        //serach products from all the suppliers
        for (SupplierEnterprise se : supplierEnterpriseList) {
            //for all org that belong to current supplier enterprise
            for (Organization org : se.getOrganizationDirectory().getOrganizationList()) {
                //only SupplierAdminOrganization has medical device catalog
                if (org.getType().getValue().equalsIgnoreCase(Organization.Type.SupplierAdminOrganization.getValue())) {
                    SupplierAdminOrganization sao = (SupplierAdminOrganization) org;
                    for (MedicalDevice md : sao.getMedicalDeviceCatalog().getMedicalDeviceList()) {
                        if (md.getDeviceName().contains(keyWord)) {
                            Object[] row = new Object[4];
                            row[0] = md;
                            row[1] = md.getDeviceId();
                            row[2] = md.getPrice();
                            row[3] = md.getAvailableQuantity();
                            dtm.addRow(row);
                        }
                    }//end of device for loop
                }//end of if
            }//end od org for loop
        } //end of supplier enterprise for loop       
    }

    private void viewProductDetailJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_viewProductDetailJButtonActionPerformed
        // TODO add your handling code here:
        //check if a product is selected fromt he list
        int selectedRow = devicetJTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a device from the list");
            return;
        }

        //get the selected product
        MedicalDevice md = (MedicalDevice) devicetJTable.getValueAt(selectedRow, 0);
        ViewDeviceDetailJPanel panel = new ViewDeviceDetailJPanel(userProcessContainer, md);
        userProcessContainer.add("ViewDeviceDetailJPanel", panel);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.next(userProcessContainer);
    }//GEN-LAST:event_viewProductDetailJButtonActionPerformed

    private void addToCartJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_addToCartJButtonActionPerformed
        // TODO add your handling code here:

        //check if a product is selected fromt the list
        int selectedRow = devicetJTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select a device from the list");
            return;
        }

        //get the selected product
        MedicalDevice md = (MedicalDevice) devicetJTable.getValueAt(selectedRow, 0);
        if (null == md) {
            JOptionPane.showMessageDialog(this, "Failed to retrieve selected device's information");
            return;
        }

        //get the quantity from the spinner
        int quantity = (Integer) quantityJSpinner.getValue();
        
        if (quantity <= 0) {
            JOptionPane.showMessageDialog(this, "Please select a valid quantity");
            return;
        }
        
        if (quantity > md.getAvailableQuantity()) {
            JOptionPane.showMessageDialog(this, "Device's specified quantity not available");
            return;
        }

        //check if selected product has already been added to the cart
        boolean flag = true;
        ArrayList<OrderItem> orderList = order.getOrderItemList();
        for (OrderItem oi : orderList) {
            if (oi.getMedicalDevice().equals(md)) {
                int oldQuantity = oi.getQuantity();
                int newQuantity = oldQuantity + quantity;
                oi.setQuantity(newQuantity);
                flag = false;
                md.setAvailableQuantity(md.getAvailableQuantity() - quantity);                
            }
        }
        
        if (flag) {
            md.setAvailableQuantity(md.getAvailableQuantity() - quantity);            
            OrderItem orderItem = order.addOrderItem(md, quantity);
        }
        isCheckedOut = false;
        
        String msg = Integer.toString(quantity) + " " + md.getDeviceName() + " added to the shopping cart";
        JOptionPane.showMessageDialog(this, msg);

        //refresh the table
        SupplierEnterprise se = (SupplierEnterprise) supplierjComboBox.getItemAt(0);
        if (null != se) {
            populateDeviceTable(se);
        }
        displayOrder();
        calculateAndDisplayTotal();
    }//GEN-LAST:event_addToCartJButtonActionPerformed
    
    private void displayOrder() {
        int rowCount = orderJTable.getRowCount();
        DefaultTableModel dtm = (DefaultTableModel) orderJTable.getModel();

        //delete all the previous rows
        for (int i = rowCount - 1; i >= 0; i--) {
            dtm.removeRow(i);
        }
        
        for (OrderItem oi : order.getOrderItemList()) {
            Object[] row = new Object[4];
            row[0] = oi;
            row[1] = oi.getMedicalDevice().getPrice();
            row[2] = oi.getQuantity();
            row[3] = (oi.getMedicalDevice().getPrice() * oi.getQuantity());
            dtm.addRow(row);
        }
    }
    
    private void calculateAndDisplayTotal() {
                
        double total = 0;
        for (OrderItem oi : order.getOrderItemList()) {
            total += oi.getMedicalDevice().getPrice() * oi.getQuantity();
        }
        totalText.setText(String.valueOf(total));
    }    
    
    private void viewItemJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_viewItemJButtonActionPerformed
        // TODO add your handling code here:
        //get the selected order
        //check if an item is selected in the order table
        int selectedRow = orderJTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select an item from the shopping cart");
            return;
        }

        //get the order item
        OrderItem orderItem = (OrderItem) orderJTable.getValueAt(selectedRow, 0);
        MedicalDevice md = orderItem.getMedicalDevice();

        //switch to view jpanl
        ViewDeviceDetailJPanel voijp = new ViewDeviceDetailJPanel(userProcessContainer, md);
        userProcessContainer.add("ViewDeviceDetailJPanel", voijp);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.next(userProcessContainer);
    }//GEN-LAST:event_viewItemJButtonActionPerformed

    private void txtNewQuantityActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_txtNewQuantityActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_txtNewQuantityActionPerformed

    private void modifyQuantityJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_modifyQuantityJButtonActionPerformed
        // TODO add your handling code here:
        try {
            //check if an item is selected in the order table
            int selectedRow = orderJTable.getSelectedRow();
            if (selectedRow < 0) {
                JOptionPane.showMessageDialog(this, "Please select an item from the shopping cart");
                return;
            }

            //get the order item
            OrderItem orderItem = (OrderItem) orderJTable.getValueAt(selectedRow, 0);
            int oldQuantity = orderItem.getQuantity();
            int oldAvail = orderItem.getMedicalDevice().getAvailableQuantity();
            
            String newQuantityStr = txtNewQuantity.getText();
            if (null == newQuantityStr || newQuantityStr.equals("")) {
                JOptionPane.showMessageDialog(this, "Please enter a valid quanity to modify");
                return;
            }
            
            int newQuantity = 0;
            try {
                newQuantity = Integer.parseInt(newQuantityStr);
            } catch (Exception e) {
                JOptionPane.showMessageDialog(this, "Please enter a valid quanity to modify");
                return;
            }
            
            if (newQuantity < 0) {
                JOptionPane.showMessageDialog(this, "Please enter a valid quantity");
                return;
            }
            
            if (newQuantity > oldQuantity + oldAvail) {
                JOptionPane.showMessageDialog(this, "Product's specified quantity not available");
                return;
            }

            //modify the item quantity
            orderItem.setQuantity(newQuantity);

            //infomr the user
            String msg = "The quantity of " + orderItem.getMedicalDevice().getDeviceName() + "has been changed from " + Integer.toString(oldQuantity) + " to " + Integer.toString(newQuantity) + " added to the shopping cart";
            JOptionPane.showMessageDialog(this, msg);

            //set the new availability
            orderItem.getMedicalDevice().setAvailableQuantity(oldQuantity + oldAvail - newQuantity);

            //refresh the tables
            SupplierEnterprise se = (SupplierEnterprise) supplierjComboBox.getItemAt(0);
            if (null != se) {
                populateDeviceTable(se);
            }
            displayOrder();
            
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "An error occurred");
        }
    }//GEN-LAST:event_modifyQuantityJButtonActionPerformed

    private void refreshCartJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_refreshCartJButtonActionPerformed
        // TODO add your handling code here:
        SupplierEnterprise se = (SupplierEnterprise) supplierjComboBox.getItemAt(0);
        if (null != se) {
            populateDeviceTable(se);
        }
        displayOrder();
    }//GEN-LAST:event_refreshCartJButtonActionPerformed

    private void removeItemJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_removeItemJButtonActionPerformed
        // TODO add your handling code here:
        int selectedRow = orderJTable.getSelectedRow();
        if (selectedRow < 0) {
            JOptionPane.showMessageDialog(this, "Please select an order from the shopping cart to remove");
            return;
        }
        
        OrderItem oi = (OrderItem) orderJTable.getValueAt(selectedRow, 0);
        order.removeOrder(oi);
        
        SupplierEnterprise se = (SupplierEnterprise) supplierjComboBox.getItemAt(0);
        if (null != se) {
            populateDeviceTable(se);
        }
        displayOrder();
        calculateAndDisplayTotal();
    }//GEN-LAST:event_removeItemJButtonActionPerformed

    private void checkoutJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_checkoutJButtonActionPerformed
        // TODO add your handling code here:
        if (!order.getOrderItemList().isEmpty()) {            
            Order addOrderItem = system.getProcurementEnterprise().getMasterOrderCatalog().addOrder(order);            
            JOptionPane.showMessageDialog(this, "Your order has been successfully placed!");
        } else {
            JOptionPane.showMessageDialog(this, "No itmes in the cart!");
        }

        //create new order
        isCheckedOut = true;

        //set the sold quantity for the selected product of selected supplier
        //get the supplier
        SupplierEnterprise se = (SupplierEnterprise) supplierjComboBox.getSelectedItem();

        //match the selected supplier's product with the product being sold in the cart
        int OrderProductCount = order.getOrderItemList().size();
        for (int i = 0; i < OrderProductCount; i++) {
            //get the product being sold
            OrderItem orderItem = (OrderItem) orderJTable.getValueAt(i, 0);
            MedicalDevice md = orderItem.getMedicalDevice();

            //get the supplier admin org
            SupplierAdminOrganization sao = null;
            for (Organization o : se.getOrganizationDirectory().getOrganizationList()) {
                if (o.getType().getValue().equals(Organization.Type.SupplierAdminOrganization.getValue())) {
                    sao = (SupplierAdminOrganization) o;
                    break;
                }
            }            
            
            int supplierProductCount = sao.getMedicalDeviceCatalog().getMedicalDeviceList().size();
            for (int j = 0; j < supplierProductCount; j++) {
                if (md.equals(sao.getMedicalDeviceCatalog().getMedicalDeviceList().get(j))) {
                    int soldQuantity = orderItem.getQuantity();

                    //set to the product
                    md.setSoldQuantity(soldQuantity);
                }
            }
        }

        //refresh the tables
        SupplierEnterprise se2 = (SupplierEnterprise) supplierjComboBox.getItemAt(0);
        if (null != se2) {
            populateDeviceTable(se2);
        }        
        displayOrder();

        //get the selected supplierEnterprise
        se2.getOrderQueue().addOrder(order);

        //add the order to the selected supplierEnterprise's shipping org's order queue
        order = new Order();
    }//GEN-LAST:event_checkoutJButtonActionPerformed

    private void backJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_backJButtonActionPerformed
        // TODO add your handling code here:
        //if user dint check out
        if (!isCheckedOut) {
            for (OrderItem oi : order.getOrderItemList()) {
                MedicalDevice md = oi.getMedicalDevice();
                int oldAvail = md.getAvailableQuantity();
                int orderedQuantity = oi.getQuantity();
                int newAvail = oldAvail + orderedQuantity;

                //set to the product
                md.setAvailableQuantity(newAvail);
            }
            JOptionPane.showMessageDialog(this, "Your order has not been checked out, so your order will be canceled!");
        } else {
            JOptionPane.showMessageDialog(this, "Your order has already been checked out. Thank you!");
        }
        
        userProcessContainer.remove(this);
        CardLayout layout = (CardLayout) userProcessContainer.getLayout();
        layout.previous(userProcessContainer);
    }//GEN-LAST:event_backJButtonActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton addToCartJButton;
    private javax.swing.JButton backJButton;
    private javax.swing.JButton checkoutJButton;
    private javax.swing.JTable devicetJTable;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JButton modifyQuantityJButton;
    private javax.swing.JTable orderJTable;
    private javax.swing.JSpinner quantityJSpinner;
    private javax.swing.JButton refreshCartJButton;
    private javax.swing.JButton removeItemJButton;
    private javax.swing.JButton searchProductJButton;
    private javax.swing.JComboBox supplierjComboBox;
    private javax.swing.JLabel titlejLabel;
    private javax.swing.JLabel titlejLabel1;
    private javax.swing.JLabel titlejLabel2;
    private javax.swing.JTextField totalText;
    private javax.swing.JTextField txtNewQuantity;
    private javax.swing.JTextField txtSearchKeyWord;
    private javax.swing.JButton viewItemJButton;
    private javax.swing.JButton viewProductDetailJButton;
    // End of variables declaration//GEN-END:variables
}
