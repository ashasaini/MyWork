/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Enterprise;

import Business.Order.MasterOrderCatalog;
import Business.Role.Role;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class SupplierEnterprise extends  Enterprise{
    MasterOrderCatalog orderQueue;
    
    public SupplierEnterprise(String f_name) {
        super(f_name, Enterprise.EnterpriseType.Supplier);
        orderQueue = new MasterOrderCatalog();
    }

    public MasterOrderCatalog getOrderQueue() {
        return orderQueue;
    }

    public void setOrderQueue(MasterOrderCatalog orderQueue) {
        this.orderQueue = orderQueue;
    }    

    @Override
    public ArrayList<Role> getSupportedRole() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
} 
