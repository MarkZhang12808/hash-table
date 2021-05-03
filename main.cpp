//Mark Zhang - 1 May 2021 - Hash table - A program that allows the user to modify a hash table that contains information of students
//the user can add and delete students manually, generate a designated number of students, and quit the program

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

//student struct contains 4 parameters for students

struct student {
    string first;
    string last;
    int ID = 0;
    float GPA = 0.0;
};

//node struct to store students in linked list 

struct node{
    student* student = NULL;
    node* next = NULL;
};

//function declarations

void print(node* table[], int size);
bool add(node* table[], int index, student* student, int size);
void delT(node* table[], int idNum, int size);
int hashNumber(int id, int size);
void generate(node* table[], int size, int genNum);
void rehash(node* newTable[], node* table[], int size);

int main(){
    cout << "Student List - Hash Table Edition - commands: ADD, GENERATE, PRINT, DELETE, EXIT" << endl;
    bool running = true;
    char input[10];
    //hash table array size initially set to 100
    int size = 100;
    node** table = new node*[size];
    //hash table is cleared
    for(int i = 0; i < size; i++){
        table[i] = NULL;
    }
    while (running == true){
        cout << "enter command" << endl;
        cin.getline(input, 10);
        if (strcmp(input, "ADD") == 0){
            int idnum = 0;
            char* firstN = new char();
            char* lastN = new char();
            float gpa = 0.0;
            cout << "enter id:" << endl;
            cin.clear();
            cin >> idnum;
            cout << "enter first name: " << endl;
            cin.clear();
            cin.ignore(9999, '\n');
            cin.getline(firstN, 50);
            cout << firstN << endl;
            cout << "enter last name: " << endl;
            cin.clear();
            cin.getline(lastN, 50);
            cout << lastN << endl;
            cout << "enter gpa: " << endl;
            cin.clear();
            cin >> gpa;
            student* newStudent = new student();
            newStudent->first = firstN;
            newStudent->last = lastN;
            newStudent->ID = idnum;
            newStudent->GPA = gpa;
            int hashnum = hashNumber(idnum, size);
            bool test = add(table, hashnum, newStudent, size);
            if(test == false){
                //if the add function indicates that rehashing is needed, rehash students to a table 2x the size of the current one
                size = size*2;
                node** newTable = new node*[size];
                rehash(newTable, table, size);
                delete [] table;
                table = newTable;
            }
        }
        else if(strcmp(input, "DELETE") == 0){
            //take an id number from the user and deletes the corresponding student
            int idNum = 0;
            cout << "enter student's ID number: " << endl;
            cin.clear();
            cin >> idNum;
            delT(table, idNum, size);
        }
        else if(strcmp(input, "PRINT") == 0){
            //print list
            print(table, size);
        }
        else if(strcmp(input, "GENERATE") == 0){
            //generate a designated number of students
            int genNum = 0;
            cout << "how many students would you like to generate? " << endl;
            cin >> genNum;
            generate(table, size, genNum);
        }
        else if(strcmp(input, "EXIT") == 0 || strcmp(input, "QUIT") == 0){
            //quit the program
            running = false;
            cout << "quitting program... " << endl;
            break;
        }
    }
    return 0;
}

bool add(node* table[], int index, student* newStudent, int size){
    node* temp = new node();
    node* list = table[index];
    temp->student = newStudent;
    if(list == NULL){
        //if the table entry is empty make the node containting the student the head of the linked list at the index
        table[index] = temp;
        table[index]->next = NULL;
        return true;
    }
    //if the table is not empty, make the node containing the new student the new head of the linked list
    temp->next = list;
    table[index] = temp;
    int collision = 0;
    node* test = table[index]->next;
    while(test != NULL){
        //count collisions, 4 nodes in a linked list = 3 collisions
        collision++;
        test = test->next;
    }
    cout << "number of collisions: " << collision << endl;
    if(collision >= 3){
        //indicates that rehashing is needed if 3 or more collisions are detected
        cout << "rehashing needed" << endl;
        /*size = size*2;
        node** newTable = new node*[size];
        for (int i = 0; i < size; i++){
            newTable[i] = NULL;
        }
        rehash(newTable, table, size);
        delete[] table;
        table = newTable;
        */
        return false;
    }
    return true;
}

void delT(node* table[], int idNum, int size){
    //applies the hash function to the find the index of the student with the chosen ID
    //goes through linked list at index until student is found and deleted or entire linked list is searched
    int i = hashNumber(idNum, size);
    node* prev = NULL;
    node* runner = table[i];
    while(runner->student->ID != idNum){
        prev = runner;
        runner = runner->next;
        if(runner->next == NULL && runner->student->ID != idNum){
            cout << "student not found" << endl;
            break;
        }
    }
    if(prev == NULL){
        table[i] = NULL;
    }
    else{
        prev->next = runner->next;
    }
    delete runner;
}

int hashNumber(int id, int size){
    //hash function takes the id number mod size, which ensures that automatically incremented IDs are distributed evenly
    //also makes sure that the hash number does not exceed the table index
    int hashNum = id % size;
    return hashNum;
}

void print(node* table[], int size){
    //prints students by looping through the each linked list at every index of the table
    cout << fixed;
    cout << setprecision(2);
    for(int i = 0; i != size; i++){
        node* temp = table[i];
        while(temp != NULL){
            cout << "\n*****************************************" << endl;
            cout << "\tFIRST NAME: "<< temp->student->first << endl;
            cout << "\tLAST NAME:  " << temp->student->last << endl;
            cout << "\tID Number:  " << temp->student->ID << endl;
            cout << "\tGPA:        " << temp->student->GPA << endl;
            cout << "\thash index: " << i << endl;
            temp = temp->next; 
        }
    }
}

void rehash(node* newTable[], node* table[], int size){
    //loops through old table, reassigning hash numbers to the students and transfers them to the new table
    for(int i = 0; i < size/2; i++){
        node* tnode = table[i];
        if(tnode != NULL){
            while(tnode != NULL){
                int hash = hashNumber(tnode->student->ID, size);
                cout << "ID: " << tnode->student->ID << " hash: " << hash << endl;
                add(newTable, hash, tnode->student, size);
                tnode = tnode->next;
            }
        }
    }
    delete[] table;
    table = newTable;
    print(table, size);
    cout << size << endl;
    cout << "table rehash complete" << endl;
}

void generate(node* table[], int size, int genNum){
    srand(time(NULL));
    int newID = 0;
    //loop goes through student list checking that the ID is available
    //if not, increments the id by 1 and then checks from the beginning
    for(int i = 0; i < size; i++){
        if(table[i] != NULL){
            node* temp = table[i];
            while(temp != NULL){
                if(newID == temp->student->ID){
                    newID++;
                    i = 0;
                    break;
                }
                temp = temp->next;
            }
        }
    }
    fstream file;
    fstream file2;
    string firstFile;
    string lastFile;
    char temp;
    string fName;
    string lName;
    cout << "please enter file with first names" << endl;
        cin >> firstFile;
    cout << "please enter file with last names" << endl;
        cin.clear();
        cin >> lastFile;
    //takes files containing first and last names then takes a random name from each
    //a random number determines which name will be picked based on its position in the file and the number of commas before it
    for(int ct = 0; ct < genNum; ct++){
        float randomGpa = rand()%500;
        randomGpa = randomGpa/100;
        int random1 = rand() % 20 + 1;
        file.open(firstFile, ios::in);
        int count = 1;
        while(1){
            file >> temp;
            if(temp != ',' && count == random1){
                fName += temp;
            }
            else if (temp == ','){
                count++;
            }
            else if(temp == ',' && count == random1){
                break;
            }
            if(file.eof()){
                break;
            }
        }
        file.close();
        count = 0;
        int random2 = rand() % 20 + 1;
        file2.open(lastFile, ios::in);
        while(1){
            file2 >> temp;
            if(temp != ',' && count == random2){
                lName += temp;
            }
            else if (temp == ','){
                count++;
            }
            else if(temp == ',' && count == random2){
                break;
            }
            if(file2.eof()){
                break;
            }
        }
        file2.close();
        student* newStudent = new student();
        //student is created with random first and last names, random GPA and incremented ID
        newStudent->first = fName;
        newStudent->last = lName;
        newStudent->ID = newID;
        newStudent->GPA = randomGpa;
        fName.clear();
        lName.clear();
        bool test = add(table, hashNumber(newStudent->ID, size), newStudent, size);
        if(test == false){
            size = size*2;
            node** newTable = new node*[size];
            rehash(newTable, table, size);
            delete [] table;
            table = newTable;
        }
        cout << size << endl;
        newID++;
    }
}

