#include <iostream>
#include <map>
#include <set>
#include <string>
#include <exception>
#include <list>
#include <vector>
#include <sstream>
using namespace std;

class Tweet {
    int id;
    set<string> likes;
    string msg;
    string username;
    Tweet* rt{nullptr};
    bool deleted{false};
    public:
    Tweet (int id, string username, string msg);
    void like (string username);
    set<string> getLikes();
    void setRt(Tweet* tw);
    bool isDeleted();
    void setDeleted();
    string getMsg();
    string getSender();
    int getId();
    friend ostream& operator<<(ostream& os, Tweet* msg);
    ~Tweet();
};

class Inbox {
    map<int, Tweet*> timeline;
    map<int, Tweet*> myTweets;
    public:
    Inbox();
    void storeInTimeline(Tweet* tweet);
    list<Tweet*> getTimeline();
    Tweet* getTweet(int id);
    void storeInMyTweets (Tweet* tweet);
    list<Tweet*> getMyTweets();
    void rmMsgsFrom(string username);
    friend ostream& operator<< (ostream& os, Inbox* in);
    ~Inbox();
};

class User {
    map<string, User*> followers;
    map<string, User*> following;
    Inbox inbox;
    string username;
    public:
    User (string id);
    void follow(User* other);
    Inbox* getInbox();
    void like (int twId);
    void sendTweet(Tweet* tw);
    void unfollow(string username);
    void unfollowAll();
    void rejectAll();
    friend ostream& operator<<(ostream& os, User* user);
    ~User();
};

class Controller {
    int nextTwId{0};
    map<int, Tweet*> tweets;
    map<string, User*> users;
    public:
    Controller();
    void addUser(string username);
    Tweet* createTweet(string username, string msg);
    User* getUser(string username);
    void sendRt(string username, int twId, string rtMsg);
    void rmUser(string username);
    void sendTweet(string username, string msg);
    friend ostream& operator<<(ostream& os, Controller* cont);
    ~Controller();
};