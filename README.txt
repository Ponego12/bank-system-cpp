Felix Bank Management System 

----How to compile and run----
1.Open terminal within the the project 
2.Compile g++ BankingManagementSystem.cpp -o bank.exe
3.Run .\bank.exe

----The Required Files----
Banksystem.h - header file that contains the definations for each type of struct
Accountclasses.h- classes used for inheritence 
Bankingmanagementsystem.cpp -the source code for the main program 

---Data files create---
teller.dat -contains tellers credentials
customers.dat -contains customer account information
transaction.dat- contains LOG file for all transactions
customers_export.csv - csv export generated when you use export to csv option
Customer.dat.bak- backup copy of customer data
tellers.dat.bak -backup copy of tellers data

---Default Teller login credentials---
T001 - pass123 - Bokang Tjadi , Johannesburg
T002 - pass456 - Dimpho mbena , Polokwane
T002 - pass789 - Charlie Branwyn, Durban
T004 - pass004 - Dylan Naidoo, Pretoria

--How to add new customer Account---
1. from main menu select 1.teller portal 
2. LOG-in using one of the above credentials. 
3. Select “1. Register New Customer Account”. 
4. Fill in all necessary information about the new customer: 
   full name 
   sa id (#13 digits) 
   Contact number (#10 digits) 
   e-mail address 
   physical address 
   Date of birth (dd/MM/yyyy) 
   type of account (*1-4*) 
   Initial Deposit 
5. write-down the Account Number and TEMP PIN that appear on screen. 
6. pass the pin number safely to the new customer

--How customers log in---
1. From main menu select “2. Customer portal”. 
2. Input the account number (format: acc-branch-xxxxx) 
3. Input the 5 digit pin # 
4. Must enter correct login information within 3 attempts or account will be locked out

---How customers can make transactions---
Once the customer has logged into thri account the cutomers menu provides them with options to perform 
transactions such as 
1. View balance – displays the current balance of the customer's account. 
2. Deposit – adds money to the customer's account.
3._withdraw – removes money from the customer's account if they have enough funds. 
4. View statement – displays detailed information of the customer's account. 
5. Change pin – allows the customer to update their pin number. They must provide their existing pin first.
6.LOG out - returns to main menu additional features

---Main menu options ---
3.view branches – lists all banks branches. 
4.demonstrate inheritance – displays examples of how object oriented programming (oop) was implemented in this assignment using different types of ACCOUNTS. 
5.export to CSV - saves the CUSTOMERS data to an Excel format file. 
6.apply interest - calculates interest and adds it to the account holders balance. 
7.compare branches - provides side-by-side statistics comparing two banks branches. 
8.backup data - creates backup copies of binary files (.dat) which cannot be viewed by humans. 
9.restore data - restores the data from backup files created in step 8. 
10.exit – exits program completely. 

--TELLER Menu--- Options: 
5.search customer - searches for a customer based on either full name or sa id interest

--Rates----
savings account: 3% 
cheque account: 1% 
fixed deposit: 8% 
student account: 2% 

---Notes ---
All passwords and pins were encrypted using XOR hashing method.
An automatic sequential numbering system is applied to create unique account numbers formatted as acc-branch-xxxx. 
Binary files (.dat) may not be readable by humans. 
A CSV export file can be read by any spreadsheet software including microsoft Excel

