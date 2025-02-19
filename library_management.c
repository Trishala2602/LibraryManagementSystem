#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a book
typedef struct Book {
    int id;
    char title[100];
    char author[100];
    int available;  // 1 if available, 0 if issued
    struct Book *next;  // Pointer for linked list
} Book;

Book *head = NULL; // Head pointer for the linked list

// Function prototypes
void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();
void deleteBook();
void saveToFile();
void loadFromFile();
void menu();

int main() {
    loadFromFile(); // Load books from file on startup
    menu(); // Display menu
    return 0;
}

// Function to display the menu
void menu() {
    int choice;
    do {
        printf("\n📚 Library Management System 📚\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: deleteBook(); break;
            case 7: saveToFile(); printf("Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 7);
}

// Function to add a new book
void addBook() {
    Book *newBook = (Book *)malloc(sizeof(Book));

    printf("Enter Book ID: ");
    scanf("%d", &newBook->id);
    getchar();

    printf("Enter Book Title: ");
    fgets(newBook->title, 100, stdin);
    newBook->title[strcspn(newBook->title, "\n")] = '\0';

    printf("Enter Author Name: ");
    fgets(newBook->author, 100, stdin);
    newBook->author[strcspn(newBook->author, "\n")] = '\0';

    newBook->available = 1;
    newBook->next = head;
    head = newBook;

    saveToFile();
    printf("Book added successfully!\n");
}

// Function to display all books
void displayBooks() {
    Book *temp = head;

    if (!temp) {
        printf("No books available.\n");
        return;
    }

    printf("\n📖 Available Books 📖\n");
    while (temp) {
        printf("ID: %d | Title: %s | Author: %s | %s\n",
               temp->id, temp->title, temp->author,
               temp->available ? "Available" : "Issued");
        temp = temp->next;
    }
}

// Function to search for a book by ID
void searchBook() {
    int id;
    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    Book *temp = head;
    while (temp) {
        if (temp->id == id) {
            printf("Book Found!\nTitle: %s\nAuthor: %s\nStatus: %s\n",
                   temp->title, temp->author, temp->available ? "Available" : "Issued");
            return;
        }
        temp = temp->next;
    }
    printf("Book Not Found.\n");
}

// Function to issue a book
void issueBook() {
    int id;
    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    Book *temp = head;
    while (temp) {
        if (temp->id == id) {
            if (temp->available) {
                temp->available = 0;
                saveToFile();
                printf("Book issued successfully!\n");
            } else {
                printf("Book is already issued.\n");
            }
            return;
        }
        temp = temp->next;
    }
    printf("Book Not Found.\n");
}

// Function to return a book
void returnBook() {
    int id;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    Book *temp = head;
    while (temp) {
        if (temp->id == id) {
            if (!temp->available) {
                temp->available = 1;
                saveToFile();
                printf("Book returned successfully!\n");
            } else {
                printf("Book was not issued.\n");
            }
            return;
        }
        temp = temp->next;
    }
    printf("Book Not Found.\n");
}

// Function to delete a book by ID
void deleteBook() {
    int id;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    Book *temp = head, *prev = NULL;
    while (temp) {
        if (temp->id == id) {
            if (prev) prev->next = temp->next;
            else head = temp->next;
            free(temp);
            saveToFile();
            printf("Book deleted successfully!\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Book Not Found.\n");
}

// Function to save books to file
void saveToFile() {
    FILE *file = fopen("library_data.txt", "w");
    if (!file) {
        printf("Error saving data.\n");
        return;
    }

    Book *temp = head;
    while (temp) {
        fprintf(file, "%d,%s,%s,%d\n", temp->id, temp->title, temp->author, temp->available);
        temp = temp->next;
    }
    fclose(file);
}

// Function to load books from file at startup
void loadFromFile() {
    FILE *file = fopen("library_data.txt", "r");
    if (!file) return;

    while (!feof(file)) {
        Book *newBook = (Book *)malloc(sizeof(Book));
        if (fscanf(file, "%d,%99[^,],%99[^,],%d\n", &newBook->id, newBook->title, newBook->author, &newBook->available) != 4) {
            free(newBook);
            break;
        }
        newBook->next = head;
        head = newBook;
    }
    fclose(file);
}
