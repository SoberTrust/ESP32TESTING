//
//  ViewController.swift
//  bluetoothconnect
//
//  Created by Leo Pelissier on 2020-01-06.
//  Copyright © 2020 Leo Pelissier. All rights reserved.
// ESP32ADC

import UIKit
import CoreBluetooth

let text = " ".data(using: .utf8)                                                   //create a value for the text box variable
class ViewController: UIViewController {
    
    @IBOutlet weak var currtext: UILabel!
    @IBOutlet weak var BLETEST: UILabel!
    @IBOutlet weak var field: UITextField!      //initiate text field for interaction
    @IBOutlet weak var BUTTON: UIButton!        //initiate button for interaction
    
    let HardwareCBUUID = CBUUID(string: "4fafc201-1fb5-459e-8fcc-c5c9c331914b")     //bluetooth id's
    let SoftwareCBUUID = CBUUID(string: "beb5483e-36e1-4688-b7f5-ea07361b26a8")
    var centralManager: CBCentralManager!
    var SoberHelpPeripheral: CBPeripheral!
    var varr = 1
    
    override func viewDidLoad() {
        super.viewDidLoad()                                                         //runs when progream starts
        // Do any additional setup after loading the view.
        centralManager = CBCentralManager(delegate: self, queue: nil)               //create CBCentral manager w ext
        //BUTTON.backgroundColor = UIColor.init(red: 230/255, green: 230/255, blue: 230/255, alpha: 1)    //set button settings
        BUTTON.layer.cornerRadius = 25.0

        
        
    }
    

    @IBAction func touch(_ sender: UIButton) {                                      //code activated on button press
        if (varr == 0){
        self.peripheral(SoberHelpPeripheral, didDiscoverServices: nil)              //refers to function in class extension
        }
    }
    
    func onSignalRecieved() {
        print("signal received")
    }
    
    
}

extension ViewController: CBCentralManagerDelegate{
    func centralManagerDidUpdateState(_ central: CBCentralManager) {            //checks bluetooth state and prints it
        switch central.state{
            
        case .unknown:
            print("central.state is unkown")
        case .resetting:
            print("central.state is resetting")
        case .unsupported:
            print("central.state is unsupported")
        case .unauthorized:
            print("central.state is unauthorized")
        case .poweredOff:
            print("central.state is powered off")
        case .poweredOn:
            print("central.state is powered on")
            centralManager.scanForPeripherals(withServices: [HardwareCBUUID]) //exactly what it sounds like
        }
    }
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,  //centralmanager function, creates the ble peripheral
                        advertisementData: [String: Any], rssi RSSI: NSNumber) {            //which is the bluetooth device
        print(peripheral)
        SoberHelpPeripheral = peripheral
        SoberHelpPeripheral.delegate = self
        centralManager.stopScan()
        centralManager.connect(SoberHelpPeripheral)                                         //this sounds like what it is first find then connect
    }
    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("connected")                                                                  //prints connected when device is found
        BLETEST.text = "BLE ON"
        varr = 0
        currtext.text = " "
        SoberHelpPeripheral.discoverServices(nil)
    }
}

extension ViewController: CBPeripheralDelegate {
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {        //check for the different characteristics/services the
        guard let services = peripheral.services else { return }                            //ble device has
        
        for service in services {                                                           //in our case there is only one characteristic which
            print(service)                                                                  //holds data that can be transfered
            peripheral.discoverCharacteristics(nil, for: service)                           //will use this to transfer picture and string of alc
        }                                                                                   //percentage in our case, one at a time to the phone
                                                                                            //which will then hold the info and be transfered to cloud
    }
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService,
                    error: Error?) {
        guard let characteristics = service.characteristics else { return }
        
        for characteristic in characteristics {
            print(characteristic)
            if characteristic.properties.contains(.read) {                                     //checks for characteristic read capabilities
                print("\(characteristic.uuid): properties contains .read")
                peripheral.readValue(for: characteristic)
            }
            if characteristic.properties.contains(.notify) {                                    //checks for notify ability
                print("\(characteristic.uuid): properties contains .notify")                    //(notify when value changes)
            }
            if characteristic.properties.contains(.write) {                                     //checks for write capabilities
                print("\(characteristic.uuid): properties contains .write")
            }
            
        }
    }
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic,     //checks characteristic data contained inside
                    error: Error?) {
        switch characteristic.uuid {
        case SoftwareCBUUID:
            print(characteristic.value ?? "no value")
        default:
            print("Unhandled Characteristic UUID: \(characteristic.uuid)")
        }
        
        var text: Data? = field.text?.data(using: .utf8)                                        //reads then converts the string to data
        peripheral.writeValue(text!, for: characteristic, type: .withResponse)                  //sets the characteristic value as that data
        var str: String? = String(decoding: characteristic.value!, as: UTF8.self)               //cnt below <<<< this reads the data and then
        currtext.text = str                                                                     //publishes it >>>>
                                                                                                //which updates the value on both devices
    }                                                                                           //making it so that the text typed is converted
}                                                                                               //on the hardware side then displayed

