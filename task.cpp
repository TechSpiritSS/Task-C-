#include <iostream>
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
    ifstream file(file_name);
    string line;
    int id = 1;
    while (getline(file, line))
    {
        if (line.length() > 0)
        {
            int priority = line[0] - '0';
            string task = line.substr(2, line.length() - 1);
            tasks_list.push_back(make_pair(id, make_pair(task, priority)));
            id++;
        }
    }
    file.close();
}

void file_write()
{
    FILE *fp = fopen("task.txt", "w");
    if (fp == NULL)
    {
        cout << "Error opening file";
        exit(1);
    }
    for (int i = 0; i < (int)tasks_list.size(); i++)
    {
        fprintf(fp, "%d %s\n", tasks_list[i].second.second, tasks_list[i].second.first.c_str());
    }
    fclose(fp);
}

void file_done_read(string file_name)
{
    ifstream file(file_name);
    string line;
    int id = 1;
    while (getline(file, line))
    {
        if (line.length() > 0)
        {
            string task = line;
            tasks_done.push_back(make_pair(id, task));
        }
    }
    file.close();
}

void file_done_write()
{
    FILE *fp = fopen("completed.txt", "w");
    if (fp == NULL)
    {
        cout << "Error opening file";
        exit(1);
    }
    for (int i = 0; i < (int)tasks_done.size(); i++)
    {
        fprintf(fp, "%s\n", tasks_done[i].second.c_str());
    }
    fclose(fp);
}

void done_task(int id)
{
    for (int i = 0; i < (int)tasks_list.size(); i++)
    {
        if (tasks_list[i].first == id)
        {
            tasks_done.push_back(make_pair(id, tasks_list[i].second.first));
            tasks_list.erase(tasks_list.begin() + i);
            break;
        }
    }

    file_write();
    file_done_write();
}

void delete_task(int id)
{
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
    FILE *fp = fopen("task.txt", "a");
    if (fp == NULL)
    {
        cout << "Error opening file";
        exit(1);
    }
    fprintf(fp, "%s %d\n", task.c_str(), priority);
    fclose(fp);

    cout << "Added task: " << task << " with priority: " << priority << endl;
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
    sort(tasks_list.begin(), tasks_list.end(), [](const pair<int, pair<string, int>> &a, const pair<int, pair<string, int>> &b)
         { return a.second.second < b.second.second; });

    // ! Fixing the ID
    int id = 1;
    for (auto &task : tasks_list)
    {
        task.first = id;
        id++;
    }

    // Range based for loop to print tasks_list
    for (int i = 0; i < (int)tasks_list.size(); i++)
    {
        cout << tasks_list[i].first << ". " << tasks_list[i].second.first << "[" << tasks_list[i].second.second << "]" << endl;
    }
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
            cout << "Error: no incomplete item with index" << argv[2] << " exists." << endl;
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

    file_read("task.txt");
    file_done_read("completed.txt");
    check_cmd(argc, argv);
}
