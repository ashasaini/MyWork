/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Doctor;

import Business.Employee.Employee;
import Business.Schedule.Schedule;

/**
 *
 * @author Asha
 */
public class Doctor extends Employee{
    private Schedule schedule;    
    
    public Doctor(String name, int age) {
        super(name, age);
        this.schedule = new Schedule();
    } 

    @Override
    public String toString() {
        return super.getEmployeeName();
    }   

    public Schedule getSchedule() {
        return schedule;
    }

    public void setSchedule(Schedule schedule) {
        this.schedule = schedule;
    }
    
    
}
