/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Supplier;

import Business.MedicalDevice.MedicalDeviceCatalog;

/**
 *
 * @author Asha
 */
public class Supplier {
    private String supplierName;
    private MedicalDeviceCatalog medicalDeviceCatalog;

    public Supplier() {
        medicalDeviceCatalog = new MedicalDeviceCatalog();
    }

    public String getSupplierName() {
        return supplierName;
    }

    public void setSupplierName(String f_supplierName) {
        this.supplierName = f_supplierName;
    }

    public MedicalDeviceCatalog getMedicalDeviceCatalog() {
        return medicalDeviceCatalog;
    }

    @Override
    public String toString() {
        return supplierName;
    }
}
