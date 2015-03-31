/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Order;

import Business.MedicalDevice.MedicalDevice;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class Order {
    private static int count = 0;
    private ArrayList<OrderItem> orderItemList;
    private int orderNumber;

    public Order() {
        count++;
        orderItemList = new ArrayList<OrderItem>();
        orderNumber = count;
    }

    public ArrayList<OrderItem> getOrderItemList() {
        return orderItemList;
    }

    public int getOrderNumber() {
        return orderNumber;
    }

    //add order item 
    public OrderItem addOrderItem(MedicalDevice medicalDevice, int quantity) {
        //create a new order item
        OrderItem oi = new OrderItem();
        oi.setMedicalDevice(medicalDevice);
        oi.setQuantity(quantity);

        //add order item to the lists
        orderItemList.add(oi);
        return oi;
    }

    //remove order item
    public void removeOrder(OrderItem oi) {
        orderItemList.remove(oi);
    }

    @Override
    public String toString() {
        return "Order{" + "orderNumber=" + orderNumber + '}';
    }   
}
