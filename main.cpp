#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
class Automata{
    private:
        int alphabet_power;
        int states_power;
        int start_state;

        char eps = 'e';
        //vector<array<int, 2>> eps_transitions;     potentially unnecessary

        set<int> current_states;
        set<int> final_states;

        map<char, vector<array<int, 2>>> transition_map;

    vector<int> extractIntegerWords(string str)
    {
        stringstream ss;
        vector<int> result;
        /* Storing the whole string into string stream */
        ss << str;

        /* Running loop till the end of the stream */
        string temp;
        int found;
        while (!ss.eof()) {

            /* extracting word by word from stream */
            ss >> temp;

            /* Checking the given word is integer or not */
            if (stringstream(temp) >> found)
                result.push_back(found);

            /* To save from space at the end of string */
            temp = "";
        }

        return result;
    }
    void save_transition(string input){
        stringstream ss;
        /* Storing the whole string into string stream */
        ss << input;

        /* Running loop till the end of the stream */
        string temp;
        int found_int;
        char found_char;

        bool is_char_found = false;
        int index =0;
        array<int,2> states{};

        while (!ss.eof()) {
            temp = "";
            /* extracting word by word from stream */
            ss >> temp;

            /* Checking the given word is integer or not */
            if (stringstream(temp) >> found_int) {

                if (index >= sizeof(states) / sizeof(int)) {
                    throw runtime_error("wrong format: there is more than two states in transition rule --> " + input);
                }
                states[index] = found_int;
                index++;

            } else if (stringstream(temp) >> found_char) {

                if (is_char_found) {
                    throw runtime_error("Wrong char(the char, not state) format in transition rule --> " + input);
                } else {
                    is_char_found = true;
                }

            }
        }
        if(index != 2){
            throw runtime_error("wrong states format: two states are not detected in transition rule --> " + input);
        }else if (!is_char_found){
            throw runtime_error("wrong char format: char is not detected in transition rule --> " + input);
        }


        if (transition_map.find(found_char) == transition_map.end()) {
            transition_map[found_char] = vector<array<int, 2>>();
        }
        transition_map[found_char].push_back(states);




    }
public:
    void display_settings(){
        cout << "|A| = " << alphabet_power << endl;
        cout << "|S| = " << states_power << endl;
        cout << "s0 = " << start_state << endl;

        cout << "current_states: ";
        for (auto i: current_states){
            cout << i << " ";
        }
        cout << endl;

        cout << "final states: ";
        for (int i: final_states){
            cout << i << " ";
        }
        cout << endl;

        cout << "transition rules: "<< endl;
        for (auto const& [key, val] : transition_map)
        {
            cout << key        // string (key)
                << ':';
                for(auto pair: val){
                    cout << "{"
                       << pair[0]
                       << ","
                       << pair[1]
                       <<   "} ";
                }
                cout << endl;
        }


    }

    bool  is_set_settings(const string& path){
        //todo: take input and put to variables
        ifstream file (path);

        string line;
        if (file.is_open())
        {
            try {
                getline(file, line);
                alphabet_power = extractIntegerWords(line)[0];
                getline(file, line);
                getline(file, line);
                states_power = extractIntegerWords(line)[0];
                getline(file, line);
                getline(file, line);
                start_state = extractIntegerWords(line)[0];
                getline(file, line);
                getline(file, line);
                for (int i: extractIntegerWords(line)) {
                    final_states.insert(i);
                }
                if (final_states.empty()) {
                    throw runtime_error("no final states was found in settings.txt");
                }
                getline(file, line);
                while (getline(file, line)) {

                    save_transition(line);
                }

            }catch (exception& ex){
                cout<<"wrong setting file format: "<< ex.what() << endl;
            }catch(...){
                cout<<"wrong setting file format";
            }




            return true;

        }
        else{
            cout<< "can not access file or the path is corrupted"<<endl;
            return false;
        }

        return true;
    }

    bool run(string input_symbols){
        current_states.clear();
        current_states.insert(start_state);
        for (char to_check:input_symbols){

            do_eps_transition();

            do_transition(to_check);

        }
        return false;
    }
    private:

        bool do_transition(char to_check){
            set<int> new_states;


            for(array<int,2> transition: transition_map[to_check]){
                if (current_states.contains( transition[0])){

                    if(final_states.contains(transition[1])){
                        return true;
                    }
                    new_states.insert(transition[1]);
                }
            }
            //todo: case for eps
            current_states.clear();
            current_states = new_states;

        }
        void do_eps_transition(){
            do_transition(eps);
        }
};

int main() {
    system("chcp 1251");

    map<char, vector<array<int, 2>>> transition_map;
    //map<char, int[][2]> transition_map2;

    string settings_path = "C:/Users/teraf/Desktop/Automata_test.txt";
    Automata new_automata;
    new_automata.is_set_settings(settings_path);

    string line = "aaa";
    new_automata.display_settings();
    new_automata.run(line);
    cout << " okey";
    return 0;
}
