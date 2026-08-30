#ifndef BANKSYSTEM_H // Prevents this file from being included twice
#define BANKSYSTEM_H

#include <iostream> 
#include <string>
#include <regex>  
#include <iomanip>
#include <fstream> 
#include <ctime>
#include <cstdlib>
#include <cstring> 

using namespace std; // So we don't have to type std:: everywhere

/* Simple Encryption Function
WHY XOR:
I used XOR as my method of encryption due to its simplicity in both
implementation and understanding.

HOW IT WORKS:
XOR  will flip each bit of input text based on the provided key.
Since XORing a number by itself results in the original number, if you
XOR your data by the same key again, you should end up back at the
starting point (i.e., the decrypted text). Thus, encrypt and decrypt are
one and the same function.

EXAMPLE:
'A' = 1000001, key 'K' = 1010011
1000001 ^ 1010011 = 0000100 (Encrypted)
0000100 ^ 1010011 = 1000001 ('A')*/

inline string hashData(string text, char key = 'K')
{
    string result = text; // Copy original text
    for (int i = 0; i < text.length(); i++)
    {
        result[i] = text[i] ^ key; // XOR each character with key
    }
    return result; // Return encrypted/decrypted string
}

struct Teller
{
    char tellerID[10];
    char fullName[50];
    char password[50];   
    char branchCode[10]; 


    void setTeller(string id, string name, string pass, string branch)
    {

        strncpy(tellerID, id.c_str(), 9);
        tellerID[9] = '\0';

        strncpy(fullName, name.c_str(), 49);
        fullName[49] = '\0';

        // Encrypt password BEFORE storing
        string hashed = hashData(pass);        // XOR encrypt the password
        strncpy(password, hashed.c_str(), 49);
        password[49] = '\0';                   

        strncpy(branchCode, branch.c_str(), 9); 
        branchCode[9] = '\0';                   
    }
};


struct Customer
{
    // --- Account identification ---
    char accountNo[20]; 
    char branchCode[10]; // Links customer to their home branch

    // --- Personal info ---
    char fullName[50];
    char saID[14]; // South African ID: 13 digits + null terminator
    char contact[11];
    char email[50];
    char address[100];
    char dob[11];

    int accountType; // 1=Savings, 2=Cheque, 3=Fixed Deposit, 4=Student
                    

    double balance; /* Account balance in Rands
                     double for decimal amounts like R500.50*/

    char pin[50]; 
};

#endif