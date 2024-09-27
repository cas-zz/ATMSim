#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const string ATM_CARD_FILENAME = "atm_card.txt";
const int MAX_ACCOUNTS = 100;

struct UserAccount {
    string accountNumber;
    string accountName;
    string birthday;
    string contactNumber;
    double balance;
    string pinCode;
};

UserAccount accounts[MAX_ACCOUNTS];
int numAccounts = 0;

void ShowMainMenu();
void RegisterAccount();
void LoadAccounts();
void SaveAccounts();
int FindAccount(const string& accountNumber);
void TransactionModule(UserAccount& account);
void BalanceInquiry(const UserAccount& account);
bool Withdraw(UserAccount& account, double amount);
bool Deposit(UserAccount& account, double amount);
bool FundTransfer(UserAccount& source, UserAccount& destination, double amount);
bool ChangePinCode(UserAccount& account);
bool IsUSBDriveInserted();
void ClearConsole();

int main() {
    if (!IsUSBDriveInserted()) {
        cout << "Please insert ATM Card to proceed." << endl;
        return 1;
    }

    LoadAccounts();

    char choice;
    do {
        ShowMainMenu();
        cin >> choice;

        switch (choice) {
            case '1':
                RegisterAccount();
                break;
            case '2':
                if (numAccounts > 0) {
                    string accountNumber;
                    cout << "Enter your account number: ";
                    cin >> accountNumber;

                    int accountIndex = FindAccount(accountNumber);

                    if (accountIndex != -1) {
                        TransactionModule(accounts[accountIndex]);
                    } else {
                        cout << "Account not found. Transaction Module failed." << endl;
                    }
                } else {
                    cout << "No registered accounts. Please register an account first." << endl;
                }
                break;
            case '0':
                SaveAccounts();
                cout << "Thank you for using the ATM simulation. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != '0');

    return 0;
}

void ShowMainMenu() {
    ClearConsole();
    cout << "ATM Simulation Menu:" << endl;
    cout << "Press 1. Register New Account" << endl;
    cout << "Press 2. Transaction Module" << endl;
    cout << "Press 0. Exit" << endl;
    cout << "Enter your choice: ";
}

void RegisterAccount() {
    if (numAccounts >= MAX_ACCOUNTS) {
        cout << "Maximum number of accounts reached. Cannot register a new account." << endl;
        return;
    }

    UserAccount newAccount;

    cout << "Account Number (5 digits): ";
    cin >> newAccount.accountNumber;
    while (newAccount.accountNumber.length() != 5) {
        cout << "Invalid account number. Please enter a 5-digit account number: ";
        cin >> newAccount.accountNumber;
    }

    cout << "Account Name: ";
    cin.ignore();
    getline(cin, newAccount.accountName);

    cout << "Birthday: ";
    getline(cin, newAccount.birthday);

    cout << "Contact number: ";
    getline(cin, newAccount.contactNumber);

    cout << "Initial Deposit (Min. 5000): $";
    cin >> newAccount.balance;
    while (newAccount.balance < 5000) {
        cout << "Initial Deposit must be at least 5000. Please enter a valid amount: $";
        cin >> newAccount.balance;
    }

    cout << "PIN Code (4 digits): ";
    cin >> newAccount.pinCode;
    while (newAccount.pinCode.length() != 4) {
        cout << "Invalid PIN Code. Please enter a 4-digit PIN: ";
        cin >> newAccount.pinCode;
    }

    accounts[numAccounts] = newAccount;
    numAccounts++;

    SaveAccounts();

    cout << "Account registration is complete. Thank you!" << endl;
}

void TransactionModule(UserAccount& account) {
    string enteredAccountNumber;
    string enteredPinCode;

    cout << "Enter your account number: ";
    cin >> enteredAccountNumber;

    int accountIndex = FindAccount(enteredAccountNumber);

    if (accountIndex != -1) {
        cout << "Enter your PIN code: ";
        cin >> enteredPinCode;

        if (enteredPinCode == accounts[accountIndex].pinCode) {
            ClearConsole();
            char choice;
            while (true) {
                cout << "Transaction Module Menu:" << endl;
                cout << "Press 1. Balance Inquiry" << endl;
                cout << "Press 2. Withdraw" << endl;
                cout << "Press 3. Deposit" << endl;
                cout << "Press 4. Fund Transfer" << endl;
                cout << "Press 5. Change PIN Code" << endl;
                cout << "Press 0. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case '1':
                        BalanceInquiry(account);
                        break;
                    case '2':
                        double withdrawAmount;
                        cout << "Enter the amount to withdraw: $";
                        cin >> withdrawAmount;
                        if (Withdraw(account, withdrawAmount)) {

                        }
                        break;
                    case '3':
                        double depositAmount;
                        cout << "Enter the amount to deposit: $";
                        cin >> depositAmount;
                        if (Deposit(account, depositAmount)) {

                        }
                        break;
                    case '4':
                        {
                            string destinationAccountNumber;
                            cout << "Enter the destination account number: ";
                            cin >> destinationAccountNumber;

                            int destinationIndex = FindAccount(destinationAccountNumber);

                            if (destinationIndex != -1) {
                                double transferAmount;
                                cout << "Enter the amount to transfer: $";
                                cin >> transferAmount;
                                if (FundTransfer(account, accounts[destinationIndex], transferAmount)) {

                                }
                            } else {
                                cout << "Destination account not found. Fund transfer failed." << endl;
                            }
                        }
                        break;
                    case '5':
                        ChangePinCode(account);
                        break;
                    case '0':
                        SaveAccounts();
                        cout << "Thank you for using the Transaction Module. Goodbye!" << endl;
                        return;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
            }
        } else {
            cout << "Incorrect PIN code. Transaction Module access denied." << endl;
        }
    } else {
        cout << "Account not found. Transaction Module failed." << endl;
    }
}

int FindAccount(const string& accountNumber) {
    for (int i = 0; i < numAccounts; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

void BalanceInquiry(const UserAccount& account) {
    ClearConsole();
    cout << "Account Number: " << account.accountNumber << endl;
    cout << "Account Name: " << account.accountName << endl;
    cout << "Current Balance: $" << account.balance << endl;
}

bool Withdraw(UserAccount& account, double amount) {
    if (amount > 0 && amount <= account.balance) {
        account.balance -= amount;
        return true;
    } else {
        cout << "Withdrawal failed. Please check the amount or balance." << endl;
        return false;
    }
}

bool Deposit(UserAccount& account, double amount) {
    if (amount > 0) {
        account.balance += amount;
        return true;
    } else {
        cout << "Deposit failed. Please check the amount." << endl;
        return false;
    }
}

bool FundTransfer(UserAccount& source, UserAccount& destination, double amount) {
    if (amount > 0 && amount <= source.balance) {
        if (Withdraw(source, amount)) {
            Deposit(destination, amount);
            return true;
        } else {
            cout << "Fund transfer failed. Please check the amount or balance." << endl;
            return false;
        }
    }
    cout << "Fund transfer failed. Please check the amount or balance." << endl;
    return false;
}


bool ChangePinCode(UserAccount& account) {
    ClearConsole();
    string newPin;
    cout << "Enter your new 4-digit PIN: ";
    cin >> newPin;
    if (newPin.length() == 4) {
        account.pinCode = newPin;
        SaveAccounts();
        cout << "PIN code changed successfully." << endl;
        return true;
    } else {
        cout << "Invalid PIN code. Please enter a valid 4-digit PIN." << endl;
        return false;
    }
}

void LoadAccounts() {
    ifstream file(ATM_CARD_FILENAME);
    if (file.is_open()) {
        file >> numAccounts;
        for (int i = 0; i < numAccounts; i++) {
            file >> accounts[i].accountNumber;
            file.ignore();
            getline(file, accounts[i].accountName);
            getline(file, accounts[i].birthday);
            getline(file, accounts[i].contactNumber);
            file >> accounts[i].balance;
            file >> accounts[i].pinCode;
        }
        file.close();
    }
}

void SaveAccounts() {
    ofstream file(ATM_CARD_FILENAME);
    if (file.is_open()) {
        file << numAccounts << endl;
        for (int i = 0; i < numAccounts; i++) {
            file << accounts[i].accountNumber << endl;
            file << accounts[i].accountName << endl;
            file << accounts[i].birthday << endl;
            file << accounts[i].contactNumber << endl;
            file << accounts[i].balance << endl;
            file << accounts[i].pinCode << endl;
        }
        file.close();
    }
}

bool IsUSBDriveInserted() {
    DWORD drives = GetLogicalDrives();

    for (int drive = 0; drive < 26; drive++) {
        if (drives & (1 << drive)) {
            char driveLetter = 'A' + drive;
            string rootPath = "\\\\.\\" + string(1, driveLetter) + ":";

            HANDLE hDrive = CreateFile(
                rootPath.c_str(),
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_FLAG_BACKUP_SEMANTICS,
                NULL
            );

            if (hDrive != INVALID_HANDLE_VALUE) {
                CloseHandle(hDrive);
                return true;
            }
        }
    }
    return false;
}

void ClearConsole() {
    system("cls");
}



