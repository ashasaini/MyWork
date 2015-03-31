/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.MedicalDevice;

import Business.MedicalDevice.MedicalDevice.Catagory;
import java.util.ArrayList;
import java.util.Date;

/**
 *
 * @author Asha
 */
public class MedicalDeviceCatalog {
     private ArrayList<MedicalDevice> medicalDeviceList;

    public MedicalDeviceCatalog() {
        this.medicalDeviceList = new ArrayList<MedicalDevice>();
    }

    public ArrayList<MedicalDevice> getMedicalDeviceList() {
        return medicalDeviceList;
    }

    public void setMedicalDeviceList(ArrayList<MedicalDevice> medicalDeviceList) {
        this.medicalDeviceList = medicalDeviceList;
    }
     
    public MedicalDevice addDevice(Catagory deviceCatagory, 
                                    String deviceName,
                                    String modelName,                                    
                                    double price,    
                                    int quantity,
                                    Date manufacutureDate,
                                    int serviceInterval,
                                    String safetyGuideline ) {
        
        MedicalDevice md = new MedicalDevice();
        md.setDeviceCatagory(deviceCatagory);
        md.setDeviceName(deviceName);
        md.setModelName(modelName);
        md.setPrice(price);        
        md.setAvailableQuantity(quantity);
        md.setManufacutureDate(manufacutureDate);
        md.setServiceInterval(serviceInterval);
        md.setSafetyGuideline(safetyGuideline);      
        medicalDeviceList.add(md);
        return md;
    }

    public void removeMedicalDevice(MedicalDevice md) {
        medicalDeviceList.remove(md);
    }

    public MedicalDevice searchProduct(int id) {        
        for (MedicalDevice md : medicalDeviceList) {
            if (md.getDeviceId() == id) {
                return md;
            }
        }
        return null;
    }   
}
