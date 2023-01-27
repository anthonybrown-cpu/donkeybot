import React, {useRef, useState} from 'react'
import { View, Animated, PanResponder, StyleSheet } from 'react-native';

interface IProps{
    onMove?: (pos: {x: number, y: number}) => void;
}

export default function CircleController(props: IProps) {
    const {
        onMove
    } = props;
    
    const pan = useRef(new Animated.ValueXY()).current;
  
    const reset = () => {
      onMove?.({x: 0, y: 0})
      pan.setValue({x: 0, y: 0})
    }
  
    const panResponder = useRef(
      PanResponder.create({
  
        onMoveShouldSetPanResponder: () => true,
  
        onPanResponderMove: (evt, gestureState) => {
          // The most recent move distance is gestureState.move{X,Y}
          // The accumulated gesture distance since becoming responder is
          // gestureState.d{x,y)
          onMove?.({x: gestureState.dx, y: gestureState.dy})
          return Animated.event([null, {
            dx: pan.x,
            dy: pan.y,
          }], {useNativeDriver: false})(evt, gestureState)
        },
  
        onPanResponderRelease: () => {
          // The user has released all touches while this view is the
          // responder. This typically means a gesture has succeeded
          reset()
        },
      })
    ).current;
    
    return (
        <View style={styles.container}>
            <Animated.View style={{transform: [{translateX: pan.x}, {translateY: pan.y}], }} {...panResponder.panHandlers} >
                <View style={styles.circle} />
            </Animated.View>
        </View>
    )

}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  circle: {
    height: 50,
    width: 50,
    borderRadius: 50 / 2,
    backgroundColor: 'gray',
  },
})