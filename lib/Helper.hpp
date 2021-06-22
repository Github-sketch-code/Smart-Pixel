#ifndef _HELPER_HPP_INCLUDED_
#define _HELPER_HPP_INCLUDED_

#include "Exception.hpp"

// Nur fuer ganze und positive Zahlen
String to_string(unsigned long num) {
	String ret;
	while(num > 0) {
		ret += num % 10;
		num /= 10;
	}
	std::reverse(ret.begin(), ret.end());
	return ret;
}

// Funktionen um den RGB-Hex-Code zu konvertieren und in dezimal Werte von 0-255 umwandeln
std::size_t power(unsigned short num, unsigned short times) {
	if(times >= 1) {
		return num * power(num, times - 1);
	}
	return 1;
}

unsigned short singleHexToDec(const char hex) {
	unsigned short ret;
	if(hex >= '0' && hex <= '9') {
		ret = hex - '0';
	} else if(hex >= 'a' && hex <= 'f') {
		ret = hex - 'a' + 10;
	} else if(hex >= 'A' && hex <= 'Z') {
		ret = hex - 'A' + 10;
	}
	return ret;
}

// https://owlcation.com/stem/Convert-Hex-to-Decimal
unsigned short hexToDec(const char* hex, std::size_t size_of_num){
	unsigned short ret = 0;
	for(unsigned short i = 0; i < size_of_num; i++) {
		// Elemente von vorne nach hinten durchlaufen(num)
		ret += singleHexToDec(hex[size_of_num - 1 - i]) * power(16, i);	// -1, weil 0 das erste element ist
	}
	return ret;
}

// https://www.geeksforgeeks.org/program-decimal-hexadecimal-conversion/
String decToHex(std::size_t n)
{
    String hexaDeciNum;

    // counter for hexadecimal number array
    int i = 0;
    while (n != 0) {
        // temporary variable to store remainder
        int temp = 0;
 
        // storing remainder in temp variable.
        temp = n % 16;
 
        // check if temp < 10
        if (temp < 10) {
            hexaDeciNum[i] += temp + 48;
            i++;
        }
        else {
            hexaDeciNum[i] += temp + 55;
            i++;
        }
 
        n = n / 16;
    }
 
    return hexaDeciNum;
}

#endif // _HELPER_HPP_INCLUDED_