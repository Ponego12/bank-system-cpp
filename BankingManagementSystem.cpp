#define _CRT_SECURE_NO_WARNINGS
#include "BankSystem.h"
#include "AccountClasses.h"
#include <ctime>

void seedTellers();
bool authenticateTeller(Teller &active);
void registerCustomer(Teller t);
void customerPortal();
void tellerMenu(Teller t);
void viewCustomerDetails();
void processTransaction();
void generateBranchReport(string branch);
bool isValidSAID(string id);
bool isValidContact(string num);
bool isValidEmail(string email);
bool isValidDOB(string dob);
double getMinDeposit(int type);

void demonstrateInheritance();
void exportToCSV();
void applyInterest();
void searchCustomer(); // search by name or ID
void backupData();
void restoreData(); // restore from backup
void compareBranches();
void logTransaction(string acc, string type, double amt);

// SA ID must be exactly 13 digits
bool isValidSAID(string id)
{
    return id.length() == 13 && id.find_first_not_of("0123456789") == string::npos;
}

// Phone numbers: exactly 10 digits, no letters or symbols
bool isValidContact(string num)
{
    return num.length() == 10 && num.find_first_not_of("0123456789") == string::npos;
}

// Email validation using regex took a while to get this pattern right
bool isValidEmail(string email)
{
    const regex p(R"((\w+)(\.{1}\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, p);
}

// DOB format check only (DD/MM/YYYY) it doesn't validate actual dates
bool isValidDOB(string dob)
{
    const regex p(R"(\d{2}/\d{2}/\d{4})");
    return regex_match(dob, p);
}

// Returns minimum deposit based on account type
// Savings: R500, Cheque: R100, Fixed: R5000, Student: R20
double getMinDeposit(int type)
{
    if (type == 1)
        return 500.0;
    if (type == 2)
        return 100.0;
    if (type == 3)
        return 5000.0;
    if (type == 4)
        return 20.0;
    return 0.0;
}

// Creates 4 default tellers if the tellers.dat file doesn't exist
// Runs once when program starts - prevents empty database errors
void seedTellers()
{
    ifstream checkFile("tellers.dat", ios::binary);
    if (!checkFile.is_open() || checkFile.peek() == EOF)
    {
        checkFile.close();
        ofstream outFile("tellers.dat", ios::binary);
        Teller t[4];
        t[0].setTeller("T001", "Bokang Tjadi", "pass123", "JHB01");
        t[1].setTeller("T002", "Dimpho Mabena", "pass456", "PLK02");
        t[2].setTeller("T003", "Charlie Branwyn", "pass789", "DBN03");
        t[3].setTeller("T004", "Dylan Naidoo", "pass004", "PTA04");
        for (int i = 0; i < 4; i++)
            outFile.write((char *)&t[i], sizeof(Teller));
        outFile.close();
    }
    else
        checkFile.close();
}

// Verifies teller credentials against stored data
// Returns true and populates active teller if successful
bool authenticateTeller(Teller &active)
{
    string id, pass;
    cout << "\n--- Teller Login ---\nID: ";
    cin >> id;
    cout << "Password: ";
    cin >> pass;
    string hashed = hashData(pass);
    ifstream file("tellers.dat", ios::binary);
    Teller t;
    while (file.read((char *)&t, sizeof(t)))
    {
        if (string(t.tellerID) == id && string(t.password) == hashed)
        {
            active = t;
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Registers a new customer and the teller's branch code is automatically linked
// Generates random account number and temporary PIN
void registerCustomer(Teller t)
{
    Customer c;
    string input;
    srand((unsigned)time(0));

    cout << "\n--- Register A New Customer ---\nName: ";
    cin.ignore();
    cin.getline(c.fullName, 50);

    // SA ID validation loop
    while (true)
    {
        cout << "SA ID (13 digits): ";
        cin >> input;
        if (isValidSAID(input))
        {
            strncpy(c.saID, input.c_str(), 13);
            c.saID[13] = '\0';
            break;
        }
        cout << "Invalid ID !.\n";
    }

    // Contact number validation
    while (true)
    {
        cout << "Contact (10 digits): ";
        cin >> input;
        if (isValidContact(input))
        {
            strncpy(c.contact, input.c_str(), 10);
            c.contact[10] = '\0';
            break;
        }
        cout << "Invalid Contact!.\n";
    }

    // Email validation regex handles most common formats
    while (true)
    {
        cout << "Email: ";
        cin >> input;
        if (isValidEmail(input))
        {
            strncpy(c.email, input.c_str(), 49);
            c.email[49] = '\0';
            break;
        }
        cout << "Invalid Email!.\n";
    }

    cout << "Address: ";
    cin.ignore();
    cin.getline(c.address, 99);

    // DOB format check
    while (true)
    {
        cout << "DATE OF BIRTH (DD/MM/YYYY): ";
        cin >> input;
        if (isValidDOB(input))
        {
            strncpy(c.dob, input.c_str(), 10);
            c.dob[10] = '\0';
            break;
        }
        cout << "Invalid Date.\n";
    }

    // Account type selection with minimum deposit enforcement
    while (true)
    {
        cout << "Type (1:Savings ACC, 2:Cheque ACC, 3:Fixed ACC, 4:Student ACC): ";
        cin >> c.accountType;
        double min = getMinDeposit(c.accountType);
        cout << "Deposit (Min R" << min << "): ";
        cin >> c.balance;
        if (c.balance >= min)
            break;
        cout << "Deposit is too low.\n";
    }

    // Generates unique account number: ACC-BRANCH-XXXXX
    string acc = "ACC-" + string(t.branchCode) + "-" + to_string(rand() % 90000 + 10000);
    strncpy(c.accountNo, acc.c_str(), 19);
    c.accountNo[19] = '\0';
    strncpy(c.branchCode, t.branchCode, 9);
    c.branchCode[9] = '\0';

    // Generates a 5-digit PIN and store hashed version
    string tempPIN = to_string(rand() % 90000 + 10000);
    string hashedPIN = hashData(tempPIN);
    strncpy(c.pin, hashedPIN.c_str(), 49);
    c.pin[49] = '\0';

    // Adds new customer to binary file
    ofstream outFile("customers.dat", ios::binary | ios::app);
    outFile.write((char *)&c, sizeof(c));
    outFile.close();

    // Show teller the temp PIN - customer must change it on first login
    cout << "\nAccount Created: " << c.accountNo << "\nTEMP PIN: " << tempPIN << endl;
}

// Displays full details for a specific account number
void viewCustomerDetails()
{
    string search;
    cout << "\nEnter Acc No: ";
    cin >> search;
    ifstream file("customers.dat", ios::binary);
    Customer c;
    bool found = false;
    while (file.read((char *)&c, sizeof(c)))
    {
        if (string(c.accountNo) == search)
        {
            cout << "\nName: " << c.fullName
                 << "\nBalance: R" << fixed << setprecision(2) << c.balance
                 << "\nID: " << c.saID
                 << "\nContact: " << c.contact
                 << "\nEmail: " << c.email
                 << "\nAddress: " << c.address
                 << "\nDOB: " << c.dob
                 << "\nType: " << c.accountType
                 << "\nBranch: " << c.branchCode << endl;
            found = true;
            break;
        }
    }
    if (!found)
        cout << "Account not found.\n";
    file.close();
}

// Search by name (partial match) or exact SA ID
// Much more flexible than searching by account number only
void searchCustomer()
{
    string keyword;
    cout << "\nEnter name or SA ID to search: ";
    cin.ignore();
    getline(cin, keyword);

    ifstream file("customers.dat", ios::binary);
    Customer c;
    bool found = false;

    while (file.read((char *)&c, sizeof(c)))
    {
        // Checks if keyword appears anywhere in name or matches ID exactly
        if (string(c.fullName).find(keyword) != string::npos || string(c.saID) == keyword)
        {
            cout << "\nFound: " << c.fullName
                 << " | " << c.accountNo
                 << " | R" << fixed << setprecision(2) << c.balance << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No matches found.\n";

    file.close();
}

// Appends to transactions.dat with timestamp
void logTransaction(string acc, string type, double amt)
{
    ofstream log("transactions.dat", ios::app);
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Format: YYYY-MM-DD , Account , Type, Amount
    log << 1900 + ltm->tm_year << "-"
        << 1 + ltm->tm_mon << "-"
        << ltm->tm_mday << " | "
        << acc << " | "
        << type << " | R"
        << fixed << setprecision(2) << amt << endl;

    log.close();
}

// Handles deposits, withdrawals, transfers, and statements
// PIN verification required before any transaction
void processTransaction()
{
    string search, pin;
    int transType;
    double amt;

    cout << "\nEnter Acc No: ";
    cin >> search;

    cout << "Enter Customer PIN: ";
    cin >> pin;
    string hashed = hashData(pin);

    // Verify PIN before proceeding and prevents unauthorized access
    ifstream verify("customers.dat", ios::binary);
    Customer check;
    bool pinValid = false;
    while (verify.read((char *)&check, sizeof(check)))
    {
        if (string(check.accountNo) == search && string(check.pin) == hashed)
        {
            pinValid = true;
            break;
        }
    }
    verify.close();

    if (!pinValid)
    {
        cout << "Invalid PIN! Transaction cancelled.\n";
        return;
    }

    cout << "\n--- TRANSACTION MENU ---\n";
    cout << "1. Deposit \n";
    cout << "2. Withdraw \n";
    cout << "3. Transfer\n";
    cout << "4. View Account Statement\n";
    cout << "Choice: ";
    cin >> transType;

    // DEPOSIT
    if (transType == 1)
    {
        cout << "Amount: ";
        cin >> amt;
        if (amt <= 0)
        {
            cout << "Invalid amount!\n";
            return;
        }

        fstream file("customers.dat", ios::binary | ios::in | ios::out);
        Customer c;
        while (file.read((char *)&c, sizeof(c)))
        {
            if (string(c.accountNo) == search)
            {
                c.balance += amt;
                file.seekp((int)file.tellg() - (int)sizeof(c));
                file.write((char *)&c, sizeof(c));
                cout << "Deposit successful. New balance: R" << fixed << setprecision(2) << c.balance << endl;
                file.close();

                logTransaction(search, "Deposit", amt);
                return;
            }
        }
        file.close();
    }
    // WITHDRAWAL
    else if (transType == 2)
    {
        cout << "Amount: ";
        cin >> amt;
        if (amt <= 0)
        {
            cout << "Invalid amount!\n";
            return;
        }

        fstream file("customers.dat", ios::binary | ios::in | ios::out);
        Customer c;
        while (file.read((char *)&c, sizeof(c)))
        {
            if (string(c.accountNo) == search)
            {
                if (c.balance < amt)
                {
                    cout << "Insufficient funds!\n";
                    file.close();
                    return;
                }
                c.balance -= amt;
                file.seekp((int)file.tellg() - (int)sizeof(c));
                file.write((char *)&c, sizeof(c));
                cout << "Withdrawal successful. New balance: R" << fixed << setprecision(2) << c.balance << endl;
                file.close();

                logTransaction(search, "Withdrawal", amt);
                return;
            }
        }
        file.close();
    }
    // TRANSFER
    else if (transType == 3)
    {
        string destAcc;
        cout << "Enter destination account number: ";
        cin >> destAcc;

        // Verify destination exists before attempting transfer
        ifstream checkDest("customers.dat", ios::binary);
        Customer dest;
        bool destFound = false;
        while (checkDest.read((char *)&dest, sizeof(dest)))
        {
            if (string(dest.accountNo) == destAcc)
            {
                destFound = true;
                break;
            }
        }
        checkDest.close();

        if (!destFound)
        {
            cout << "Destination account not found!\n";
            return;
        }

        cout << "Amount to transfer: ";
        cin >> amt;
        if (amt <= 0)
        {
            cout << "Invalid amount!\n";
            return;
        }

        // Deduct from source
        fstream file("customers.dat", ios::binary | ios::in | ios::out);
        Customer c;
        while (file.read((char *)&c, sizeof(c)))
        {
            if (string(c.accountNo) == search)
            {
                if (c.balance < amt)
                {
                    cout << "Insufficient funds!\n";
                    file.close();
                    return;
                }
                c.balance -= amt;
                file.seekp((int)file.tellg() - (int)sizeof(c));
                file.write((char *)&c, sizeof(c));
                file.close();
                break;
            }
        }

        // Add to destination, got this working after some debugging
        fstream destFile("customers.dat", ios::binary | ios::in | ios::out);
        while (destFile.read((char *)&dest, sizeof(dest)))
        {
            if (string(dest.accountNo) == destAcc)
            {
                dest.balance += amt;
                destFile.seekp((int)destFile.tellg() - (int)sizeof(dest));
                destFile.write((char *)&dest, sizeof(dest));
                destFile.close();

                logTransaction(search, "Transfer to " + destAcc, amt);
                cout << "Transfer successful! R" << fixed << setprecision(2) << amt << " sent to " << destAcc << endl;
                return;
            }
        }
        destFile.close();
    }
    // VIEW STATEMENT
    else if (transType == 4)
    {
        ifstream file("customers.dat", ios::binary);
        Customer c;
        while (file.read((char *)&c, sizeof(c)))
        {
            if (string(c.accountNo) == search)
            {
                cout << "\n--- ACCOUNT STATEMENT ---\n";
                cout << "Account: " << c.accountNo << endl;
                cout << "Name: " << c.fullName << endl;
                cout << "Type: " << c.accountType << endl;
                cout << "Balance: R" << fixed << setprecision(2) << c.balance << endl;
                cout << "Branch: " << c.branchCode << endl;
                cout << "Contact: " << c.contact << endl;
                cout << "Email: " << c.email << endl;
                file.close();
                return;
            }
        }
        file.close();
    }
    else
    {
        cout << "Invalid choice!\n";
    }
}

// Generates report showing all customers in a specific branch
void generateBranchReport(string branch)
{
    ifstream file("customers.dat", ios::binary);
    Customer c;
    cout << "\n--- REPORT: " << branch << " ---\n";
    double total = 0;
    int count = 0;
    while (file.read((char *)&c, sizeof(c)))
    {
        if (string(c.branchCode) == branch)
        {
            cout << c.fullName << " | " << c.accountNo << " | R" << fixed << setprecision(2) << c.balance << endl;
            total += c.balance;
            count++;
        }
    }
    cout << "----------------------\n";
    cout << "Total Accounts: " << count << endl;
    cout << "Total Balance: R" << fixed << setprecision(2) << total << endl;
    file.close();
}

// Compares all branches side by side
// Useful for management to see which branches perform best
void compareBranches()
{
    string branches[] = {"JHB01", "PLK02", "DBN03", "PTA04"};
    int numBranches = 4;

    cout << "\n--- BRANCH COMPARISON --\n";

    for (int b = 0; b < numBranches; b++)
    {
        ifstream file("customers.dat", ios::binary);
        Customer c;
        double total = 0;
        int count = 0;

        while (file.read((char *)&c, sizeof(c)))
        {
            if (string(c.branchCode) == branches[b])
            {
                total += c.balance;
                count++;
            }
        }

        cout << branches[b] << ": " << count << " accounts, R" << fixed << setprecision(2) << total << endl;
        file.close();
    }
}

// Main teller interface its displayed after successful login
void tellerMenu(Teller t)
{
    int choice;
    while (true)
    {
        cout << "\n--- TELLER MENU (" << t.fullName << ") ----";
        cout << "\n1. Register New Customer Account";
        cout << "\n2. View Customer Details";
        cout << "\n3. Process Transaction";
        cout << "\n4. Generate Branch Report";
        cout << "\n5. Search Customer";
        cout << "\n6. Logout";
        cout << "\nChoice: ";
        cin >> choice;

        if (choice == 1)
            registerCustomer(t);
        else if (choice == 2)
            viewCustomerDetails();
        else if (choice == 3)
            processTransaction();
        else if (choice == 4)
            generateBranchReport(t.branchCode);
        else if (choice == 5)
            searchCustomer();
        else if (choice == 6)
            break;
        else
            cout << "Invalid choice!\n";
    }
}

// Customer self-service login and banking interface
// 3 maximum attempts for security
void customerPortal()
{
    string acc, pin;
    int attempts = 0;
    Customer loggedIn;
    bool found = false;

    // Login loop with attempt tracking
    while (attempts < 3)
    {
        cout << "\n--- Customer Login ---\nAcc No: ";
        cin >> acc;
        cout << "PIN: ";
        cin >> pin;
        string hashed = hashData(pin);
        ifstream file("customers.dat", ios::binary);
        Customer c;
        while (file.read((char *)&c, sizeof(c)))
        {
            if (string(c.accountNo) == acc && string(c.pin) == hashed)
            {
                cout << "\nWelcome " << c.fullName << "!\n";
                loggedIn = c;
                found = true;
                file.close();
                break;
            }
        }
        file.close();

        if (found)
            break;

        attempts++;
        cout << "Failed. Left: " << (3 - attempts) << endl;
    }

    if (!found)
    {
        cout << "Account Locked.\n";
        return;
    }

    int choice;
    do
    {
        cout << "\n--- CUSTOMER MENU ---\n";
        cout << "1. View Balance\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. View Statement\n";
        cout << "5. Change PIN\n";
        cout << "6. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "\nCurrent Balance: R" << fixed << setprecision(2) << loggedIn.balance << endl;
        }
        else if (choice == 2)
        {
            double amt;
            cout << "Enter amount to deposit: R";
            cin >> amt;
            if (amt > 0)
            {
                fstream file("customers.dat", ios::binary | ios::in | ios::out);
                Customer c;
                while (file.read((char *)&c, sizeof(c)))
                {
                    if (string(c.accountNo) == string(loggedIn.accountNo))
                    {
                        c.balance += amt;
                        loggedIn.balance = c.balance;
                        file.seekp((int)file.tellg() - (int)sizeof(c));
                        file.write((char *)&c, sizeof(c));
                        cout << "Deposited R" << fixed << setprecision(2) << amt
                             << ". New balance: R" << c.balance << endl;
                        file.close();
                        break;
                    }
                }
            }
            else
            {
                cout << "Invalid amount!\n";
            }
        }
        else if (choice == 3)
        {
            double amt;
            cout << "Enter the amount to withdraw: R";
            cin >> amt;
            if (amt > 0 && amt <= loggedIn.balance)
            {
                fstream file("customers.dat", ios::binary | ios::in | ios::out);
                Customer c;
                while (file.read((char *)&c, sizeof(c)))
                {
                    if (string(c.accountNo) == string(loggedIn.accountNo))
                    {
                        c.balance -= amt;
                        loggedIn.balance = c.balance;
                        file.seekp((int)file.tellg() - (int)sizeof(c));
                        file.write((char *)&c, sizeof(c));
                        cout << "Withdrawn R" << fixed << setprecision(2) << amt
                             << ". New balance: R" << c.balance << endl;
                        file.close();
                        break;
                    }
                }
            }
            else
            {
                cout << "Invalid amount or insufficient funds!\n";
            }
        }
        else if (choice == 4)
        {
            cout << "\n--- ACCOUNT STATEMENT ---\n";
            cout << "Account: " << loggedIn.accountNo << endl;
            cout << "Name: " << loggedIn.fullName << endl;
            cout << "Type: " << loggedIn.accountType << endl;
            cout << "Balance: R" << fixed << setprecision(2) << loggedIn.balance << endl;
            cout << "Branch: " << loggedIn.branchCode << endl;
            cout << "Contact: " << loggedIn.contact << endl;
            cout << "Email: " << loggedIn.email << endl;
        }
        else if (choice == 5)
        {
            string currentPin, newPin, confirmPin;
            cout << "Enter current PIN: ";
            cin >> currentPin;
            if (hashData(currentPin) != string(loggedIn.pin))
            {
                cout << "Incorrect current PIN!\n";
                continue;
            }
            cout << "Enter new PIN (5 digits): ";
            cin >> newPin;
            cout << "Confirm new PIN: ";
            cin >> confirmPin;
            if (newPin == confirmPin && newPin.length() == 5)
            {
                fstream file("customers.dat", ios::binary | ios::in | ios::out);
                Customer c;
                while (file.read((char *)&c, sizeof(c)))
                {
                    if (string(c.accountNo) == string(loggedIn.accountNo))
                    {
                        string hashedNew = hashData(newPin);
                        strncpy(c.pin, hashedNew.c_str(), 49);
                        strncpy(loggedIn.pin, hashedNew.c_str(), 49);
                        file.seekp((int)file.tellg() - (int)sizeof(c));
                        file.write((char *)&c, sizeof(c));
                        cout << "PIN changed successfully!\n";
                        file.close();
                        break;
                    }
                }
            }
            else
            {
                cout << "PINs don't match or not 5 digits!\n";
            }
        }
        else if (choice == 6)
        {
            cout << "Thank You for Banking with us!\n";
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    } while (choice != 6);
}

// Creates backup copies of all data files
// Run before major operations to prevent data loss
void backupData()
{
    // Backup customers.dat
    ifstream src1("customers.dat", ios::binary);
    ofstream dst1("customers.dat.bak", ios::binary);
    dst1 << src1.rdbuf();
    src1.close();
    dst1.close();

    // Backup tellers.dat as well - felt thorough
    ifstream src2("tellers.dat", ios::binary);
    ofstream dst2("tellers.dat.bak", ios::binary);
    dst2 << src2.rdbuf();
    src2.close();
    dst2.close();

    cout << "\nBackup created: customers.dat.bak, tellers.dat.bak\n";
}

// Restores from backup files
// Useful if something corrupts the main data files
void restoreData()
{
    // Restore customers.dat from backup
    ifstream src1("customers.dat.bak", ios::binary);
    ofstream dst1("customers.dat", ios::binary);
    dst1 << src1.rdbuf();
    src1.close();
    dst1.close();

    // Restore tellers.dat from backup
    ifstream src2("tellers.dat.bak", ios::binary);
    ofstream dst2("tellers.dat", ios::binary);
    dst2 << src2.rdbuf();
    src2.close();
    dst2.close();

    cout << "\nRestored from backup.\n";
}

// Demonstrates inheritance using all four account types
void demonstrateInheritance()
{
    cout << "\n---INHERITANCE DEMONSTRATION ---\n";
    cout << "Creating sample accounts to show polymorphism...\n\n";

    Account *accounts[4];

    accounts[0] = new SavingsAccount("ACC-JHB01-11111", "Sthe Zulu", 50000.0, "JHB01");
    accounts[1] = new ChequeAccount("ACC-PLK02-22222", "Jane Nzimande", 2500.0, "PLK02");
    accounts[2] = new FixedDepositAccount("ACC-DBN03-33333", "Bobby Brown", 10000.0, "DBN03");
    accounts[3] = new StudentAccount("ACC-PTA04-44444", "Alice Wonderland", 800.0, "PTA04");

    for (int i = 0; i < 4; i++)
    {
        cout << "--- Account " << (i + 1) << " ---\n";
        accounts[i]->display();
        cout << "\n";
    }

    for (int i = 0; i < 4; i++)
    {
        delete accounts[i];
    }

    cout << "---END DEMONSTRATION ---\n";
}

// Exports all customer data to CSV format for excel analysis
void exportToCSV()
{
    ifstream inFile("customers.dat", ios::binary);
    ofstream outFile("customers_export.csv");

    if (!inFile.is_open())
    {
        cout << "No customer data found!\n";
        return;
    }

    outFile << "AccountNumber,FullName,SA_ID,Contact,Email,Address,DOB,AccountType,Balance,BranchCode\n";

    Customer c;
    while (inFile.read((char *)&c, sizeof(c)))
    {
        outFile << c.accountNo << ","
                << "\"" << c.fullName << "\","
                << c.saID << ","
                << c.contact << ","
                << "\"" << c.email << "\","
                << "\"" << c.address << "\","
                << c.dob << ","
                << c.accountType << ","
                << fixed << setprecision(2) << c.balance << ","
                << c.branchCode << "\n";
    }

    inFile.close();
    outFile.close();

    cout << "\nExport Done, File saved as: customers_export.csv\n";
    cout << "You can open this file in Excel.\n";
}

// Applies interest to all accounts based on their type
// Rates Savings 3%, Cheque 1%, Fixed Deposit 8%, Student 2%
void applyInterest()
{
    fstream file("customers.dat", ios::binary | ios::in | ios::out);

    if (!file.is_open())
    {
        cout << "No customer data found!\n";
        return;
    }

    cout << "\n---APPLYING INTEREST ---\n";

    Customer c;
    double totalInterest = 0;
    int count = 0;

    while (file.read((char *)&c, sizeof(c)))
    {
        double rate = 0.0;
        string typeName;

        if (c.accountType == 1)
        {
            rate = 0.03;
            typeName = "Savings";
        }
        else if (c.accountType == 2)
        {
            rate = 0.01;
            typeName = "Cheque";
        }
        else if (c.accountType == 3)
        {
            rate = 0.08;
            typeName = "Fixed Deposit";
        }
        else if (c.accountType == 4)
        {
            rate = 0.02;
            typeName = "Student";
        }

        double interest = c.balance * rate;
        c.balance += interest;
        totalInterest += interest;
        count++;

        file.seekp((int)file.tellg() - (int)sizeof(c));
        file.write((char *)&c, sizeof(c));

        cout << c.fullName << " (" << typeName << "): R"
             << fixed << setprecision(2) << interest << " interest applied\n";
    }

    file.close();

    cout << "\nTotal accounts updated: " << count << endl;
    cout << "Total interest applied: R" << fixed << setprecision(2) << totalInterest << endl;
    cout << "--- INTEREST APPLICATION COMPLETE ---\n";
}

// Program entry point -initializes data and displays main menu
int main()
{
    seedTellers();

    int choice;
    Teller active;

    while (true)
    {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Teller Portal\n";
        cout << "2. Customer Portal\n";
        cout << "3. View Branches\n";
        cout << "4. Demonstrate Inheritance\n";
        cout << "5. Export to CSV\n";
        cout << "6. Apply Interest\n";
        cout << "7. Compare Branches\n"; // 1.4.2
        cout << "8. Backup Data\n";      // 1.7.1
        cout << "9. Restore Data\n";     // 1.7.1
        cout << "10. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            if (authenticateTeller(active))
                tellerMenu(active);
            else
                cout << "Login failed.\n";
        }
        else if (choice == 2)
            customerPortal();
        else if (choice == 3)
        {
            cout << "\n--- BRANCHES ---\n";
            cout << "JHB01 - Johannesburg \n";
            cout << "PLK02 - Polokwane \n";
            cout << "DBN03 - Durban \n";
            cout << "PTA04 - Pretoria \n";
        }
        else if (choice == 4)
        {
            demonstrateInheritance();
        }
        else if (choice == 5)
        {
            exportToCSV();
        }
        else if (choice == 6)
        {
            applyInterest();
        }
        else if (choice == 7)
        {
            compareBranches();
        }
        else if (choice == 8)
        {
            backupData();
        }
        else if (choice == 9)
        {
            restoreData();
        }
        else if (choice == 10)
        {
            cout << "Thank you for using Felix Bank System!\n";
            return 0;
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    }
}