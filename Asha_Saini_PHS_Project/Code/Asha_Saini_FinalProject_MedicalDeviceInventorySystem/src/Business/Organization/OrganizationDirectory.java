/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Organization;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class OrganizationDirectory {

    private ArrayList<Organization> organizationList;

    public OrganizationDirectory() {
        organizationList = new ArrayList<Organization>();
    }

    public ArrayList<Organization> getOrganizationList() {
        return organizationList;
    }

    public Organization createOrganization(Organization.Type f_type, String f_name) {
        Organization l_organization = null;
        if (f_type.getValue().equals(Organization.Type.SupplierAdminOrganization.getValue())) {
            l_organization = new SupplierAdminOrganization(f_name, f_type);
            organizationList.add(l_organization);
        } else if (f_type.getValue().equals(Organization.Type.SupplierShippingOrganization.getValue())) {
            l_organization = new SupplierShippingOrganization(f_name, f_type);
            organizationList.add(l_organization);
        } else if (f_type.getValue().equals(Organization.Type.ProcurementAdminOrganization.getValue())) {
            l_organization = new ProcurementAdminOrganization(f_name, f_type);
            organizationList.add(l_organization);
        } else if (f_type.getValue().equals(Organization.Type.InventoryAdminOrganization.getValue())) {
            l_organization = new InventoryAdminOrganization(f_name, f_type);
            organizationList.add(l_organization);
        } else if (f_type.getValue().equals(Organization.Type.MaintenanceAdminOrganization.getValue())) {
            l_organization = new MaintenanceAdminOrganization(f_name, f_type);
            organizationList.add(l_organization);
        } else if (f_type.getValue().equals(Organization.Type.HospitalAdminOrganization.getValue())) {
            l_organization = new HospitalAdminOrganization(f_name, f_type);
            organizationList.add(l_organization);
        } else if (f_type.getValue().equals(Organization.Type.HospitalDoctorOrganization.getValue())) {
            l_organization = new HospitalDoctorOrganization(f_name, f_type);
            organizationList.add(l_organization);
        } else if (f_type.getValue().equals(Organization.Type.HospitalStaffOrganization.getValue())) {
            l_organization = new HospitalStaffOrganization(f_name, f_type);
            organizationList.add(l_organization);
        }
        return l_organization;
    }
}
