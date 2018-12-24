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
	vector<int> remainingPrefs;
	string name;
};

struct ClassInfo
{
	int type,
		size;
	string name;

	// type 1: , type 2: , type 3: 
};

struct StudentSelection
{
	size_t class_id,
		period,
		studentI,
		cost;
};

void initializeStudents(vector<studentInfo> &students)
{
	CSVReader<12> in("input-student.csv");
	in.read_header(ignore_no_column, "grade", "pref0", "pref1", "pref2", "pref3", "pref4", "pref5", "pref6", "pref7", "pref8", "pref9", "name");
	int grade, pref0, pref1, pref2, pref3, pref4, pref5, pref6, pref7, pref8, pref9; string name;
	while (in.read_row(grade, pref0, pref1, pref2, pref3, pref4, pref5, pref6, pref7, pref8, pref9, name))
	{
		auto pref = vector<int> { pref0, pref1, pref2, pref3, pref4, pref5, pref6, pref7, pref8, pref9 };
		for (size_t i = 0; i < pref.size() - 1; i++)
		{
			for (size_t j = i + 1; j < pref.size(); j++)
			{
				if (pref[j] == pref[i])
				{
					pref.erase(pref.begin() + j);
					j--;
				}
			}
		}

		students.push_back({
				grade,
				pref,
				pref,
				name });
	}

	return;
}

void initializeClasses(vector<ClassInfo> &classes)
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
	vector<ClassInfo> &classes,
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
	vector<ClassInfo> &classes,
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
				const size_t size = 6;
				int grades[size];
				for (size_t i = 0; i < size; i++)
				{ grades[i] = 0; }

				if (classes[classI].size < (int) currClassPeriod.size())
				{ return INT_MAX; }

				for (size_t stuI = 0; stuI < currClassPeriod.size(); stuI++)
				{
					auto& currentStudent = students[currClassPeriod[stuI]];
					grades[currentStudent.grade - 6]++;
					int prefCost = find(
							currentStudent.remainingPrefs.begin(),
							currentStudent.remainingPrefs.end(),
							classI)
						- currentStudent.remainingPrefs.begin();
					cost += prefCost;
					emptyPeriods--;
				}


				for (size_t gradeI = 0; gradeI < size; gradeI++)
				{
					auto minmax = minmax_element(grades, grades + size);
					auto diff = *minmax.second - *minmax.first + 1;
					cost += diff * diff * diff * diff;
				}
			}
		}
	}

	return cost + emptyPeriods * 128;
}

int main()
{
	int numPeriods = 3, numPrefs = 10;
	vector<studentInfo> students;
	vector<ClassInfo> classes;

	initializeStudents(students);
	initializeClasses(classes);

	vector<int> preferenceStats(numPrefs), classStatsA(classes.size()), classStatsC(classes.size());
	vector<vector<int>> classStatsB(classes.size(), vector<int>(numPrefs));
	vector<vector<vector<int>>> gradeStats(classes.size(), vector<vector<int>> (numPeriods, vector<int> (6)));

	vector<int> studentIdx(students.size());
	vector<vector<vector<int>>> classSchedule(int(classes.size()), vector<vector<int>>(numPeriods));
	vector<vector<int>> studentSchedule(int(students.size()), vector<int>(numPeriods, -1)),
		studentScheduleInfo(int(students.size()), vector<int>(numPeriods, -1));

	for (size_t i = 0; i < studentIdx.size(); i++)
	{
		studentIdx[i] = i;
	}

	// creates valid schedule

	int cost = INT_MAX;

	auto bestSelections = vector<StudentSelection>();
	for (size_t iStudent = 0; iStudent < students.size(); iStudent++)
	{
		for (size_t iPref = 0; iPref < students[iStudent].prefs.size(); iPref++)
		{
			auto class_id = students[iStudent].prefs[iPref];
			classStatsA[class_id]++;
			classStatsB[class_id][iPref]++;
		}
	}

	srand(0);
	while(true)
	{
		random_shuffle(studentIdx.begin(), studentIdx.end());
		bestSelections.clear();

		for (size_t iStudentIdx = 0; iStudentIdx < studentIdx.size(); iStudentIdx++)
		{
			size_t iStudent = studentIdx[iStudentIdx];

			auto& remainingPrefs = students[iStudent].remainingPrefs;
			auto& currStuScheduleInfo = studentScheduleInfo[iStudent];
			auto studentSelection = StudentSelection
			{
				0,
				0,
				iStudent,
				INT_MAX
			};

			for (size_t iPref = 0;
				iPref < min((size_t)10, remainingPrefs.size());
				iPref++)
			{
				auto class_id = remainingPrefs[iPref];
				auto choiceCost = 1 << (2 * max((long)iPref - 3, (long)0));

				if (class_id >= 0)
				{
					auto& currClass = classes[class_id];
					auto& currClassSchedule = classSchedule[class_id];

					if (currClass.type > 1
						&& find(currStuScheduleInfo.begin(), currStuScheduleInfo.end(), currClass.type) != currStuScheduleInfo.end())
					{ continue; }

					size_t randI = (int)rand();

					for (size_t tmpPeriodI = randI; tmpPeriodI < randI + numPeriods; tmpPeriodI++)
					{
						int periodI = tmpPeriodI % 3;
						if (currStuScheduleInfo[periodI] != -1) { continue; }

						auto& currClassPeriod = currClassSchedule[periodI];
						if (currClassPeriod.size() >= currClass.size) continue;

						currClassPeriod.push_back(iStudent);
						int newCost = findCost(
							classSchedule,
							students,
							classes,
							numPeriods);

						newCost += choiceCost;

						if (newCost < studentSelection.cost)
						{
							studentSelection.cost = newCost;
							studentSelection.class_id = class_id;
							studentSelection.period = periodI;
						}

						currClassPeriod.pop_back();
						assert(currClassPeriod.size() < currClass.size);
					}
				}
			}

			if (studentSelection.cost < INT_MAX)
			{
				bestSelections.push_back(studentSelection);
			}
		}

		sort(bestSelections.begin(), bestSelections.end(), [](auto l, auto r) {return l.cost < r.cost; });
		auto studentsPicked = set<size_t>();
		auto classPicked = set<tuple<size_t, size_t>>();

		if (bestSelections.size() == 0)
		{ break; }

		auto bestCost = bestSelections[0].cost;
		for (auto itr = bestSelections.begin(); itr < bestSelections.end(); itr++)
		{
			auto student_id = itr->studentI;
			auto class_id = itr->class_id;
			auto period = itr->period;
			auto cost = itr->cost;
			if (cost > bestCost) break;

			auto classComboTupl = make_tuple(class_id, period);
			if (studentsPicked.count(student_id) > 0
				|| classPicked.count(classComboTupl) > 0)
			{ continue; }

			auto &student = students[student_id];
			auto studentPref = find(student.prefs.begin(), student.prefs.end(), class_id) - student.prefs.begin();

			assert(numPeriods - count(studentScheduleInfo[student_id].begin(), studentScheduleInfo[student_id].end(), -1) == student.prefs.size() - student.remainingPrefs.size());

			student.remainingPrefs.erase(find(student.remainingPrefs.begin(), student.remainingPrefs.end(), class_id));

			studentSchedule[student_id][period] = class_id;
			studentScheduleInfo[student_id][period] = classes[class_id].type;
			classSchedule[class_id][period].push_back(student_id);
			assert(classSchedule[class_id][period].size() <= classes[class_id].size);
			assert(numPeriods - count(studentScheduleInfo[student_id].begin(), studentScheduleInfo[student_id].end(), -1) == student.prefs.size() - student.remainingPrefs.size());

			if (student.prefs.size() == student.remainingPrefs.size() + numPeriods)
			{ studentIdx.erase(find(studentIdx.begin(), studentIdx.end(), student_id)); }

			preferenceStats[studentPref]++;
			classStatsC[class_id]++;
			gradeStats[class_id][period][student.grade - 6]++;

			studentsPicked.insert(student_id);
			classPicked.insert(classComboTupl);
		}
	}

	// attempts to fill empty slots

	//for (size_t stuI = 0; stuI < studentSchedule.size(); stuI++)
	//{
	//	auto& currStuSchedule = studentSchedule[stuI];
	//	auto& currStuScheduleInfo = studentScheduleInfo[stuI];

	//	for (int periodI = 0; periodI < numPeriods; periodI++)
	//	{
	//		if (currStuSchedule[periodI] < 0)
	//		{
	//			for (size_t classI = 0; classI < classSchedule.size(); classI++)
	//			{
	//				auto currClass = classes[classI];

	//				if (currClass.size - classSchedule[classI][periodI].size() > 0
	//					&& find(currStuSchedule.begin(), currStuSchedule.end(), classI) == currStuSchedule.end()
	//					&& find(currStuScheduleInfo.begin(), currStuScheduleInfo.end(), currClass.type) == currStuScheduleInfo.end())
	//				{
	//					currStuSchedule[periodI] = classI;
	//					classSchedule[classI][periodI].push_back(stuI);
	//					classStatsC[classI]++;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

	printInputOutput(students, classes, studentSchedule, classSchedule, numPeriods);

	for (size_t i = 0; i < preferenceStats.size(); i++)
	{
		cout << "# of people who got their choice " << i + 1 << ": " << preferenceStats[i] << endl;
	}

	cout << endl;

	for (size_t i = 0; i < classStatsA.size(); i++)
	{
		cout << classes[i].name << " was preferred " << classStatsA[i] << " times { ";

		for (size_t j = 0; j < classStatsB.size(); j++)
		{
			cout << classStatsB[i][j] << " ";
		}

		cout << "} and was given " << classStatsC[i] << " times" << endl;
	}

	cout << endl;

	for (size_t i = 0; i < gradeStats.size(); i++)
	{
		cout << classes[i].name << ": " << endl;

		for (size_t j = 0; j < gradeStats[i].size(); j++)
		{
			cout << "{ ";

			for (size_t k = 0; k < gradeStats[i][j].size(); k++)
			{
				cout << gradeStats[i][j][k] << " ";
			}

			cout << "}" << endl;
		}

		cout << endl;
	}

	return 0;
}
