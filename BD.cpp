#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <istream>
#include <ctype.h>

using namespace std;

void PrintVector (const vector <string>& v){
	for (int i = 0; i<v.size();i++){
		if(i<(v.size())){
			cout<<v[i]<<endl;
		}
	}
}

void EnsureNextSymbolAndSkip (stringstream& ss,const string& s)
{
	if (ss.peek() != '-'){
				stringstream ssEr;
				ssEr << "Wrong date format: " <<s;
				throw runtime_error(ssEr.str());
	}
	ss.ignore(1);
}

int ReadFromStreamToIntAndCheck(stringstream& ss,const string& sData){
	int n;
	if(!(ss>>n)){
		    stringstream ssEr;
			if(sData == ""){
				 ssEr << "Wrong date format:";
			}
			else{
				ssEr << "Wrong date format: " + sData;
			}
			throw runtime_error(ssEr.str());
		}else{
			return n;
		}
}

class Date {
public:
	Date(string sData){
		stringstream stream;
		stream<<sData;
		year = ReadFromStreamToIntAndCheck(stream,sData);
		EnsureNextSymbolAndSkip(stream,sData);
		month = ReadFromStreamToIntAndCheck(stream,sData);
		EnsureNextSymbolAndSkip(stream,sData);
		day  = ReadFromStreamToIntAndCheck(stream,sData);
		if( stream.peek() != -1){
			stringstream ssEr;
			ssEr<< "Wrong date format: " + sData;
			throw runtime_error(ssEr.str());
		}
		if ((month>12)||(month<1)){
			stringstream ssEr;
			ssEr << "Month value is invalid: " <<month;
			throw runtime_error(ssEr.str());
		}
		if ((day>31)||(day<1)){
					stringstream ssEr;
					ssEr << "Day value is invalid: " <<day;
					throw runtime_error(ssEr.str());
		}
	}
  int GetYear() const{
	  return year;
  }
  int GetMonth() const{
	  return month;
  }
  int GetDay() const{
	  return day;
  }
private:
  int year;
  int month;
  int day;
};

ostream& operator << (ostream& os, const Date& d) {
  return os <<setw(4)<<setfill('0')<< d.GetYear() << '-'
		    <<setw(2)<<setfill('0') << d.GetMonth()<< '-'
			<<setw(2)<<setfill('0') <<d.GetDay();
}

bool operator<(const Date& lhs, const Date& rhs){
	if ((lhs.GetYear()*372 + lhs.GetMonth()*31 + lhs.GetDay())<(rhs.GetYear()*372 + rhs.GetMonth()*31 + rhs.GetDay())){
		return true;
	}
	return false;
}

class Database {
public:
  void AddEvent(const Date& date, const string& event){
	  if (mBd.count(date) > 0) {
	 	 	 	vector <string> vEvent;
	 	 	 	int flag = 1;
	 	 	 	vEvent = mBd.at(date);
	 	 	 	for(auto c:vEvent){
	 	 	 		if (c == event){
	 	 	 			flag = 0;
	 	 	 			break; // событие уже содержиться
	 	 	 		}
	 	 	 	}
	 	 	 	if (flag){
	 	 	 		mBd[date].push_back(event);
	 	 	 		sort(begin(mBd[date]),end(mBd[date]));
	 	 	 	}

	 	 }else{
	 		mBd[date].push_back(event);
	 		sort(begin(mBd[date]),end(mBd[date]));
	 	 }
  }
  bool DeleteEvent(const Date& date, const string& event){
	  if (mBd.count(date) > 0) {
	 	 		 vector <string> vEvent;
	 	 	     vEvent = mBd.at(date);
	 	 	for(int i = 0; i<vEvent.size(); i++){
	 	 		if (vEvent[i] == event){
	 	 			vEvent.erase(vEvent.begin() + i);
	 	 			mBd[date] = vEvent;
	 	 			return true;
	 	 		}
	 	 	}
	 	 	return false;
	 }
	  else{
		  return false;
	  }
  }

  int  DeleteDate(const Date& date){
	  int size = 0;
	 	  if (mBd.count(date) > 0) {
	 		 vector <string> event;
	 	     event = mBd.at(date);
	 	     size = event.size();
	 	     mBd.erase(date);
	 	     return size;
	 	   }
	 	 return size;
  }

  vector<string> Find(const Date& date) const{
	  vector <string> event;
	  if (mBd.count(date) > 0) {
	     event = mBd.at(date);

	   }
	  return event;
  }

  void Print() const{
	  for (auto k:mBd){
		  for (auto x:k.second){
			  cout<<k.first<<" ";
			  cout<<x<<endl;
		  }
	  }
  }
private:
  map <Date, vector<string>> mBd;
};

int main() {
  Database db;
  string command;
 try{
  while (getline(cin, command)) {
//  command = "Find 1-1-2";
		 // Считайте команды с потока ввода и обработайте каждую
		 stringstream sComand;
		 int flag = 0;// для проверки что команда распознана
		 if(!command.empty()){
			  string sFirst;
			  sComand<<command;
			  sComand>>sFirst;
			  if(sFirst == "Add"){
				 flag = 1;
				 string sDate;
				 string event;
				 sComand>>sDate;
				 Date date(sDate);
				 sComand>>event;
				 db.AddEvent(date, event);
			  }
			  else if(sFirst == "Find"){
					flag = 1;
					string sDate;
					sComand>>sDate;
					Date date(sDate);
					vector<string> test = db.Find(date);
					if(test.size() != 0){
						PrintVector(test);
						//PrintVector(db.Find(date)); // не перевожу строку при печати вектора!
						cout<<endl;
					}
			  }
			  else if(sFirst == "Print"){
				  flag = 1;
				  db.Print();
			  }
			  else if (sFirst == "Del"){
				  flag = 1;
				  string sDate;
				  string event;
				  sComand>>sDate;
				  Date date(sDate);
				  sComand>>event;
				  if (event.empty()){
					  int n = db.DeleteDate(date);
					  cout<<"Deleted "<<n<<" events"<<endl;
				  }
				  else{
					  if (db.DeleteEvent(date,event)){
						  cout<<"Deleted successfully"<<endl;
					  }else{
						  cout<<"Event not found"<<endl;
					  }
				  }
			  }
			  else if(sFirst == "\r\n"){
						  flag = 1;
						//  cout<<"перевод строки";
					  }
			  else if(flag == 0){
				  stringstream ssEr;
				  ssEr << "Unknown command: " <<sFirst;
				  throw runtime_error(ssEr.str());
			  }
		   }
	  }       // от wile
}catch (exception& ex){
	  cout<<ex.what()<<endl;
    }

  return 0;
}
