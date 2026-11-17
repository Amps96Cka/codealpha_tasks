/*
 * CodeAlpha C Programming Internship - Task 3
 * Student Management System
 * Menu-driven program: Add, Display, Search, Update, Delete.
 * Uses structures + binary file handling so records are stored permanently.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME   "students.dat"
#define TEMPFILE   "temp.dat"
#define NAME_LEN   50
#define COURSE_LEN 50

/* One student record. This exact struct is written to / read from the file. */
typedef struct {
    int   id;
    char  name[NAME_LEN];
    int   age;
    char  course[COURSE_LEN];
    float gpa;
} Student;

/* ---------- Safe input helpers (all use fgets, so no buffer problems) ---------- */

void readString(const char *prompt, char *buf, int size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, size, stdin) == NULL) {
            printf("\nInput ended. Exiting.\n");
            exit(0);
        }
        buf[strcspn(buf, "\n")] = '\0';          /* strip the newline */
        if (buf[0] != '\0') return;
        printf("  Input cannot be empty.\n");
    }
}

int readInt(const char *prompt, int min, int max) {
    char line[64], *end;
    long value;
    while (1) {
        readString(prompt, line, sizeof line);
        value = strtol(line, &end, 10);
        if (*end == '\0' && value >= min && value <= max) return (int)value;
        printf("  Enter a whole number between %d and %d.\n", min, max);
    }
}

float readFloat(const char *prompt, float min, float max) {
    char line[64], *end;
    double value;
    while (1) {
        readString(prompt, line, sizeof line);
        value = strtod(line, &end);
        if (*end == '\0' && value >= min && value <= max) return (float)value;
        printf("  Enter a number between %.1f and %.1f.\n", min, max);
    }
}

/* ---------- Small utilities ---------- */

/* Case-insensitive "does text contain pattern?" */
int containsIgnoreCase(const char *text, const char *pattern) {
    size_t n = strlen(text), m = strlen(pattern);
    if (m == 0 || m > n) return 0;
    for (size_t i = 0; i + m <= n; i++) {
        size_t j = 0;
        while (j < m && tolower((unsigned char)text[i + j]) ==
                        tolower((unsigned char)pattern[j])) j++;
        if (j == m) return 1;
    }
    return 0;
}

int idExists(int id) {
    FILE *fp = fopen(FILENAME, "rb");
    Student s;
    if (fp == NULL) return 0;                      /* no file yet = no records */
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) { fclose(fp); return 1; }
    }
    fclose(fp);
    return 0;
}

void printHeader(void) {
    printf("\n%-8s %-22s %-5s %-22s %-5s\n", "ID", "Name", "Age", "Course", "GPA");
    printf("--------------------------------------------------------------------\n");
}

void printStudent(const Student *s) {
    printf("%-8d %-22s %-5d %-22s %-5.2f\n",
           s->id, s->name, s->age, s->course, s->gpa);
}

/* ---------- Features ---------- */

void addStudent(void) {
    Student s;
    FILE *fp;

    printf("\n--- Add Student ---\n");
    s.id = readInt("Student ID (1-999999): ", 1, 999999);
    if (idExists(s.id)) {
        printf("Error: a student with ID %d already exists.\n", s.id);
        return;
    }
    readString("Full name: ", s.name, NAME_LEN);
    s.age = readInt("Age (10-100): ", 10, 100);
    readString("Course: ", s.course, COURSE_LEN);
    s.gpa = readFloat("GPA (0.0-4.0): ", 0.0f, 4.0f);

    fp = fopen(FILENAME, "ab");                    /* "ab" = append binary */
    if (fp == NULL) {
        printf("Error: could not open %s for writing.\n", FILENAME);
        return;
    }
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student added successfully.\n");
}

void displayStudents(void) {
    FILE *fp = fopen(FILENAME, "rb");
    Student s;
    int count = 0;

    printf("\n--- All Students ---\n");
    if (fp == NULL) {
        printf("No records found yet.\n");
        return;
    }
    printHeader();
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printStudent(&s);
        count++;
    }
    fclose(fp);
    if (count == 0) printf("No records found yet.\n");
    else printf("\nTotal students: %d\n", count);
}

void searchStudent(void) {
    FILE *fp;
    Student s;
    int found = 0, choice;

    printf("\n--- Search Student ---\n");
    printf("  1. Search by ID\n  2. Search by name\n");
    choice = readInt("Choose (1-2): ", 1, 2);

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found yet.\n");
        return;
    }

    if (choice == 1) {
        int id = readInt("Enter ID: ", 1, 999999);
        while (fread(&s, sizeof(Student), 1, fp) == 1) {
            if (s.id == id) {
                if (!found) printHeader();
                printStudent(&s);
                found = 1;
                break;
            }
        }
    } else {
        char term[NAME_LEN];
        readString("Enter name (or part of it): ", term, NAME_LEN);
        while (fread(&s, sizeof(Student), 1, fp) == 1) {
            if (containsIgnoreCase(s.name, term)) {
                if (!found) printHeader();
                printStudent(&s);
                found = 1;
            }
        }
    }
    fclose(fp);
    if (!found) printf("No matching student found.\n");
}

void updateStudent(void) {
    FILE *fp = fopen(FILENAME, "rb+");             /* read + write, file must exist */
    Student s;
    int id, found = 0, field;

    printf("\n--- Update Student ---\n");
    if (fp == NULL) {
        printf("No records found yet.\n");
        return;
    }
    id = readInt("Enter ID of student to update: ", 1, 999999);

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) { found = 1; break; }
    }
    if (!found) {
        printf("No student with ID %d.\n", id);
        fclose(fp);
        return;
    }

    printHeader();
    printStudent(&s);
    printf("\nWhich field to update?\n");
    printf("  1. Name\n  2. Age\n  3. Course\n  4. GPA\n");
    field = readInt("Choose (1-4): ", 1, 4);

    switch (field) {
        case 1: readString("New name: ", s.name, NAME_LEN);          break;
        case 2: s.age = readInt("New age (10-100): ", 10, 100);      break;
        case 3: readString("New course: ", s.course, COURSE_LEN);    break;
        case 4: s.gpa = readFloat("New GPA (0.0-4.0): ", 0.0f, 4.0f); break;
    }

    /* Jump back one record and overwrite it in place */
    fseek(fp, -(long)sizeof(Student), SEEK_CUR);
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Record updated successfully.\n");
}

void deleteStudent(void) {
    FILE *fp = fopen(FILENAME, "rb");
    FILE *tmp;
    Student s;
    int id, found = 0;
    char confirm[8];

    printf("\n--- Delete Student ---\n");
    if (fp == NULL) {
        printf("No records found yet.\n");
        return;
    }
    id = readInt("Enter ID of student to delete: ", 1, 999999);

    /* Show the record first and ask for confirmation */
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) { found = 1; break; }
    }
    if (!found) {
        printf("No student with ID %d.\n", id);
        fclose(fp);
        return;
    }
    printHeader();
    printStudent(&s);
    readString("Delete this record? (y/n): ", confirm, sizeof confirm);
    if (tolower((unsigned char)confirm[0]) != 'y') {
        printf("Deletion cancelled.\n");
        fclose(fp);
        return;
    }

    /* Copy every record EXCEPT the deleted one into a temp file */
    rewind(fp);
    tmp = fopen(TEMPFILE, "wb");
    if (tmp == NULL) {
        printf("Error: could not create temporary file.\n");
        fclose(fp);
        return;
    }
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id != id) fwrite(&s, sizeof(Student), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    /* Swap the temp file in as the real file */
    remove(FILENAME);
    rename(TEMPFILE, FILENAME);
    printf("Record deleted successfully.\n");
}

/* ---------- Main menu ---------- */

int main(void) {
    int choice;

    do {
        printf("\n=====================================\n");
        printf("      STUDENT MANAGEMENT SYSTEM\n");
        printf("=====================================\n");
        printf("  1. Add student\n");
        printf("  2. Display all students\n");
        printf("  3. Search student\n");
        printf("  4. Update student\n");
        printf("  5. Delete student\n");
        printf("  6. Exit\n");
        choice = readInt("Choose an option (1-6): ", 1, 6);

        switch (choice) {
            case 1: addStudent();      break;
            case 2: displayStudents(); break;
            case 3: searchStudent();   break;
            case 4: updateStudent();   break;
            case 5: deleteStudent();   break;
            case 6: printf("\nGoodbye! All records are saved in %s.\n", FILENAME); break;
        }
    } while (choice != 6);

    return 0;
}