#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;
#define LOGINFILE "loginInfo.txt"
#define ACCOUNTFILE "accountInfo.txt"
#define MENU "\t\t1-WITHDRAW (PARA ÇEK) \n\t\t2-DEPOSIT(PARA YATIR)\n\t\t3-VIEW BALANCE\n\t\t4-CHANGE PASSWORD\n\t\t5-EXIT\n"
#define MAX_WITHDRAWAL_LIMIT 5000
int loginF();
void readTxtFile();
void printAll();
int findBalance(string);
void saveFile(int);
void menu();
void start();
void cls();

void withdraw(int);
void deposit(int);
void viewBalance(int);
void changePassword(int);

struct loginInfo
{
    string accountNumber;
    string userName;
    string password;
    int errorRate;
};
vector<loginInfo> login;

struct accountInfo
{
    string accountNumber;
    int balance;
};
vector<accountInfo> account;

int main()
{
    start();
    return 0;
}

void start()
{
    readTxtFile();
    cout << "\tWELCOME TO X BANK  !" << endl;
    menu();
}

void menu()
{
    int selectedUser;
    int choose;
    selectedUser = loginF();
    do
    {
        cls();
        cout << "\tWELCOME TO X BANK  !" << endl;
        if (selectedUser != -1)
        {
            cout << "\tHi " << login.at(selectedUser).userName
                 << endl
                 << "\tYour Account Number : "
                 << login.at(selectedUser).accountNumber
                 << endl;
        }

        cout << MENU;
        cout << "What is the action you want to do? : ";
        cin >> choose;

        switch (choose)
        {
        case 1:
            withdraw(selectedUser);
            break;
        case 2:
            deposit(selectedUser);
            break;
        case 3:
            viewBalance(selectedUser);
            break;
        case 4:
            changePassword(selectedUser);
            break;
        case 5:
            cls();
            cout << "\tEXITED! WAITING FOR YOU." << endl;
            break;
        default:
            break;
        }
    } while (choose != 5);
}
void withdraw(int selectedUser)
{
    cls();
    int amount;
    cout << "\tHi " << login.at(selectedUser).userName
         << endl
         << "\tYour Balance : "
         << account.at(findBalance(login.at(selectedUser).accountNumber)).balance
         << " TL "
         << endl;
    cout << "\tEnter the amount of money you want to withdraw: ";
    cin >> amount;
    if (amount <= static_cast<int>(MAX_WITHDRAWAL_LIMIT) && amount <= account.at(findBalance(login.at(selectedUser).accountNumber)).balance)
    {
        account.at(findBalance(login.at(selectedUser).accountNumber)).balance -= amount;
        saveFile(2);
        cout << "\tWithdrawal completed successfully." << endl;
    }
    else
        cout << "\tWithdrawal failed!" << endl;
    cout << "Press any key to go to the main menu.";
    string a;
    cin >> a;
}

void deposit(int selectedUser)
{
    cls();
    int amount;
    cout << "\tHi " << login.at(selectedUser).userName
         << endl
         << "\tYour Balance : "
         << account.at(findBalance(login.at(selectedUser).accountNumber)).balance
         << " TL "
         << endl;
    cout << "\tEnter the amount of money you want to deposit: ";
    cin >> amount;
    if (amount <= static_cast<int>(MAX_WITHDRAWAL_LIMIT))
    {
        account.at(findBalance(login.at(selectedUser).accountNumber)).balance += amount;
        saveFile(2);
        cout << "\tYour deposit has been successfully completed." << endl;
    }
    else
        cout << "\tDeposit failed!" << endl;
    cout << "Press any key to go to the main menu.";
    string a;
    cin >> a;
}

void viewBalance(int selectedUser)
{
    cls();
    cout << "\tHi " << login.at(selectedUser).userName
         << endl
         << "\tYou are viewing your account balance."
         << endl
         << "\tYour Account Number : "
         << login.at(selectedUser).accountNumber
         << endl
         << "\tYour Balance : "
         << account.at(findBalance(login.at(selectedUser).accountNumber)).balance
         << " TL "
         << endl;
    cout << "Press any key to go to the main menu.";
    string a;
    //cin >> a;
    //getline(std::cin, a);
    //cin.get();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void changePassword(int selectedUser)
{
    cls();
    int ok = 0;
    string oldPassword, newPassword, newPassword2;
    while (!ok)
    {
        cout << "\tEnter your old password:" << endl;
        cin >> oldPassword;
        cout << "\tEnter your new password:" << endl;
        cin >> newPassword;
        cout << "\tRe-enter your new password:" << endl;
        cin >> newPassword2;
        if (oldPassword == login[selectedUser].password && newPassword == newPassword2)
        {
            login.at(selectedUser).password = newPassword;
            ok = 1;
            cout << "Your password has been successfully changed." << endl;
            saveFile(1);
        }
        else
        {
            cout << "Please, try again!" << endl;
        }
    }
}

int loginF()
{
    int j = 0, accountOK = 0, uid;
    string _accountNumber, _password;

    while (accountOK == 0)                       // Account number check is done here.
    {                                            // Unlimited trials are available for the account number.
        cout << "Please enter account number :"; // When the account number matches, the password control phase is started.
        cin >> _accountNumber;
        j = 0;
        for (int i = 0; i < login.size(); i++)
        {
            // cout << account[i].balance << endl;
            if (_accountNumber == login[i].accountNumber)
            {
                accountOK = 1;
                uid = i;
                break;
            }
        }
        if (accountOK == 0)
            cls();
        cout << "Account number not found." << endl;
    }

    if (accountOK == 1)                                  // After the account number matches, the error rate check (RLA) is done first.
    {                                                    //
        cls();                                           // Number of RLA =0 --> Account Blocked.
        cout << "Hi " << login.at(uid).userName << endl; // RLA number>0 passes to the password control stage.
        int RLA = 3 - login.at(uid).errorRate;           // Remaining login attempt

        if (RLA == 0)
        {
            cout << "Your account has been blocked!" << endl;
            return -1;
        }
        else
        {
            while (RLA)
            {

                cout << "Please enter password :";
                cin >> _password;
                if (login.at(uid).password == _password)       // If the password for the relevant account number
                {                                              // is correct, the system is logged in.
                    cls();                                     //
                    cout << "Successfully logged in." << endl; // If the login is successful, the error rate is reset.
                    login.at(uid).errorRate = 0;               // and the necessary updates are written to the file.
                    saveFile(1);
                    return uid;
                }
                else
                {

                    login.at(uid).errorRate += 1;
                    RLA--;
                    saveFile(1);
                    cout << "Password is incorrect ! Your remaining attempts: " << RLA << endl;
                }
            }
        }
    }
    return -1;
}

int findBalance(string accNumber) // Returns account balance information
{                                 // for that user's account.
    int i = 0;
    for (const auto &arr : account)
    {
        if (accNumber == arr.accountNumber)
        {
            return i;
        }
        i++;
    }
    return i;
}

void saveFile(int choose) // If the file to be updated is loginInfo, choose=1
{                         // If the file to be updated is accountInfo, choose=2
    if (choose == 1)      // is selected and necessary updates are made in the file.
    {
        ofstream outFile(LOGINFILE);
        for (const auto &e : login)
            outFile << e.accountNumber << " " << e.userName << " " << e.password << " " << e.errorRate << endl;
    }
    if (choose == 2)
    {
        ofstream outFile1(ACCOUNTFILE);
        for (const auto &e : account)
            outFile1 << e.accountNumber << " " << e.balance << endl;
    }
}

void readTxtFile() // Login and Account information are read from the file
{                  // and saved in the vector data structure.
    int failed = 0;
    ifstream fin;
    loginInfo recordLogin;
    accountInfo recordAccount;
    fin.open(static_cast<string>(LOGINFILE).c_str());
    while (fin >> recordLogin.accountNumber >> recordLogin.userName >> recordLogin.password >> recordLogin.errorRate)
        login.push_back(recordLogin);
    fin.close();
    fin.open(static_cast<string>(ACCOUNTFILE).c_str());
    while (fin >> recordAccount.accountNumber >> recordAccount.balance)
        account.push_back(recordAccount);
    fin.close();
}

void printAll()
{
    cout << "Login Information" << endl;
    for (const auto &arr : login)
    {
        cout << "Account Name: " << arr.accountNumber << endl
             << "Username: " << arr.userName << endl
             << "Password: " << arr.password << endl
             << "Error Rate: " << arr.errorRate << endl;
    }
    cout << "Account Balance Information" << endl;
    for (const auto &arr : account)
    {
        cout << "Account Name: " << arr.accountNumber << endl
             << "Balance: " << arr.balance << endl;
    }
}

void cls()
{
    system("clear");
}