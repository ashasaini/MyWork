/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Enterprise;

import Business.Order.MasterOrderCatalog;
import Business.Role.Role;
import Business.Warehouse.Warehouse;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class InventoryEnterprise extends  Enterprise{
    MasterOrderCatalog orderDelivery;
    Warehouse warehouse;
    

    public InventoryEnterprise(String f_name) {
        super(f_name, Enterprise.EnterpriseType.Inventory);
        orderDelivery = new MasterOrderCatalog();
        warehouse = new Warehouse();
    }

    public MasterOrderCatalog getOrderDelivery() {
        return orderDelivery;
    }

    public void setOrderDelivery(MasterOrderCatalog orderDelivery) {
        this.orderDelivery = orderDelivery;
    }

    public Warehouse getWarehouse() {
        return warehouse;
    }

    public void setWarehouse(Warehouse warehouse) {
        this.warehouse = warehouse;
    }
    
    @Override
    public ArrayList<Role> getSupportedRole() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
