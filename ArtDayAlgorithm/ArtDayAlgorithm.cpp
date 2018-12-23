#include "stdafx.h"
#include "csv.h"
#include <algorithm>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include<iterator>
#include <list>
#include <math.h>
#include<numeric>
#include <queue>
#include <random>
#include <set>
#include<stack>
#include <string>
#include <tuple>
#include <unordered_set>
#include<utility>
#include <vector>

using namespace io;
using namespace std;

struct studentInfo
{
	int grade;
	vector<int> prefs;
	string name;
};

struct classInfo
{
	int type,
		size;
	string name;

	// type 1: , type 2: , type 3: 
};

void initializeStudents(vector<studentInfo> &students)
{
	CSVReader<12> in("input-student.csv");
	in.read_header(ignore_no_column, "grade", "pref0", "pref1", "pref2", "pref3", "pref4", "pref5", "pref6", "pref7", "pref8", "pref9", "name");
	int grade, pref0, pref1, pref2, pref3, pref4, pref5, pref6, pref7, pref8, pref9; string name;
	while (in.read_row(grade, pref0, pref1, pref2, pref3, pref4, pref5, pref6, pref7, pref8, pref9, name))
	{ students.push_back({ grade, vector<int> { pref0, pref1, pref2, pref3, pref4, pref5, pref6, pref7, pref8, pref9 }, name }); }
	return;
}

void initializeClasses(vector<classInfo> &classes)
{
	CSVReader<3> in("input-class.csv");
	in.read_header(ignore_no_column, "type", "size", "name");
	int type, size; string name;
	while (in.read_row( type, size, name ))
	{ classes.push_back({ type, size, name }); }
	return;
}

void printInputOutput(
	vector<studentInfo> &students,
	vector<classInfo> &classes,
	vector<vector<int>> &studentSchedule,
	vector<vector<vector<int>>> &classSchedule,
	int &numPeriods)
{
	// prints classSchedule

	ofstream classOutput;
	classOutput.open("output-class.csv");

	for (size_t classI = 0; classI < classSchedule.size(); classI++)
	{
		classOutput << classes[classI].name << ":\n";

		for (int period = 0; period < numPeriods; period++)
		{
			classOutput << "P" << period << "(" << classSchedule[classI][period].size() << " students),";

			for (size_t stuI = 0; stuI < classSchedule[classI][period].size(); stuI++)
			{
				auto studentIndex = classSchedule[classI][period][stuI];
				classOutput << students[studentIndex].name << ",";
			}

			classOutput << "\n";
		}
	}

	// prints studentSchedule

	ofstream studentOutput;
	studentOutput.open("output-student.csv");

	studentOutput << "name,P1,P2,P3\n";

	for (size_t stuI = 0; stuI < studentSchedule.size(); stuI++)
	{
		studentOutput << students[stuI].name << ",";

		for (int period = 0; period < numPeriods; period++)
		{
			if (studentSchedule[stuI][period] >= 0) { studentOutput << classes[studentSchedule[stuI][period]].name << ","; }
			else { studentOutput << "NO CLASS,"; }
		}

		studentOutput << "\n";
	}

	// prints stats


}

int findCost(
	vector<vector<vector<int>>> &schedule,
	vector<studentInfo> &students,
	vector<classInfo> &classes,
	int &numPeriods)
{
	int cost = 0, emptyPeriods = students.size() * numPeriods;

	// assigns cost to classSchedule

	for (size_t classI = 0; classI < classes.size(); classI++)
	{
		auto& currClass = schedule[classI];

		for (int periodI = 0; periodI < numPeriods; periodI++)
		{
			auto& currClassPeriod = currClass[periodI];

			if (currClassPeriod.size() > 0)
			{
				vector<int> grades(6, 0);

				if (classes[classI].size < (int) currClassPeriod.size()) { return INT_MAX; }

				for (size_t stuI = 0; stuI < currClassPeriod.size(); stuI++)
				{
					auto& currentStudent = students[currClassPeriod[stuI]];
					grades[currentStudent.grade - 6]++;
					int prefCost = find(
							currentStudent.prefs.begin(),
							currentStudent.prefs.end(),
							classI)
						- currentStudent.prefs.begin();
					cost += prefCost;
					emptyPeriods--;
				}

				for (size_t gradeI = 0; gradeI < grades.size(); gradeI++)
				{ cost += grades[gradeI] * grades[gradeI]; }
			}
		}
	}

	return cost + emptyPeriods * 100;
}

int main()
{
	int numPeriods = 3, numPrefs = 10;
	vector<int> preferenceStats(numPrefs);
	vector<studentInfo> students;
	vector<classInfo> classes;

	initializeStudents(students);
	initializeClasses(classes);

	random_shuffle(students.begin(), students.end());

	vector<vector<vector<int>>> classSchedule(int(classes.size()), vector<vector<int>>(numPeriods));
	vector<vector<int>> studentSchedule(int(students.size()), vector<int>(numPeriods, -1)),
		studentScheduleInfo(int(students.size()), vector<int>(numPeriods, -1));

	// creates valid schedule

	int cost = INT_MAX;

	for (size_t prefI = 0; prefI < numPrefs; prefI++)
	{
		for (size_t stuI = 0; stuI < students.size(); stuI++)
		{
			bool newClass = false;
			auto& student = students[stuI];
			auto& studentPrefs = student.prefs;
			auto& studentPref = studentPrefs[prefI];
			auto& currStuSchedule = studentSchedule[stuI];
			auto& currStuScheduleInfo = studentScheduleInfo[stuI];

			for (size_t idx = find(studentPrefs.begin(), studentPrefs.end(), studentPref) - studentPrefs.begin() + 1;
				idx < studentPrefs.size(); idx++)
			{
				if (studentPrefs[idx] == studentPref)
				{
					studentPrefs[idx] = -1;
				}
			}


			if (studentPref >= 0)
			{
				int newCost;
				auto& currClass = classes[studentPref];
				auto newClassSchedule = classSchedule;
				auto& currClassSchedule = newClassSchedule[studentPref];

				if (currClass.type > 1
					&& find(currStuScheduleInfo.begin(), currStuScheduleInfo.end(), currClass.type) != currStuScheduleInfo.end())
				{ continue; }

				int randI = rand();
				for (int tmpPeriodI = randI; tmpPeriodI < randI + numPeriods; tmpPeriodI++)
				{
					int periodI = tmpPeriodI % 3;
					if (currStuScheduleInfo[periodI] != -1) { continue; }
					auto& currClassPeriod = currClassSchedule[periodI];
					currClassPeriod.push_back(stuI);

					newCost = findCost(
						newClassSchedule,
						students,
						classes,
						numPeriods);

					if (newCost < cost)
					{
						bool invalid = false;
						for (size_t idx = 0; idx < currStuSchedule.size(); idx++)
						{
							if (currStuSchedule[idx] == studentPref)
							{
								currStuSchedule[idx] = -1;
							}
						}

						cost = newCost;
						classSchedule = newClassSchedule;
						currStuSchedule[periodI] = studentPref;
						currStuScheduleInfo[periodI] = currClass.type;
						newClass = true;
					}

					currClassPeriod.pop_back();
				}
			}

			if (newClass)
			{
				preferenceStats[prefI]++;
			}
		}
	}

	// attempts to fill empty slots

	for (size_t stuI = 0; stuI < studentSchedule.size(); stuI++)
	{
		auto& currStuSchedule = studentSchedule[stuI];
		auto& currStuScheduleInfo = studentScheduleInfo[stuI];

		for (int periodI = 0; periodI < numPeriods; periodI++)
		{
			if (currStuSchedule[periodI] < 0)
			{
				for (size_t classI = 0; classI < classSchedule.size(); classI++)
				{
					auto currClass = classes[classI];

					if (currClass.size - classSchedule[classI][periodI].size() > 0
						&& find(currStuSchedule.begin(), currStuSchedule.end(), classI) == currStuSchedule.end()
						&& find(currStuScheduleInfo.begin(), currStuScheduleInfo.end(), currClass.type) == currStuScheduleInfo.end())
					{
						currStuSchedule[periodI] = classI;
						classSchedule[classI][periodI].push_back(stuI);
						break;
					}
				}
			}
		}
	}

	printInputOutput(students, classes, studentSchedule, classSchedule, numPeriods);

	for (size_t i = 0; i < preferenceStats.size(); i++)
	{
		cout << "# with choice" << i + 1 << ": " << preferenceStats[i] << "\n";
	}

	return 0;
}
