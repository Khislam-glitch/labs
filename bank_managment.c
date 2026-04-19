#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CLIENT_FILE "clients.dat"
#define ACCOUNT_FILE "accounts.dat"

typedef struct {
  int year;
  int mounth;
  int day;
} date;

typedef struct {
  int id;
  char name[30];
  char first_name[30];
  date dob;
  char address[50];
  char tel[50];
} Client;

typedef struct {
  int client_id;
  char type;
  float balance;
  bool blocked;
} Account;

int calc_age(int birthYear) { return 2026 - birthYear; }

bool find_account(FILE *fp, int id, Account *acc) {
  rewind(fp);
  while (fread(acc, sizeof(Account), 1, fp)) {
    if (acc->client_id == id) {
      return true;
    }
  }
  return false;
}

bool find_client(FILE *fp, int id, Client *acc) {
  rewind(fp);
  while (fread(acc, sizeof(Client), 1, fp)) {
    if (acc->id == id) {
      return true;
    }
  }
  return false;
}

float get_amount() {
  float n;
  printf("enter the amount of money: ");

  if (scanf("%f", &n) != 1 || n <= 0) {
    printf("Invalid amount!\n");
    return 0;
  }

  return n;
}

void deposit(float amount) {
  int id;
  Account acc;

  if (amount <= 0) {
    printf("Invalid amount!\n");
    return;
  }

  FILE *fp = fopen(ACCOUNT_FILE, "rb+");
  if (!fp) {
    printf("File not found!\n");
    return;
  }

  printf("Enter account id: ");
  scanf("%d", &id);

  while (fread(&acc, sizeof(Account), 1, fp)) {
    if (acc.client_id == id) {

      if (acc.blocked) {
        printf("Account is blocked!\n");
        fclose(fp);
        return;
      }

      acc.balance += amount;

      fseek(fp, -sizeof(Account), SEEK_CUR);
      fwrite(&acc, sizeof(Account), 1, fp);

      printf("Deposit successful! New balance: %.2f\n", acc.balance);
      fclose(fp);
      return;
    }
  }

  printf("Account not found!\n");
  fclose(fp);
}

void withdraw(float amount) {
  int id;
  Account acc;

  if (amount <= 0) {
    printf("Invalid amount!\n");
    return;
  }

  FILE *fp = fopen(ACCOUNT_FILE, "rb+");
  if (!fp) {
    printf("File not found!\n");
    return;
  }

  printf("Enter account id: ");
  scanf("%d", &id);

  while (fread(&acc, sizeof(Account), 1, fp)) {
    if (acc.client_id == id) {

      if (acc.blocked) {
        printf("Account is blocked!\n");
        fclose(fp);
        return;
      }

      if (acc.balance < amount) {
        printf("Insufficient balance!\n");
        fclose(fp);
        return;
      }

      acc.balance -= amount;

      fseek(fp, -sizeof(Account), SEEK_CUR);
      fwrite(&acc, sizeof(Account), 1, fp);

      printf("Withdrawal successful! New balance: %.2f\n", acc.balance);
      fclose(fp);
      return;
    }
  }

  printf("Account not found!\n");
  fclose(fp);
}

void transfer() {
  int sender, resiver;
  float amount;
  Account acc1, acc2;
  long pos1 = -1, pos2 = -1;

  FILE *fp = fopen(ACCOUNT_FILE, "rb+");
  if (!fp) {
    printf("File not found!\n");
    return;
  }

  printf("sender ID: ");
  scanf("%d", &sender);
  printf("resiver ID: ");
  scanf("%d", &resiver);

  rewind(fp);

  while (fread(&acc1, sizeof(Account), 1, fp)) {
    if (acc1.client_id == sender)
      pos1 = ftell(fp) - sizeof(Account);
    if (acc1.client_id == resiver)
      pos2 = ftell(fp) - sizeof(Account);
  }

  if (pos1 == -1 || pos2 == -1) {
    printf("Account not found!\n");
    fclose(fp);
    return;
  }

  printf("Amount: ");
  scanf("%f", &amount);

  if (amount <= 0) {
    printf("Invalid amount!\n");
    fclose(fp);
    return;
  }

  // Withdraw
  fseek(fp, pos1, SEEK_SET);
  fread(&acc1, sizeof(Account), 1, fp);

  if (acc1.balance < amount || acc1.blocked || acc2.blocked) {
    printf("Transfer failed!\n");
    fclose(fp);
    return;
  }

  acc1.balance -= amount;
  fseek(fp, pos1, SEEK_SET);
  fwrite(&acc1, sizeof(Account), 1, fp);

  // Deposit
  fseek(fp, pos2, SEEK_SET);
  fread(&acc2, sizeof(Account), 1, fp);

  acc2.balance += amount;
  fseek(fp, pos2, SEEK_SET);
  fwrite(&acc2, sizeof(Account), 1, fp);

  printf("Transfer successful!\n");
  fclose(fp);
}

void modify_type() {
  int id;
  char choice;
  Account account;
  bool found = false;

  FILE *fp = fopen(ACCOUNT_FILE, "rb+");
  if (!fp) {
    printf("File not found!\n");
    return;
  }

  printf("Enter account id: ");
  scanf("%d", &id);

  while (fread(&account, sizeof(Account), 1, fp)) {
    if (account.client_id == id) {

      printf("enter the type you want to change to:");
      printf("  'p' - Personal (age 18+)\n");
      printf("  'c' - Commercial \n");
      scanf(" %c", &choice);
      account.type = tolower(choice);

      fseek(fp, -sizeof(Account), SEEK_CUR);

      if (fwrite(&account, sizeof(Account), 1, fp) != 1) {
        printf("Error writing to file!\n");
      } else {
        printf("Change successful!\n");
      }

      found = true;
      break;
    }
  }
  if (!found) {
    printf("Account not found!\n");
  }
  fclose(fp);
}

void account_inquiry() {

  int id;
  Account account;

  FILE *fp = fopen(ACCOUNT_FILE, "rb");
  if (!fp) {
    printf("File not found!\n");
    return;
  }

  printf("Enter account id: ");
  scanf("%d", &id);

  while (fread(&account, sizeof(Account), 1, fp)) {
    if (account.client_id == id) {
      printf("\n========== ACCOUNT INQUIRY ==========\n");
      printf("Account ID: %d\n", account.client_id);
      printf("Account Type: %c\n", account.type);
      printf("Current Balance: $%0.2f\n", account.balance);
      printf("Account Status: %s\n", account.blocked ? "blocked" : "active");
      printf("=====================================\n");
    }
  }
  fclose(fp);
}

void add_client() {

  FILE *fc = fopen(CLIENT_FILE, "ab+");
  FILE *fa = fopen(ACCOUNT_FILE, "ab+");

  if (!fc) {
    printf("Error opening clients file!\n");
    return;
  }

  if (!fa) {
    printf("Error opening accounts file!\n");
    return;
  }

  Client client;
  Client temp;
  Account account;
  Account g_account;
  int guardianId;

  printf("\n--- Add New Client  ---\n");
  printf("Enter ID: ");
  scanf("%d", &client.id);

  if (find_client(fc, client.id, &temp)) {
    printf("Error: Client ID already exists.\n");
    fclose(fc);
    fclose(fa);
    return;
  }

  printf("Name: ");
  scanf("%29s", client.name);
  printf("First name:  ");
  scanf("%29s", client.first_name);
  printf("Date of Birth (DD MM YYYY): ");
  scanf("%d %d %d", &client.dob.day, &client.dob.mounth, &client.dob.year);
  printf("Address: ");
  scanf("%49s", client.address);
  printf("Tel: ");
  scanf("%49s", client.tel);

  printf("\nEnter account type,id:\n");
  printf("  'p' - Personal (age 18+)\n");
  printf("  'c' - Commercial \n");
  printf("  'm' - Minor (under 18, needs guardian)\n");
  printf(">>");
  scanf(" %c", &account.type);

  if (calc_age(client.dob.year) < 18 &&
      (tolower(account.type) == 'p' || tolower(account.type) == 'c')) {
    printf(
        "Client is under 18. Cannot create Personal or Commercial account.\n");
    fclose(fa);
    fclose(fc);
    return;
  }

  rewind(fa);

  if (tolower(account.type) == 'm') {
    printf("Enter Guardian's Client ID: ");
    scanf("%d", &guardianId);
    if (!find_account(fa, guardianId, &g_account)) {
      printf("Access Denied: Guardian must have an existing account.\n");
      fclose(fc);
      fclose(fa);
      return;
    }
  }

  account.client_id = client.id;
  account.balance = 0;
  account.blocked = false;

  fwrite(&client, sizeof(Client), 1, fc);
  fwrite(&account, sizeof(Account), 1, fa);
  fclose(fc);
  fclose(fa);

  printf("\nAccount created successfully!\n");
  printf("Account ID: %d, Type: %c, Balance: %f\n", account.client_id,
         account.type, account.balance);
}

void search_client() {
  int id;
  Client client;

  FILE *fp = fopen(CLIENT_FILE, "rb");
  if (!fp) {
    printf("File not found!\n");
    return;
  }

  printf("\nEnter Client ID to search: ");
  scanf("%d", &id);

  if (find_client(fp, id, &client)) {
    printf("Client Found: %s %s, Tel: %s\n", client.first_name, client.name,
           client.tel);
    fclose(fp);
  } else {
    printf("Client not found.\n");
    fclose(fp);
  }
}

void modify_client() {
  int id;
  Client client;

  FILE *fp = fopen(CLIENT_FILE, "rb+");
  if (!fp) {
    printf("File not found!\n");
    return;
  }

  printf("\nEnter Client ID to modify: ");
  scanf("%d", &id);

  while (fread(&client, sizeof(Client), 1, fp)) {
    if (client.id == id) {
      printf("Current Name: %s. Enter New Name: ", client.name);
      scanf("%s", client.name);
      printf("Current First Name: %s. Enter New First Name: ",
             client.first_name);
      scanf("%s", client.first_name);
      printf("Current Address: %s. Enter New Address: ", client.address);
      scanf("%s", client.address);
      printf("Current Tel: %s. Enter New Tel: ", client.tel);
      scanf("%s", client.tel);

      fseek(fp, -sizeof(Client), SEEK_CUR);

      if (fwrite(&client, sizeof(Client), 1, fp) != 1) {
        printf("Error writing to file!\n");
      } else {
        printf("Client updated successfully.\n");
      }

      fclose(fp);
      return;
    }
  }

  printf("Client not found.\n");
  fclose(fp);
}

void delete_client() {
  int id;
  Client client;
  Account account;
  bool found = false;

  FILE *fc = fopen(CLIENT_FILE, "rb");
  FILE *fa = fopen(ACCOUNT_FILE, "rb");

  FILE *temp_c = fopen("temp_c.dat", "wb");
  FILE *temp_a = fopen("temp_a.dat", "wb");

  if (!fc || !fa || !temp_c || !temp_a) {
    printf("Error opening file!\n");

    if (fc)
      fclose(fc);
    if (fa)
      fclose(fa);
    if (temp_c)
      fclose(temp_c);
    if (temp_a)
      fclose(temp_a);

    return;
  }

  printf("\nEnter Client ID to delete: ");
  scanf("%d", &id);

  while (fread(&account, sizeof(Account), 1, fa)) {
    if (account.client_id != id) {
      fwrite(&account, sizeof(Account), 1, temp_a);
    } else {
      found = true;
    }
  }

  while (fread(&client, sizeof(Client), 1, fc)) {
    if (client.id != id) {
      fwrite(&client, sizeof(Client), 1, temp_c);
    }
  }

  if (!found) {
    printf("Client not found!\n");
    remove("temp_c.dat");
    remove("temp_a.dat");
    fclose(fc);
    fclose(fa);
    fclose(temp_c);
    fclose(temp_a);
    return;
  }

  fclose(fa);
  fclose(temp_a);
  fclose(fc);
  fclose(temp_c);

  remove(ACCOUNT_FILE);
  rename("temp_a.dat", ACCOUNT_FILE);

  remove(CLIENT_FILE);
  rename("temp_c.dat", CLIENT_FILE);

  printf("Client deleted successfully!\n");
}

void print_menu() {
  printf("\n");
  printf("╔══════════════════════════════════════╗\n");
  printf("║     BANK MANAGEMENT SYSTEM           ║\n");
  printf("╠══════════════════════════════════════╣\n");
  printf("║  1. Add client                       ║\n");
  printf("║  2. Modify client                    ║\n");
  printf("║  3. Search for client                ║\n");
  printf("║  4. Delete client                    ║\n");
  printf("║  5. Account Management               ║\n");
  printf("║  6. Exit                             ║\n");
  printf("╚══════════════════════════════════════╝\n");
  printf(">> ");
}

void account_functions() {
  int choice;
  float money;

  do {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║     BANK ACCOUNT MANAGEMENT SYSTEM   ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1 - Deposit Money                   ║\n");
    printf("║  2 - Transfer Funds                  ║\n");
    printf("║  3 - Withdraw Money                  ║\n");
    printf("║  4 - Modify Account Type             ║\n");
    printf("║  5 - Account Inquiry                 ║\n");
    printf("║  6 - Quit                            ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      money = get_amount();
      deposit(money);
      break;

    case 2:
      transfer();
      break;

    case 3:
      money = get_amount();
      withdraw(money);
      break;

    case 4:
      modify_type();
      break;

    case 5:
      account_inquiry();
      break;

    case 6:
      printf("Quiting ...");
      break;

    default:
      printf("invalid option \n");
      return;
    }
  } while (choice != 6);
}

int main() {
  int choice;
  system("clear");

  do {
    print_menu();
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      add_client();
      break;
    case 2:
      modify_client();
      break;
    case 3:
      search_client();
      break;
    case 4:
      delete_client();
      break;
    case 5:
      account_functions();
      break;
    case 6:
      printf("Quiting ... \n");
      break;
    default:
      printf("Invalid choice!\n");
    }
  } while (choice != 6);

  return 0;
}
