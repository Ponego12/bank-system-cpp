// I created this separate from the main system because i forgot add it to I/O code
#ifndef ACCOUNTCLASSES_H
#define ACCOUNTCLASSES_H

#include <iostream>
#include <string>
using namespace std;

//Virtual function
class Account
{
protected:
    string accountNumber;
    string fullName;
    double balance;
    string branchCode;

public:
    Account()
    {
        balance = 0.0;
    }

    Account(string acc, string name, double bal, string branch)
    {
        accountNumber = acc;
        fullName = name;
        balance = bal;
        branchCode = branch;
    }

    // Getters - standard practice to access private/protected fields
    string getAccountNumber() { return accountNumber; }
    string getFullName() { return fullName; }
    double getBalance() { return balance; }
    string getBranchCode() { return branchCode; }

    // Virtual function - each account type calculates interest differently
    virtual double calculateInterest()
    {
        return 0.0; // Base account has no interest by default
    }

    virtual string getTypeName()
    {
        return "Base Account";
    }

    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
        }
    }

    //withdraw with balance check
    bool withdraw(double amount)
    {
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            return true;
        }
        return false;
    }

    virtual void display()
    {
        cout << "Account: " << accountNumber << endl;
        cout << "Name: " << fullName << endl;
        cout << "Type: " << getTypeName() << endl;
        cout << "Balance: R" << balance << endl;
        cout << "Branch: " << branchCode << endl;
    }
};
// Savings accounts earn interest (4% annually).

class SavingsAccount : public Account
{
private:
    double interestRate; // Annual interest rate (e.g., 0.04 = 4%)

public:
    SavingsAccount() : Account()
    {
        interestRate = 0.04; // 4% default for savings
    }

    SavingsAccount(string acc, string name, double bal, string branch)
        : Account(acc, name, bal, branch)
    {
        interestRate = 0.04;
    }

    // Override: Savings accounts earn interest
    double calculateInterest() override
    {
        return balance * interestRate;
    }

    // Override: Return type name
    string getTypeName() override
    {
        return "Savings Account";
    }

    // Override: Display with interest info
    void display() override
    {
        Account::display(); // Call parent display first
        cout << "Interest Rate: " << (interestRate * 100) << "%" << endl;
        cout << "Annual Interest: R" << calculateInterest() << endl;
    }
};

// Cheqe accounts have little or no interest.

class ChequeAccount : public Account
{
private:
    double interestRate;

public:
    ChequeAccount() : Account()
    {
        interestRate = 0.01; // 1% for cheque accounts
    }

    ChequeAccount(string acc, string name, double bal, string branch)
        : Account(acc, name, bal, branch)
    {
        interestRate = 0.01;
    }

    double calculateInterest() override
    {
        return balance * interestRate;
    }

    string getTypeName() override
    {
        return "Cheque Account";
    }

    void display() override
    {
        Account::display();
        cout << "Interest Rate: " << (interestRate * 100) << "%" << endl;
        cout << "Annual Interest: R" << calculateInterest() << endl;
    }
};

// Derived class: FixedDepositAccount
// Fixed deposits have higher interest rates but typically lock funds for a period.
class FixedDepositAccount : public Account
{
private:
    double interestRate;
    int termMonths; // Duration of fixed deposit

public:
    FixedDepositAccount() : Account()
    {
        interestRate = 0.1; // 10% for fixed deposits
        termMonths = 12;    // 12 month default
    }

    FixedDepositAccount(string acc, string name, double bal, string branch)
        : Account(acc, name, bal, branch)
    {
        interestRate = 0.1;
        termMonths = 12;
    }

    double calculateInterest() override
    {
        // Fixed deposit: interest = principal * rate * (term/12)
        return balance * interestRate * (termMonths / 12.0);
    }

    string getTypeName() override
    {
        return "Fixed Deposit Account";
    }

    void display() override
    {
        Account::display();
        cout << "Interest Rate: " << (interestRate * 100) << "%" << endl;
        cout << "Term: " << termMonths << " months" << endl;
        cout << "Maturity Interest: R" << calculateInterest() << endl;
    }
};

// Student accounts have low/no minimum balance

class StudentAccount : public Account
{
private:
    double interestRate;

public:
    StudentAccount() : Account()
    {
        interestRate = 0.02; // 2% for students
    }

    StudentAccount(string acc, string name, double bal, string branch)
        : Account(acc, name, bal, branch)
    {
        interestRate = 0.02;
    }

    double calculateInterest() override
    {
        return balance * interestRate;
    }

    string getTypeName() override
    {
        return "Student Account";
    }

    void display() override
    {
        Account::display();
        cout << "Interest Rate: " << (interestRate * 100) << "%" << endl;
        cout << "Annual Interest: R" << calculateInterest() << endl;
    }
};


#endif // ACCOUNTCLASSES_H