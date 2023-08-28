#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string.h>
using namespace std;

void PrintMainMenu();
void Functions();

const string FileName = "Client.txt";

namespace MainFunc {
    enum Func
    {
        Show = 1, Add, Delete, Update, Find, Transactions, Exit };
}
namespace TransFunc {
enum Func
{
    Deposit = 1, Withdraw, Total_Balances, Main_Menu };
}

struct stData {
    string AccountNumber;
    string PINCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool ToDelete = false;
};

void PrintTransactionsMenu();

vector<string> Split(string TheString, string Delim = " ")
{
    vector<string> vWords;
    int count = 0;
    int pos = 0;
    string Word;
    while ((pos = TheString.find(Delim)) != std::string::npos) {
        Word = TheString.substr(0, pos);
        if (Word != " ") {
            vWords.push_back(Word);
        }
        TheString.erase(0, (pos + Delim.length()));
    }
    if (TheString != " ") {
        vWords.push_back(TheString);
    }
    return vWords;
}


stData LineToRecord(string Line, string Delim = "#//#") {
    vector<string> vData = Split(Line, "#//#");
    stData Data;
    Data.AccountNumber = vData[0];
    Data.PINCode = vData[1];
    Data.Name = vData[2];
    Data.Phone = vData[3];
    Data.AccountBalance = stod(vData[4]);
    return Data;
}

vector<stData> LoadFileToVector (string FileName) {
    fstream ClientsFile;
    vector<stData> vClientsData;

    ClientsFile.open(FileName, ios::in);
    if (ClientsFile.is_open()) {
        string Line;
        stData Client;

        while (getline(ClientsFile, Line)) {
            Client = LineToRecord(Line);
            vClientsData.push_back(Client);
        }

        ClientsFile.close();

    }
    return vClientsData;
}


void PrintMainMenu() {
    system ("cls");
    cout << "========================================================================\n";
    cout << "\t\t\t   Main Menu Screen \n";
    cout << "========================================================================\n";
    cout << "\t [1] Show Clients List.\n";
    cout << "\t [2] Add New Client.\n";
    cout << "\t [3] Delete Client.\n";
    cout << "\t [4] Update Client.\n";
    cout << "\t [5] Find Client.\n";
    cout << "\t [6] Transactions.\n";
    cout << "\t [7] Exit.\n";
    cout << "========================================================================\n";
    cout << "Choose what do you want to do? [1 to 6] ?\n";
    Functions();
}

void PrintClients(vector<stData> Data) {
    system ("cls");
    cout << "                          Client list (" << Data.size() << ") Client(s): " << endl;
    cout << "_______________________________________________________________________________________________________\n";
    cout << "| " << setw(20) << left << "Account Number" << "| " << setw(12) << "Pin Code" << "| " << setw(34) << "Client Name" << "| " << setw(18) << "Phone Number" << "| " << setw(12) << "Balance";
    cout << "\n_______________________________________________________________________________________________________\n";

    for (stData &Record : Data) {
        cout <<  "| " << setw(20) << Record.AccountNumber <<  "| " << setw(12) << Record.PINCode << "| " << setw(34) << Record.Name << "| " << setw(18) << Record.Phone << "| " << setw(12) << Record.AccountBalance << endl;
    }
}

string ReadString(string Massage)
{
    string TheString;
    cout << Massage;
    getline(cin, TheString);
    return TheString;
}

stData FillClientData(string ID) {
    stData Data;
    Data.AccountNumber = ID;
    cout << "Enter Your PIN Code : ";
    getline(cin >> ws, Data.PINCode);
    Data.Name = ReadString("Enter Your Name : ");
    Data.Phone = ReadString("Enter Your Phone Number: ");
    Data.AccountBalance = stod(ReadString("Enter Your Account Balance : "));
    return Data;
}

string ConvertRecordToLine(stData Data, string Seperator = "#//#") {
    string Line = "";
    
    Line += Data.AccountNumber + Seperator; 
    Line += Data.PINCode + Seperator;
    Line += Data.Name + Seperator;
    Line += Data.Phone + Seperator;
    Line += to_string(Data.AccountBalance);

    return Line;
}

void ClientToFile(string FileName, string Line) {
    
    fstream ClientFile;

    ClientFile.open(FileName, ios::out | ios::app );
    if (ClientFile.is_open()) {
        ClientFile << Line << endl;
        ClientFile.close();
    }
    
}

bool SearchClient(string ID, vector<stData> Clients, stData& Client) {
    for (stData &C : Clients) {
        if (C.AccountNumber == ID) {
            Client = C;
            return true;
        }
    } 
    return false;
}

void AddNewClient(string ID) {
    stData ClientData;
    ClientData = FillClientData(ID);
    ClientToFile(FileName, ConvertRecordToLine(ClientData));
}
void AddClients (vector<stData> Clients) {
    stData Client;
    char Answer = 'Y';
    string ID = "";
    do {
        system("cls");
        cout << "Enter the ID you want to add\n";
        cin >> ID;
        system ("cls");
        if (!SearchClient(ID, Clients, Client)) {
        cout << "Adding new clients : \n\n";
        
        AddNewClient(ID);

        cout << "Added succefully!!\n";
        }
        else cout << "This ID is taken\n";
        cout << "\nDo You Want to Add More Clients ? [y/n]  \n";
        cin >> Answer;
    } while (toupper(Answer) == 'Y');
    cout << "\n All clients were added to the file [" << FileName << "]";
}


void PrintRecord (stData Data) {
    cout << "\n-----------------------------------------\n";
    cout << "Account Numer : " << Data.AccountNumber << endl;
    cout << "PIN Code : " << Data.PINCode << endl;
    cout << "Name : " << Data.Name << endl;
    cout << "Phone : " << Data.Phone << endl;
    cout << "Account Balance : " << Data.AccountBalance << endl;
    cout << "\n-----------------------------------------\n";
}

void MarkClientForDeletion(string ID, vector<stData> &Clients) {
    for (stData &C : Clients) {
        if (C.AccountNumber == ID) {
            C.ToDelete = true;
        }
    }
}

void LoadVectorToFile(string FileName, vector<stData> Clients) {
    fstream MyFile;

    MyFile.open(FileName, ios::out);
    
    if (MyFile.is_open()) {
        for (stData &C : Clients) {
            if (!C.ToDelete) {
                MyFile << ConvertRecordToLine(C) << endl;
            }
        } 
    }
}

bool DeleteClientByID(vector<stData> &Clients) {
    string ID;
    cout << "What ID do you want to delete? \n";
    cin >> ID;
    char Answer = 'N';
    stData Client;

    if (SearchClient(ID, Clients, Client)) {
        PrintRecord(Client);

        cout << "Are you sure you want to delete this record\n";
        cin >> Answer;
        
        if (toupper(Answer) == 'Y') {
            MarkClientForDeletion(ID, Clients);
            LoadVectorToFile(FileName, Clients);

            cout << "Client was deleted successfully!\n";
            Clients = LoadFileToVector(FileName);
        }
    }
    else {
        cout << "Client was not found!\n";
    }
}

stData ChangeClientData(stData &Data) {
    cout << ("Enter Your PIN Code : ");
    getline(cin >> ws, Data.PINCode);
    Data.Name = ReadString("Enter Your Name : ");
    Data.Phone = ReadString("Enter Your Phone Number: ");
    Data.AccountBalance = stod(ReadString("Enter Your Account Balance : "));
    return Data;
}

bool UpdateClientByID(vector<stData> &Clients) {
    string ID;
    cout << "What ID do you want to update? \n";
    cin >> ID;
    char Answer = 'N';
    stData Client;

    if (SearchClient(ID, Clients, Client)) {
        PrintRecord(Client);

        cout << "Are you sure you want to update this record\n";
        cin >> Answer;
        
        if (toupper(Answer) == 'Y') {
            for (stData &C : Clients)
            {
                if (C.AccountNumber == ID)
                {
                    C = ChangeClientData(C);
                    break;
                }
            }
            LoadVectorToFile(FileName, Clients);

            cout << "Client was Updated successfully!\n";
            Clients = LoadFileToVector(FileName);
        }
    }
    else {
        cout << "Client was not found!\n";
    }
}

void PressToContinue() {
            cout << "\nPress Enter To Continue...\n";
            system("pause>0");
            PrintMainMenu();
}

void TransPressToContinue() {
            cout << "\nPress Enter To Continue...\n";
            system("pause>0");
            PrintTransactionsMenu();
}

void FindClient(vector<stData> Clients) {
    string ID;
    cout << "Enter the ID you want to find? \n";
    cin >> ID;
    stData Client;
    if (SearchClient(ID, Clients, Client)) {
    PrintRecord(Client);
    }
    else {
        cout << "This ID wasn't found\n";
    }
}

double WithdrawFromClient(double Amount) {
    double Depo;
    cout << "How much do you want to deposit ? \n";
    cin >> Depo;
    Amount -= Depo;
    return Amount;
}
double DepositToClient(double Amount) {
    double Depo;
    cout << "How much do you want to deposit ? \n";
    cin >> Depo;
    Amount += Depo;
    return Amount;
}
void Deposit(vector<stData> Clients) {
    stData Client;
    string ID;
    char Answer;
    cout << "Enter the Account ID you want to Deposit into\n";
    cin >> ID;
    if (SearchClient(ID, Clients, Client)) {
        PrintRecord(Client);

        cout << "Are you sure you want to deposit to this account? \n";
        cin >> Answer;
        
        if (toupper(Answer) == 'Y') {
            for (stData &C : Clients)
            {
                if (C.AccountNumber == ID)
                {
                    C.AccountBalance = DepositToClient(C.AccountBalance);
                    break;
                }
            }
            LoadVectorToFile(FileName, Clients);

            cout << "Deposit Success\n";
            Clients = LoadFileToVector(FileName);
        }
    }
}

void Withdraw(vector<stData> Clients) {
    stData Client;
    string ID;
    char Answer;
    cout << "Enter the Account ID you want to Withdraw from\n";
    cin >> ID;
    if (SearchClient(ID, Clients, Client)) {
        PrintRecord(Client);

        cout << "Are you sure you want to withdraw from this account? \n";
        cin >> Answer;
        
        if (toupper(Answer) == 'Y') {
            for (stData &C : Clients)
            {
                if (C.AccountNumber == ID)
                {
                    C.AccountBalance = WithdrawFromClient(C.AccountBalance);
                    break;
                }
            }
            LoadVectorToFile(FileName, Clients);

            cout << "Withdraw Success\n";
            Clients = LoadFileToVector(FileName);
        }
    }
}

void PrintTotalBalances(vector<stData> Data) {
    system ("cls");
    cout << "                          Client list (" << Data.size() << ") Client(s): " << endl;
    cout << "_______________________________________________________________________________________________________\n";
    cout << "| " << setw(20) << left << "Account Number" << "| " << setw(34) << "Client Name" << "| " << setw(12) << "Balance";
    cout << "\n_______________________________________________________________________________________________________\n";

    for (stData &Record : Data) {
        cout <<  "| " << setw(20) << Record.AccountNumber << "| " << setw(34) << Record.Name << "| " << setw(12) << Record.AccountBalance << endl;
    }
}

void TransactionFunctions() {
    vector<stData> Data;
    int ans;
    cin >> ans;
    vector<stData> Clients = LoadFileToVector(FileName);

    switch (ans) {
        case TransFunc::Deposit: {
            Deposit(Clients);
            TransPressToContinue();
            break;
        }
        case TransFunc::Withdraw: {
            Withdraw(Clients);
            TransPressToContinue();
            break;
        }
        case TransFunc::Total_Balances: {
            PrintTotalBalances(LoadFileToVector(FileName));
            TransPressToContinue();
            break;
        }
        case TransFunc::Main_Menu: {
            PrintMainMenu();
            break;
        }
    }
}
void PrintTransactionsMenu() {
    system ("cls");
    cout << "========================================================================\n";
    cout << "\t\t\t   Transactions Screen \n";
    cout << "========================================================================\n";
    cout << "\t [1] Deposit.\n";
    cout << "\t [2] Withdraw.\n";
    cout << "\t [3] Total Balances.\n";
    cout << "\t [4] Main Menu.\n";
    cout << "========================================================================\n";
    cout << "Choose what do you want to do? [1 to 4] ?\n";
    TransactionFunctions();
}

void Functions() {
    int Ans;
    cin >> Ans;
    vector<stData> Clients = LoadFileToVector(FileName);
    
    switch(Ans) {
        case MainFunc::Show: {
            PrintClients(LoadFileToVector(FileName));
            PressToContinue();
            break;
        }
        case MainFunc::Add: {
            system("cls");
            AddClients(Clients);
            PressToContinue();
            break;
        }
        case MainFunc::Delete: {
            system("cls");
            DeleteClientByID(Clients);
            PressToContinue();
            break;
        }
        case MainFunc::Update: {
            system("cls");
            UpdateClientByID(Clients);
            PressToContinue();
            break;
        }
        case MainFunc::Find: {
            system("cls");
            FindClient(Clients);
            PressToContinue();
            break;
        }
        case MainFunc::Transactions: {
            PrintTransactionsMenu();
        }
        case MainFunc::Exit: {
            system("cls");
            cout << "\n HAVE A NICE DAY~! \n";
            cout << "\n\n Exitting ... \n\n";
            break;
        }
        default: {

        }
    }
}
void StartBank() {
    PrintMainMenu();
}

int main() {
    StartBank();
}