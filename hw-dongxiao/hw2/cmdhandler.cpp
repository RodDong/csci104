#include "cmdhandler.h"
#include "util.h"
#include <sstream>
using namespace std;

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
  : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
	return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}

AndHandler::AndHandler()
{

}

AndHandler::AndHandler(Handler* next)
  : Handler(next)
{

}

bool AndHandler::canHandle(const std::string& cmd) const
{
	return cmd == "AND";

}

Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, std::istream& instr) const
{
    string a;
    getline(instr,a);
    a = trim(a);
    stringstream ss(a);
    vector<string> terms;
    while(ss>>a){
        terms.push_back(a);
    }

    vector<Tweet*> res = eng->search(terms,0);
    displayHits(res);

	return HANDLER_OK;
}

OrHandler::OrHandler()
{

}

OrHandler::OrHandler(Handler* next)
  : Handler(next)
{

}

bool OrHandler::canHandle(const std::string& cmd) const
{
	return cmd == "OR";

}

Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, std::istream& instr) const
{
    string a;
    getline(instr,a);
    a = trim(a);
    stringstream ss(a);
    vector<string> terms;
    while(ss>>a){
        terms.push_back(a);
    }

    vector<Tweet*> res = eng->search(terms,1);
    displayHits(res);

	return HANDLER_OK;
}

TweetHandler::TweetHandler()
{

}

TweetHandler::TweetHandler(Handler* next)
  : Handler(next)
{

}

bool TweetHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TWEET";
}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, std::istream& instr) const
{
    string a,user,text;
    getline(instr,a);
    a = trim(a);
    stringstream ss(a);
    ss>>user;
    getline(ss,text);
    text = trim(text);
    eng->addTweet(user,DateTime(),text);
    cout << text << endl;
	return HANDLER_OK;
}
