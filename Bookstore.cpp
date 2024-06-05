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
	char name[101];	
	char title[101];
	char author[101];
	struct Book* left;
	struct Book* right;
}; // AVL Tree

struct Store{
	char storeID[101]; // Foreign Key
	char bookID[101]; // Primary Key
	char name[101];
	char address[101];
	char coordinate[101];
	struct Book* root; // Linked List of AVL Tree
	struct Store* prev;
	struct Store* next;
}*head = NULL, *tail = NULL;

struct Book* createBook(int key, char storeID[], char bookID[], char name[], char title[], char author[]){
	struct Book* book = (struct Book*)malloc(sizeof(struct Book));
	book->key = key;
	strcpy(book->storeID, storeID);
	strcpy(book->bookID, bookID);
	strcpy(book->name, name);
	strcpy(book->title, title);
	strcpy(book->author, author);
	book->left = NULL;
	book->right = NULL;
	return book;
}

int max(int a, int b){
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
	
	root->height = 1 + max(getHeight(root->left), getHeight(root->right));
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	return x;
}

struct Book* rightRotate(struct Book* root){
	struct Book* x = root->left;
	struct Book* subX = x->right;
	
	x->right = root;
	root->left = subX;
	
	root->height = 1 + max(getHeight(root->left), getHeight(root->right));
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	return x;
}

struct Book* insertTree(struct Book* root, int key, char storeID[], char bookID[], char name[], char title[], char author[]){
	if(root == NULL){
		return createBook(key, storeID, bookID, name, title, author);
	}
	else{
		if(key < root->key){
			root->left = insertTree(root->left, key, storeID, bookID, name, title, author);
		}
		else if(key > root->key){
			root->right = insertTree(root->right, key, storeID, bookID, name, title, author);
		}
	}
	
	root->height = 1 + max(getHeight(root->left), getHeight(root->right));
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

void enqueue(char storeID[], char bookID[], char name[], char address[], char coordinate[]){
	struct Store* node = (struct Store*)malloc(sizeof(struct Store));
	strcpy(node->storeID, storeID);
	strcpy(node->bookID, bookID);
	strcpy(node->name, name);
	strcpy(node->address, address);
	strcpy(node->coordinate, coordinate);
	node->prev = NULL;
	node->next = NULL;
	
	// AVL Tree insertion
	
	
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
	if(strcmp(difficultyString, "easy") != 0){
		strcpy(fileName, "bookstore_easy.txt");	
	}
	else if(strcmp(difficultyString, "medium") != 0){
		strcpy(fileName, "bookstore_medium.txt");	
	}
	else if(strcmp(difficultyString, "hard") != 0){
		strcpy(fileName, "bookstore_hard.txt");
	}
	
	char storeID[101];
	char bookID[101];
	char name[101];
	char address[101];
	char coordinate[101];
	FILE* file = fopen(fileName, "r");
	if(file == NULL){
		printf("WARNING! ERROR IN FILE!");
		printf("\n");
		exit(1);
	}
	while(fscanf(file, "%[^#]#%[^#]#%[^#]#%[^#]#%[^#]\n", storeID, bookID, name, address, coordinate) != EOF){
		
	}
	fclose(file);
}

// Front-End Side
void hideCursor(HANDLE handle, bool state) {
	CONSOLE_CURSOR_INFO info;
   	info.dwSize = 100;
   	if (state) {
   		info.bVisible = FALSE;
	}
   	else {
   		info.bVisible = TRUE;	
	}
   	SetConsoleCursorInfo(handle, &info);
}

void clearScreen(){
	system("cls");
}

void Color(int ColorValue){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorValue);
}

void selectOption1(){
	Color(7);
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
	printf("Simulating %s difficulty\n", difficultyString);
	printf("Choose Menu\n");
	Color(9);
	puts("1. View Store <<");
	Color(7);
	puts("2. View Customer");
	puts("3. Navigate");
	puts("4. Exit");
}

void viewCustomerOption(char difficultyString[]){
	Color(7);
	printf("Simulating %s difficulty\n", difficultyString);
	printf("Choose Menu\n");
	puts("1. View Store");
	Color(9);
	puts("2. View Customer <<");
	Color(7);
	puts("3. Navigate");
	puts("4. Exit");
}

void navigateOption(char difficultyString[]){
	Color(7);
	printf("Simulating %s difficulty\n", difficultyString);
	printf("Choose Menu\n");
	puts("1. View Store");
	puts("2. View Customer");
	Color(9);
	puts("3. Navigate <<");
	Color(7);
	puts("4. Exit");
}

void exitOption(char difficultyString[]){
	Color(7);
	printf("Simulating %s difficulty\n", difficultyString);
	printf("Choose Menu\n");
	puts("1. View Store");
	puts("2. View Customer");
	puts("3. Navigate");
	Color(9);
	puts("4. Exit <<");
	Color(7);
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
	hideCursor(consoleHandle, true);
	chooseDifficulty();
	return 0;
}