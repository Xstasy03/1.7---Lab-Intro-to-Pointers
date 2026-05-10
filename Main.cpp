#include <iostream>
#include <climits>
#include <cstdio>

// =======================
// BASE CLASS: Account
// =======================
class Account
{
protected:
    char accountName[50];

    unsigned int* accountId;
    unsigned int* transactionLimit;
    unsigned int* overdraftFee;

    double accountBalance;
    double accountInterestRate;

public:
    Account(const char name[],
        unsigned int id,
        unsigned int limit,
        unsigned int overdraft,
        double balance,
        double interestRate)
    {
        int i = 0;
        while (name[i] != '\0' && i < 49)
        {
            accountName[i] = name[i];
            i++;
        }
        accountName[i] = '\0';

        accountId = new unsigned int(id > 0 ? id : 1);
        transactionLimit = new unsigned int(limit > 0 ? limit : 1);
        overdraftFee = new unsigned int(overdraft);

        accountBalance = balance;
        accountInterestRate = interestRate;
    }

    virtual ~Account()
    {
        delete accountId;
        delete transactionLimit;
        delete overdraftFee;
    }

    // Getters
    const char* GetName()             const { return accountName; }
    unsigned int GetId()               const { return *accountId; }
    unsigned int GetTransactionLimit() const { return *transactionLimit; }
    unsigned int GetOverdraftFee()     const { return *overdraftFee; }
    double       GetBalance()          const { return accountBalance; }
    double       GetInterestRate()     const { return accountInterestRate; }

    // Pointer getters (for address printing)
    unsigned int* GetIdPtr()               const { return accountId; }
    unsigned int* GetTransactionLimitPtr() const { return transactionLimit; }
    unsigned int* GetOverdraftFeePtr()     const { return overdraftFee; }

    // Pure virtual methods
    virtual void Withdraw(double* amount) = 0;
    virtual void Deposit(double* amount) = 0;
    virtual void AddInterest() = 0;
};

// =======================
// DERIVED CLASS: Checking
// =======================
class Checking : public Account
{
private:
    char accountNickname[50];

public:
    Checking(const char name[],
        const char nickname[],
        unsigned int id,
        unsigned int limit,
        unsigned int overdraft,
        double balance,
        double interestRate)
        : Account(name, id, limit, overdraft, balance, interestRate)
    {
        int i = 0;
        while (nickname[i] != '\0' && i < 49)
        {
            accountNickname[i] = nickname[i];
            i++;
        }
        accountNickname[i] = '\0';
    }

    const char* GetNickname() const { return accountNickname; }

    void Withdraw(double* amount) override
    {
        accountBalance -= *amount;

        // Overdraft fee only applied when balance drops below 0
        if (accountBalance < 0)
        {
            accountBalance -= static_cast<double>(*overdraftFee);
        }
    }

    void Deposit(double* amount) override
    {
        accountBalance += *amount;
    }

    void AddInterest() override
    {
        accountBalance += accountBalance * accountInterestRate;
    }
};

// =========================================
// Forward Declarations
// =========================================
void RunWithdrawChecking(Checking* account);
void RunDepositChecking(Checking* account);
void Print(Checking* account);

// =========================================
// Print Function
// =========================================
void Print(Checking* account)
{
    std::printf("-----------------------------\n");
    std::printf("       Account Information   \n");
    std::printf("-----------------------------\n");

    std::printf("Name          : %s  (addr: %p)\n",
        account->GetName(), (void*)account->GetName());

    std::printf("Nickname      : %s  (addr: %p)\n",
        account->GetNickname(), (void*)account->GetNickname());

    std::printf("ID            : %u  (addr: %p)\n",
        account->GetId(), (void*)account->GetIdPtr());

    std::printf("Balance       : $%.2f\n",
        account->GetBalance());

    std::printf("Interest Rate : %.2f\n",
        account->GetInterestRate());

    std::printf("Trans. Limit  : $%u  (addr: %p)\n",
        account->GetTransactionLimit(), (void*)account->GetTransactionLimitPtr());

    std::printf("Overdraft Fee : $%u  (addr: %p)\n",
        account->GetOverdraftFee(), (void*)account->GetOverdraftFeePtr());

    std::printf("-----------------------------\n\n");
}

// =========================================
// RunWithdrawChecking
// =========================================
void RunWithdrawChecking(Checking* account)
{
    double amount = 0.0;
    double limit = static_cast<double>(account->GetTransactionLimit());

    while (true)
    {
        std::printf("Enter withdrawal amount: ");
        std::cin >> amount;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');

        if (amount <= 0.0)
        {
            std::printf("Amount must be positive. Please try again.\n");
            continue;
        }

        if (amount > limit)
        {
            std::printf("Amount exceeds transaction limit of $%.2f. Please try again.\n", limit);
            continue;
        }

        break;
    }

    account->Withdraw(&amount);
    std::printf("Withdrew $%.2f successfully.\n\n", amount);
}

// =========================================
// RunDepositChecking
// =========================================
void RunDepositChecking(Checking* account)
{
    double amount = 0.0;

    while (true)
    {
        std::printf("Enter deposit amount: ");
        std::cin >> amount;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');

        if (amount <= 0.0)
        {
            std::printf("Amount must be positive. Please try again.\n");
            continue;
        }

        break;
    }

    account->Deposit(&amount);
    std::printf("Deposited $%.2f successfully.\n\n", amount);
}

// =========================================
// Function Pointer Typedef
// =========================================
typedef void (*PrintFuncPtr)(Checking*);

// =========================================
// Global Function Pointers
// =========================================
PrintFuncPtr printCheck = Print;
PrintFuncPtr printCheckMem = &Print;

// =========================================
// Main
// =========================================
int main()
{
    std::printf("===Bad Bank LLC.===\n\n");

    // --------------------------------
    // Default Checking Account
    // --------------------------------
    std::printf("==Default Checking==\n");

    // Temporary variables
    unsigned int defaultId = 1002;
    unsigned int defaultLimit = 100;
    unsigned int defaultFee = 100;
    double       defaultBalance = 100.0;
    double       defaultRate = 1.25;

    // Pointers to temporaries
    unsigned int* pDefaultId = &defaultId;
    unsigned int* pDefaultLimit = &defaultLimit;
    unsigned int* pDefaultFee = &defaultFee;

    Checking defaultChecking(
        "Urdnot Grunt",
        "Mr. Money",
        *pDefaultId,
        *pDefaultLimit,
        *pDefaultFee,
        defaultBalance,
        defaultRate
    );

    Checking* pDefault = &defaultChecking;

    printCheck(pDefault);
    printCheckMem(pDefault);

    std::printf("== Withdraw from Default Checking ==\n");
    RunWithdrawChecking(pDefault);

    printCheck(pDefault);
    printCheckMem(pDefault);

    std::printf("== Deposit into Default Checking ==\n");
    RunDepositChecking(pDefault);

    printCheck(pDefault);
    printCheckMem(pDefault);

    std::printf("== Adding Interest for Default Checking ==\n");
    pDefault->AddInterest();

    printCheck(pDefault);
    printCheckMem(pDefault);

    // --------------------------------
    // User-Created Checking Account
    // --------------------------------
    std::printf("==Created Checking==\n");

    char         userName[50];
    char         userNickname[50];
    unsigned int userId;
    double       userBalance;
    double       userRate;

    std::printf("Enter account name: ");
    std::cin.getline(userName, 50);

    std::printf("Enter account balance: ");
    std::cin >> userBalance;
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');

    std::printf("Enter account ID: ");
    std::cin >> userId;
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');

    std::printf("Enter interest rate (e.g. 1.25): ");
    std::cin >> userRate;
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');

    std::printf("Enter account nickname: ");
    std::cin.getline(userNickname, 50);

    // Reuse limit and fee pointers from the default account
    Checking userChecking(
        userName,
        userNickname,
        userId,
        *pDefaultLimit,
        *pDefaultFee,
        userBalance,
        userRate
    );

    Checking* pUser = &userChecking;

    printCheck(pUser);
    printCheckMem(pUser);

    std::printf("== Withdraw from Created Checking ==\n");
    RunWithdrawChecking(pUser);

    printCheck(pUser);
    printCheckMem(pUser);

    std::printf("== Deposit into Created Checking ==\n");
    RunDepositChecking(pUser);

    printCheck(pUser);
    printCheckMem(pUser);

    std::printf("== Adding Interest for Created Checking ==\n");
    pUser->AddInterest();

    printCheck(pUser);
    printCheckMem(pUser);

    return 0;
}
