#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// Function prototypes
unsigned int enterChoice(void);
void initializeFile(FILE *fPtr);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAllRecords(FILE *fPtr);
void searchRecord(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);

int main()
{
    FILE *cfPtr;

    // Open file
    cfPtr = fopen("credit.dat", "rb+");

    // Create file if it does not exist
    if (cfPtr == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be created.\n");
            return 1;
        }

        initializeFile(cfPtr);
    }

    unsigned int choice;

    while ((choice = enterChoice()) != 9)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            displayAllRecords(cfPtr);
            break;

        case 6:
            searchRecord(cfPtr);
            break;

        case 7:
            depositMoney(cfPtr);
            break;

        case 8:
            withdrawMoney(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    printf("\nThank You...\n");

    return 0;
}

// Initialize file with blank records
void initializeFile(FILE *fPtr)
{
    struct clientData blankClient = {0, "", "", 0.0};

    for (int i = 0; i < 100; i++)
    {
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
    }
}

// Menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========= BANK MANAGEMENT SYSTEM =========\n");
    printf("1. Generate accounts.txt\n");
    printf("2. Update Account\n");
    printf("3. Add New Account\n");
    printf("4. Delete Account\n");
    printf("5. Display All Accounts\n");
    printf("6. Search Account\n");
    printf("7. Deposit Money\n");
    printf("8. Withdraw Money\n");
    printf("9. Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);

    return choice;
}

// Generate text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s %-15s %-15s %-10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d %-15s %-15s %.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt created successfully.\n");
}

// Add new account
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter lastname firstname balance:\n");

        scanf("%14s %9s %lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account added successfully.\n");
    }
}

// Update account
void updateRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;
    double transaction;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Current Balance: %.2f\n", client.balance);

        printf("Enter transaction amount (+/-): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Updated Balance: %.2f\n", client.balance);
    }
}

// Delete account
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// Display all accounts
void displayAllRecords(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s %-15s %-15s %-10s\n",
           "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-6d %-15s %-15s %.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// Search account
void searchRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;

    printf("Enter account number to search: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Found\n");

        printf("Account Number : %d\n", client.acctNum);
        printf("Last Name     : %s\n", client.lastName);
        printf("First Name    : %s\n", client.firstName);
        printf("Balance       : %.2f\n", client.balance);
    }
}

// Deposit money
void depositMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Deposit successful.\n");
    printf("New Balance: %.2f\n", client.balance);
}

// Withdraw money
void withdrawMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    if (amount > client.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Withdrawal successful.\n");
    printf("Remaining Balance: %.2f\n", client.balance);
}