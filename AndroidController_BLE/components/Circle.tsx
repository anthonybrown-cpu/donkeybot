import React from 'react'
import { View, StyleSheet } from 'react-native';

type CircleProps = {
    posX: number;
    posY: number;
}

export const Circle: React.FunctionComponent<CircleProps> = (props) => {
    const {posX, posY} = props;
    return <View style={{...styles.circle, top: posX, left: posY}} />;
};

const styles = StyleSheet.create({
    circle: {
        width: 100,
        height: 100,
        borderRadius: 100 / 2,
        backgroundColor: "red",
    },
})