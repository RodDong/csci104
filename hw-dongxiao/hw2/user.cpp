#include <string>
#include <set>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

#include "user.h"

using namespace std;

User::User(std::string name){
    username = name;
}

User::~User(){

}

string User::name() const{
    return username;
}

set<User*> User::followers() const{
    return follower;
}

set<User*> User::following() const{
    return follow;
}

list<Tweet*> User::tweets() const{
    return tweet;
}

void User::addFollower(User* u){
    follower.insert(u);
}

void User::addFollowing(User* u){
    follow.insert(u);
}

void User::addTweet(Tweet* t){
    tweet.push_back(t);
}

vector<Tweet*> User::getFeed(){
    vector<Tweet*> result;
    list<Tweet*>::iterator selftwit = tweet.begin();
    while(selftwit!=tweet.end()){
        result.push_back(*selftwit);
        selftwit++;
    }
    set<User*>::iterator fouit = follow.begin();
    while(fouit!=follow.end()){
        User* fou = *fouit;
        list<Tweet*> foutw = fou->tweets();
        list<Tweet*>::iterator foutwit = foutw.begin();
        while(foutwit!=foutw.end()){
            result.push_back(*foutwit);
            foutwit++;
        }
        fouit++;
    }

    return result;
}
