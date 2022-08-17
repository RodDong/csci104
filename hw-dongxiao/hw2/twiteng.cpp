#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include "twiteng.h"
#include "tweet.h"
// Add other header files


using namespace std;


// Update as needed
TwitEng::TwitEng()
{

}

// Update as needed
TwitEng::~TwitEng()
{

}

// Complete this function
bool TwitEng::parse(char* filename)
{
    ifstream ifile(filename);
	if (ifile.fail()) {
		cerr << "Unable to open " << filename << endl;
		return true;
	}
	int numUsers;
	ifile >> numUsers;
	if (ifile.fail()) {
		cerr << "Unable to read number of users " << endl;
		ifile.close();
		return true;
	}
	// Finish the parsing of the input file. Return false if successful,
	// true otherwise.
    string temp;
    getline(ifile,temp);
    while(numUsers--){
        getline(ifile,temp);
        stringstream ss(temp);
        string tu,su;
        ss>>su;
        list<User>::iterator sit =
            find_if(u.begin(),u.end(),[su](const User& x){
                return x.name()==su;
            });
        if(sit==u.end()){
            u.push_back(User(su));
            sit = u.end();
            sit--;
        }
        while(ss>>tu){
            list<User>::iterator tit =
                find_if(u.begin(),u.end(),[tu](const User& x){
                    return x.name()==tu;
                });
            if(tit==u.end()){
                u.push_back(User(tu));
                tit = u.end();
                tit--;
            }
            sit->addFollowing(&(*tit));
            tit->addFollower(&(*sit));

        }
    }
    while(getline(ifile,temp)){
        string name,text;
        stringstream ss(temp);
        DateTime t;
        ss>>t;
        ss>>name;
        getline(ss,text,' ');
        getline(ss,text);
        list<User>::iterator uit =
            find_if(u.begin(),u.end(),[name](const User& x){
                return x.name()==name;
            });
        tweet.push_back(Tweet(&(*uit), t, text));
        list<Tweet>::iterator tit=tweet.end();
        uit->addTweet(&(*--tit));
    }
    return false;
}

// Implement other member functions below
void TwitEng::addTweet(const std::string& username, const DateTime& time, const std::string& text){
	if(validUser(username)==false)
        return;
	list<User>::iterator it =
		find_if(u.begin(),u.end(),[username](const User& x){
			return x.name()==username;
		});
	tweet.push_back(Tweet(&(*it), time, text));
	list<Tweet>::iterator tit = tweet.end();
	it->addTweet(&(*--tit));
}
std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy){
	vector<Tweet*> result;
	if(strategy==0){
		list<Tweet>::iterator tit=tweet.begin();
		while(tit!=tweet.end()){
			bool flag = true;
			set<string> tag = tit->hashTags();
			vector<string>::iterator teit = terms.begin();
			while(teit!=terms.end()){
                string temp = *teit;
				set<string>::iterator tait =
					find_if(tag.begin(),tag.end(),[temp](const string& x){
						return x==temp;
					});
				if(tait==tag.end()) {
					flag = false;
					break;
				}
				teit++;
			}
			if(flag)
				result.push_back(&(*tit));
			tit++;
		}
	}
	else{
		list<Tweet>::iterator tit=tweet.begin();
		while(tit!=tweet.end()){
			bool flag = false;
			set<string> tag = tit->hashTags();
			vector<string>::iterator teit = terms.begin();
			while(teit!=terms.end()){
                string temp = *teit;
				set<string>::iterator tait =
					find_if(tag.begin(),tag.end(),[temp](const string& x){
						return x==temp;
					});
				if(tait!=tag.end()) {
					flag = true;
					break;
				}
				teit++;
			}
			if(flag)
				result.push_back(&(*tit));
			tit++;
		}
	}
	return result;
}

void TwitEng::dumpFeeds() {
	list<User>::iterator uit = u.begin();
	while(uit!=u.end()){
		vector<Tweet*> res = uit->getFeed();
		sort(res.begin(),res.end(),TweetComp());
		vector<Tweet*>::iterator resit = res.begin();
		ofstream ofile(uit->name()+string(".feed"));
        ofile << uit->name() << endl;
		while(resit!=res.end()){
            ofile << (*resit)->time() << ' ';
            ofile << (*resit)->user()->name() << ' ';
            ofile << (*resit)->text() << endl;
            resit++;
		}
		uit++;
	}
}

bool TwitEng::validUser(const std::string& name) const{
	list<User>::const_iterator it =
		find_if(u.begin(),u.end(),[name](const User& x){
			return x.name()==name;
		});
	if(it == u.end()) return false;
	return true;
}
