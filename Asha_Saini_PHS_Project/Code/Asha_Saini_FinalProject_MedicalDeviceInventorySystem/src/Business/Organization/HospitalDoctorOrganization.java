/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Organization;

import Business.Role.Role;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class HospitalDoctorOrganization extends Organization{

    public HospitalDoctorOrganization(String f_organizationName, Type f_type) {
        super(f_organizationName, f_type);
    }

    @Override
    public ArrayList<Role> getSupportedRole() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
