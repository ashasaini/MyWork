/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Organization;

import Business.Role.HospitalStaffRole;
import Business.Role.ProcurementAdminRole;
import Business.Role.Role;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class HospitalStaffOrganization extends Organization {

    public HospitalStaffOrganization(String f_organizationName, Type f_type) {
        super(f_organizationName, f_type);
    }

    @Override
    public ArrayList<Role> getSupportedRole() {
        ArrayList<Role> roles = new ArrayList<Role>();
        roles.add(new HospitalStaffRole());
        return roles;        
    }
    
}
