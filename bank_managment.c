#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ACCOUNTS 50
#define MAX_CLIENTS 50

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
} client;

typedef struct {
  int client_id;
  char type;
  int balance;
  bool blocked;
} account;

client clients[MAX_CLIENTS];
account accounts[MAX_ACCOUNTS];
int client_count = 0;
int account_count = 0;

int calc_age(int birthYear) { return 2026 - birthYear; }

int find_client_index(int id) {
  for (int i = 0; i < client_count; i++) {
    if (clients[i].id == id) return i;
  }
  return -1;
}

int find_account_index(int client_id) {
  for (int i = 0; i < account_count; i++) {
    if (accounts[i].client_id == client_id) {
      return i; // Return the index of the account
    }
  }
  return -1;
}

bool hasAccount(int id) {
  for (int i = 0; i < account_count; i++) {
    if (accounts[i].client_id == id)
      return true;
  }
  return false;
}

int get_amount(){
 int n;	
 printf("enter the amount of money: ");	

   if(scanf("%d",&n) != 1 || n <= 0){
        printf("Invalid amount!\n");
        return 0;
    }

  return n;
}

void deposit(int client_id, int amount){
   int index = find_account_index(client_id);
   if(index == -1){
        printf("Account not found!\n");
        return;
    }

   if(accounts[index].blocked){
    printf("Account is blocked! Cannot perform transaction.\n");
    return;
   }

   accounts[index].balance += amount;
   printf("Deposit successful! New balance: $%d\n", accounts[index].balance);
}

void withdraw(int client_id, int amount){
   int index = find_account_index(client_id);
   if(index == -1){
        printf("Account not found!\n");
        return;
    }
   
   if(accounts[index].blocked ){
    printf("Account is blocked! Cannot perform transaction.\n");
    return;
    }

   if(amount <= 0){
        printf("Invalid amount!\n");
        return;
    }
    if(accounts[index].balance < amount){
        printf("Insufficient balance! Available: $%d\n", accounts[index].balance);
        return;
    }

   accounts[index].balance -= amount;
   printf("Withdrawal successful! New balance: $%d\n", accounts[index].balance);
}

void transfer(int send_id, int resive_id, int amount){
  int s_index = find_account_index(send_id);
  int r_index = find_account_index(resive_id);
  if(s_index == -1 || r_index == -1){
    printf("Account not found!\n");
    return;
    }

  if(accounts[s_index].blocked || accounts[r_index].blocked){
    printf("Account is blocked! Cannot perform transaction.\n");
    return;
    }

  if(accounts[s_index].balance < amount){
    printf("Insufficient balance for transfer!\n");
    return;
    }

   accounts[s_index].balance -= amount;
   accounts[r_index].balance += amount;
}

void modify_type(int client_id){
   int index = find_account_index(client_id);
   if(index == -1){
        printf("Account not found!\n");
        return;
    }

   char choice;
   printf("enter the type you want to transfer:");
   printf("  'p' - Personal (age 18+)\n");
   printf("  'c' - Commercial \n");
   scanf(" %c",&choice);
   accounts[index].type = tolower(choice);
	
}

void account_inquiry(int client_id){
    int index = find_account_index(client_id);
    
    if(index == -1){
        printf("Account not found!\n");
        return;
    }
    
    printf("\n========== ACCOUNT INQUIRY ==========\n");
    printf("Account ID: %d\n", accounts[index].client_id);
    printf("Account Type: %c\n", accounts[index].type);
    printf("Current Balance: $%d\n", accounts[index].balance);
    printf("Account Status: %s\n", accounts[index].blocked ? "blocked" : "active");
    printf("=====================================\n");
}

void add_client() {
  client S;
  account a = {0};
  int guardianId;

  if (account_count >= MAX_ACCOUNTS) {
    printf("Maximum number of accounts reached!\n");
    return;
  }

  printf("\n--- Add New Client  ---\n");
  printf("Enter ID: ");
  scanf("%d", &S.id);

  if (find_client_index(S.id) != -1) {
    printf("Error: Client ID already exists.\n");
    return;
  }

  printf("Name: ");
  scanf("%29s", S.name);
  printf("First name:  ");
  scanf("%29s", S.first_name);
  printf("Date of Birth (DD MM YYYY): ");
  scanf("%d %d %d", &S.dob.day, &S.dob.mounth, &S.dob.year);
  printf("Address: ");
  scanf("%49s", S.address);
  printf("Tel: ");
  scanf("%49s", S.tel);

  printf("\nEnter account type,id:\n");
  printf("  'p' - Personal (age 18+)\n");
  printf("  'c' - Commercial \n");
  printf("  'm' - Minor (under 18, needs guardian)\n");
  printf(">>");
  scanf(" %c", &a.type);

  if (calc_age(S.dob.year) < 18 && (tolower(a.type) == 'p' || tolower(a.type) == 'c')) {
    printf(
        "Client is under 18. Cannot create Personal or Commercial account.\n");
    return;
  }
  if (a.type == 'm') {
    printf("Enter Guardian's Client ID: ");
    scanf("%d", &guardianId);
    if (!hasAccount(guardianId)) {
      printf("Access Denied: Guardian must have an existing account.\n");
      return;
    }
  }

  clients[client_count] = S;
  client_count++;

  a.client_id = S.id;
  a.balance = 0;
  a.blocked = false;

  accounts[account_count] = a;
  account_count++;

  printf("\nAccount created successfully!\n");
  printf("Account ID: %d, Type: %c, Balance: %d\n", a.client_id, a.type,
         a.balance);
}

void search_client() {
  int id;
  printf("\nEnter Client ID to search: ");
  scanf("%d", &id);
  int idx = find_client_index(id);

  if (idx != -1) {
    printf("Client Found: %s %s, Tel: %s\n", clients[idx].first_name,
           clients[idx].name, clients[idx].tel);
  } else {
    printf("Client not found.\n");
  }
}

void modify_client() {
  int id, idx;
  printf("\nEnter Client ID to modify: ");
  scanf("%d", &id);
  idx = find_client_index(id);

  if (idx != -1) {
    printf("Current Name: %s. Enter New Name: ", clients[idx].name);
    scanf("%s", clients[idx].name);
    printf("Current Tel: %s. Enter New Tel: ", clients[idx].tel);
    scanf("%s", clients[idx].tel);
    printf("Client updated successfully.\n");
  } else {
    printf("Client not found.\n");
  }
}

void delete_client() {
  int id, idx, acc_idx = -1;
  printf("\nEnter Client ID to delete: ");
  scanf("%d", &id);
  idx = find_client_index(id);

  if (idx != -1) {
    // 1. Shift Clients array
    for (int i = idx; i < client_count - 1; i++) {
      clients[i] = clients[i + 1];
    }
    client_count--;

    for (int i = 0; i < account_count; i++) {
      if (accounts[i].client_id == id) {
        acc_idx = i;
        break;
      }
    }

    if (acc_idx != -1) {
      for (int i = acc_idx; i < account_count - 1; i++) {
        accounts[i] = accounts[i + 1];
      }
      account_count--;
    }

    printf("Client ID %d and their account have been removed.\n", id);
  } else {
    printf("Client not found.\n");
  }
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
    int choice,id,resive_id,money;
   
    printf("enter your id");
    scanf("%d",&id);
  do{  
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
    switch (choice){
      case 1:
	      money = get_amount();
	      deposit(id,money);
       break;
      case 2:
              money = get_amount();
	      printf("enter resiver id :");
              scanf("%d",&resive_id);
	      transfer(id,resive_id,money);
	      
        break;

      case 3:
              money = get_amount();
              withdraw(id,money);
	break;

      case 4:
              modify_type(id);
	break;
      
      case 5:
              account_inquiry(id);
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
