import React from 'react'
import { GestureResponderEvent, TouchableOpacity, Text } from 'react-native';
import { StyleSheet } from 'react-native/types';

type ButtonProps = {
    title: string;
    onPress: (event: GestureResponderEvent) => void;
}

export const Button: React.FunctionComponent<ButtonProps> = (props) => {
    const {onPress, title} = props;
    return <TouchableOpacity onPress={onPress} style={styles.appButtonContainer}>
        <Text style={styles.appButtonText}>{title}</Text>
    </TouchableOpacity>
}

const styles = StyleSheet.create({
    appButtonContainer: {
        elevation: 8,
        backgroundColor: "#009688",
        borderRadius: 10,
        paddingVertical: 10,
        paddingHorizontal: 12
    },
    appButtonText: {
        fontSize: 18,
        color: "#fff",
        fontWeight: "bold",
        alignSelf: "center",
        textTransform: "uppercase"
    }
});