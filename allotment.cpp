#include <bits/stdc++.h>

using namespace std;

class Student;

class Department{
    private: 
        int index;
        string preference_file_name;
        int max_number_of_students;

    public: 
        Department(string file_name, int max_students){
            preference_file_name = file_name;
            max_number_of_students = max_students;
            load_file();
        }

        void load_file(){
            ifstream file(preference_file_name);
            int pref_rank = 1;
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                preference_map[line] = pref_rank++;
            }
            file.close();
        }


        int get_max_number_of_students(){
            return max_number_of_students;
        }

        unordered_map<string, int> preference_map; //student index, preference rank
        map<int, Student*> probables; 

        void write_list();
};

#define STUDENT_FILENAME "student.csv"
#define NUM_OF_PREFERENCES 10 

queue<Student*> students;
vector<Department*> Departments;

unordered_map<string, int> dept_map = {
    {"EVOLVE", 0},
    {"O_IP", 1},
    {"DAM", 2},
    {"WEBOPS", 3},
    {"PUBLICITY", 4},
    {"ENVISAGE", 5},
    {"EVENTS_WORKSHOPS", 6},
    {"FINANCE", 7},
    {"SHOWS_EXHIBITIONS", 8},
    {"SPONS_PR", 9},
};

class Student{
    private:  
        string name;
        int index;

    public: 
        Student(int index, string name){
            this->name = name; 
            this->index = index;
        }

        void process(){
            while(cur_index < NUM_OF_PREFERENCES && Departments[preferences[cur_index]]->preference_map[name] == 0){
                cur_index++;
            }
            if(cur_index == NUM_OF_PREFERENCES) return; 
            alloted = preferences[cur_index];
            Departments[alloted]->probables.insert({Departments[alloted]->preference_map[name], this}); 
            if(Departments[alloted]->probables.size() > Departments[alloted]->get_max_number_of_students()){
                auto it = --Departments[alloted]->probables.end();
                (it->second)->alloted = -1; 
                students.push(it->second);
                Departments[alloted]->probables.erase(it);
            }
        }

        vector<int> preferences;
        int cur_index = 0;
        int alloted = -1; 

        string get_name(){
            return name;
        }
};

void Department::write_list(){
    ofstream file;
    string output_file_name = preference_file_name.substr(0, preference_file_name.find('.')) + "_allotment.txt";
    file.open(output_file_name);
    for(auto it : probables){
        file << it.second->get_name() << endl;    
    }
    file.close();
}   

int main(){
    ifstream student_file(STUDENT_FILENAME);
    if(!student_file.is_open()){
        cout << "Error in opening file " << STUDENT_FILENAME << endl;
        return 0;
    }

    Departments.push_back(new Department("EVOLVE.csv", 20));
    Departments.push_back(new Department("O_IP.csv", 20));
    Departments.push_back(new Department("DAM.csv", 20));       
    Departments.push_back(new Department("WEBOPS.csv", 20));
    Departments.push_back(new Department("PUBLICITY.csv", 20));
    Departments.push_back(new Department("ENVISAGE.csv", 20));
    Departments.push_back(new Department("EVENTS_WORKSHOPS.csv", 20));
    Departments.push_back(new Department("FINANCE.csv", 20));
    Departments.push_back(new Department("SHOWS_EXHIBITIONS.csv", 20));
    Departments.push_back(new Department("SPONS_PR.csv", 20));

    string line; 
    int index = 0; 

    while(getline(student_file, line)){
        stringstream ss(line);
        string name;
        getline(ss, name, ',');
        Student* student = new Student(index, name);
        string cell;  
        while(getline(ss, cell, ',')) {
            student->preferences.push_back(dept_map[cell]);
        }
        students.push(student);
        index++;
    }

    while(!students.empty()){
        Student* student = students.front();
        students.pop();
        student->process();
    }

    for(Department* i : Departments){
        i->write_list();
    }
        
}
