#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
class Point{
    string name;
    double latitude; //широта
    double longitude; //долгота
    double penalty; //штраф в часах. 0 - тачка обязательная
public:
    Point(string name, double latitude, double longitude, double penalty = 0): name(name),latitude(latitude), longitude(longitude), penalty(penalty){}
    ~Point(){}
    string get_name()const{
        return name;
    }
    double get_latitude()const{
        return latitude;
    }
    double get_longitude()const{
        return longitude;
    }
    double get_penalty(){
        return penalty;
    }
};
class ReportForm{
public:
    virtual string ReportString(Point point) = 0;
};
class SummaryForm: public ReportForm{
public:
    string ReportString(Point point)override{
        string result = point.get_name() + "\t" + to_string(point.get_latitude()) + "\t" + to_string(point.get_longitude())
                + "\t";
        if(point.get_penalty() == 0){
            result += "незачёт СУ";
        }else{
            result += to_string(point.get_penalty());
        }
        return result;
    }
};
class PenaltyForm: public ReportForm{
public:
    string ReportString(Point point)override{
        if (point.get_penalty() == 0)
            return "";
        string result = point.get_name() + "\t" + to_string(point.get_penalty());
        return result;
    }
};
class Report{
    ReportForm* form;
    vector<Point> track;
public:
    void SetForm(ReportForm* form){
        this->form = form;
    }
    string RepString(Point pont){
        return this->form->ReportString(pont);
    }
    void SummaryReport(){
        cout << "______________________________________________" << endl;
        for(auto i : track){
            cout << RepString(i) << endl;
        }
        cout << "______________________________________________" << endl;
    }
    void PenaltyReport(){
        string result_string;
        double result_penalty = 0;
        cout << "______________________________________________" << endl;
        for(auto i : track){
            result_string = RepString(i);
            if (result_string != ""){
                cout << result_string << endl;
            }
            result_penalty += i.get_penalty();
        }
        cout << "Итого штрафных часов на трассе: " << result_penalty << endl;
        cout << "______________________________________________" << endl;
    }
    Report(ReportForm* form, vector<Point> &track): form(form), track(track){}
    ~Report(){
        delete form;
    }
};
int main() {
    setlocale(LC_ALL, "");
    srand(time(0));
    vector<Point> track;
    //unordered_map<int,Point> track;
    Point* point01 = new Point("First", -120, 0);
    track.push_back(*point01);
    Point* point02 = new Point("Second", -100, 20, 2);
    track.push_back(*point02);
    Point* point03 = new Point("Third", -80, 25);
    track.push_back(*point03);
    Point* point04 = new Point("Forth", -60, 30, 3);
    track.push_back(*point04);
    Point* point05 = new Point("Fifth", -80, 45);
    track.push_back(*point05);
    Report* rep = new Report(new SummaryForm, track);
    rep->SummaryReport();
    rep->SetForm(new PenaltyForm);
    rep->PenaltyReport();
    //Cleaning
    delete point01;
    delete point02;
    delete point03;
    delete point04;
    delete point05;
    return 0;
}