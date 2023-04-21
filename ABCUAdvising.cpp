//============================================================================
// Name        : ABCUAdvising.cpp
// Author      : Babatunde Ali-Brown
// Date        : 04/15/2023
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "FileParser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseNumber; // unique identifier
    string courseName;
    vector<string> prerequisites;
};

//Print course number and name information to console
void displayCourse(Course course) {
   
    cout << course.courseNumber << ", " << course.courseName << endl;

    return;
}

//Prompt user for course information using console

void getCourse(vector<Course> courses) {
    Course course;
    string courseNumber;
    bool courseFound = false;

    cout << endl;
    cout << "Enter Course Number(For example: CSCI200): ";
    cin.ignore();
    getline(cin, courseNumber);

    for (int i = 0; i < courses.size(); i++) {
        if (courseNumber == courses.at(i).courseNumber) {
            course = courses.at(i);
            courseFound = true;
            break;
        }
    }

    if (courseFound) {
        cout << endl;
        displayCourse(course);

        if (course.prerequisites.size() > 0) {
            cout << "Prerequisites: ";
            for (int i = 0; i < course.prerequisites.size(); i++) {
                cout << course.prerequisites.at(i);
                if (i < course.prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
    else {
        cout << endl;
        cout << "Course Not Found!!" << endl;
    }
}

//Load a CSV file containing courses into a vector

vector<Course> loadCourses(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of courses.
    vector<Course> courses;
    vector<string> courseNumbers;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        for (int i = 0; i < file.rowCount(); i++) {
            courseNumbers.push_back(file[i][0]);
        }
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of courses
            Course course;
            course.courseNumber = file[i][0];
            course.courseName = file[i][1];

            // Check third column of row for a prerequisite and add it to the course
            if (file[i][2] != "" && find(courseNumbers.begin(), courseNumbers.end(), file[i][2]) != courseNumbers.end()) {
                course.prerequisites.push_back(file[i][2]);
            }

            // Check 4th column of row for a prerequisite and add it to the course
            if (file[i][3] != "" && find(courseNumbers.begin(), courseNumbers.end(), file[i][2]) != courseNumbers.end()) {
                course.prerequisites.push_back(file[i][3]);
            }

            courses.push_back(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return courses;
}


/**
 * Partition the vector of courses into two parts, low and high
 *
 * @param courses Address of the vector<Course> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Course>& courses, int begin, int end) {
    //set low and high equal to begin and end
    int lowIndex = begin;
    int highIndex = end;

    // pick the middle element as pivot point
    int midIndex = lowIndex + (highIndex - lowIndex) / 2;
    Course pivotCourse = courses.at(midIndex);

    bool done = false;

    // while not done 
    while (!done) {
        // keep incrementing low index while courses[low] < courses[pivot]
        while (courses.at(lowIndex).courseNumber < pivotCourse.courseNumber) {
            lowIndex += 1;
        }

        // keep decrementing high index while courses[pivot] < courses[high]
        while (pivotCourse.courseNumber < courses.at(highIndex).courseNumber) {
            highIndex -= 1;
        }

        /* If there are zero or one elements remaining,
            all courses are partitioned. Return high */
        if (lowIndex >= highIndex) {
            done = true;
        }
        // else swap the low and high courses (built in vector method)
        else {
            Course tempCourse = courses.at(lowIndex);
            courses.at(lowIndex) = courses.at(highIndex);
            courses.at(highIndex) = tempCourse;

            // move low and high closer ++low, --high
            lowIndex += 1;
            highIndex -= 1;
        }

    }
    //return high;
    return highIndex;
}

/**
 * Perform a quick sort on course courseName
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param courses address of the vector<Course> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Course>& courses, int begin, int end) {
    //set mid equal to 0
    int midIndex = 0;

    /* Base case: If there are 1 or zero courses to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/
    if (begin >= end) {
        return;
    }

    /* Partition courses into low and high such that
     midpoint is location of last element in low */
    midIndex = partition(courses, begin, end);
    // recursively sort low partition (begin to mid)
    quickSort(courses, begin, midIndex);

    // recursively sort high partition (mid+1 to end)
    quickSort(courses, midIndex + 1, end);

}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "ABCU_Data.csv";
    }

    // Define a vector to hold all the courses
    vector<Course> courses;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Vector Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do?";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading courses
            ticks = clock();

            // Complete the method call to load the courses
            courses = loadCourses(csvPath);

            cout << courses.size() << " courses read" << endl;
            cout << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            cout << endl;

            break;

        case 2:
            // Load the courses
            courses = loadCourses(csvPath);

            //Sort Courses
            quickSort(courses, 0, courses.size() - 1);

            cout << "Here is a sample schedule:" << endl;
            cout << endl;

            // Loop and display the sorted courses
            for (int i = 0; i < courses.size(); ++i) {
                displayCourse(courses.at(i));
            }
            cout << endl;

            break;

        case 3:
            // Load the courses
            courses = loadCourses(csvPath);

            //call getCourse method on courses to search course by user input
            getCourse(courses);

            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;

    return 0;
}

