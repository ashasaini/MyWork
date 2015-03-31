/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Role;

import Bsuiness.EcoSystem;
import Business.Enterprise.Enterprise;
import Business.Enterprise.InventoryEnterprise;
import Business.Enterprise.ProcurementEnterprise;
import Business.Enterprise.SupplierEnterprise;
import Business.Organization.InventoryAdminOrganization;
import Business.Organization.Organization;
import Business.Organization.ProcurementAdminOrganization;
import Business.Organization.SupplierAdminOrganization;
import Business.Organization.SupplierShippingOrganization;
import Business.UserAccount.UserAccount;
import User.Interface.HospitalStaffWorkArea.HospitalStaffWorkAreaJPanel;
import javax.swing.JPanel;

/**
 *
 * @author Asha
 */
public class MaintenanceAdminRole extends Role{

    @Override
    public JPanel createWorkArea(JPanel userProcessContainer,
            UserAccount account,
            Organization organization,
            Enterprise enterprise, 
            EcoSystem business,
            SupplierEnterprise supplierEnterprise,
            SupplierAdminOrganization supplierAdminOrganization,
            SupplierShippingOrganization supplierShippingOrganization,
            ProcurementEnterprise procurementEnterprise,
            ProcurementAdminOrganization procurementAdminOrganization,
            InventoryEnterprise inventoryEnterprise,
            InventoryAdminOrganization inventoryAdminOrganization) {
        //return new Main();
        return null;
    }
    
}
