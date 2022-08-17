#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <set>
#include "datetime.h"
#include "tweet.h"
#include "user.h"

Tweet::Tweet(){
    u = NULL;
    content = "";
}

Tweet::Tweet(User* user, const DateTime& time, const std::string& text){
    u = user;
    timestamp = time;
    content = text;
    size_t pos = 0;
    while(true){
        pos = text.find('#',pos);
        if(pos==text.npos) break;
        size_t endof = text.find(' ',pos+1);
        if(endof == text.npos) endof=text.size();
        tag.insert(text.substr(pos+1,endof-pos-1));
        pos = endof;
    }
}

DateTime const & Tweet::time() const {
    return timestamp;
}

std::string const & Tweet::text() const {
    return content;

}
std::set<std::string> Tweet::hashTags() const {
    return tag;
}

bool Tweet::operator<(const Tweet& other) const {
    return timestamp < other.timestamp;
}

std::ostream& operator<<(std::ostream& os, const Tweet& t){
    os << t.timestamp;
    os << ' ' << t.u->name();
    os << ' ' << t.content;
    return os;
}

User* Tweet::user() const {
    return u;
}
