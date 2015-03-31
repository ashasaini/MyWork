/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Organization;

import Business.MedicalDevice.MedicalDevice.Catagory;
import Business.MedicalDevice.MedicalDeviceCatalog;
import Business.Role.Role;
import Business.Role.SupplierAdminRole;
import java.util.ArrayList;
import java.util.Date;

/**
 *
 * @author Asha
 */
public class SupplierAdminOrganization extends Organization {

    MedicalDeviceCatalog medicalDeviceCatalog;

    public SupplierAdminOrganization(String f_organizationName, Organization.Type f_type) {
        super(f_organizationName, f_type);
        super.setType(type);
        medicalDeviceCatalog = new MedicalDeviceCatalog();
        InitializeMedicalDeviceCatalog();
    }

    public MedicalDeviceCatalog getMedicalDeviceCatalog() {
        return medicalDeviceCatalog;
    }

    public void setMedicalDeviceCatalog(MedicalDeviceCatalog medicalDeviceCatalog) {
        this.medicalDeviceCatalog = medicalDeviceCatalog;
    }

    private void InitializeMedicalDeviceCatalog() {
        Catagory deviceCatagory = Catagory.Class_I;
        String deviceName = "";
        String modelName = "";        
        double price = 0.0;
        int availableQuantity = 0;        
        Date manufacutureDate = new Date();
        int serviceInterval = 0;
        String safetyGuideline = "";

        for (int i = 0; i < 5; i++) {
            switch (i+1) {
                case 1:
                    deviceCatagory = Catagory.Class_I;
                    deviceName = "MRI Scanner";
                    modelName = "Model A";
                    price = 120000;
                    availableQuantity = 20;
                    manufacutureDate = new Date();
                    serviceInterval = 1 ;
                    safetyGuideline = "MRI Scanner";
                    break;

                case 2:
                    deviceCatagory = Catagory.Class_II;
                    deviceName = "CT Scanner";
                    modelName = "Model B";
                    price = 130000;
                    availableQuantity = 20;
                    manufacutureDate = new Date();
                    serviceInterval = 1 ;
                    safetyGuideline = "CT Scanner";
                    break;

                case 3:
                   deviceCatagory = Catagory.Class_III;
                    deviceName = "PET Scanner";
                    modelName = "Model C";
                    price = 130000;
                    availableQuantity = 20;
                    manufacutureDate = new Date();
                    serviceInterval = 1 ;
                    safetyGuideline = "PET Scanner";
                    break;

                case 4:
                    deviceCatagory = Catagory.Class_I;
                    deviceName = "C Arm";
                    modelName = "Model D";
                    price = 1250000;
                    availableQuantity = 20;
                    manufacutureDate = new Date();
                    serviceInterval = 1 ;
                    safetyGuideline = "C Arm";
                    break;

                case 5:
                    deviceCatagory = Catagory.Class_II;
                    deviceName = "X-Ray Scanner";
                    modelName = "Model E";
                    price = 126000;
                    availableQuantity = 20;
                    manufacutureDate = new Date();
                    serviceInterval = 1 ;
                    safetyGuideline = "X-Ray Scanner";
                    break;
            }
            medicalDeviceCatalog.addDevice(deviceCatagory,
                                    deviceName,
                                    modelName,
                                    price,
                                    availableQuantity,
                                    manufacutureDate,
                                    serviceInterval,
                                    safetyGuideline);
        }
    }

    @Override
    public ArrayList<Role> getSupportedRole() {
        ArrayList<Role> roles = new ArrayList<Role>();
        roles.add(new SupplierAdminRole());
        return roles;
    }
}
