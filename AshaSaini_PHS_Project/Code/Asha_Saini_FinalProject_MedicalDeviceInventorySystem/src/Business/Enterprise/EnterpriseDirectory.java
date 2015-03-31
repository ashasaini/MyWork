/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Enterprise;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class EnterpriseDirectory {
     private ArrayList<Enterprise> enterpriseList;

    public EnterpriseDirectory() {
        enterpriseList = new ArrayList<>();
    }

    public ArrayList<Enterprise> getEnterpriseList() {
        return enterpriseList;
    }
    
    public Enterprise createAndAddEnterprise(String name, Enterprise.EnterpriseType type){
        Enterprise enterprise = null;
        if (type == Enterprise.EnterpriseType.Hospital){
            enterprise = new HospitalEnterprise(name);
            enterpriseList.add(enterprise);
        }else if(type == Enterprise.EnterpriseType.Inventory){
            enterprise = new InventoryEnterprise(name);
            enterpriseList.add(enterprise);
        }else if(type == Enterprise.EnterpriseType.Maintenance){
            enterprise = new MaintenanceEnterprise(name);
            enterpriseList.add(enterprise);
        }else if(type == Enterprise.EnterpriseType.Procurement){
            enterprise = new ProcurementEnterprise(name);
            enterpriseList.add(enterprise);
        }else if(type == Enterprise.EnterpriseType.Supplier){
            enterprise = new SupplierEnterprise(name);
            enterpriseList.add(enterprise);
        }        
        return enterprise;
    }
    
    public Enterprise getEnterprise(Enterprise.EnterpriseType type){
        for(Enterprise e: enterpriseList){
            if(type.getValue().equalsIgnoreCase(e.getEnterpriseType().getValue())){
                return e;
            }
        }
        return null;
    }
}
