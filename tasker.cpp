#include <iostream>
#include <limits>
#include <string>
using namespace std;

enum class Priority
{
    HIGH,
    MEDIUM,
    LOW,
    NONE
};

class Task
{
private:
    string description;
    Priority priority;

public:
    Task(const string &desc, Priority prio) : description(desc), priority(prio) {}
    string get_description() const { return description; }
    Priority get_priority() const { return priority; }
};

template <typename T>
class Queue
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &item) : data(item), next(nullptr) {}
    };

    Node *frontNode;
    Node *rearNode;
    int count;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    ~Queue()
    {
        while (frontNode != nullptr)
        {
            Node *temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
    }

    int size() const
    {
        return count;
    }

    void enqueue(const T &item)
    {
        Node *newNode = new Node(item);
        if (rearNode == nullptr)
        {
            frontNode = newNode;
            rearNode = newNode;
        }
        else
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    const T &front() const
    {
        if (isEmpty())
        {
            throw std::runtime_error("front: queue is empty");
        }
        return frontNode->data;
    }

    void dequeue()
    {
        if (isEmpty())
        {
            throw std::runtime_error("dequeue: queue is empty");
        }

        Node *temp = frontNode;
        frontNode = frontNode->next;

        if (frontNode == nullptr)
        {
            rearNode = nullptr;
        }

        delete temp;
        count--;
    }

    bool isEmpty() const
    {
        return count == 0;
    }
};

class TaskManager
{
private:
    Queue<Task> highPriority;
    Queue<Task> mediumPriority;
    Queue<Task> lowPriority;

public:
    TaskManager() {}
    ~TaskManager() {}

    void addTask(const string &description, Priority priority)
    {
        Task task(description, priority);
        switch (priority)
        {
        case Priority::HIGH:
            highPriority.enqueue(task);
            break;
        case Priority::MEDIUM:
            mediumPriority.enqueue(task);
            break;
        case Priority::LOW:
            lowPriority.enqueue(task);
            break;
        default:
            cout << "Invalid priority" << endl;
            break;
        }
    }

    void removeTask(const string &description)
    {
        removeFromQueue(description, highPriority);
        removeFromQueue(description, mediumPriority);
        removeFromQueue(description, lowPriority);
    }

    void removeFromQueue(const string &description, Queue<Task> &queue)
    {
        Queue<Task> tempQueue;

        while (!queue.isEmpty())
        {
            Task task = queue.front();
            queue.dequeue();
            if (task.get_description() != description)
            {
                tempQueue.enqueue(task);
            }
        }

        while (!tempQueue.isEmpty())
        {
            queue.enqueue(tempQueue.front());
            tempQueue.dequeue();
        }
    }

    void announceTasks(int count)
    {
        int announced = 0;
        while (announced < count)
        {
            if (!highPriority.isEmpty())
            {
                announceTask(highPriority.front());
                highPriority.dequeue();
                announced++;
            }
            else if (!mediumPriority.isEmpty())
            {
                announceTask(mediumPriority.front());
                mediumPriority.dequeue();
                announced++;
            }
            else if (!lowPriority.isEmpty())
            {
                announceTask(lowPriority.front());
                lowPriority.dequeue();
                announced++;
            }
            else
            {
                break;
            }
        }
    }

    void announceTask(const Task &task)
    {
        cout << "Task: " << task.get_description() << " [Priority: " << getPriorityString(task.get_priority()) << "]" << endl;
    }

    void listTasks() const
    {
        cout << "High Priority Tasks:" << endl;
        printQueue(highPriority);
        cout << "Medium Priority Tasks:" << endl;
        printQueue(mediumPriority);
        cout << "Low Priority Tasks:" << endl;
        printQueue(lowPriority);
    }

    void printQueue(const Queue<Task> &queue) const
    {
        Queue<Task> tempQueue = queue; // Make a copy of the queue

        while (!tempQueue.isEmpty())
        {
            Task task = tempQueue.front();
            cout << "- " << task.get_description() << " [Priority: " << getPriorityString(task.get_priority()) << "]" << endl;
            tempQueue.dequeue();
        }
    }

    string getPriorityString(Priority priority) const
    {
        switch (priority)
        {
        case Priority::HIGH:
            return "HIGH";
        case Priority::MEDIUM:
            return "MEDIUM";
        case Priority::LOW:
            return "LOW";
        default:
            return "NONE";
        }
    }
};

Priority getPriorityFromUser()
{
    int priority;
    cout << "Enter priority (1 = HIGH, 2 = MEDIUM, 3 = LOW): ";
    cin >> priority;
    switch (priority)
    {
    case 1:
        return Priority::HIGH;
    case 2:
        return Priority::MEDIUM;
    case 3:
        return Priority::LOW;
    default:
        return Priority::NONE;
    }
}

void clearInputBuffer()
{
    cin.clear(); // Clear the error flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the input
}

int main()
{
    TaskManager taskManager;
    int choice;

    do
    {
        cout << "\nTask Manager Menu:" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. Remove Task" << endl;
        cout << "3. Announce Tasks" << endl;
        cout << "4. List All Tasks" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number between 1 and 5." << endl;
            clearInputBuffer();
            continue;
        }

        clearInputBuffer(); // Clear the buffer for the next input

        switch (choice)
        {
        case 1:
        {
            string description;
            cout << "Enter task description: ";
            getline(cin, description);
            Priority priority = getPriorityFromUser();
            if (priority == Priority::NONE)
            {
                cout << "Invalid priority. Task not added." << endl;
            }
            else
            {
                taskManager.addTask(description, priority);
                cout << "Task added successfully." << endl;
            }
            break;
        }
        case 2:
        {
            string description;
            cout << "Enter task description to remove: ";
            getline(cin, description);
            taskManager.removeTask(description);
            cout << "Task removed successfully." << endl;
            break;
        }
        case 3:
        {
            int count;
            cout << "Enter number of tasks to announce: ";
            cin >> count;
            if (cin.fail()) {
                cout << "Invalid input. Please enter a valid number." << endl;
                clearInputBuffer();
                continue;
            }
            taskManager.announceTasks(count);
            break;
        }
        case 4:
            taskManager.listTasks();
            break;
        case 5:
            cout << "Exiting Task Manager." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
