CXX=g++
CXXFLAGS=-g -Wall -std=c++11

all: twitter

# Finish this target by adding other dependencies and 
# the action to compile the final twitter executable
twitter: twitter.o user.o cmdhandler.o  handler.o  tweet.o  twiteng.o util.o
	$(CXX) $(CXXFLAGS) twitter.o user.o cmdhandler.o  handler.o  tweet.o  twiteng.o util.o -o $@

# Complete	
twitter.o: twitter.cpp twiteng.h cmdhandler.h util.h
	$(CXX) $(CXXFLAGS) -c twitter.cpp -o twitter.o

# Add other rules/targets
cmdhandler.o: cmdhandler.h util.h handler.h
	$(CXX) $(CXXFLAGS) -c cmdhandler.cpp -o cmdhandler.o

handler.o: handler.cpp util.h twiteng.h
	$(CXX) $(CXXFLAGS) -c handler.cpp -o handler.o

tweet.o: tweet.cpp datetime.h tweet.h util.h user.h
	$(CXX) $(CXXFLAGS) -c tweet.cpp -o tweet.o

twiteng.o: twiteng.cpp datetime.h tweet.h util.h user.h
	$(CXX) $(CXXFLAGS) -c twiteng.cpp -o twiteng.o

user.o: user.cpp datetime.h tweet.h util.h user.h
	$(CXX) $(CXXFLAGS) -c user.cpp -o user.o
util.o: util.cpp
	$(CXX) $(CXXFLAGS) -c util.cpp -o util.o

clean:
	rm -f *.o twitter *.feed
