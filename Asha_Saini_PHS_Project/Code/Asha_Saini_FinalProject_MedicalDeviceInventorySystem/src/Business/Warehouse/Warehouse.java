/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Warehouse;

import Business.MedicalDevice.MedicalDeviceCatalog;

/**
 *
 * @author Asha
 */
public class Warehouse {
    MedicalDeviceCatalog mdeciDeviceCatalog;

    public Warehouse() {
        this.mdeciDeviceCatalog = new MedicalDeviceCatalog();
    }

    public MedicalDeviceCatalog getMdeciDeviceCatalog() {
        return mdeciDeviceCatalog;
    }

    public void setMdeciDeviceCatalog(MedicalDeviceCatalog mdeciDeviceCatalog) {
        this.mdeciDeviceCatalog = mdeciDeviceCatalog;
    }   
}
