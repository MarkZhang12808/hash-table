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

struct student {
    string first;
    string last;
    int ID = 0;
    float GPA = 0.0;
};

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
void rehash(node* oldTable[], int& oldSize);
void addRandom(node* table[], int size, int newID, vector<string> first, vector<string> last);

int main(){
    cout << "Student List - Hash Table Edition - commands: ADD, GENERATE, PRINT, DELETE, EXIT" << endl;
    bool running = true;
    char input[10];
    int size = 100;
    node** table = new node*[size];
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
            bool rehashTest = add(table, hashnum, newStudent, size);
            //if(rehashTest == false){
                /*cout << "rehash started through ADD" << endl;
                int newSize = size * 2;
                node** newTable = new node*[newSize];
                cout << "new size: " << newSize << endl;
                for(int i = 0; i != size; i++){
                    if(table[i] != NULL){
                        node* temp = table[i];
                        while(temp->student != NULL){
                            int newHashNumber = hashNumber(temp->student->ID, newSize);
                            add(newTable, newHashNumber, temp->student, newSize);
                            cout << i << endl;
                            temp = temp->next;
                        }
                    }
                }
                delete [] table;
                table = newTable;
                size = newSize;
                cout << "table rehashed" << endl; */
              //  rehash(table, size);
            //}
        }
        else if(strcmp(input, "DELETE") == 0){
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
            //generate students
            int genNum = 0;
            cout << "how many students would u like to generate? " << endl;
            cin >> genNum;
            generate(table, size, genNum);
        }
        else if(strcmp(input, "EXIT") == 0 || strcmp(input, "QUIT") == 0){
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
        table[index] = temp;
        table[index]->next = NULL;
        return true;
    }
    temp->next = list;
    table[index] = temp;
    int collision = 0;
    node* test = table[index]->next;
    while(test != NULL){
        collision++;
        test = test->next;
    }
    cout << "number of collisions: " << collision << endl;
    if(collision == 3){
        cout << "rehashing needed" << endl;
        rehash(table, size);
        return false;
    }
    return true;
}

void delT(node* table[], int idNum, int size){
    int i = hashNumber(idNum, size);
    node* prev = NULL;
    node* runner = table[i];
    while(runner->student->ID != idNum){
        prev = runner;
        runner = runner->next;
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
    /*
    cout << "ID number: " << id << endl;
    int hashNum = 0;
    int firstDigit = 0;
    int xDigit = size/100 + 1;
    int count = 1;
    while(id > 0){
        hashNum = hashNum + id % 10;
        firstDigit = id % 10;
        if(count == xDigit){
            cout << "count: " << count << endl;
            xDigit = firstDigit;
        }
        id = id / 10;
        count++;
    }
    cout << "first digit: " << firstDigit << endl;
    cout << "digit " << xDigit << endl;
        hashNum = hashNum * firstDigit / 5;
        hashNum = hashNum * xDigit/5;
    cout << "Hash result: " << hashNum << endl;
    return hashNum;
    */
    int hashNum = id % size;
    return hashNum;
}

void print(node* table[], int size){
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

void rehash(node* table[], int& size){
    /*int newSize = oldSize * 2;
    cout << "new size: " << newSize << endl;
    for(int i = 0; i != oldSize; i++){
        if(oldTable[i] != NULL){
            node* temp = oldTable[i];
            while(temp->student != NULL){
                int newHashNumber = hashNumber(temp->student->ID, newSize);
                add(newTable, newHashNumber, temp->student, newSize);
                cout << i << endl;
                temp = temp->next;
            }
        }
    }
    cout << "table rehashed" << endl;
    */
    cout << "rehash started" << endl;
    int newSize = size * 2;
    node** newTable = new node*[newSize];
    cout << "new size: " << newSize << endl;
    for(int i = 0; i != size; i++){
        if(table[i] != NULL){
            node* temp = table[i];
            while(temp->student != NULL){
                int newHashNumber = hashNumber(temp->student->ID, newSize);
                add(newTable, newHashNumber, temp->student, newSize);
                cout << i << endl;
                temp = temp->next;
            }
        }
    }
    delete [] table;
    table = newTable;
    size = newSize;
    cout << "table rehashed" << endl;
}

void generate(node* table[], int size, int genNum){
    srand(time(NULL));
    int newID = 0;
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
    for(int ct = 0; ct < genNum; ct++){
        float randomGpa = rand()%500;
        randomGpa = randomGpa/100;
        int random1 = rand() % 10 + 1;
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
        cout << "gpa: " << randomGpa<< endl;
        cout << "first name: " << fName << endl;
        file.close();
        count = 0;
        int random2 = rand() % 10 + 1;
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
        cout << "last name: " << lName << endl;
        file2.close();
        student* newStudent = new student();
        newStudent->first = fName;
        newStudent->last = lName;
        newStudent->ID = newID;
        newStudent->GPA = randomGpa;
        fName.clear();
        lName.clear();
        bool test = add(table, hashNumber(newStudent->ID, size), newStudent, size);
        newID++;
    }
}

