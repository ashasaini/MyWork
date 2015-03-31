/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Organization;

import Business.Order.MasterOrderCatalog;
import Business.Role.Role;
import Business.Role.SupplierShippingRole;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class SupplierShippingOrganization extends Organization{
   
    public SupplierShippingOrganization(String f_organizationName, Organization.Type f_type) {
        super(f_organizationName,f_type);
        super.setType(type);
    }

    @Override
    public ArrayList<Role> getSupportedRole() {
        ArrayList<Role> roles = new ArrayList<Role>();
        roles.add(new SupplierShippingRole());
        return roles;
    }    
}
