
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_NAME 50
#define NUM_MODULES 5
#define MAX_MODULE_NAME 30

typedef struct Student {
    int id;
    char name[MAX_NAME];
    char student_id[20];
    char module_names[NUM_MODULES][MAX_MODULE_NAME];
    float scores[NUM_MODULES];
    float average;
    char classification[30];
    struct Student* next;
} Student;

Student* head = NULL;
sqlite3* db;

void initialize_database() {
    int rc = sqlite3_open("students.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    char* sql = "CREATE TABLE IF NOT EXISTS students ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT, student_id TEXT, "
                "module1_name TEXT, module1_score REAL, "
                "module2_name TEXT, module2_score REAL, "
                "module3_name TEXT, module3_score REAL, "
                "module4_name TEXT, module4_score REAL, "
                "module5_name TEXT, module5_score REAL, "
                "average REAL, classification TEXT);";

    char* err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

float get_valid_score(const char* module_name) {
    float score;
    char input[100];
    while (1) {
        printf("Enter score for %s (0-100): ", module_name);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        
        char* endptr;
        score = strtof(input, &endptr);
        
        if (endptr == input || *endptr != '\n') {
            printf("Error: Please enter a valid number.\n");
        } else if (score < 0 || score > 100) {
            printf("Error: Score must be between 0 and 100.\n");
        } else {
            return score;
        }
    }
}

float calculate_average(float scores[], int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += scores[i];
    }
    return sum / n;
}

const char* get_classification(float average) {
    if (average >= 69.5) return "1st (First-Class)";
    if (average >= 59.5) return "2.1 (Upper Second-Class)";
    if (average >= 49.5) return "2.2 (Lower Second-Class)";
    if (average >= 39.5) return "3rd";
    return "FAIL";
}

void add_student() {
    Student* new_student = (Student*)malloc(sizeof(Student));
    if (new_student == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter student name: ");
    fgets(new_student->name, MAX_NAME, stdin);
    new_student->name[strcspn(new_student->name, "\n")] = 0;

    printf("Enter student ID: ");
    fgets(new_student->student_id, 20, stdin);
    new_student->student_id[strcspn(new_student->student_id, "\n")] = 0;

    for (int i = 0; i < NUM_MODULES; i++) {
        printf("Enter name for Module %d: ", i + 1);
        fgets(new_student->module_names[i], MAX_MODULE_NAME, stdin);
        new_student->module_names[i][strcspn(new_student->module_names[i], "\n")] = 0;
        new_student->scores[i] = get_valid_score(new_student->module_names[i]);
    }

    new_student->average = calculate_average(new_student->scores, NUM_MODULES);
    strcpy(new_student->classification, get_classification(new_student->average));

    // Insert into database
    char* sql = sqlite3_mprintf("INSERT INTO students (name, student_id, "
                                "module1_name, module1_score, module2_name, module2_score, "
                                "module3_name, module3_score, module4_name, module4_score, "
                                "module5_name, module5_score, average, classification) "
                                "VALUES ('%q', '%q', '%q', %f, '%q', %f, '%q', %f, '%q', %f, '%q', %f, %f, '%q');",
                                new_student->name, new_student->student_id,
                                new_student->module_names[0], new_student->scores[0],
                                new_student->module_names[1], new_student->scores[1],
                                new_student->module_names[2], new_student->scores[2],
                                new_student->module_names[3], new_student->scores[3],
                                new_student->module_names[4], new_student->scores[4],
                                new_student->average, new_student->classification);

    char* err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        new_student->id = sqlite3_last_insert_rowid(db);
        printf("Student added successfully with ID: %d\n", new_student->id);
    }
    sqlite3_free(sql);

    // Add to linked list
    new_student->next = head;
    head = new_student;
}

void display_student(Student* student) {
    printf("\nStudent ID: %d\n", student->id);
    printf("Name: %s\n", student->name);
    printf("Student ID: %s\n", student->student_id);
    printf("Module Scores:\n");
    for (int i = 0; i < NUM_MODULES; i++) {
        printf("%s: %.2f\n", student->module_names[i], student->scores[i]);
    }
    printf("Final Average: %.2f\n", student->average);
    printf("Classification: %s\n", student->classification);
    if (strcmp(student->classification, "1st (First-Class)") == 0) {
        printf("Congratulations! The student qualifies for a graduation prize.\n");
    } else {
        printf("The student does not qualify for a graduation prize.\n");
    }
}

void display_all_students() {
    Student* current = head;
    while (current != NULL) {
        display_student(current);
        current = current->next;
    }
}

void remove_student() {
    char student_id[20];
    printf("Enter the student ID to remove: ");
    fgets(student_id, sizeof(student_id), stdin);
    student_id[strcspn(student_id, "\n")] = 0;

    Student* current = head;
    Student* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->student_id, student_id) == 0) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }

            // Remove from database
            char* sql = sqlite3_mprintf("DELETE FROM students WHERE student_id = '%q';", student_id);
            char* err_msg = 0;
            int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", err_msg);
                sqlite3_free(err_msg);
            } else {
                printf("Student removed successfully.\n");
            }
            sqlite3_free(sql);

            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Student not found.\n");
}

void load_students_from_db() {
    char* sql = "SELECT * FROM students;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Student* new_student = (Student*)malloc(sizeof(Student));
        new_student->id = sqlite3_column_int(stmt, 0);
        strcpy(new_student->name, (const char*)sqlite3_column_text(stmt, 1));
        strcpy(new_student->student_id, (const char*)sqlite3_column_text(stmt, 2));
        for (int i = 0; i < NUM_MODULES; i++) {
            strcpy(new_student->module_names[i], (const char*)sqlite3_column_text(stmt, 3 + i*2));
            new_student->scores[i] = sqlite3_column_double(stmt, 4 + i*2);
        }
        new_student->average = sqlite3_column_double(stmt, 13);
        strcpy(new_student->classification, (const char*)sqlite3_column_text(stmt, 14));

        new_student->next = head;
        head = new_student;
    }

    sqlite3_finalize(stmt);
}

int main() {
    initialize_database();
    load_students_from_db();

    int choice;
    while (1) {
        printf("\nUniversity of Suffolk Graduation Prize Determination Program\n");
        printf("-----------------------------------------------------------\n");
        printf("1. Add new student record\n");
        printf("2. Display all records\n");
        printf("3. Remove student record\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                add_student();
                break;
            case 2:
                display_all_students();
                break;
            case 3:
                remove_student();
                break;
            case 4:
                printf("Exiting program. Goodbye!\n");
                sqlite3_close(db);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
