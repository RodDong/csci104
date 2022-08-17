#ifndef DATETIME_H
#define DATETIME_H
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
/**
 * Models a timestamp in format YYYY-MM-DD HH:MM:SS
 */
struct DateTime
{
	/**
	* Default constructor for current system time/date
	*/
	DateTime(){
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        year = timeinfo->tm_year+1900;
        month = timeinfo->tm_mon+1;
        day = timeinfo->tm_mday;
        hour = timeinfo->tm_hour;
        min = timeinfo->tm_min;
        sec = timeinfo->tm_sec;
    }

	/**
	* Alternative constructor
	*/
	DateTime(int hh, int mm, int ss, int yy, int oo, int dd){
		year = yy;
        month = oo;
        day = dd;
        hour = hh;
        min = mm;
        sec = ss;
	}

	/**
	* Return true if the timestamp is less-than other's
	*
	* @return result of less-than comparison of timestamp
	*/
	bool operator<(const DateTime& other) const{
	    if(year!=other.year)
            return year < other.year;
	    else if(month!=other.month)
            return month < other.month;
        else if(day!=other.day)
            return day < other.day;
        else if(hour!=other.hour)
            return hour < other.hour;
        else if(min!=other.min)
            return min < other.min;
        return sec<other.sec;
	}

	/**
	* Outputs the timestamp to the given ostream in format:
	*   YYYY-MM-DD HH:MM:SS
	*
	* @return the ostream passed in as an argument
	*/
	friend std::ostream& operator<<(std::ostream& os, const DateTime& other){
        os<<other.year<<'-'<<other.month<<'-'<<other.day<<' ' << other.hour<<':'<<other.min<<':'<<other.sec;
        return os;
    }

	/**
	* Inputs the timestamp from the given istream expecting the format:
	*   YYYY-MM-DD HH:MM:SS
	*  Returns if an error is found in the format but sets the DateTime
	*   to the current system time/date
	*
	*
	* @return the istream passed in as an argument
	*/
	friend std::istream& operator>>(std::istream& is, DateTime& dt){
        std::string d,t;
        is>>d>>t;
        struct DateTime now;
        size_t pos=d.find('-',0);
        if(pos==d.npos) {
            dt=now;
            return is;
        }
        dt.year = atoi(d.substr(0,pos).c_str());
        size_t pos2=d.find('-',pos+1);
        if(pos2==d.npos) {
            dt=now;
            return is;
        }
        dt.month = atoi(d.substr(pos+1,pos2).c_str());
        if(pos2+1==d.size()) {
            dt=now;
            return is;
        }
        dt.day = atoi(d.substr(pos2+1,d.size()).c_str());

        pos=t.find(':',0);

        if(pos==t.npos) {
            dt=now;
            return is;
        }
        dt.hour = atoi(t.substr(0,pos).c_str());
        pos2=t.find(':',pos+1);
        if(pos2==t.npos) {
            dt=now;
            return is;
        }
        dt.min = atoi(t.substr(pos+1,pos2).c_str());
        if(pos2+1==t.size()) {
            dt=now;
            return is;
        }
        dt.sec = atoi(t.substr(pos2+1,t.size()).c_str());

        return is;
    }

	/* Data members -- they can all be public
	* which is why we've made this a struct */
	int hour;
	int min;
	int sec;
	int year;
	int month;
	int day;
};

#endif
