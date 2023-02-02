import React, {useState, useEffect} from 'react';
import {
  View,
  Text,
  Dimensions,
} from 'react-native';
import CircleController from './components/CircleController';
//import { BluetoothController } from './components/BluetoothManager';
import { Button } from './components/Button';

// TODO: Bluetooth connect button

export default function App() {

  const [controlPos, setControlPos] = useState({x: 0, y: 0});
  const [isConnected, setConnected] = useState(false)

  useEffect(() => {
    Dimensions.addEventListener('change', ({window: {width, height}}) => {
      setWindowHeight(height)
      setWindowWidth(width)
    })
  })
  
  const [windowHeight, setWindowHeight] = useState(Dimensions.get("window").height);
  const [windowWidth, setWindowWidth] = useState(Dimensions.get("window").width);

  const handlePosition = (position: {x: number, y: number}) => {
    setControlPos(position)
  }
  
  const handleDisconnect = () => {
    console.log("DEVICE DISCONNECT")
  }

  const handleConnect = () => {
    console.log("DEVICE CONNECT")
  }

  return (
    <View>
      <View style={{height: windowHeight, width: windowWidth, position: 'absolute'}}>
        <CircleController onMove={(pos) => handlePosition(pos)}/>
      </View>
      <View style={{position: 'absolute', right:0, top:0}}>
        {isConnected ? <Button title='DISCONNECT' onPress={handleDisconnect}/> : <Button title='CONNECT' onPress={handleConnect}/>}
      </View>
      <View>
        <Text>X: {controlPos.x}</Text>
        <Text>Y: {controlPos.y}</Text>
      </View>
    </View>
  );
}