//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Michael Abell
// Version     : 1.0
// Description : Project 2
//============================================================================

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


// Define a structure to hold course information
struct Course {
    string courseID;
    string title;
    string prerequisite;
};

// displaying course info in a specific format
void displayCourse(const Course& course) {
    cout << course.courseID << ", " << course.title << endl;
}

void displayCourseReq(const Course& course) {
    cout << endl;
    cout << course.courseID << ", " << course.title << endl;
    cout << "Prerequisites: " << course.prerequisite << endl;
}


// Loading CSV file
vector<Course> loadCourses(const string& csvPath) {
    cout << "Loading CSV file: " << csvPath << endl;

    //Create a vector to house the data
    vector<Course> courses;

    //Make sure it's a good file
    ifstream file(csvPath);
    if (!file) {
        throw runtime_error("Failed to open the file: " + csvPath);
    }
    //Grab data line by line
    string line;
    while (getline(file, line)) {
        string courseId, title, prerequisite;

        // Assuming CSV format: courseId,title,prerequisite
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        //Make sure we have the correct formatting from the file
        if (pos1 == string::npos || pos2 == string::npos) {
            throw runtime_error("Invalid CSV format in file: " + csvPath);
        }

        courseId = line.substr(0, pos1);
        title = line.substr(pos1 + 1, pos2 - pos1 - 1);
        prerequisite = line.substr(pos2 + 1);

        Course course{ courseId, title, prerequisite };
        courses.push_back(course);
    }
    // Always close the file
    file.close();

    return courses;
}

// Implementing quicksort functionality over course.courseID
int partition(vector<Course>& courses, int begin, int end) {
    int low = begin;
    int high = end;

    // Middle element being set as the pivot point for the quicksort
    int pivot = low + (high - low) / 2;

    bool done = false;
    while (!done) {
        // Increment low index while courses[low].courseID < courses[pivot].courseID
        while (courses.at(low).courseID.compare(courses.at(pivot).courseID) < 0) {
            low += 1;
        }

        // Decrement high index while courses[pivot].courseID < courses[high].courseID
        while (courses.at(pivot).courseID.compare(courses.at(high).courseID) < 0) {
            high -= 1;
        }

        if (low >= high) {
            done = true;
        }
        else {
            // Swap the high and low courses using built-in vector methods
            swap(courses.at(low), courses.at(high));
            ++low;
            --high;
        }
    }

    return high;
}

// Performing a quicksort on course ID that's been partitioned
void quickSort(vector<Course>& courses, int begin, int end) {
    int mid = 0;

    if (begin >= end) {
        return;
    }
    /* Partition bids into low and high such that
     midpoint is location of last element in low */
    mid = partition(courses, begin, end);
    // recursively sort low partition (begin to mid)
    quickSort(courses, begin,mid);
    // recursively sort high partition (mid+1 to end)
    quickSort(courses, mid + 1, end);
}
    // Print the menu anytime this called, useful I/O for user
void printMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << endl;
    cout << "What would you like to do? " << endl;
}
    //This is called to print all the courses
void printCourseList(const vector<Course>& courses) {
    for (const auto& course : courses) {
        displayCourse(course);
    }
    cout << endl;
}
    // Search function to match on courseID and then display PreRequisites
void searchCourse(const vector<Course>& courses) {
    string courseID;
    cout << "What course do you want to know about? ";
    cin >> courseID;

    bool courseFound = false;
    for (const auto& course : courses) {
        if (course.courseID == courseID) {
            //this actually prints out the results if there is match of user input
            displayCourseReq(course);
            courseFound = true;
            break;
        }
    }
    // In case user misstypes
    if (!courseFound) {
        cout << "Course ID " << courseID << " not found." << endl;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    string csvPath;
    // 9 gets the user to exit program
    const int EXIT_CHOICE = 9;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    }
    //This vector holds all the course data
    vector<Course> courseInformation;

    //start choice at 0
    int choice = 0;
    //Set this outside the while loop so it only displays at opening
    cout << "Welcome to the course planner." << endl;
    //While loop to take user input until they input the exit choice
    while (choice != EXIT_CHOICE) {
        //Print out the menu for user interface
        printMenu();
        cin >> choice;
        // based on user input functions are called
        switch (choice) {

         // load the csv
        case 1:
            try {
                
                courseInformation = loadCourses(csvPath);

                cout << courseInformation.size() << " course(s) loaded" << endl;
               }
            catch (const exception& e) {
                cerr << "Error loading courses: " << e.what() << endl;
            }
            break;
        // Once loaded, display all the data and sort it first
        case 2:
            cout << "Here is a sample schedule:" << endl;
            cout << endl;

            quickSort(courseInformation, 0, courseInformation.size() - 1);

            printCourseList(courseInformation);

            break;
        // seach a specific course then print it's complete data, print function is within search dunction
        case 3:
            try {               

               searchCourse(courseInformation);
                
            }
            catch (const exception& e) {
                cerr << "Please try again: " << e.what() << endl;
            }
            break;
        }
    }
    //goodbye message for user to know they correctly exited.
    cout << "Thank you for using the course planner!" << endl;
    //kills the program
    return 0;
}
