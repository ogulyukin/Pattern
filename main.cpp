#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class cache{
    string str;
public:
    string get_string(){
        return str;
    }
    void set_str(string str){
        this->str = str;
    }
    cache(string str = ""): str(str){}
};
class History{
    vector<string> buffer;
    int position;
public:
    int get_size(){
        return position;
    }
    void add_operation(string str){
        if(buffer.size() != position){
            while(buffer.back() != buffer[position])
                buffer.pop_back();
        }
        buffer.push_back(str);
        position++;
    }
    string read_operation_back(){
        if (position - 1 < 0)
            return buffer[0];
        position--;
        return buffer[position];
    }
    string read_operation_forward(){
        if(position + 1 >= buffer.size()){
            return buffer[position];
        }
        position++;
        return buffer[position];

    }
    History(string str){
        add_operation(str);
        this->position = 0;
    }
};
class Command {
public:
    virtual void execute(string& str, cache& buffer, History& hist) = 0;
};
class copy : public Command{
    int idx1;
    int idx2;
public:
    void execute(string& str, cache& buffer, History& hist)override{
        buffer.set_str(str.substr(idx1,idx2));
    }
    copy(int idx1, int idx2): idx1(idx1), idx2(idx2){}
};
class paste: public Command{
    int idx1;
public:
    void execute(string& str, cache& buffer, History& hist)override{
        string temp = str.substr(idx1);
        string temp2 = str.substr(0,idx1);
        str = temp2 + buffer.get_string() + temp;
        hist.add_operation(str);
    }
    paste(int idx1) : idx1(idx1){}
};
class insert: public Command{
    int idx1;
    string newstr;
public:
    void execute(string& str, cache& buffer, History& hist)override{
        string temp = str.substr(idx1);
        string temp2 = str.substr(0,idx1);
        str = temp2 + newstr + temp;
        hist.add_operation(str);
    }
    insert(string newstr, int idx1): newstr(newstr), idx1(idx1){}
};
class Delete: public Command{
    int idx1;
    int idx2;
public:
    void execute(string& str, cache& buffer, History& hist)override{
        string temp = str.substr(0,idx1);
        string temp2 = str.substr(idx2);
        str = temp + temp2;
        hist.add_operation(str);
    }
    Delete(int idx1, int idx2) : idx1(idx1), idx2(idx2){}
};
class undo: public Command{
public:
    void execute(string& str, cache& buffer, History& hist)override{
        str = hist.read_operation_back();
    }
    undo(){}
};
class redo: public Command{
public:
    void execute(string& str, cache& buffer, History& hist)override{
        str = hist.read_operation_forward();
    }
    redo(){}
};
class Editor{
    Command *command;
    string str;
    History hist;
    cache buffer;
public:
    void copy(int idx1, int idx2){
        command = new class copy(idx1, idx2);
        command->execute(str,buffer,hist);
        delete command;
    }
    void paste(int idx1){
        command = new class paste(idx1);
        command->execute(str,buffer,hist);
        delete command;
    }
    void insert(string newstr, int idx1){
        command = new class insert(newstr, idx1);
        command->execute(str,buffer,hist);
        delete command;
    }
    void Delete(int idx1, int idx2){
        command = new class Delete(idx1, idx2);
        command->execute(str,buffer,hist);
        delete command;
    }
    void undo(){
        command = new class undo();
        command->execute(str,buffer,hist);
        delete command;
    }
    void redo(){
        command = new class redo();
        command->execute(str,buffer,hist);
        delete command;
    }
    string get_string(){
        return str;
    }
    Editor(string &str, History &hist, cache &buffer, Command *command = nullptr) : str(str), hist(hist), buffer(buffer){}
};
class Parcer{
    string filename;
    Editor editor;
public:
    void read_command(){
        ifstream is;
        is.open(filename, ifstream::in);
        if(!is.is_open())
            throw exception();
        string command = "";
        while (!is.eof()){
            is >> command;
            command_processing(command, is);
        }
        is.close();
    }
    ifstream& command_processing(string& s01, ifstream& is){
        if(s01 == "copy"){
            cout << "copy command" << endl;
            string s02 = "";
            string s03 = "";
            is >> s02;
            is >> s03;
           editor.copy(to_int(s02),to_int(s03));
           cout << editor.get_string() << endl;
        }else if (s01 == "paste"){
            cout << "paste command" << endl;
            string s02 = "";
            is >> s02;
            editor.paste(to_int(s02));
            cout << editor.get_string() << endl;
        }else if (s01 == "insert"){
            cout << "insert command" << endl;
            string s02 = "";
            string s03 = "";
            is >> s02;
            is >> s03;
            editor.insert(s02, to_int(s03));
            cout << editor.get_string() << endl;
        }else if (s01 == "delete"){
            cout << "delete command" << endl;
            string s02 = "";
            string s03 = "";
            is >> s02;
            is >> s03;
            editor.Delete(to_int(s02),to_int(s03));
            cout << editor.get_string() << endl;
        }else if (s01 == "undo"){
            cout << "undo command" << endl;
            editor.undo();
            cout << editor.get_string() << endl;
        } else if (s01 == "redo"){
            cout << "redo command" << endl;
            editor.redo();
            cout << editor.get_string() << endl;
        }else{
            cout << "Unknown command!!" << endl;
        }
        return is;
    }
    int to_int(string s01){
        int size = s01.size();
        int result = 0;
        int count = 1;
        for(int i = size - 1; i >= 0 ; i--){
            if(int(s01[i]) > 47 && int(s01[i]) < 58){
                result += int(s01[i]) - 48;
                count *= 10;
            }
        }
        return result;
    }
    string get_result_string(){
        return editor.get_string();
    }
    Parcer(Editor editor, string filename = "commands2.txt") : filename(filename), editor(editor){}
};
int main() {
    setlocale(LC_ALL, "");
    string workstring = "This is a test string";
    cache buffer;
    History hist(workstring);
    Editor editor(workstring, hist, buffer);
    Parcer par(editor);
    par.read_command();
    return 0;
}