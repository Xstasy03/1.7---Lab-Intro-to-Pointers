class Checking : public Account
{
private:
    char accountNickname[50];

public:
    // Overloaded constructor
    Checking(const char name[],
        const char nickname[],
        int id,
        int limit,
        int overdraft,
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

    // Getter
    const char* GetNickname()
    {
        return accountNickname;
    }

    // Override Withdraw
    void Withdraw(double* amount) override
    {
        accountBalance -= *amount;

        // apply overdraft fee only if balance goes below 0
        if (accountBalance < 0)
        {
            accountBalance -= *overdraftFee;
        }
    }

    // Override Deposit
    void Deposit(double* amount) override
    {
        accountBalance += *amount;
    }

    // Override AddInterest
    void AddInterest() override
    {
        accountBalance += accountBalance * accountInterestRate;
    }
};
