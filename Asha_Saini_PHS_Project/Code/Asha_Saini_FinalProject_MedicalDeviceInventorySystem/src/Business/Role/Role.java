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
import javax.swing.JPanel;

/**
 *
 * @author Asha
 */
public abstract class Role {
    public enum RoleType{
        SystemAdminRole("System Admin Role"),
        SupplierEnterpriseAdminRole("Supplier Enterprise Admin Role"),
        SupplierAdminRole("Supplier Admin Role"),
        SupplierShippingRole("Supplier Shipping Role"),
        ProcurementAdminRole("Procurement Admin Role"),
        HospitalAdminRole("HospitalAdminRole"),
        HospitalDoctorRole("HospitalDoctorRole"),
        HospitalStaffRole("HospitalStaffRole"),
        InventoryAdminRole("InventoryAdminRole"),
        MaintenanceAdminRole("MaintenanceAdminRole");       
        
        private String value;
        private RoleType(String value){
            this.value = value;
        }

        public String getValue() {
            return value;
        }

        @Override
        public String toString() {
            return value;
        }
    }
    
    public abstract JPanel createWorkArea(JPanel userProcessContainer, 
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
            InventoryAdminOrganization inventoryAdminOrganization);

    @Override
    public String toString() {
        //return this.getClass().getName();
        return this.getClass().getSimpleName();
    }
}
