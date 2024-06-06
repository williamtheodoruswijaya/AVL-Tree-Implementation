#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

// Back-End Side
struct Book{
	int key;
	int height;
	char storeID[101]; // Foreign Key
	char bookID[101]; // Primary Key
	char title[101];
	char author[101];
	struct Book* left;
	struct Book* right;
}; // AVL Tree

struct Store{
	char storeID[101]; // Primary Key
	char name[101];
	char address[101];
	char coordinate[101];
	struct Book* root; // Linked List of AVL Tree
	struct Store* prev;
	struct Store* next;
}*head = NULL, *tail = NULL;

struct Book* createBook(int key, char storeID[], char bookID[], char title[], char author[]){
	struct Book* book = (struct Book*)malloc(sizeof(struct Book));
	book->key = key;
	strcpy(book->storeID, storeID);
	strcpy(book->bookID, bookID);
	strcpy(book->title, title);
	strcpy(book->author, author);
	book->height = 1;
	book->left = NULL;
	book->right = NULL;
	return book;
}

int getMax(int a, int b){
	return (a > b) ? a : b;
}

int getHeight(struct Book* root){
	if(root == NULL){
		return 0;
	}
	return root->height;
}

int getBalance(struct Book* root){
	if(root == NULL){
		return 0;
	}
	return getHeight(root->left) - getHeight(root->right);
}

struct Book* leftRotate(struct Book* root){
	struct Book* x = root->right;
	struct Book* subX = x->left;
	
	x->left = root;
	root->right = subX;
	
	root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));
	x->height = 1 + getMax(getHeight(x->left), getHeight(x->right));
	return x;
}

struct Book* rightRotate(struct Book* root){
	struct Book* x = root->left;
	struct Book* subX = x->right;
	
	x->right = root;
	root->left = subX;
	
	root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));
	x->height = 1 + getMax(getHeight(x->left), getHeight(x->right));
	return x;
}

struct Book* insertTree(struct Book* root, int key, char storeID[], char bookID[], char title[], char author[]){
	if(root == NULL){
		return createBook(key, storeID, bookID, title, author);
	}
	else{
		if(key < root->key){
			root->left = insertTree(root->left, key, storeID, bookID, title, author);
		}
		else if(key > root->key){
			root->right = insertTree(root->right, key, storeID, bookID, title, author);
		}
	}
	
	root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));
	int balance = getBalance(root);
	
	// Left-left case
	if(balance > 1 && key < root->left->key){
		return rightRotate(root);
	}
	// Left-right case
	if(balance > 1 && key > root->left->key){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	// Right-right case
	if(balance < -1 && key > root->right->key){
		return leftRotate(root);
	}
	// Right left case
	if(balance < -1 && key < root->right->key){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	return root;
}

void readFiletoTree(struct Book** root, char storeID[], char difficultyString[]){
	char fileName[101];
	if(strcmp(difficultyString, "Easy") == 0){
		strcpy(fileName, "book_easy.txt");	
	}
	else if(strcmp(difficultyString, "Medium") == 0){
		strcpy(fileName, "book_medium.txt");	
	}
	else if(strcmp(difficultyString, "Hard") == 0){
		strcpy(fileName, "book_hard.txt");
	}
	FILE* file = fopen(fileName, "r");
	if(file == NULL){
		printf("WARNING! ERROR IN BOOK FILE!");
		printf("\n");
		exit(1);
	}
	int key;
	char bookID[101];
	char storeIDTemp[101];
	char title[101];
	char author[101];
	while(fscanf(file, "%d#%[^#]#%[^#]#%[^#]#%[^\n]\n", &key, bookID, storeIDTemp, title, author) == 5){
		if(strcmp(storeID, storeIDTemp) == 0){
			*root = insertTree(*root, key, storeID, bookID, title, author);
		}
	}
	fclose(file);
}

void enqueue(char storeID[], char name[], char address[], char coordinate[], char difficultyString[]){
	struct Store* node = (struct Store*)malloc(sizeof(struct Store));
	strcpy(node->storeID, storeID);
	strcpy(node->name, name);
	strcpy(node->address, address);
	strcpy(node->coordinate, coordinate);
	node->prev = NULL;
	node->next = NULL;
	node->root = NULL;
	
	// AVL Tree insertion
	readFiletoTree(&node->root, storeID, difficultyString);
	
	if(head == NULL){
		head = tail = node;
	}
	else{
		tail->next = node;
		node->prev = tail;
		tail = tail->next;
	}
}

void readStore(char difficultyString[]){
	char fileName[101];
	if(strcmp(difficultyString, "Easy") == 0){
		strcpy(fileName, "bookstore_easy.txt");	
	}
	else if(strcmp(difficultyString, "Medium") == 0){
		strcpy(fileName, "bookstore_medium.txt");	
	}
	else if(strcmp(difficultyString, "Hard") == 0){
		strcpy(fileName, "bookstore_hard.txt");
	}
	
	char storeID[101];
	char name[101];
	char address[101];
	char coordinate[101];
	FILE* file = fopen(fileName, "r");
	if(file == NULL){
		printf("WARNING! ERROR IN STORE FILE!");
		printf("\n");
		exit(1);
	}
	while(fscanf(file, "%[^#]#%[^#]#%[^#]#%[^#]#\n", storeID, name, address, coordinate) == 4){
		enqueue(storeID, name, address, coordinate, difficultyString);
	}
	fclose(file);
}

void popHead(){
	if(head == NULL){
		return;
	}
	else if(head == tail){
		free(head);
		head = tail = NULL;
	}
	else{
		struct Store* temp = head;
		head = head->next;
		head->prev = NULL;
		free(temp);
	}
}

void popQueue(){
	while(head){
		popHead();
	}
}

// Front-End Side
void clearScreen(){
	system("cls");
}

void Color(int ColorValue){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorValue);
}

void selectOption1(){
	Color(7);
	puts(" _____                  _    ____              _        _                 ");
	puts("|  __ \\                (_)  |  _ \\            | |      | |                ");
	puts("| |__) |   _ _   _ _ __ _   | |_) | ___   ___ | | _____| |_ ___  _ __ ___ ");
	puts("|  ___/ | | | | | | '__| |  |  _ < / _ \\ / _ \\| |/ / __| __/ _ \\| '__/ _ \\");
	puts("| |   | |_| | |_| | |  | |  | |_) | (_) | (_) |   <\\__ \\ || (_) | | |  __/");
	puts("|_|    \\__, |\\__,_|_|  |_|  |____/ \\___/ \\___/|_|\\_\\___/\\__\\___/|_|  \\___|");
	puts("        __/ |                                                             ");
	puts("       |___/                                                              ");
	puts("Choose Difficulty");
	Color(9);
	printf("1. Easy <<\n");
	Color(7);
	puts("2. Medium");
	puts("3. Hard");
	puts("4. Exit");
}

void selectOption2(){
	Color(7);
	puts(" _____                  _    ____              _        _                 ");
	puts("|  __ \\                (_)  |  _ \\            | |      | |                ");
	puts("| |__) |   _ _   _ _ __ _   | |_) | ___   ___ | | _____| |_ ___  _ __ ___ ");
	puts("|  ___/ | | | | | | '__| |  |  _ < / _ \\ / _ \\| |/ / __| __/ _ \\| '__/ _ \\");
	puts("| |   | |_| | |_| | |  | |  | |_) | (_) | (_) |   <\\__ \\ || (_) | | |  __/");
	puts("|_|    \\__, |\\__,_|_|  |_|  |____/ \\___/ \\___/|_|\\_\\___/\\__\\___/|_|  \\___|");
	puts("        __/ |                                                             ");
	puts("       |___/                                                              ");
	puts("Choose Difficulty");
	puts("1. Easy");
	Color(9);
	printf("2. Medium <<\n");
	Color(7);
	puts("3. Hard");
	puts("4. Exit");
}

void selectOption3(){
	Color(7);
	puts(" _____                  _    ____              _        _                 ");
	puts("|  __ \\                (_)  |  _ \\            | |      | |                ");
	puts("| |__) |   _ _   _ _ __ _   | |_) | ___   ___ | | _____| |_ ___  _ __ ___ ");
	puts("|  ___/ | | | | | | '__| |  |  _ < / _ \\ / _ \\| |/ / __| __/ _ \\| '__/ _ \\");
	puts("| |   | |_| | |_| | |  | |  | |_) | (_) | (_) |   <\\__ \\ || (_) | | |  __/");
	puts("|_|    \\__, |\\__,_|_|  |_|  |____/ \\___/ \\___/|_|\\_\\___/\\__\\___/|_|  \\___|");
	puts("        __/ |                                                             ");
	puts("       |___/                                                              ");
	puts("Choose Difficulty");
	puts("1. Easy");
	puts("2. Medium");
	Color(9);
	printf("3. Hard <<\n");
	Color(7);
	puts("4. Exit");
}

void selectOption4(){
	Color(7);
	puts(" _____                  _    ____              _        _                 ");
	puts("|  __ \\                (_)  |  _ \\            | |      | |                ");
	puts("| |__) |   _ _   _ _ __ _   | |_) | ___   ___ | | _____| |_ ___  _ __ ___ ");
	puts("|  ___/ | | | | | | '__| |  |  _ < / _ \\ / _ \\| |/ / __| __/ _ \\| '__/ _ \\");
	puts("| |   | |_| | |_| | |  | |  | |_) | (_) | (_) |   <\\__ \\ || (_) | | |  __/");
	puts("|_|    \\__, |\\__,_|_|  |_|  |____/ \\___/ \\___/|_|\\_\\___/\\__\\___/|_|  \\___|");
	puts("        __/ |                                                             ");
	puts("       |___/                                                              ");
	puts("Choose Difficulty");
	puts("1. Easy");
	puts("2. Medium");
	puts("3. Hard");
	Color(9);
	printf("4. Exit <<\n");
	Color(7);
}

void viewStoreOption(char difficultyString[]){
	Color(7);
	puts(" _____                  _    ____              _        _                 ");
	puts("|  __ \\                (_)  |  _ \\            | |      | |                ");
	puts("| |__) |   _ _   _ _ __ _   | |_) | ___   ___ | | _____| |_ ___  _ __ ___ ");
	puts("|  ___/ | | | | | | '__| |  |  _ < / _ \\ / _ \\| |/ / __| __/ _ \\| '__/ _ \\");
	puts("| |   | |_| | |_| | |  | |  | |_) | (_) | (_) |   <\\__ \\ || (_) | | |  __/");
	puts("|_|    \\__, |\\__,_|_|  |_|  |____/ \\___/ \\___/|_|\\_\\___/\\__\\___/|_|  \\___|");
	puts("        __/ |                                                             ");
	puts("       |___/                                                              ");
	printf("Simulating %s difficulty\n", difficultyString);
	printf("Choose Menu\n");
	Color(9);
	puts("1. View Store <<");
	Color(7);
	puts("2. View Customer");
	puts("3. Navigate");
	puts("4. Back");
}

void viewCustomerOption(char difficultyString[]){
	Color(7);
	puts(" _____                  _    ____              _        _                 ");
	puts("|  __ \\                (_)  |  _ \\            | |      | |                ");
	puts("| |__) |   _ _   _ _ __ _   | |_) | ___   ___ | | _____| |_ ___  _ __ ___ ");
	puts("|  ___/ | | | | | | '__| |  |  _ < / _ \\ / _ \\| |/ / __| __/ _ \\| '__/ _ \\");
	puts("| |   | |_| | |_| | |  | |  | |_) | (_) | (_) |   <\\__ \\ || (_) | | |  __/");
	puts("|_|    \\__, |\\__,_|_|  |_|  |____/ \\___/ \\___/|_|\\_\\___/\\__\\___/|_|  \\___|");
	puts("        __/ |                                                             ");
	puts("       |___/                                                              ");
	printf("Simulating %s difficulty\n", difficultyString);
	printf("Choose Menu\n");
	puts("1. View Store");
	Color(9);
	puts("2. View Customer <<");
	Color(7);
	puts("3. Navigate");
	puts("4. Back");
}

void navigateOption(char difficultyString[]){
	Color(7);
	puts(" _____                  _    ____              _        _                 ");
	puts("|  __ \\                (_)  |  _ \\            | |      | |                ");
	puts("| |__) |   _ _   _ _ __ _   | |_) | ___   ___ | | _____| |_ ___  _ __ ___ ");
	puts("|  ___/ | | | | | | '__| |  |  _ < / _ \\ / _ \\| |/ / __| __/ _ \\| '__/ _ \\");
	puts("| |   | |_| | |_| | |  | |  | |_) | (_) | (_) |   <\\__ \\ || (_) | | |  __/");
	puts("|_|    \\__, |\\__,_|_|  |_|  |____/ \\___/ \\___/|_|\\_\\___/\\__\\___/|_|  \\___|");
	puts("        __/ |                                                             ");
	puts("       |___/                                                              ");
	printf("Simulating %s difficulty\n", difficultyString);
	printf("Choose Menu\n");
	puts("1. View Store");
	puts("2. View Customer");
	Color(9);
	puts("3. Navigate <<");
	Color(7);
	puts("4. Back");
}

void exitOption(char difficultyString[]){
	Color(7);
	puts(" _____                  _    ____              _        _                 ");
	puts("|  __ \\                (_)  |  _ \\            | |      | |                ");
	puts("| |__) |   _ _   _ _ __ _   | |_) | ___   ___ | | _____| |_ ___  _ __ ___ ");
	puts("|  ___/ | | | | | | '__| |  |  _ < / _ \\ / _ \\| |/ / __| __/ _ \\| '__/ _ \\");
	puts("| |   | |_| | |_| | |  | |  | |_) | (_) | (_) |   <\\__ \\ || (_) | | |  __/");
	puts("|_|    \\__, |\\__,_|_|  |_|  |____/ \\___/ \\___/|_|\\_\\___/\\__\\___/|_|  \\___|");
	puts("        __/ |                                                             ");
	puts("       |___/                                                              ");
	printf("Simulating %s difficulty\n", difficultyString);
	printf("Choose Menu\n");
	puts("1. View Store");
	puts("2. View Customer");
	puts("3. Navigate");
	Color(9);
	puts("4. Back <<");
	Color(7);
}

void viewBook(struct Book* root){
	if (root == NULL) {
        return;
    }
    viewBook(root->left);
    printf("%s - %s\n", root->title, root->author);
    viewBook(root->right);
}

void viewStoreMenus(char difficultyString[]){
	readStore(difficultyString);
	clearScreen();
	printf("Bookstore Lists\n");
	printf("Choose Bookstore:\n");
	struct Store* curr = head;
	int i = 1;
	while(curr){
		printf("%d. %s - %s\n", i++, curr->name, curr->address);
		curr = curr->next;
	}
	printf("0. Back\n");

	int valid = 0;
	char select[101];
	do{
		printf(">> ");
		fgets(select, sizeof(select), stdin);
		select[strcspn(select, "\n")] = 0;
		if(strcmp(select, "0") == 0){
			popQueue();
			return;
		}
		curr = head;
		while(curr){
			if(strcmp(curr->name, select) == 0){
				valid = 1;
				break;
			}
		curr = curr->next;
		}
	}while(valid == 0);

	clearScreen();
    viewBook(curr->root);
	printf("\nPress any key to continue...");
	getch();
	popQueue();
}

int mainMenuSelection(int menuSelection, char difficultyString[]){
	unsigned char control;
	clearScreen();
	while(true){
		clearScreen();
		switch(menuSelection){
			case 1:
				viewStoreOption(difficultyString);
				break;
			case 2:
				viewCustomerOption(difficultyString);
				break;
			case 3:
				navigateOption(difficultyString);
				break;
			case 4:
				exitOption(difficultyString);
				break;
		}
		control = getch();
		if(control == 0 || control == 0xE0){
			control = getch();
		}
		if(control == 72){
			if(menuSelection != 1){
				menuSelection--;
			}
		}
		else if(control == 80){
			if(menuSelection != 4){
				menuSelection++;
			}
		}
		else if(control == 13){
			return menuSelection;
		}
	}
}

void mainMenu(char difficultyString[]){
	int select = 1;
	do{
		clearScreen();
		select = mainMenuSelection(select, difficultyString);
		switch(select){
			case 1:
				viewStoreMenus(difficultyString);
				break;
			case 2:
				
				break;
			case 3:
				
				break;
		}
	}while(select != 4);
}

int selection(int menuSelection){
	unsigned char control;
	clearScreen();
	while(true){
		clearScreen();
		switch(menuSelection){
			case 1:
				selectOption1();
				break;
			case 2:
				selectOption2();
				break;
			case 3:
				selectOption3();
				break;
			case 4:
				selectOption4();
				break;
		}
		control = getch();
		if(control == 0 || control == 0xE0){
			control = getch();
		}
		if(control == 72){
			if(menuSelection != 1){
				menuSelection--;
			}
		}
		else if(control == 80){
			if(menuSelection != 4){
				menuSelection++;
			}
		}
		else if(control == 13){
			return menuSelection;
		}
	}
}

void chooseDifficulty(){
	int select = 1;
	do{	
		clearScreen();
		select = selection(select);	
		switch(select){
			case 1:
				mainMenu("Easy");
				break;
			case 2:
				mainMenu("Medium");
				break;
			case 3:
				mainMenu("Hard");
				break;
		}
	}while(select != 4);
	return;
}

int main(){
	keybd_event(VK_F11, 0, 0, 0); // enter fullscreen mode
	keybd_event(VK_F11, 0, KEYEVENTF_KEYUP, 0);
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	chooseDifficulty();
	return 0;
}