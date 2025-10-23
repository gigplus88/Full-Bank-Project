//Project with CRUD Bank console c++ App

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenue();
void ShowTransactionsClientScreen();
void GoBackToTransactionMenue();
void ShowLoginMenue();
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

sClient ClientCurrent;

// ============================================
// UTILITY FUNCTIONS
// ============================================

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

// ============================================
// FILE OPERATIONS
// ============================================

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector <sClient> SaveClientsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

// ============================================
// CLIENT OPERATIONS
// ============================================

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

// ============================================
// INPUT FUNCTIONS
// ============================================

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

string ReadPinCode()
{
    string PinCode;
    cout << "\nPlease enter PinCode? ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, PinCode);
    return PinCode;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

// ============================================
// DISPLAY FUNCTIONS
// ============================================

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number   : " << Client.AccountNumber;
    cout << "\nPin Code        : " << Client.PinCode;
    cout << "\nName            : " << Client.Name;
    cout << "\nPhone           : " << Client.Phone;
    cout << "\nAccount Balance : " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientRecordLineForBalances(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

// ============================================
// CRUD OPERATIONS
// ============================================

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

// ============================================
// TRANSACTIONS
// ============================================

void DepositAmount(vector <sClient>& vClients, string AccountNumber, double& NewBalance)
{
    double DepositAmount = 0;
    char Transact = 'n';

    cout << "Please enter deposit amount? ";
    cin >> DepositAmount;

    cout << "Are you sure you want perform this transaction? y/n? ";
    cin >> Transact;

    if (Transact == 'Y' || Transact == 'y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += DepositAmount;
                NewBalance = C.AccountBalance;
                break;
            }
        }
    }
}

void DepositByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        double NewBalance = 0;
        DepositAmount(vClients, AccountNumber, NewBalance);
        SaveClientsDataToFile(ClientsFileName, vClients);

        cout << "\n\nDone Successfully. New balance is: " << NewBalance;
    }
    else
    {
        do
        {
            cout << "Client with[" << AccountNumber << "] does not exist.\n";
            cout << "\nPlease enter AccountNumber? ";
            cin >> AccountNumber;
        } while (!FindClientByAccountNumber(AccountNumber, vClients, Client));

        DepositByAccountNumber(AccountNumber, vClients);
    }
}

void WithdrawAmount(vector <sClient>& vClients, string AccountNumber, double& NewBalance)
{
    double WithdrawAmount = 0;
    char Transact = 'n';

    cout << "Please enter Withdraw amount? ";
    cin >> WithdrawAmount;

    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            while (WithdrawAmount > C.AccountBalance)
            {
                cout << "Amount Exceeds the balance, you can withdraw up to: " << C.AccountBalance << endl;
                cout << "Please enter another amount? ";
                cin >> WithdrawAmount;
            }

            cout << "Are you sure you want perform this transaction? y/n? ";
            cin >> Transact;

            if (Transact == 'Y' || Transact == 'y')
            {
                C.AccountBalance -= WithdrawAmount;
                NewBalance = C.AccountBalance;
            }
            break;
        }
    }
}

void WithdrawByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        double NewBalance = 0;
        WithdrawAmount(vClients, AccountNumber, NewBalance);
        SaveClientsDataToFile(ClientsFileName, vClients);

        cout << "\n\nDone Successfully. New balance is: " << NewBalance;
    }
    else
    {
        do
        {
            cout << "Client with[" << AccountNumber << "] does not exist.\n";
            cout << "\nPlease enter AccountNumber? ";
            cin >> AccountNumber;
        } while (!FindClientByAccountNumber(AccountNumber, vClients, Client));

        WithdrawByAccountNumber(AccountNumber, vClients);
    }
}

void PrintTotalBalances(vector <sClient>& vClients)
{
    double Total = 0.0;

    for (sClient& C : vClients)
    {
        Total += C.AccountBalance;
    }

    cout << "\n\t\t\tTotal Balances = " << Total;
}

// ============================================
// SCREEN FUNCTIONS
// ============================================

void ShowAllClientsScreen()
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    AddNewClients();
}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowDepositScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DepositByAccountNumber(AccountNumber, vClients);
}

void ShowWithdrawScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    WithdrawByAccountNumber(AccountNumber, vClients);
}

void ShowTotalBalancesScreen()
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n\n_____________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n\n______________________________________________________________________\n\n";

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient& Client : vClients)
        {
            PrintClientRecordLineForBalances(Client);
            cout << "\n";
        }

    cout << "\n\n______________________________________________________________________\n\n";
    cout << "\t\t\t";
    PrintTotalBalances(vClients);
}


// ============================================
// MENU FUNCTIONS
// ============================================

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6, eExit = 7
};

enum enTransactionsMenueOption
{
    enDeposit = 1, enWithdraw = 2,
    enTotalBalances = 3, enMainMenue = 4
};

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

void GoBackToTransactionMenue()
{
    cout << "\n\nPress any key to go back to Transaction Menue...";
    system("pause>0");
    system("cls");
    ShowTransactionsClientScreen();
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 7]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}
void GoBackToLogin()
{
    system("cls");

    cout << "\n\nPress any key to go back to Login ...";
    system("pause>0");
    ShowLoginMenue();
}
void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

    GoBackToLogin();
}
void PerformTransactionsMenueOption(enTransactionsMenueOption TransactionsMenueOption)
{
    switch (TransactionsMenueOption)
    {
    case enDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionMenue();
        break;

    case enWithdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionMenue();
        break;

    case enTotalBalances:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionMenue();
        break;

    case enMainMenue:
        ShowMainMenue();
        break;
    }
}

void ShowTransactionsClientScreen()
{
    cout << "\n============================";
    cout << "============================\n";
    cout << "\t\tTransaction Menue Screen";
    cout << "\n============================";
    cout << "============================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.";
    cout << "\n============================";
    cout << "============================\n";

    PerformTransactionsMenueOption((enTransactionsMenueOption)ReadTransactionsMenueOption());
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eTransactions:
        system("cls");
        ShowTransactionsClientScreen();
        break;

    case enMainMenueOptions::eExit:
        system("cls");
        ShowEndScreen();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "===========================================\n";

    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

// ============================================
// LOGIN SYSTEM
// ============================================

bool LoadUserInfo(string AccountNumber, string PinCode)
{
    sClient Client;
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, ClientCurrent))
        return true;
    else
        return false;
}

void PerformLoginOptions()
{
    bool LoginFailed = false;
    string AccountNumber = "";
    string PinCode = "";

    do
    {
        if (LoginFailed)
        {
            cout << "\nInvalid AccountNumber or PinCode, Please enter other Client Info." << endl;
        }

        AccountNumber = ReadClientAccountNumber();
        PinCode = ReadPinCode();

        LoginFailed = !LoadUserInfo(AccountNumber, PinCode);

    } while (LoginFailed);
}

void ShowLoginMenue()
{
    cout << "\n-----------------------------------\n\n";
    cout << "\tLogin Screen\n";
    cout << "\n-----------------------------------\n";

    PerformLoginOptions();
    ShowMainMenue();
}


// ============================================
// MAIN FUNCTION
// ============================================

bool FileExists(const string& filename)
{
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}
int main()
{
    if (!FileExists(ClientsFileName))
    {
        cout << "⚠️ File not found! Creating new file..." << endl;

        fstream NewFile;
        NewFile.open(ClientsFileName, ios::out); // إنشاء ملف جديد
        NewFile.close();

        cout << "✅ File created successfully: " << ClientsFileName << endl;
    }
    else
    {
        cout << "✅ File exists: " << ClientsFileName << endl;
    }
    ShowLoginMenue();
    system("pause>0");
    return 0;
}