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
public class OperationHistory {
    private ArrayList<Operation> operationList;
    
    public OperationHistory(){
        //create the ArrayList at the same time we create the object of OperationHistory
        operationList = new ArrayList<Operation>(); 
    }

    public ArrayList<Operation> getOperationList() {
        return operationList;
    }

    public void setOperationList(ArrayList<Operation> operationList) {
        this.operationList = operationList;
    }   
    
    //add operation into the arrayList
    public void addOperation(float bloodPressure, float temperature, int pulse, String date){
         //create a Operation object
        Operation o = new Operation();
        
        //set the attribute for Operation object
//        o.setBloodPressure(bloodPressure);
//        o.setPulse(pulse);
//        o.setTemperature(temperature);
//        o.setDate(date);
        
        //add that object to the ArrayList
        operationList.add(o);    
    }
    
    //delete the Operation 
    public void deleteOperation(Operation o){
        operationList.remove(o);
    }   
}
