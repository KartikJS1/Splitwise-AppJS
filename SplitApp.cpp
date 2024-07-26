#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>

using namespace std;

enum ExpenseType
{
    EQUAL,
    EXACT,
    PERCENT
};

class User
{
public:
    string userId;
    string name;
    string email;
    string mobile;

    User(string id = "", string name = "", string email = "", string mobile = "")
    {
        this->userId = id;
        this->name = name;
        this->email = email;
        this->mobile = mobile;
    }
};

class Expense
{
public:
    string paidBy;
    double amount;
    ExpenseType type;
    vector<string> participants;
    vector<double> shares; // For exact or percent shares

    Expense(string paidBy, double amount, ExpenseType type, vector<string> participants, vector<double> shares)
    {
        this->paidBy = paidBy;
        this->amount = amount;
        this->type = type;
        this->participants = participants;
        this->shares = shares;
    }
};

class ExpenseManager
{
private:
    unordered_map<string, User> users;
    vector<Expense> expenses;
    unordered_map<string, unordered_map<string, double>> balances;

public:
    void addUser(string id, string name, string email, string mobile)
    {
        users[id] = User(id, name, email, mobile);
    }

    void addExpense(string paidBy, double amount, ExpenseType type, vector<string> participants, vector<double> shares)
    {
        Expense expense(paidBy, amount, type, participants, shares);
        expenses.push_back(expense);

        // Update balances
        updateBalances(expense);
    }

    void updateBalances(Expense expense)
    {
        if (expense.type == EQUAL)
        {
            double splitAmount = expense.amount / expense.participants.size();
            for (const string &participant : expense.participants)
            {
                if (participant != expense.paidBy)
                {
                    balances[participant][expense.paidBy] += splitAmount;
                }
            }
        }
        else if (expense.type == EXACT)
        {
            for (size_t i = 0; i < expense.participants.size(); ++i)
            {
                if (expense.participants[i] != expense.paidBy)
                {
                    balances[expense.participants[i]][expense.paidBy] += expense.shares[i];
                }
            }
        }
        else if (expense.type == PERCENT)
        {
            for (size_t i = 0; i < expense.participants.size(); ++i)
            {
                double shareAmount = (expense.shares[i] / 100) * expense.amount;
                if (expense.participants[i] != expense.paidBy)
                {
                    balances[expense.participants[i]][expense.paidBy] += shareAmount;
                }
            }
        }
    }

    void showBalances()
    {
        bool noBalances = true;
        for (const auto &userBalance : balances)
        {
            for (const auto &balance : userBalance.second)
            {
                if (balance.second != 0)
                {
                    noBalances = false;
                    cout << userBalance.first << " owes " << balance.first << ": " << fixed << setprecision(2) << balance.second << endl;
                }
            }
        }
        if (noBalances)
        {
            cout << "No balances" << endl;
        }
    }

    void showBalances(string userId)
    {
        if (balances.find(userId) == balances.end() || balances[userId].empty())
        {
            cout << "No balances" << endl;
            return;
        }
        bool noBalances = true;
        for (const auto &balance : balances[userId])
        {
            if (balance.second != 0)
            {
                noBalances = false;
                cout << userId << " owes " << balance.first << ": " << fixed << setprecision(2) << balance.second << endl;
            }
        }
        if (noBalances)
        {
            cout << "No balances" << endl;
        }
    }
};

int main()
{
    ExpenseManager expenseManager;

    // Adding users
    expenseManager.addUser("u1", "User1", "user1@example.com", "1234567890");
    expenseManager.addUser("u2", "User2", "user2@example.com", "1234567891");
    expenseManager.addUser("u3", "User3", "user3@example.com", "1234567892");
    expenseManager.addUser("u4", "User4", "user4@example.com", "1234567893");

    // Adding expenses
    expenseManager.addExpense("u1", 1000, EQUAL, {"u1", "u2", "u3", "u4"}, {});
    expenseManager.addExpense("u1", 1250, EXACT, {"u2", "u3"}, {370, 880});
    expenseManager.addExpense("u4", 1200, PERCENT, {"u1", "u2", "u3", "u4"}, {40, 20, 20, 20});

    // Show all balances
    cout << "All balances:" << endl;
    expenseManager.showBalances();

    // Show balances for a specific user
    cout << "Balances for u1:" << endl;
    expenseManager.showBalances("u1");

    return 0;
}
