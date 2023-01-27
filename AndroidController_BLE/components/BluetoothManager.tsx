import React, { useState } from "react";
import { PermissionsAndroid } from "react-native/types";
import { BleManager, Device, DeviceId } from "react-native-ble-plx";

const BLTManager = new BleManager();

const DEVICE_NAME = 'BLEExample';

const SERVICE_UUID = '4fafc201-1fb5-459e-8fcc-c5c9c331914b';
const MESSAGE_UUID = '6d68efe5-04b6-4a85-abc4-c2670b7bf7fd';

export class BluetoothController{
    isConnected: boolean;
    connectedDevice?: Device;
    
    onConnectCallback?: Function;
    onDisconnectCallback?: Function;

    constructor(){
        this.isConnected = false;
    }

    
    public onConnect(listener: (device: Device | null) => void){
        this.onConnectCallback = listener
    };

    public onDisconnect(listener: (device: Device | null) => void){
        this.onDisconnectCallback = listener
    }

    public getDevice(){
        return this.connectedDevice
    }
  
    // Scans availbale BLT Devices and then call connectDevice
    public async scanDevices() {
        PermissionsAndroid.request(
            PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION,
            {
            title: 'Permission Localisation Bluetooth',
            message: 'Requirement for Bluetooth',
            buttonNeutral: 'Later',
            buttonNegative: 'Cancel',
            buttonPositive: 'OK',
            },
        ).then(answer => {
            console.log('scanning');
            // display the Activityindicator

            BLTManager.startDeviceScan(null, null, (error, scannedDevice) => {
                if (error) {
                    console.warn(error);
                }
                
                // Found a device to connect to
                if (scannedDevice && scannedDevice.name == DEVICE_NAME) {
                    BLTManager.stopDeviceScan();
                    this.connectDevice(scannedDevice);
                }
            });

            // stop scanning devices after 5 seconds
            setTimeout(() => {
                BLTManager.stopDeviceScan();
            }, 5000);
        });
    }

    // handle the device disconnection (poorly)
    public async disconnectDevice() {
        console.log('Disconnecting start');

        if (this.connectedDevice != null) {
            const isDeviceConnected = await this.connectedDevice.isConnected();
            if (isDeviceConnected) {
                BLTManager.cancelTransaction('messagetransaction');
                BLTManager.cancelTransaction('nightmodetransaction');

                BLTManager.cancelDeviceConnection(this.connectedDevice.id).then(() =>
                    console.log('DC completed'),
                );
            }

            const connectionStatus = await this.connectedDevice.isConnected();
            if (!connectionStatus) {
                this.isConnected = false;
                this.connectedDevice = undefined;
                this.onDisconnectCallback?.(this.connectedDevice)
            }
        }
    }

    private connectDevice(device: Device) {
        console.log('connecting to Device:', device.name);

        device
            .connect()
            .then(device => {
                this.connectedDevice = device;
                this.isConnected = true;
                return device.discoverAllServicesAndCharacteristics();
            })
            .then(device => {
                //  Set what to do when DC is detected
                BLTManager.onDeviceDisconnected(device.id, (error, device) => {
                    console.log('Device DC');
                    this.isConnected = false;
                    this.connectedDevice = undefined;
                    this.onConnectCallback?.(device)
                });
            })

           /*  //Message - inital read
            device
                .readCharacteristicForService(SERVICE_UUID, MESSAGE_UUID)
                .then(valenc => {
                    setMessage(base64.decode(valenc.value));
                });

            //Message - read on message change
            device.monitorCharacteristicForService(
                SERVICE_UUID,
                MESSAGE_UUID,
                (error, characteristic) => {
                if (characteristic?.value != null) {
                    setMessage(base64.decode(characteristic?.value));
                    console.log(
                    'Message update received: ',
                    base64.decode(characteristic?.value),
                    );
                }
                },
                'messagetransaction',
            );

            console.log('Connection established');
            }); */
        
    }
}