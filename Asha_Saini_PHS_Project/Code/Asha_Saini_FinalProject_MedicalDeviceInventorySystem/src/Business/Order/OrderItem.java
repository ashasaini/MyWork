/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Order;

import Business.MedicalDevice.MedicalDevice;

/**
 *
 * @author Asha
 */
public class OrderItem {

    private MedicalDevice medicalDevice;
    int quantity;

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public MedicalDevice getMedicalDevice() {
        return medicalDevice;
    }

    public void setMedicalDevice(MedicalDevice medicalDevice) {
        this.medicalDevice = medicalDevice;
    }

    @Override
    public String toString() {
        return medicalDevice.getDeviceName();
    }
}
