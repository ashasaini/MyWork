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
public class ProcurementEnterprise extends  Enterprise{

    MasterOrderCatalog masterOrderCatalog;
            
    public ProcurementEnterprise(String f_name) {
        super(f_name, Enterprise.EnterpriseType.Procurement);
        masterOrderCatalog = new MasterOrderCatalog();
    }

    public MasterOrderCatalog getMasterOrderCatalog() {
        return masterOrderCatalog;
    }

    public void setMasterOrderCatalog(MasterOrderCatalog masterOrderCatalog) {
        this.masterOrderCatalog = masterOrderCatalog;
    }
    
    @Override
    public ArrayList<Role> getSupportedRole() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
