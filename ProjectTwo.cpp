//===========================================================
// CS 300 Project Two Final
// Advising Assistance Program
//
// Author: Hakeem Womack
// October 18, 2023
// Purpose:
// This program loads course information from a CSV file
// into a Binary Search Tree (BST). Advisors can:
//
// 1. Load course data
// 2. Display all courses in alphanumeric order
// 3. Display information for a specific course
// 4. Exit the program
//
// A Binary Search Tree was selected because it provides
// efficient searching and naturally supports sorted output
// through an in-order traversal.
//===========================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

//-----------------------------------------------------------
// Course Structure
//-----------------------------------------------------------

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

//-----------------------------------------------------------
// BST Node Structure
//-----------------------------------------------------------

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course newCourse) {
        course = newCourse;
        left = nullptr;
        right = nullptr;
    }
};

//-----------------------------------------------------------
// Binary Search Tree Class
//-----------------------------------------------------------

class BinarySearchTree {

private:
    Node* root;

    // Insert a course into the BST
    void insertNode(Node* node, Course course) {

        if (course.courseNumber < node->course.courseNumber) {

            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                insertNode(node->left, course);
            }
        }
        else {

            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                insertNode(node->right, course);
            }
        }
    }

    // Traverse left, visit node, then traverse right.
    // This naturally displays courses in alphanumeric order.
    void inOrder(Node* node) {

        if (node == nullptr) {
            return;
        }

        inOrder(node->left);

        cout << node->course.courseNumber
             << ", "
             << node->course.courseTitle
             << endl;

        inOrder(node->right);
    }

public:

    BinarySearchTree() {
        root = nullptr;
    }

    //-------------------------------------------------------
    // Insert a course into the BST
    //-------------------------------------------------------

    void insert(Course course) {

        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            insertNode(root, course);
        }
    }

    //-------------------------------------------------------
    // Search for a course by course number
    //-------------------------------------------------------

    Course search(string courseNumber) {

        Node* current = root;

        while (current != nullptr) {

            if (courseNumber == current->course.courseNumber) {
                return current->course;
            }

            if (courseNumber < current->course.courseNumber) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        // Return an empty course if not found
        return Course();
    }

    //-------------------------------------------------------
    // Print all courses in sorted order
    //-------------------------------------------------------

    void printCourseList() {

        cout << "\nHere is a sample schedule:\n" << endl;

        inOrder(root);
    }
};

//-----------------------------------------------------------
// Global Data
//-----------------------------------------------------------

BinarySearchTree bst;

// Store all courses so prerequisite titles
// can be displayed when requested.
vector<Course> allCourses;

//-----------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------

// Convert course IDs to uppercase so searches work
// whether the user types uppercase or lowercase.
string toUpperCase(string text) {

    transform(text.begin(),
              text.end(),
              text.begin(),
              ::toupper);

    return text;
}

// Find a course title based on the course number.
// This allows prerequisite titles to be displayed.
string getCourseTitle(string courseNumber) {

    for (const Course& course : allCourses) {

        if (course.courseNumber == courseNumber) {
            return course.courseTitle;
        }
    }

    return "";
}

//-----------------------------------------------------------
// Load Course Data
//-----------------------------------------------------------

// Read each line from the CSV file and build a
// Course object before inserting it into the BST.
void loadCourseData(string fileName) {

    ifstream file(fileName);

    if (!file.is_open()) {

        cout << "Error: Unable to open file." << endl;
        return;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);
        string token;

        Course course;

        // First field is the course number
        getline(ss, course.courseNumber, ',');

        // Second field is the course title
        getline(ss, course.courseTitle, ',');

        course.courseNumber =
            toUpperCase(course.courseNumber);

        // Remaining fields are prerequisites
        while (getline(ss, token, ',')) {

            if (!token.empty()) {

                course.prerequisites.push_back(
                    toUpperCase(token)
                );
            }
        }

        bst.insert(course);
        allCourses.push_back(course);
    }

    file.close();

    cout << "Data loaded successfully." << endl;
}

//-----------------------------------------------------------
// Display Course Information
//-----------------------------------------------------------

// Display detailed information for a single course.
void displayCourseInformation(string courseNumber) {

    courseNumber = toUpperCase(courseNumber);

    Course course = bst.search(courseNumber);

    // If the search returns an empty course,
    // the course number was not found.
    if (course.courseNumber.empty()) {

        cout << "Course not found." << endl;
        return;
    }

    cout << endl;
    cout << course.courseNumber
         << ", "
         << course.courseTitle
         << endl;

    if (course.prerequisites.empty()) {

        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites:" << endl;

    for (const string& prereq : course.prerequisites) {

        cout << prereq;

        string title = getCourseTitle(prereq);

        if (!title.empty()) {
            cout << ", " << title;
        }

        cout << endl;
    }
}

//-----------------------------------------------------------
// Main Program
//-----------------------------------------------------------

int main() {

    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;

        cout << "What would you like to do? ";

        // Validate menu input before using it.
        if (!(cin >> choice)) {

            cin.clear();
            cin.ignore(1000, '\n');

            cout << "Invalid input." << endl;
            continue;
        }

        switch (choice) {

        case 1:

            cout << "Enter file name: ";

            cin.ignore();
            getline(cin, fileName);

            loadCourseData(fileName);
            break;

        case 2:

            bst.printCourseList();
            break;

        case 3:

            cout << "What course do you want to know about? ";

            cin >> courseNumber;

            displayCourseInformation(courseNumber);
            break;

        case 9:

            cout << "Thank you for using the course planner!"
                 << endl;
            break;

        default:

            cout << choice
                 << " is not a valid option."
                 << endl;
        }
    }

    return 0;
}