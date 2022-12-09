/*
 * Project Name: DES Algorithm implementation in C++ language.
 * File Name: des_g19.cpp
 * Authors:
 * Mohamed Saad Ahmed
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
typedef unsigned long long ll;
#define GET_BIT(DATA,BIT) ((DATA & ((ll)1<<((ll)BIT))) >> ((ll)BIT))

/*********************************************************************
 *                             TABLES                                 *
 *********************************************************************/
const char Permutation_Choice_1[56] = { 57 ,49 ,41 ,33 ,25 ,17 ,9  ,
		1  ,58 ,50 ,42 ,34 ,26 ,18 ,
		10 ,2  ,59 ,51 ,43 ,35 ,27 ,
		19 ,11 ,3  ,60 ,52 ,44 ,36 ,
		63 ,55 ,47 ,39 ,31 ,23 ,15 ,
		7  ,62 ,54 ,46 ,38 ,30 ,22 ,
		14 ,6  ,61 ,53 ,45 ,37 ,29 ,
		21 ,13 ,5  ,28 ,20 ,12 ,4 };
const char Permutation_Choice_2[48] = { 14 ,17 ,11 ,24 ,1  ,5  ,
		3  ,28 ,15 ,6  ,21 ,10 ,
		23 ,19 ,12 ,4  ,26 ,8  ,
		16 ,7  ,27 ,20 ,13 ,2  ,
		41 ,52 ,31 ,37 ,47 ,55 ,
		30 ,40 ,51 ,45 ,33 ,48 ,
		44 ,49 ,39 ,56 ,34 ,53 ,
		46 ,42 ,50 ,36 ,29 ,32 };
const char Initial_Permutation_t[64] = { 58 ,50 ,42 ,34 ,26 ,18 ,10 ,2 ,
		60 ,52 ,44 ,36 ,28 ,20 ,12 ,4 ,
		62 ,54 ,46 ,38 ,30 ,22 ,14 ,6 ,
		64 ,56 ,48 ,40 ,32 ,24 ,16 ,8 ,
		57 ,49 ,41 ,33 ,25 ,17 ,9  ,1 ,
		59 ,51 ,43 ,35 ,27 ,19 ,11 ,3 ,
		61 ,53 ,45 ,37 ,29 ,21 ,13 ,5 ,
		63 ,55 ,47 ,39 ,31 ,23 ,15 ,7 };
const char Final_permutation[64] = { 40, 8, 48, 16 ,56, 24, 64, 32,
		39, 7 ,47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41, 9, 49, 17, 57, 25 };
const char permutation_t[32] = { 16 ,7  ,20 ,21 ,
		29 ,12 ,28 ,17 ,
		1  ,15 ,23 ,26 ,
		5  ,18 ,31 ,10 ,
		2  ,8  ,24 ,14 ,
		32 ,27 ,3  ,9  ,
		19 ,13 ,30 ,6  ,
		22 ,11 ,4  ,25 };
const int Expantion_t[48] = { 32 ,1  ,2  ,3  ,4  ,5  ,
		4  ,5  ,6  ,7  ,8  ,9  ,
		8  ,9  ,10 ,11 ,12 ,13 ,
		12 ,13 ,14 ,15 ,16 ,17 ,
		16 ,17 ,18 ,19 ,20 ,21 ,
		20 ,21 ,22 ,23 ,24 ,25 ,
		24 ,25 ,26 ,27 ,28 ,29 ,
		28 ,29 ,30 ,31 ,32 ,1 };
const int S[8][4][16] = {                        // S-box
		{
				{ 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
				{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
				{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
				{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 }
		},
		{
				{ 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
				{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
				{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
				{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 }
		},
		{
				{ 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
				{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
				{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
				{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 }
		},
		{
				{ 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
				{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
				{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
				{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 }
		},
		{
				{ 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
				{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
				{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
				{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 }
		},
		{
				{ 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
				{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
				{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
				{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 }
		},
		{
				{ 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
				{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
				{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
				{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 }
		},
		{
				{ 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
				{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
				{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
				{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 }
		}
};
const int P_1[64] = { 40 ,8  ,48 ,16 ,56 ,24 ,64 ,32 ,
		39 ,7  ,47 ,15 ,55 ,23 ,63 ,31 ,
		38 ,6  ,46 ,14 ,54 ,22 ,62 ,30 ,
		37 ,5  ,45 ,13 ,53 ,21 ,61 ,29 ,
		36 ,4  ,44 ,12 ,52 ,20 ,60 ,28 ,
		35 ,3  ,43 ,11 ,51 ,19 ,59 ,27 ,
		34 ,2  ,42 ,10 ,50 ,18 ,58 ,26 ,
		33 ,1  ,41 ,9  ,49 ,17 ,57 ,25 };
const char num_leftShift[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
ll keys[16];

/*********************************************************************
 *                            OPERATIONS                              *
 *********************************************************************/
ll C_16To64Bit(string s)
{
	unsigned long long bin = 0;
	for (int i = 0; i < s.size(); i++)
	{
		switch (s[i])
		{
		case '0': break;
		case '1':
		{
			bin |= ((ll)1 << i * (ll)4 + 3);
			break;
		}
		case '2':
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			break;
		}
		case '3':
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 3);
			break;
		}
		case '4':
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			break;
		}
		case '5':
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 3);
			break;
		}
		case '6':
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 2);
			break;
		}
		case '7':
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 3);
			break;
		}
		case '8':
		{
			bin |= ((ll)1 << (i * (ll)4));
			break;
		}
		case '9':
		{
			bin |= ((ll)1 << (i * (ll)4) + 3);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 'A':
		case 'a':
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 'B':
		case 'b':
		{
			bin |= ((ll)1 << (i * (ll)4) + 3);
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 'C':
		case 'c':
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 'D':
		case 'd':
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 3);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 'E':
		case 'e':
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 'F':
		case 'f':
		{
			bin |= ((ll)1 << (i * (ll)4) + 3);
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}

		}
	}
	return bin;
}
string plain_to_hex(string plain)
{
	string s = "";
	unsigned char x = 0;
	unsigned char right_digit, left_digit;
	for (int i = 0; i < plain.size(); i++)
	{
		x = plain[i];
		left_digit = x / 16;
		right_digit = x % 16;
		switch (left_digit)
		{
		case 0:
		{
			s += '0';

			break;
		}
		case 1:
		{
			s += '1';

			break;
		}
		case 2:
		{
			s += '2';

			break;
		}
		case 3:
		{
			s += '3';
			break;
		}
		case 4:
		{
			s += '4';
			break;
		}
		case 5:
		{
			s += '5';
			break;
		}
		case 6:
		{
			s += '6';
			break;
		}
		case 7:
		{
			s += '7';
			break;
		}
		case 8:
		{
			s += '8';
			break;
		}
		case 9:
		{
			s += '9';
			break;
		}
		case 10:
		{
			s += 'A';
			break;
		}
		case 11:
		{
			s += 'B';
			break;
		}
		case 12:
		{
			s += 'C';
			break;
		}
		case 13:
		{
			s += 'D';
			break;
		}
		case 14:
		{
			s += 'E';
			break;
		}
		case 15:
		{
			s += 'F';
			break;
		}

		}
		switch (right_digit)
		{
		case 0:
		{
			s += '0';

			break;
		}
		case 1:
		{
			s += '1';

			break;
		}
		case 2:
		{
			s += '2';

			break;
		}
		case 3:
		{
			s += '3';
			break;
		}
		case 4:
		{
			s += '4';
			break;
		}
		case 5:
		{
			s += '5';
			break;
		}
		case 6:
		{
			s += '6';
			break;
		}
		case 7:
		{
			s += '7';
			break;
		}
		case 8:
		{
			s += '8';
			break;
		}
		case 9:
		{
			s += '9';
			break;
		}
		case 10:
		{
			s += 'A';
			break;
		}
		case 11:
		{
			s += 'B';
			break;
		}
		case 12:
		{
			s += 'C';
			break;
		}
		case 13:
		{
			s += 'D';
			break;
		}
		case 14:
		{
			s += 'E';
			break;
		}
		case 15:
		{
			s += 'F';
			break;
		}

		}
	}
	return s;
}
string hex_to_plain(string hex)
{
	string plain = "";
	unsigned char right_digit, left_digit, c;
	for (int i = 0; i < hex.size(); i++)
	{
		switch (hex[i])
		{
		case '0':
		{
			left_digit = 0;
			break;
		}
		case '1':
		{
			left_digit = 1;
			break;
		}
		case '2':
		{
			left_digit = 2;
			break;
		}
		case '3':
		{
			left_digit = 3;
			break;
		}
		case '4':
		{
			left_digit = 4;
			break;
		}
		case '5':
		{
			left_digit = 5;
			break;
		}
		case '6':
		{
			left_digit = 6;
			break;
		}
		case '7':
		{
			left_digit = 7;
			break;
		}
		case '8':
		{
			left_digit = 8;
			break;
		}
		case '9':
		{
			left_digit = 9;
			break;
		}
		case 'A':
		case 'a':
		{
			left_digit = 10;
			break;
		}
		case 'B':
		case 'b':
		{
			left_digit = 11;
			break;
		}
		case 'C':
		case 'c':
		{
			left_digit = 12;
			break;
		}
		case 'D':
		case 'd':
		{
			left_digit = 13;
			break;
		}
		case 'E':
		case 'e':
		{
			left_digit = 14;
			break;
		}
		case 'F':
		case 'f':
		{
			left_digit = 15;
			break;
		}

		}
		switch (hex[++i])
		{
		case '0':
		{
			right_digit = 0;
			break;
		}
		case '1':
		{
			right_digit = 1;
			break;
		}
		case '2':
		{
			right_digit = 2;
			break;
		}
		case '3':
		{
			right_digit = 3;
			break;
		}
		case '4':
		{
			right_digit = 4;
			break;
		}
		case '5':
		{
			right_digit = 5;
			break;
		}
		case '6':
		{
			right_digit = 6;
			break;
		}
		case '7':
		{
			right_digit = 7;
			break;
		}
		case '8':
		{
			right_digit = 8;
			break;
		}
		case '9':
		{
			right_digit = 9;
			break;
		}
		case 'A':
		case 'a':
		{
			right_digit = 10;
			break;
		}
		case 'B':
		case 'b':
		{
			right_digit = 11;
			break;
		}
		case 'C':
		case 'c':
		{
			right_digit = 12;
			break;
		}
		case 'D':
		case 'd':
		{
			right_digit = 13;
			break;
		}
		case 'E':
		case 'e':
		{
			right_digit = 14;
			break;
		}
		case 'F':
		case 'f':
		{
			right_digit = 15;
			break;
		}

		}
		c = left_digit * 16 + right_digit;
		plain += c;
	}
	return plain;
}
ll init_P(string hexatext)
{
	ll IP = 0;
	int x;
	unsigned long long temp = C_16To64Bit(hexatext);
	for (int i = 0; i < 64; i++)
	{
		x = GET_BIT((temp), (Initial_Permutation_t[i] - (ll)1));
		if (x)
		{
			IP |= ((ll)1 << i);
		}

		//	IP |=  GET_BIT((temp), (Initial_Permutation_t[i] - (ll)1)) << (ll)i;

	}
	return IP;
}
ll Permuted_C1(string hexatext)
{
	long long PC_1 = 0;
	unsigned long long temp = C_16To64Bit(hexatext);
	for (int i = 0; i < 56; i++)
	{
		PC_1 |= ((GET_BIT(temp, (ll)Permutation_Choice_1[i] - 1)) << (ll)i);

	}
	return PC_1;
}
ll Permuted_C2(ll LCS56bit)
{
	long long PC_2_bit48 = 0;
	for (int i = 0; i < 48; i++)
	{
		PC_2_bit48 |= ((GET_BIT(LCS56bit, (ll)Permutation_Choice_2[i] - 1)) << (ll)i);

	}
	return PC_2_bit48;
}
ll Expantion(unsigned int R)
{
	ll E = 0;
	for (int i = 0; i < 48; i++)
	{
		E |= ((GET_BIT(R, (ll)Expantion_t[i] - 1)) << (ll)i);
	}
	return E;
}
int  permutation(unsigned int S_box)
{
	int p = 0;
	for (int i = 0; i < 32; i++)
	{
		p |= ((GET_BIT(S_box, (ll)permutation_t[i] - 1)) << (ll)i);
	}
	return p;
}
int S_box(ll num)
{
	int bin = 0;
	int temp = 0;
	int x = 0;
	int t = 0;
	for (int i = 0; i < 8; i++)
	{

		if ((GET_BIT(num, (i * 6)) == 0) && (GET_BIT(num, ((i * 6) + 5)) == 0))
			t = 0;
		else if ((GET_BIT(num, (i * 6)) == 0) && (GET_BIT(num, ((i * 6) + 5)) == 1))
			t = 1;
		else if ((GET_BIT(num, (i * 6)) == 1) && (GET_BIT(num, ((i * 6) + 5)) == 0))
			t = 2;
		else if ((GET_BIT(num, (i * 6)) == 1) && (GET_BIT(num, ((i * 6) + 5)) == 1))
			t = 3;

		for (int j = 0; j < 4; j++)
		{
			x |= ((GET_BIT(num, ((i * 6)) + (j + 1))) << 3 - j);
		}

		temp = S[i][t][x];
		x = 0;

		switch (temp)
		{
		case 0: break;
		case 1:
		{
			bin |= ((ll)1 << i * (ll)4 + 3);
			break;
		}
		case 2:
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			break;
		}
		case 3:
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 3);
			break;
		}
		case 4:
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			break;
		}
		case 5:
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 3);
			break;
		}
		case 6:
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 2);
			break;
		}
		case 7:
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 3);
			break;
		}
		case 8:
		{
			bin |= ((ll)1 << (i * (ll)4));
			break;
		}
		case 9:
		{
			bin |= ((ll)1 << (i * (ll)4) + 3);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 10:
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 11:
		{
			bin |= ((ll)1 << (i * (ll)4) + 3);
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 12:
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 13:
		{
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 3);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 14:
		{
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}
		case 15:
		{
			bin |= ((ll)1 << (i * (ll)4) + 3);
			bin |= ((ll)1 << (i * (ll)4) + 2);
			bin |= ((ll)1 << (i * (ll)4) + 1);
			bin |= ((ll)1 << (i * (ll)4) + 0);
			break;
		}

		}

	}
	return bin;
}
void Round(ll k, unsigned int& R, unsigned int& L)
{
	int r = R;
	ll temp = 0;
	temp = Expantion(R);
	temp = temp ^ k;
	R = S_box(temp);
	R = permutation(R);
	R = R ^ L;
	L = r;
}
long LCS(unsigned int& d, unsigned int sh)
{
	int h = 0;
	for (int i = 0; i < 28; i++)
	{
		h |= GET_BIT(d, ((sh + i) % 28)) << i;
	}
	return h;
}
void split_key(ll k, unsigned int& D, unsigned int& C)
{
	for (int i = 0; i < 28; i++)
	{
		C |= GET_BIT(k, i) << i;
	}
	for (int i = 0; i < 28; i++)
	{
		D |= GET_BIT(k, (i + (ll)28)) << i;
	}
}
ll concatenate(unsigned int D, unsigned int C)
{
	ll k = 0;
	for (int i = 0; i < 28; i++)
	{
		k |= GET_BIT(C, i) << i;
	}
	for (int i = 0; i < 28; i++)
	{
		k |= GET_BIT(D, i) << (i + 28);
	}
	return k;
}
ll generate_key_i(unsigned int& D, unsigned int& C, unsigned int i)
{
	ll key_i = 0;
	D = LCS(D, num_leftShift[i - 1]);
	C = LCS(C, num_leftShift[i - 1]);
	key_i = concatenate(D, C);
	key_i = Permuted_C2(key_i);
	return key_i;

}
void  generate_16_key(unsigned int& D, unsigned int& C)
{
	ll key_i = 0;
	for (int i = 1; i < 17; i++)
	{
		key_i = 0;
		D = LCS(D, num_leftShift[i - 1]);
		C = LCS(C, num_leftShift[i - 1]);
		key_i = concatenate(D, C);
		key_i = Permuted_C2(key_i);
		keys[i - 1] = key_i;
	}

}
ll In_p(ll c)
{
	ll f = 0;
	for (int i = 0; i < 64; i++)
	{
		f |= ((GET_BIT(c, (ll)P_1[i] - 1)) << (ll)i);

	}
	return f;
}
string Bit_64to16C(ll cipher)
{
	string s = "";
	char c = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c |= ((GET_BIT(cipher, ((i * 4)) + j)) << 3 - j);
		}
		switch (c)
		{
		case 0:
		{
			s += '0';

			break;
		}
		case 1:
		{
			s += '1';

			break;
		}
		case 2:
		{
			s += '2';

			break;
		}
		case 3:
		{
			s += '3';
			break;
		}
		case 4:
		{
			s += '4';
			break;
		}
		case 5:
		{
			s += '5';
			break;
		}
		case 6:
		{
			s += '6';
			break;
		}
		case 7:
		{
			s += '7';
			break;
		}
		case 8:
		{
			s += '8';
			break;
		}
		case 9:
		{
			s += '9';
			break;
		}
		case 10:
		{
			s += 'A';
			break;
		}
		case 11:
		{
			s += 'B';
			break;
		}
		case 12:
		{
			s += 'C';
			break;
		}
		case 13:
		{
			s += 'D';
			break;
		}
		case 14:
		{
			s += 'E';
			break;
		}
		case 15:
		{
			s += 'F';
			break;
		}

		}
		c = 0;
	}
	return s;
}
string encryption(string hexatext, string key)
{
	ll IP = 0, out = 0;
	ll k = 0, key_i = 0;
	unsigned int D = 0, C = 0;
	unsigned int R = 0, L = 0;
	string cipher = "";

	IP = init_P(hexatext);
	int* ptr = (int*)&IP;
	L = *ptr;
	ptr = ptr + 1;
	R = *ptr;
	k = Permuted_C1(key);

	split_key(k, D, C);
	for (int i = 1; i < 17; i++)
	{
		key_i = generate_key_i(D, C, i);
		Round(key_i, R, L);

	}
	for (int i = 0; i < 32; i++)
	{
		out |= GET_BIT(R, i) << i;
	}
	for (int i = 0; i < 32; i++)
	{
		out |= GET_BIT(L, i) << (i + 32);
	}

	out = In_p(out);
	cipher = Bit_64to16C(out);
	return cipher;
}
string decryption(string hexaCipherText, string key)
{
	ll IP = 0, out = 0;
	ll k = 0, key_i = 0;
	unsigned int D = 0, C = 0;
	unsigned int R = 0, L = 0;
	string plain = "";
	IP = init_P(hexaCipherText);
	int* ptr = (int*)&IP;
	L = *ptr;
	ptr = ptr + 1;
	R = *ptr;
	k = Permuted_C1(key);
	split_key(k, D, C);
	generate_16_key(D, C);
	for (int i = 1; i < 17; i++)
	{
		Round(keys[16 - i], R, L);

	}
	for (int i = 0; i < 32; i++)
	{
		out |= GET_BIT(R, i) << i;
	}
	for (int i = 0; i < 32; i++)
	{
		out |= GET_BIT(L, i) << (i + 32);
	}

	out = In_p(out);
	plain = Bit_64to16C(out);
	return plain;

}

/*********************************************************************
 *                             MAIN()                                 *
 *********************************************************************/
 /*
  * main execution will be through Terminal
  * >des_g19.exe encrypt data.txt key.txt output.dat
  * >des_g19.exe decrypt data.dat key.txt output.txt
  * so argv will have 5 elements (argv[0] : argv[4])
  * argv[0] always = the program name
  * arg[1] = the opeation required
  */

int main(int argc, char** argv)

{
	/* Variables declaration */
	string input = "", key = "", result = "";
	ifstream f;
	ofstream p;
	unsigned char cnt = 0, choice = 0, read_char, count_key = 0;
	string s1 = argv[1], s2 = argv[2], s3 = argv[3], s4 = argv[4];

	/* get the key */
	f.open(s3);
	while (count_key != 16)
	{
		read_char = f.get();
		key += read_char;
		count_key++;
	}
	f.close();

	/* encrypt */
	if (s1 == "encrypt")
	{
		f.open(s2, ifstream::binary);
		p.open(s4, ifstream::binary);
		while (f)
		{
			read_char = f.get();
			input += read_char;
			cnt++;
			if (cnt == 8)
			{
				result = encryption(plain_to_hex(input), key);
				p << hex_to_plain(result);
				cnt = 0;
				input = "";
			}
		}
		f.close();
		p.close();
	}

	/* decrypt */
	else if (s1 == "decrypt")
	{
		f.open(s2, ifstream::binary);
		p.open(s4, ifstream::binary);
		int i = 0;
		while (f)
		{
			read_char = f.get();
			input += read_char;
			cnt++;
			if (cnt == 8)
			{
				result = decryption(plain_to_hex(input), key);
				p << hex_to_plain(result);
				cnt = 0;
				input = "";
			}
		}
		f.close();
		p.close();
	}

	/* wrong choice */
	else
	{
		printf("\nWrong Choice!!!\n");
	}

	return 0;
}
