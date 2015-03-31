/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.MedicalDevice;

import Business.MaintenanceSchedule.MaintenanceSchedule;
import Business.Schedule.Schedule;
import java.util.Calendar;
import java.util.Date;

/**
 *
 * @author Asha
 */
public class MedicalDevice {
    
    public enum Catagory{
        Class_I("Class I"),
        Class_II("Class II"),
        Class_III("Class III");
        
        private String value;
        private Catagory(String value){
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
        
    private Catagory deviceCatagory;
    private String deviceName;
    private String modelName;
    private int deviceId;
    private double price;
    private int availableQuantity;
    private int soldQuantity;
    private Date manufacutureDate;
    private int serviceInterval;
    private String safetyGuideline;
    private Schedule schedule;
    private MaintenanceSchedule maintenanceSchedule;
    private boolean isUptoDate;
    private static int numUses = 0;

    private static int count = 0;
     
    public MedicalDevice() {
        this.count++;
        this.deviceId = count;
        this.soldQuantity = 0;
        this.schedule = new Schedule();
        this.maintenanceSchedule = new MaintenanceSchedule();
        this.isUptoDate = true;
    }   

    public Catagory getDeviceCatagory() {
        return deviceCatagory;
    }

    public void setDeviceCatagory(Catagory deviceCatagory) {
        this.deviceCatagory = deviceCatagory;
    }

    public String getDeviceName() {
        return deviceName;
    }

    public void setDeviceName(String deviceName) {
        this.deviceName = deviceName;
    }

    public String getModelName() {
        return modelName;
    }

    public void setModelName(String modelName) {
        this.modelName = modelName;
    }

    public int getDeviceId() {
        return deviceId;
    }

    public void setDeviceId(int deviceId) {
        this.deviceId = deviceId;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public int getAvailableQuantity() {
        return availableQuantity;
    }

    public void setAvailableQuantity(int availableQuantity) {
        this.availableQuantity = availableQuantity;
    }

    public int getSoldQuantity() {
        return soldQuantity;
    }

    public void setSoldQuantity(int soldQuantity) {
        this.soldQuantity = soldQuantity;
    }

    public Date getManufacutureDate() {
        return manufacutureDate;
    }

    public void setManufacutureDate(Date manufacutureDate) {
        this.manufacutureDate = manufacutureDate;
    }

    public int getServiceInterval() {
        return serviceInterval;
    }

    public void setServiceInterval(int serviceInterval) {
        this.serviceInterval = serviceInterval;
    }

    public String getSafetyGuideline() {
        return safetyGuideline;
    }

    public void setSafetyGuideline(String safetyGuideline) {
        this.safetyGuideline = safetyGuideline;
    }   

    @Override
    public String toString() {
        return deviceName;
    }  

    public Schedule getSchedule() {
        return schedule;
    }

    public void setSchedule(Schedule schedule) {
        numUses++;
        this.schedule = schedule;
    }

    public MaintenanceSchedule getMaintenanceSchedule() {
        return maintenanceSchedule;
    }

    public void setMaintenanceSchedule(MaintenanceSchedule maintenanceSchedule) {
        this.maintenanceSchedule = maintenanceSchedule;
    }

    public boolean isIsUptoDate() {
        return isUptoDate;
    }

    public void setIsUptoDate(boolean isUptoDate) {
        this.isUptoDate = isUptoDate;
    }
    
    
    public void setMaintenanceSchedule(){
        
        //get last maintenance date
        Date lastmaintainedOn = maintenanceSchedule.getLastMaintenanceDate();
           
        //find next maintenance date
        Date nextMaintenanceDueOn = new Date();
        Calendar c = Calendar.getInstance();
        c.setTime(lastmaintainedOn);
        c.add(Calendar.DAY_OF_MONTH,(30*serviceInterval));
        nextMaintenanceDueOn = c.getTime();       

        //set next maintenanc date       
        maintenanceSchedule.setNextMaintenanceDate(nextMaintenanceDueOn);
    }

    public static int getNumUses() {
        return numUses;
    }

    public static void setNumUses(int numUses) {
        MedicalDevice.numUses = numUses;
    }
    
    
}