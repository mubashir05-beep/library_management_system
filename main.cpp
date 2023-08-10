#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class Book {
private:
    string title;
    string author;
    int bookId;
    bool isAvailable;

public:
    Book(string title, string author, int bookId)
        : title(title), author(author), bookId(bookId), isAvailable(true) {}

    int getBookId() const {
        return bookId;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    bool checkAvailability() const {
        return isAvailable;
    }

    void borrowBook() {
        isAvailable = false;
    }

    void returnBook() {
        isAvailable = true;
    }

    void displayDetails() const {
        cout << "Book ID: " << bookId << "\n";
        cout << "Title: " << title << "\n";
        cout << "Author: " << author << "\n";
        cout << "Availability: " << (isAvailable ? "Available" : "Not available") << "\n";
    }
};

class Library {
private:
    Book* books[100];
    int numBooks;

public:
    Library() : numBooks(0) {}

    void addBook(Book* book) {
        books[numBooks++] = book;
    }

    Book* getBook(int index) {
        return books[index];
    }

    int getNumBooks() const {
        return numBooks;
    }

    Book* findBook(int bookId) {
        for (int i = 0; i < numBooks; i++) {
            if (books[i]->getBookId() == bookId) {
                return books[i];
            }
        }
        return nullptr;
    }

    void displayAllBooks() {
        cout << "Books in the library:\n";
        for (int i = 0; i < numBooks; i++) {
            books[i]->displayDetails();
            cout << "-----------------------\n";
        }
    }
};

void saveDataToFile(const Library& library) {
    ofstream outFile("library_data.txt");

    if (!outFile) {
        cerr << "Error creating file." << endl;
        return;
    }

    for (int i = 0; i < library.getNumBooks(); i++) {
        Book* book = library.getBook(i);
        outFile << book->getTitle() << "," << book->getAuthor() << "," << book->getBookId() << "," << book->checkAvailability() << endl;
    }

    outFile.close();
}

void loadDataFromFile(Library& library) {
    ifstream inFile("library_data.txt");

    if (!inFile) {
        cerr << "No existing data found." << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string title, author;
        int bookId;
        bool isAvailable;

        getline(ss, title, ',');
        getline(ss, author, ',');
        ss >> bookId >> isAvailable;

        library.addBook(new Book(title, author, bookId));
        if (!isAvailable) {
            Book* book = library.findBook(bookId);
            if (book) {
                book->borrowBook();
            }
        }
    }

    inFile.close();
}

void displayMenu() {
    cout << "\nLibrary Management System\n";
    cout << "1. Find Book by ID\n";
    cout << "2. Display All Books\n";
    cout << "3. Borrow a Book\n";
    cout << "4. Return a Book\n";
    cout << "5. Add Book (Admin)\n";
    cout << "6. Exit\n";
}

int main() {
    Library library;

    // Load data from the file
    loadDataFromFile(library);

    while (true) {
        displayMenu();

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int bookId;
                cout << "Enter book ID: ";
                cin >> bookId;

                Book* foundBook = library.findBook(bookId);

                if (foundBook != nullptr) {
                    cout << "\nBook found:\n";
                    foundBook->displayDetails();
                } else {
                    cout << "\nBook not found.\n";
                }
                break;
            }
            case 2: {
                cout << "\nAll Books in the Library:\n";
                library.displayAllBooks();
                break;
            }
            case 3: {
                int bookId;
                cout << "Enter book ID to borrow: ";
                cin >> bookId;

                Book* foundBook = library.findBook(bookId);

                if (foundBook != nullptr) {
                    if (foundBook->checkAvailability()) {
                        foundBook->borrowBook();
                        cout << "\nYou have successfully borrowed the book.\n";
                    } else {
                        cout << "\nThis book is not available for borrowing.\n";
                    }
                } else {
                    cout << "\nBook not found.\n";
                }
                break;
            }
            case 4: {
                int bookId;
                cout << "Enter book ID to return: ";
                cin >> bookId;

                Book* foundBook = library.findBook(bookId);

                if (foundBook != nullptr) {
                    if (!foundBook->checkAvailability()) {
                        foundBook->returnBook();
                        cout << "\nYou have successfully returned the book.\n";
                    } else {
                        cout << "\nThis book is already available.\n";
                    }
                } else {
                    cout << "\nBook not found.\n";
                }
                break;
            }
            case 5: {
                string title, author;
                int bookId;

                cin.ignore(); // Clear newline character
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter book ID: ";
                cin >> bookId;

                library.addBook(new Book(title, author, bookId));
                cout << "\nBook added successfully.\n";
                break;
            }
            case 6:
                // Save data to a file before exiting
                saveDataToFile(library);
                cout << "\nExiting Library Management System. Goodbye!\n";
                return 0;
            default:
                cout << "\nInvalid choice. Please select a valid option.\n";
                break;
        }
    }

    return 0;
}
