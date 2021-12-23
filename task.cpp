#include <iostream>
#include <Windows.h>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

// Global Variables
vector<pair<int, pair<string, int>>> tasks_list; // <ID, <Task, Priority> >
vector<pair<int, string>> tasks_done;            // <ID, Task>

void file_read(string file_name)
{
    // Reads the file and stores the tasks in the tasks_list vector

    ifstream file(file_name);
    string taskData;
    int id = 1;
    while (true)
    {
        if (file.eof())
            break;

        // Reads the file line by line
        string priority;
        // split the line into task and priority
        getline(file, priority, ' ');
        getline(file, taskData);

        // if priority is not present, then it is not a valid task
        if (priority == "")
            break;

        tasks_list.push_back(make_pair(id, make_pair(taskData, stoi(priority))));
        id++;
    }
    file.close();
}

void file_write()
{
    // Writes the tasks_done vector to the file
    fstream fp;
    fp.open("task.txt", ios::out);
    if (!fp)
    {
        cout << "File not created!";
        exit(1);
    }

    for (int i = 0; i < (int)tasks_list.size(); i++)
    {
        string data = to_string(tasks_list[i].second.second) + " " + tasks_list[i].second.first + "\n";
        fp << data;
    }
    fp.close();
}

void file_done_read(string file_name)
{
    // Reads the file and stores the tasks in the tasks_done vector

    ifstream file(file_name);
    string line;
    int id = 1;
    while (getline(file, line))
    {
        if (line.length() > 0)
        {
            string task = line;
            tasks_done.push_back(make_pair(id, task));
            id++;
        }
    }
    file.close();
}

void file_done_write()
{
    fstream fp;
    fp.open("completed.txt", ios::out);
    if (!fp)
    {
        cout << "File not created!";
        exit(1);
    }

    for (int i = 0; i < (int)tasks_done.size(); i++)
    {
        string data = tasks_done[i].second;
        fp << data << endl;
    }
    fp.close();
}

void done_task(int id)
{
    // Adds the task to the tasks_done vector
    for (int i = 0; i < (int)tasks_list.size(); i++)
    {
        if (tasks_list[i].first == id)
        {
            tasks_done.push_back(make_pair(id, tasks_list[i].second.first));

            // remove the task from the tasks_list vector
            tasks_list.erase(tasks_list.begin() + i);
            break;
        }
    }

    cout << "Marked item as done." << endl;
    file_write();
    file_done_write();
}

void delete_task(int id)
{
    // Removes the task from the tasks_list vector
    for (int i = 0; i < (int)tasks_list.size(); i++)
    {
        if (tasks_list[i].first == id)
        {
            tasks_list.erase(tasks_list.begin() + i);
            break;
        }
    }

    file_write();
}

void add_task(string task, int priority)
{
    // Adds the task to the tasks_list vector and writes it to the file
    tasks_list.push_back(make_pair(tasks_list.size() + 1, make_pair(task, priority)));
    file_write();
    cout << "Added task: \"" << task << "\" with priority " << priority << endl;
}

bool sort_priority(pair<int, pair<string, int>> a, pair<int, pair<string, int>> b)
{
    // Sorts the tasks_list vector according to priority
    return a.second.second < b.second.second;
}

void list_task()
{
    /*
    // ! Test Scenario
    tasks_list.push_back(make_pair(1, make_pair("Task 1", 3)));
    tasks_list.push_back(make_pair(2, make_pair("Task 2", 1)));
    tasks_list.push_back(make_pair(3, make_pair("Task 3", 2)));
    */

    if (tasks_list.size() == 0)
    {
        cout << "No tasks to list" << endl;
        return;
    }

    // ! Sorting tasks_list by priority (ascending)
    // Using Lambda function to sort tasks_list by priority alternate method can be to make bool cmp function
    sort(tasks_list.begin(), tasks_list.end(), sort_priority);

    // ! Fixing the ID
    int id = 1;
    for (int i = 0; i < (int)tasks_list.size(); i++)
    {
        tasks_list[i].first = id;
        id++;
    }

    // Range based for loop to print tasks_list
    for (int i = 0; i < (int)tasks_list.size(); i++)
    {
        cout << tasks_list[i].first << ". " << tasks_list[i].second.first << " [" << tasks_list[i].second.second << "]" << endl;
    }

    file_write();
}

void list_done()
{

    /*
    // ! Test Scenario
    tasks_done.push_back(make_pair(1, "Task 1"));
    tasks_done.push_back(make_pair(2, "Task 2"));
    tasks_done.push_back(make_pair(3, "Task 3"));
    */
    cout << "Completed : " << tasks_done.size() << endl;
    // Range based for loop to print tasks_done
    for (int i = 0; i < (int)tasks_done.size(); i++)
    {
        cout << tasks_done[i].first << ". " << tasks_done[i].second << endl;
    }
}

void check_cmd(int argc, char *argv[])
{
    // Checking the number of arguments and for help
    if (argc < 2 || strcmp(argv[1], "help") == 0)
    {
        cout << "Usage :-"
             << endl
             << "$ ./task add 2 hello world    # Add a new item with priority 2 and text \"hello world\" to the list"
             << endl
             << "$ ./task ls                   # Show incomplete priority list items sorted by priority in ascending order"
             << endl
             << "$ ./task del INDEX            # Delete the incomplete item with the given index"
             << endl
             << "$ ./task done INDEX           # Mark the incomplete item with the given index as complete"
             << endl
             << "$ ./task help                 # Show usage"
             << endl
             << "$ ./task report               # Statistics"
             << endl;
    } // Addition Command
    else if (strcmp(argv[1], "add") == 0)
    {
        if (argc < 4)
        {
            cout << "Usage :- $ ./task add 2 hello world" << endl;
            return;
        }

        add_task(argv[3], atoi(argv[2]));
    }
    else if (strcmp(argv[1], "ls") == 0)
    {
        list_task();
    }
    else if (strcmp(argv[1], "del") == 0)
    {
        if (argc < 3)
        {
            cout << "Please enter the index of the task to delete" << endl;
            return;
        }

        if (atoi(argv[2]) > (int)tasks_list.size())
        {
            cout << "Error: item with index " << argv[2] << " does not exist. Nothing deleted." << endl;
            return;
        }
        delete_task(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "done") == 0)
    {
        if (argc < 3)
        {
            cout << "Please enter the index of the task to mark as done" << endl;
            return;
        }

        if (atoi(argv[2]) > (int)tasks_list.size())
        {
            cout << "Error: no incomplete item with index " << argv[2] << " exists." << endl;
            return;
        }

        done_task(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "report") == 0)
    {
        cout << "Pending : " << tasks_list.size() << endl;
        list_task();
        cout << endl;
        list_done();
    }
    else
    {
        cout << "Invalid command" << endl;
    }
}

int main(int argc, char *argv[])
{
    /*
    // From StackOverflow

    // Set console code page to UTF-8 so console known how to interpret string data
    SetConsoleOutputCP(CP_UTF8);

    // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
    setvbuf(stdout, nullptr, _IOFBF, 1000);
    */
    tasks_list.clear();
    tasks_done.clear();

    file_read("task.txt");
    file_done_read("completed.txt");
    check_cmd(argc, argv);
}
