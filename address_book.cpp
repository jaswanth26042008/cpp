#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cctype>
using namespace std;

const int MAX = 100;
const int ID_START = 1001;

struct Contact {
    int id;
    char name[40];
    char phone[12];
    char email[40];
    char address[60];
    char city[20];
};

class AddressBook {

private:
    Contact contacts[MAX];
    int count;
    int nextId;

    int isValidPhone(const char *phone) {
        int len = strlen(phone);

        if (len != 10)
            return 0;

        for (int i = 0; i < len; i++) {
            if (!isdigit(phone[i]))
                return 0;
        }

        return 1;
    }

    int isDuplicatePhone(const char *phone) {
        for (int i = 0; i < count; i++) {
            if (strcmp(contacts[i].phone, phone) == 0)
                return 1;
        }

        return 0;
    }

    int findById(int id) {
        for (int i = 0; i < count; i++) {
            if (contacts[i].id == id)
                return i;
        }

        return -1;
    }

    void strToLower(char *dest, const char *src) {
        int i;

        for (i = 0; src[i] != '\0'; i++) {
            if (src[i] >= 'A' && src[i] <= 'Z')
                dest[i] = src[i] + 32;
            else
                dest[i] = src[i];
        }

        dest[i] = '\0';
    }

    void printLine(char ch, int len) {
        for (int i = 0; i < len; i++)
            cout << ch;

        cout << "\n";
    }

    void printTableHeader() {
        printLine('=', 108);

        cout << "| "
             << left << setw(4) << "ID"
             << " | " << setw(18) << "NAME"
             << " | " << setw(12) << "PHONE"
             << " | " << setw(22) << "EMAIL"
             << " | " << setw(22) << "ADDRESS"
             << " | " << setw(10) << "CITY"
             << " |\n";

        printLine('=', 108);
    }

    void printRow(const Contact &c) {
        cout << "| "
             << left << setw(4) << c.id
             << " | " << setw(18) << c.name
             << " | " << setw(12) << c.phone
             << " | " << setw(22) << c.email
             << " | " << setw(22) << c.address
             << " | " << setw(10) << c.city
             << " |\n";
    }

    void clearScreen() {
        system("cls");
    }

public:

    AddressBook() {
        count = 0;
        nextId = ID_START;
    }

    AddressBook(int startId) {
        count = 0;
        nextId = startId;
    }

    void showBanner() {
        clearScreen();

        cout << "\n\n";

        printLine('*', 60);

        cout << "*                                                          *\n";
        cout << "*          *** ADDRESS BOOK SYSTEM ***                     *\n";
        cout << "*        C++ Object Oriented Programming Project           *\n";
        cout << "*                                                          *\n";

        printLine('*', 60);

        cout << "\n";

        printLine('-', 60);

        cout << "  Student  : [Your Name]\n";
        cout << "  Roll No  : [Your Roll Number]\n";
        cout << "  Subject  : Object Oriented Programming\n";
        cout << "  College  : [Your College Name]\n";

        printLine('-', 60);

        cout << "\n  Press ENTER to continue to the main menu...";

        cin.ignore(1000, '\n');
        cin.get();
    }

    void showMenu() {
        clearScreen();

        cout << "\n";

        printLine('=', 42);

        cout << "=         ADDRESS BOOK SYSTEM          =\n";

        printLine('=', 42);

        cout << "=                                      =\n";
        cout << "=  1. Add New Contact                  =\n";
        cout << "=  2. Display All Contacts             =\n";
        cout << "=  3. Search Contact by Name           =\n";
        cout << "=  4. Search Contact by Phone          =\n";
        cout << "=  5. Update Contact                   =\n";
        cout << "=  6. Delete Contact                   =\n";
        cout << "=  7. Sort Contacts by Name            =\n";
        cout << "=  8. Count Total Contacts             =\n";
        cout << "=  9. Exit                             =\n";
        cout << "=                                      =\n";

        printLine('=', 42);

        cout << "\n  Enter your choice (1-9) : ";
    }

    void addContact() {
        clearScreen();

        cout << "\n";

        printLine('=', 60);

        cout << "             ADD NEW CONTACT\n";

        printLine('=', 60);

        if (count >= MAX) {
            cout << "\n  [ERROR] Address book FULL! Maximum "
                 << MAX << " contacts.\n";
            return;
        }

        Contact c;
        char tempPhone[12];

        cin.ignore(1000, '\n');

        cout << "\n  Enter Full Name          : ";
        cin.getline(c.name, 40);

        if (strlen(c.name) == 0) {
            cout << "  [ERROR] Name cannot be empty!\n";
            return;
        }

        while (1) {
            cout << "  Enter Mobile No (10 digits) : ";
            cin.getline(tempPhone, 12);

            if (!isValidPhone(tempPhone)) {
                cout << "  [ERROR] Must be exactly 10 numeric digits!\n";
                cout << "          Example: 9876543210\n";
                continue;
            }

            if (isDuplicatePhone(tempPhone)) {
                cout << "  [ERROR] Phone already exists! Duplicate not allowed.\n";
                continue;
            }

            strcpy(c.phone, tempPhone);
            break;
        }

        cout << "  Enter Email              : ";
        cin.getline(c.email, 40);

        cout << "  Enter Address            : ";
        cin.getline(c.address, 60);

        cout << "  Enter City               : ";
        cin.getline(c.city, 20);

        c.id = nextId++;

        contacts[count] = c;
        count++;

        cout << "\n";

        printLine('-', 60);

        cout << "  [SUCCESS] Contact added successfully!\n";
        cout << "  Contact ID  : " << c.id << "\n";
        cout << "  Name        : " << c.name << "\n";
        cout << "  Phone       : " << c.phone << "\n";

        printLine('-', 60);
    }

    void displayAll() {
        clearScreen();

        cout << "\n";

        printLine('=', 60);

        cout << "   ALL CONTACTS  [Total: "
             << count << " record(s)]\n";

        printLine('=', 60);

        if (count == 0) {
            cout << "\n  [INFO] No contacts found! Add contacts first.\n";
            return;
        }

        cout << "\n";

        printTableHeader();

        for (int i = 0; i < count; i++)
            printRow(contacts[i]);

        printLine('=', 108);

        cout << "\n  Total contacts : " << count << "\n";
    }

    void searchByName() {
        clearScreen();

        cout << "\n";

        printLine('=', 60);

        cout << "             SEARCH BY NAME\n";

        printLine('=', 60);

        if (count == 0) {
            cout << "\n  [INFO] No contacts in address book.\n";
            return;
        }

        char keyword[40];
        char lKey[40];
        char lName[40];

        cin.ignore(1000, '\n');

        cout << "\n  Enter name to search : ";
        cin.getline(keyword, 40);

        strToLower(lKey, keyword);

        int found = 0;

        cout << "\n  Results for : \"" << keyword << "\"\n\n";

        printTableHeader();

        for (int i = 0; i < count; i++) {

            strToLower(lName, contacts[i].name);

            if (strstr(lName, lKey) != NULL) {
                printRow(contacts[i]);
                found++;
            }
        }

        printLine('=', 108);

        if (!found)
            cout << "\n  [NOT FOUND] No contact with name: "
                 << keyword << "\n";
        else
            cout << "\n  [FOUND] " << found
                 << " matching contact(s).\n";
    }

    void searchByPhone() {
        clearScreen();

        cout << "\n";

        printLine('=', 60);

        cout << "             SEARCH BY PHONE NUMBER\n";

        printLine('=', 60);

        if (count == 0) {
            cout << "\n  [INFO] No contacts in address book.\n";
            return;
        }

        char phone[12];

        cin.ignore(1000, '\n');

        cout << "\n  Enter phone number to search : ";
        cin.getline(phone, 12);

        int found = 0;

        cout << "\n  Result for phone : "
             << phone << "\n\n";

        printTableHeader();

        for (int i = 0; i < count; i++) {

            if (strcmp(contacts[i].phone, phone) == 0) {
                printRow(contacts[i]);
                found++;
                break;
            }
        }

        printLine('=', 108);

        if (!found)
            cout << "\n  [NOT FOUND] No contact with phone: "
                 << phone << "\n";
        else
            cout << "\n  [FOUND] Contact found successfully!\n";
    }

    void updateContact() {
        clearScreen();

        cout << "\n";

        printLine('=', 60);

        cout << "             UPDATE CONTACT\n";

        printLine('=', 60);

        if (count == 0) {
            cout << "\n  [INFO] No contacts available.\n";
            return;
        }

        displayAll();

        int id;

        cout << "\n  Enter Contact ID to update : ";
        cin >> id;

        int idx = findById(id);

        if (idx == -1) {
            cout << "\n  [ERROR] Contact ID "
                 << id << " not found!\n";
            return;
        }

        cout << "\n  Current Contact Details:\n";

        printLine('-', 60);

        cout << "  ID      : " << contacts[idx].id << "\n";
        cout << "  Name    : " << contacts[idx].name << "\n";
        cout << "  Phone   : " << contacts[idx].phone << "\n";
        cout << "  Email   : " << contacts[idx].email << "\n";
        cout << "  Address : " << contacts[idx].address << "\n";
        cout << "  City    : " << contacts[idx].city << "\n";

        printLine('-', 60);

        cout << "\n  SELECT FIELD TO UPDATE:\n";
        cout << "  1. Name\n";
        cout << "  2. Phone\n";
        cout << "  3. Email\n";
        cout << "  4. Address\n";
        cout << "  5. City\n";
        cout << "  6. All Fields\n";

        cout << "\n  Enter choice : ";

        int ch;
        cin >> ch;

        char tp[12];

        switch (ch) {

            case 1:

                cin.ignore(1000, '\n');

                cout << "  New Name    : ";
                cin.getline(contacts[idx].name, 40);

                break;

            case 2:

                cin.ignore(1000, '\n');

                while (1) {

                    cout << "  New Phone (10 digits) : ";
                    cin.getline(tp, 12);

                    if (!isValidPhone(tp)) {
                        cout << "  [ERROR] Invalid phone!\n";
                        continue;
                    }

                    if (isDuplicatePhone(tp) &&
                        strcmp(tp, contacts[idx].phone) != 0) {

                        cout << "  [ERROR] Phone already used!\n";
                        continue;
                    }

                    strcpy(contacts[idx].phone, tp);
                    break;
                }

                break;

            case 3:

                cin.ignore(1000, '\n');

                cout << "  New Email   : ";
                cin.getline(contacts[idx].email, 40);

                break;

            case 4:

                cin.ignore(1000, '\n');

                cout << "  New Address : ";
                cin.getline(contacts[idx].address, 60);

                break;

            case 5:

                cin.ignore(1000, '\n');

                cout << "  New City    : ";
                cin.getline(contacts[idx].city, 20);

                break;

            case 6:

                cin.ignore(1000, '\n');

                cout << "  New Name    : ";
                cin.getline(contacts[idx].name, 40);

                while (1) {

                    cout << "  New Phone (10 digits) : ";
                    cin.getline(tp, 12);

                    if (!isValidPhone(tp)) {
                        cout << "  [ERROR] Invalid phone!\n";
                        continue;
                    }

                    if (isDuplicatePhone(tp) &&
                        strcmp(tp, contacts[idx].phone) != 0) {

                        cout << "  [ERROR] Phone already used!\n";
                        continue;
                    }

                    strcpy(contacts[idx].phone, tp);
                    break;
                }

                cout << "  New Email   : ";
                cin.getline(contacts[idx].email, 40);

                cout << "  New Address : ";
                cin.getline(contacts[idx].address, 60);

                cout << "  New City    : ";
                cin.getline(contacts[idx].city, 20);

                break;

            default:

                cout << "\n  [ERROR] Invalid choice.\n";
                return;
        }

        cout << "\n";

        printLine('-', 60);

        cout << "  [SUCCESS] Contact ID "
             << id << " updated!\n";

        printLine('-', 60);
    }

    void deleteContact() {
        clearScreen();

        cout << "\n";

        printLine('=', 60);

        cout << "             DELETE CONTACT\n";

        printLine('=', 60);

        if (count == 0) {
            cout << "\n  [INFO] No contacts to delete.\n";
            return;
        }

        displayAll();

        int id;

        cout << "\n  Enter Contact ID to delete : ";
        cin >> id;

        int idx = findById(id);

        if (idx == -1) {
            cout << "\n  [ERROR] Contact ID "
                 << id << " not found!\n";
            return;
        }

        cout << "\n  Contact to be DELETED:\n";

        printLine('-', 60);

        cout << "  ID    : " << contacts[idx].id << "\n";
        cout << "  Name  : " << contacts[idx].name << "\n";
        cout << "  Phone : " << contacts[idx].phone << "\n";

        printLine('-', 60);

        char confirm;

        cout << "\n  Confirm DELETE? (y/n) : ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {

            for (int i = idx; i < count - 1; i++)
                contacts[i] = contacts[i + 1];

            count--;

            cout << "\n  [SUCCESS] Contact deleted successfully!\n";
            cout << "  Remaining contacts : " << count << "\n";

        } else {

            cout << "\n  [CANCELLED] No changes made.\n";
        }
    }

    void sortByName() {
        clearScreen();

        cout << "\n";

        printLine('=', 60);

        cout << "             SORT CONTACTS BY NAME\n";

        printLine('=', 60);

        if (count <= 1) {

            cout << "\n  [INFO] Not enough contacts to sort.\n";

            displayAll();

            return;
        }

        Contact temp;

        char a[40];
        char b[40];

        for (int i = 0; i < count - 1; i++) {

            for (int j = 0; j < count - i - 1; j++) {

                strToLower(a, contacts[j].name);
                strToLower(b, contacts[j + 1].name);

                if (strcmp(a, b) > 0) {

                    temp = contacts[j];

                    contacts[j] = contacts[j + 1];

                    contacts[j + 1] = temp;
                }
            }
        }

        cout << "\n  [SUCCESS] Contacts sorted alphabetically!\n\n";

        displayAll();
    }

    void countContacts() {

        clearScreen();

        cout << "\n";

        printLine('=', 60);

        cout << "             CONTACT COUNT\n";

        printLine('=', 60);

        cout << "\n";

        printLine('-', 42);

        cout << "  Total Contacts Stored   : "
             << count << "\n";

        cout << "  Maximum Capacity        : "
             << MAX << "\n";

        cout << "  Remaining Free Slots    : "
             << (MAX - count) << "\n";

        printLine('-', 42);

        if (count == 0)

            cout << "\n  Status : Address book is EMPTY.\n";

        else if (count == MAX)

            cout << "\n  Status : Address book is FULL!\n";

        else

            cout << "\n  Status : "
                 << (MAX - count)
                 << " more contact(s) can be added.\n";
    }

    void pressEnter() {

        cout << "\n";

        printLine('-', 60);

        cout << "\n  Press ENTER to return to menu...";

        cin.clear();

        cin.ignore(1000, '\n');

        cin.get();
    }

    void run() {

        showBanner();

        int choice;
        int running = 1;

        while (running) {

            showMenu();

            cin >> choice;

            switch (choice) {

                case 1:
                    addContact();
                    pressEnter();
                    break;

                case 2:
                    displayAll();
                    pressEnter();
                    break;

                case 3:
                    searchByName();
                    pressEnter();
                    break;

                case 4:
                    searchByPhone();
                    pressEnter();
                    break;

                case 5:
                    updateContact();
                    pressEnter();
                    break;

                case 6:
                    deleteContact();
                    pressEnter();
                    break;

                case 7:
                    sortByName();
                    pressEnter();
                    break;

                case 8:
                    countContacts();
                    pressEnter();
                    break;

                case 9:

                    clearScreen();

                    cout << "\n\n";

                    printLine('*', 60);

                    cout << "*                                                          *\n";
                    cout << "*      Thank you for using Address Book System!            *\n";
                    cout << "*      Developed by : [Your Name]                          *\n";
                    cout << "*      College      : [Your College Name]                  *\n";
                    cout << "*                                                          *\n";

                    printLine('*', 60);

                    cout << "\n";

                    running = 0;

                    break;

                default:

                    cout << "\n  [ERROR] Invalid choice! Enter 1-9.\n";

                    pressEnter();

                    break;
            }
        }
    }
};

int main() {

    AddressBook ab;

    ab.run();

    return 0;
}
