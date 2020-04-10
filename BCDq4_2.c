#include <stdio.h>
#include <stdlib.h>

void printBCD(unsigned char* BCD, int bitno) {
	printf("The sign byte:%d%d%d%d%d%d%d%d\n\n", (BCD[0] & 0x80), (BCD[0] & 0x40), (BCD[0] & 0x20), (BCD[0] & 0x10), (BCD[0] & 0x08), (BCD[0] & 0x04), (BCD[0] & 0x02), (BCD[0] & 0x01));
	for (int i = 0; i < bitno; i++) {
		printf("%d%d%d%d%d%d%d%d\n", (BCD[100 - i] & 0x80), (BCD[100 - i] & 0x40), (BCD[100 - i] & 0x20), (BCD[100 - i] & 0x10), (BCD[100 - i] & 0x08), (BCD[100 - i] & 0x04), (BCD[100 - i] & 0x02), (BCD[100 - i] & 0x01));
	}
}

void getBCD(int number, unsigned char* BCD, int* bitno) {
	int s_l = 0;
	int n_loop = number;
	//loop calculates how many digits the number has
	while (n_loop) {
		s_l++;
		n_loop = n_loop / 10;
	}
	*bitno = s_l;


	if (number >= 0)
		BCD[0] = 0;
	else {
		BCD[0] = 0xFF;
		number = (-number);//so that next step works 
	}
	//modulus ten takes the value of the digit, dividing by ten reduces a digit
	//this loop collects the digits into the array
	int i = 0;
	while (number) {
		BCD[100 - i] = (number % 10);
		i++;
		number = number / 10;
	}
}

//BCD[101 - i] += (number % 10);
//how to check whether a number is actually encoded properly?
//unsigned char a = 5;

//printf("print d : %d%d%d%d%d%d%d%d\n", (a & 0x80), (a & 0x40), (a & 0x20), (a & 0x10), (a & 0x08), (a & 0x04), (a & 0x02), (a & 0x01));

//above code prints 0000 0401 and that four is printed in decimal because of %d
//this means BCD[101-i] += number%10 actually gets the number properly bit by bit


void addBCD(unsigned char* result, unsigned char const* one, unsigned char const* two, int bitno1, int bitno2,int *bitnoresult) {
	int size = 0;
	int no1_bigger;
	if (bitno1 > bitno2) {
		no1_bigger = 1;
		size = bitno1;
	}
	if (bitno1 < bitno2) {
		no1_bigger = 2;
		size = bitno2;
	}
	if (bitno1 == bitno2) {
		no1_bigger = 0;
		size = bitno1;
	}
	unsigned char carry = 0;

	if ((one[0] == 0) & (two[0] == 0)) {//if both are positive
		result[0] = 0;//result will be positive
		for (int i = 0; i < size; i++) {
			result[100 - i] = one[100 - i] + two[100 - i];//add the results
			result[100 - i] += carry;//add the carry
			carry = 0;
			if (result[100 - i] >= 10) {//if the sum is bigger than 9, there is an overflow to the next digit
				result[100 - i] += 6;//restore 6 to the lower nibble, because 16 has been passed to the upper nibble 
				result[100 - i] -= 16;//what should be a one in the higher digit, is mistakenly set as 0001 xxxx
				carry = 1;			  //carry that ''one'' to the next digit
			}
		}
		result[100 - size] += carry; //maybe one extra digit will be produced at last (0000 0001)
		carry = 0;//reset carry because it will be used again
	}
	if ((one[0] == 0xFF) & (two[0] == 0xFF)) {
		result[0] = 0xFF; //same as both positive, this time both going lower
		for (int i = 0; i < size; i++) {
			result[100 - i] = one[100 - i] + two[100 - i];
			result[100 - i] += carry;
			carry = 0;
			if (result[100 - i] >= 10) {
				result[100 - i] += 6;
				result[100 - i] -= 16;
				carry = 1;
			}
		}
		result[100 - size] += carry;
		carry = 0; //reset carry

	}
	if (((one[0] == 0) & (two[0] == 0xFF)) | ((one[0] == 0xFF) & (two[0] == 0))) {
		if (no1_bigger == 2) {//two has more digits than one
			for (int i = 0; i < size; i++) {


				if (two[100 - i] < (one[100 - i]+carry)) {//if the minuend digit is smaller than the subtrahend+carry

				//there can never be a case in which the result of this subtraction is bigger than 9
				//if a borrow is needed, this subtraction is sure to make the result lower than 10
				//because the point of borrowing from the next digit is that the subtraction goes below zero,
				//and even if it goes one below zero which is the minimum to necessitate a borrow,
				//adding a ten to it will leave nine
				//thus adding 0110 to the lower nibble will not be needed


					result[100-i] = (two[100 - i] + 10) - (one[100 - i]+carry);//get 10 from the next digit and do the subtraction
					carry = 1;//carry one to subtract in next step
				} 


				if(two[100-i]>=(one[100-i]+carry)) {//if minuend is bigger than subtrahend+carry
					result[100 - i] = two[100 - i] - (one[100 - i]+carry);
					carry = 0;//there won't be a borow from the next digit
				}
			}
			//bigger number of digits allows us to tell these for sure...
			if (one[0] == 0)	//one is positive while two negative
				result[0] = 0xFF;//result should be negative
			if (one[0] == 0xFF) //one is negative while two positive
				result[0] = 0;//result should be positive
		}
		if (no1_bigger == 1) {//one has more digits than two
			for (int i = 0; i < size; i++) {
				if (one[100 - i] < (two[100 - i] + carry)) {
					result[100 - i] = (one[100 - i] + 10) - (two[100 - i] + carry);
					carry = 1;
				}
				if (one[100 - i] >= (two[100 - i] + carry)) {
					result[100 - i] = one[100 - i] - (two[100 - i] + carry);
					carry = 0;
				}
			}
			//bigger number of digits allows us to tell these for sure...
			if (one[0] == 0)	
				result[0] = 0;
			if (one[0] == 0xFF) 
				result[0] = 0xFF;

		}

		if (no1_bigger == 0) { //both have the same no of digits 

			if (one[0] == 0) { //to know which one the subtrahend is
				for (int i = 0; i < size; i++) {
					//to get 1's complement, subtract from 0000 1111, and to get 2's complement, add 1 
					result[100 - i] = one[100 - i] + ((15 - two[100 - i]) + 1);
					//if the sum has produced a carry, the result is correct and positive
					//if there is no carry, the result is negative but not correct, 2's complt. again to correct
					if (result[100 - i] < 16)
						result[100 - i] = (15 - result[100 - i]) + 1;
				}
			}
			if (one[0] == 0xFF) {//same loop but this time subtrahend is one
				for (int i = 0; i < size; i++) {
					result[100 - i] = two[100 - i] + ((15 - one[100 - i]) + 1);
					if (result[100 - i] < 16)
						result[100 - i] = (15 - result[100 - i]) + 1;
				}
			}

			//the above loops can produce result array elements such as
			//0001 0110 which would mean subtraction produced a positive six
			//0000 0110 which would be a negative six
			//then to know whether end result is positive or negative, after all of this subtraction
			//we will use the first nibble of every byte

			//in each loop check whether byte is bigger than 16, which means subtraction produced a positive
			//if it has, then that means the minuend was greater than the subtrahend,
			//thus the subtraction should produce a positive number

			//however, even when we decide the sign of the end result, remaining bytes still remain false
			//they have 1s in their greater nibbles while they shouldn't have
			//so we should continue looping while not changing the sign of the end result, so a new loop is required

			int counter = 1;
			int i_ct = size - 1;
			while ((counter != 0) & (i_ct >= 0)) {
				if (result[100-i_ct] < 16) { //know for sure result negative
					result[0] = 0xFF;
					//negative result so no mistake in greater nibble
					counter = 0;		//exit loop
				}
				if (result[100-i_ct] > 16) {//know for sure result positive
					result[0] = 0;
					//2's complement produces a carry if result is positive, 
					//remove the carry from the greater nibble to fix the result
					result[100-i_ct] = result[100-i_ct] - 16;
					counter = 0;
				}
				if (result[100-i_ct] == 16) {//don't know, so keep looping until sign byte can be determined
					result[100-i_ct] = result[100-i_ct] - 16;
				}
				i_ct--;
			}
			while (i_ct>=0) {//new loop to fix the result elements
				if (result[100-i_ct] > 16) 
					result[100-i_ct] = result[100-i_ct] - 16;
				if (result[100-i_ct] == 16) 
					result[100-i_ct] = result[100-i_ct] - 16;
				i_ct--; 
			}
		}



	}

	int ct = 1;
	int bit_i = size; //the addition of two can drive to the next digit at most 1
	while ((ct != 0) & (bit_i != 0)) {
		if (result[100-bit_i] != 0) {
			*bitnoresult = bit_i+1; //gets number of elements the results array has
			ct = 0;
			bit_i++;//because I want it to exit without decrementing one last time
		}
		bit_i--;
	}
	if (bit_i == 0)
		*bitnoresult = bit_i+1;
}

int bcd_compare(unsigned char* p_n1, unsigned char* p_n2,int *bitno1,int *bitno2) {

	int size = ((bitno1 > bitno2) ? bitno1 : bitno2); //if bitno1==bitno2 then condition false and second result, which would be the same as bitno1

	if ((p_n1[0] == 0) & (p_n2[0] == 0xFF)) //for sure p_n1 bigger
		return 1;

	if ((p_n1[0] == 0xFF) & (p_n2[0] == 0)) //for sure p_n2 smaller
		return -1;

	if (p_n1[0] == p_n2[0]) {	//lets loop
		for (int i = (size-1); i >=0; i--) {
			if (p_n1[100 - i] > p_n2[100 - i])
				return 1;
			if (p_n1[100 - i] < p_n2[100 - i])
				return -1;
		}
		return 0;
	}

}


int main() {

	//addition
	int no1 = 55;
	unsigned char BCD1[101] = { 0 };
	int bitno1 = 0;
	getBCD(no1, BCD1, &bitno1);
	printBCD(BCD1, bitno1);

	int no2 = 46;
	unsigned char BCD2[101] = { 0 };
	int bitno2 = 0;
	getBCD(no2, BCD2, &bitno2);
	printBCD(BCD2, bitno2);

	int bitnoresult1=0;
	unsigned char BCDresult1[101] = { 0 };
	addBCD(BCDresult1, BCD1, BCD2, bitno1, bitno2,&bitnoresult1);
	printBCD(BCDresult1, bitnoresult1);

	//same number of digits subtraction
	int no3 = -116;
	unsigned char BCD3[101] = { 0 };
	int bitno3 = 0;
	getBCD(no3, BCD3, &bitno3);
	printBCD(BCD3, bitno3);

	int no4 = 105;
	unsigned char BCD4[101] = { 0 };
	int bitno4 = 0;
	getBCD(no4, BCD4, &bitno4);
	printBCD(BCD4, bitno4);

	int bitnoresult2 = 0;
	unsigned char BCDresult2[101] = { 0 };
	addBCD(BCDresult2, BCD3, BCD4, bitno3, bitno4, &bitnoresult2);
	printBCD(BCDresult2, bitnoresult2);

	//different number of digits subtraction 
	//no1 has less digits
	int no5 = 95;
	unsigned char BCD5[101] = { 0 };
	int bitno5 = 0;
	getBCD(no5, BCD5, &bitno5);
	printBCD(BCD5, bitno5);

	int no6 = -116;
	unsigned char BCD6[101] = { 0 };
	int bitno6 = 0;
	getBCD(no6, BCD6, &bitno6);
	printBCD(BCD6, bitno6);

	int bitnoresult3 = 0;
	unsigned char BCDresult3[101] = { 0 };
	addBCD(BCDresult3, BCD5, BCD6, bitno5, bitno6, &bitnoresult3);
	printBCD(BCDresult3, bitnoresult3);

	//different number of digits subtraction
	//no1 has more digits
	int no7 = -205;
	unsigned char BCD7[101] = { 0 };
	int bitno7 = 0;
	getBCD(no7, BCD7, &bitno7);
	printBCD(BCD7, bitno7);

	int no8 = 92;
	unsigned char BCD8[101] = { 0 };
	int bitno8 = 0;
	getBCD(no8, BCD8, &bitno8);
	printBCD(BCD8, bitno8);

	int bitnoresult4 = 0;
	unsigned char BCDresult4[101] = { 0 };
	addBCD(BCDresult4, BCD7, BCD8, bitno7, bitno8, &bitnoresult4);
	printBCD(BCDresult4, bitnoresult4);

	//different number of digits addition
	int no9 = 205;
	unsigned char BCD9[101] = { 0 };
	int bitno9 = 0;
	getBCD(no9, BCD9, &bitno9);
	printBCD(BCD9, bitno9);

	int no10 = 1192;
	unsigned char BCD10[101] = { 0 };
	int bitno10 = 0;
	getBCD(no10, BCD10, &bitno10);
	printBCD(BCD10, bitno10);

	int bitnoresult5 = 0;
	unsigned char BCDresult5[101] = { 0 };
	addBCD(BCDresult5, BCD9, BCD10, bitno9, bitno10, &bitnoresult5);
	printBCD(BCDresult5, bitnoresult5);

	printf("Compare 205 and 1192 : %d\n", bcd_compare(BCD9, BCD10,bitno9,bitno10));
	printf("Compare -205 and 92 : %d\n", bcd_compare(BCD7, BCD8,bitno7,bitno8));
	printf("Compare 95 and -116 : %d\n", bcd_compare(BCD5, BCD6,bitno5,bitno6));
	printf("Compare -116 and 105 : %d\n", bcd_compare(BCD3, BCD4,bitno3,bitno4));
	printf("Compare 205 and 205 : %d\n", bcd_compare(BCD9, BCD9,bitno9,bitno9));
}