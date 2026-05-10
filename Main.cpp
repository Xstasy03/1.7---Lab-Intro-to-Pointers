#include <iostream>

class Account
{
protected:
    char accountName[50];

    int* accountId;
    int* transactionLimit;
    int* overdraftFee;

    double accountBalance;
    double accountInterestRate;

public:
    Account(const char name[],
        int id,
        int limit,
        int overdraft,
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

        accountId = new int(id > 0 ? id : 1);
        transactionLimit = new int(limit > 0 ? limit : 1);
        overdraftFee = new int(overdraft > 0 ? overdraft : 0);

        accountBalance = balance;
        accountInterestRate = interestRate;
    }

    virtual ~Account()
    {
        delete accountId;
        delete transactionLimit;
        delete overdraftFee;
    }

    const char* GetName() { return accountName; }
    int GetId() { return *accountId; }
    int GetTransactionLimit() { return *transactionLimit; }
    int GetOverdraftFee() { return *overdraftFee; }
    double GetBalance() { return accountBalance; }
    double GetInterestRate() { return accountInterestRate; }

    virtual void Withdraw(double* amount) = 0;
    virtual void Deposit(double* amount) = 0;
    virtual void AddInterest() = 0;
};

int main()
{
    return 0;
}
