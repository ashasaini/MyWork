/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Patient;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class VitalSignHistory {
    private ArrayList<VitalSign> vitalSignList;
    
    public VitalSignHistory(){
        //create the ArrayList at the same time we create the object of VitalSignHistory
        vitalSignList = new ArrayList<VitalSign>(); 
    }

    public ArrayList<VitalSign> getVitalSignList() {
        return vitalSignList;
    }

    public void setVitalSignList(ArrayList<VitalSign> vitalSignList) {
        this.vitalSignList = vitalSignList;
    }   
    
    //add vitalSign into the arrayList
    public void addVitalSign(float bloodPressure, float temperature, int pulse, String date){
         //create a VitalSign object
        VitalSign v = new VitalSign();
        
        //set the attribute for VitalSign object
        v.setBloodPressure(bloodPressure);
        v.setPulse(pulse);
        v.setTemperature(temperature);
        v.setDate(date);
        
        //add that object to the ArrayList
        vitalSignList.add(v);    
    }
    
    //delete the VitalSign 
    public void deleteVitalSign(VitalSign v){
        vitalSignList.remove(v);
    }   
    
}
