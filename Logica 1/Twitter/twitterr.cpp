#include "twitter.hpp"

//Tweet
Tweet::Tweet (int id, string username, string msg) {
    this->id = id;
    this->username = username;
    this->msg = msg;   
}

int Tweet::getId() {
    return id;
}

string Tweet::getSender() {
    return this->username;
}

string Tweet::getMsg() {
    return this->msg;
}

ostream& operator<<(ostream& os, Tweet* msg) {
    os << msg->id << ":" <<msg->username << " (" << msg->msg << ")";
    if (!(msg->likes.empty())) {
        os << "[ ";
        for (auto l : msg->likes) {
            os << l << " ";
        }
        os << "]";
    }
    os << '\n';
    if (msg->rt != nullptr) {
        os << "     " << msg->rt->id << ":" << msg->rt->username << " (" << msg->rt->msg << ") " << '\n';
    }
    return os;
}
//
void Tweet::like (string username) {
    likes.insert(likes.end(), username);
}

set<string> Tweet::getLikes() {
    return this->likes;
}
//
void Tweet::setRt(Tweet* tw) {
    this->rt = tw;
}
//
void Tweet::setDeleted() {
    this->deleted = true;
    this->username = "";
    this->msg = "esse tweet foi deletado";
    this->likes.clear();
}

bool Tweet::isDeleted() {
    return this->deleted;
}

Tweet::~Tweet() {
    delete this->rt;
}

//Inbox
Inbox::Inbox() {}
//
list<Tweet*> Inbox::getTimeline () {
    list<Tweet*> lista;

    for (auto i : this->timeline) {
        lista.push_front(i.second);
    }
    return lista;
}
//
Tweet* Inbox::getTweet(int id) {
    auto it = this->timeline.find(id);
    if (it == timeline.end())
        throw runtime_error("fail: tweet nao existe");
    return it->second;
}

list<Tweet*> Inbox::getMyTweets() {
    list<Tweet*> lista;

    for (auto i : this->myTweets) {
        lista.push_front(i.second);
    }
    return lista;
}
//
void Inbox::storeInTimeline(Tweet* tweet) {
    this->timeline.insert(make_pair(tweet->getId(), tweet));
}

void Inbox::storeInMyTweets (Tweet* tweet) {
    this->myTweets.insert(make_pair(tweet->getId(), tweet));
    this->timeline.insert(make_pair(tweet->getId(), tweet));
}

void Inbox::rmMsgsFrom(string username) {
    set<int> ids;
    for (auto& obj : this->timeline) {
        if (obj.second->getSender() == username)
            ids.insert(obj.first);
    }
    for (int it : ids) {
        this->timeline.erase(it);
    }
}

Inbox::~Inbox() {
    for (auto i : timeline) {
        delete i.second;
    }
}

ostream& operator<< (ostream& os, Inbox* in) {
    auto lista = in->getTimeline();
    for (auto obj : lista) {
        if (!(obj->isDeleted()))
            os << obj;
    }
    return os;
}

//User
User::User (string id) {
    this->username = id;
}

void User::follow(User* other) {
    if (this->username == other->username)
        throw runtime_error("fail: impossivel seguir a si proprio");

    if (this->following.find(other->username) != this->following.end())
        throw runtime_error("fail: usuario ja é seguido");
    this->following.insert(make_pair(other->username, other));
    
    other->followers.insert(make_pair(this->username, this));
}

Inbox* User::getInbox() {
    return &inbox;
}

void User::like (int twId) {
    auto it = this->inbox.getTweet(twId);
    it->like(this->username);
}

void User::sendTweet(Tweet* tw) {
    this->inbox.storeInMyTweets(tw);
    for (auto i : this->followers) {
        auto other = i.second->getInbox();
        other->storeInTimeline(tw);
    }
}

void User::unfollow(string username) {
    auto it = this->following.find(username);
    if (it == this->following.end())
        throw runtime_error("fail: usuario nao encontrado");

    this->inbox.rmMsgsFrom(username);

    it->second->followers.erase(this->username);
    this->following.erase(username);
}

void User::unfollowAll() {
    if (this->following.empty())
        return;
    auto cop = this->following;
    for (auto it : cop) {
        this->unfollow(it.first);
    }
}

void User::rejectAll() {
    if ((this->followers.empty()))
        return;
    auto cop = this->followers;
    for(auto obj : cop) {
        obj.second->unfollow(this->username);
    }
}

User::~User() {
    for (auto i : followers) {
        delete i.second;
    }
    for (auto i : following) {
        delete i.second;
    }
}

ostream& operator<<(ostream& os, User* user) {
    os << user->username << '\n';
    os << "   seguidos   [";
    int count = {1};
    for (auto i : user->following) {
        if (count >= (int) user->following.size()){
            os << i.second->username;
            count = 1;
            break;
        }
        os << i.second->username << ", ";
        count++;
    }
    os << "]\n";

    os << "   seguidores [";
    for (auto i : user->followers) {
        if (count >= (int) user->followers.size()){
            os << i.second->username;
            break;
        }
        os << i.second->username << ", ";
        count++;
    }
    os << "]";
    return os;

}

//Controller
Controller::Controller (){}

void Controller::addUser(string username) {
    auto it =   users.find(username);

    if (it != users.end())
        throw runtime_error("fail: user ja existe");
    
    User* u = new User(username);
    users.insert(make_pair(username, u));
}

User* Controller::getUser(string username) {
    auto it = users.find(username);

    if (it == users.end())
        throw runtime_error("fail: user nao encontrado");

    return it->second;
}

Tweet* Controller::createTweet(string username, string msg){
    Tweet* m = new Tweet (nextTwId, username, msg);
    return m;
}

void Controller::sendRt(string username, int twId, string rtMsg) {
    auto it = this->tweets.find(twId);
    if (it == this->tweets.end())
        throw runtime_error("fail: tweet nao existe");
    auto user = this->getUser(username);
    auto rt = this->createTweet(username, rtMsg);
    rt->setRt(it->second);
    this->tweets.insert(make_pair(nextTwId, rt));    
    user->sendTweet(rt);
    nextTwId++;
}

void Controller::rmUser(string username) {
    auto user = this->getUser(username);
    
    auto tws = user->getInbox()->getMyTweets();
    for (auto& tw : tws) {
        if (!(tw->isDeleted()))
            tw->setDeleted();
    }
    user->unfollowAll();
    user->rejectAll();
    
    this->users.erase(username);
}

void Controller::sendTweet(string username, string msg) {
    auto it = getUser(username);
    auto m = createTweet(username, msg);
    this->tweets.insert(make_pair(nextTwId, m));
    nextTwId++;
    it->sendTweet(m);
}

Controller::~Controller () {
    for (auto i : tweets) {
        delete i.second;
    }
    for (auto i : users) {
        delete i.second;
    }
}

ostream& operator<<(ostream& os, Controller* cont) {
    for (auto u : cont->users) {
        os << u.second << '\n';
    }
    return os;
}

int main() {
    Controller* cont = new Controller();
    auto quebrar = [](string linha, string remover) {
        string res{""};
        for (int i{(int) remover.length()}; i < (int) linha.length(); i++)
            res += linha[i];
        return res;
    };
    while(true) {
        string line, cmd;
        cin >> cmd;
        getline(cin, line);
        cout << "$" << cmd << line << '\n';
        stringstream ss(line);
        try {
            if (cmd == "show")
                cout << cont;
            else if (cmd == "end")
                break;
            else if (cmd == "timeline") {
                string user;
                ss >> user;
                auto it = cont->getUser(user);
                cout << it->getInbox();
            }
            else if (cmd == "follow") {
                string us1, us2;
                ss >> us1 >> us2;
                auto it = cont->getUser(us1);
                auto other = cont->getUser(us2);
                it->follow(other);
            }
            else if (cmd == "twittar") {
                string user, msg;
                ss >> user;
                msg = quebrar(ss.str(), user + " ");
                cont->sendTweet(user, msg);
            }
            else if (cmd == "unfollow") {
                string us1, us2;
                ss >> us1 >> us2;
                auto it = cont->getUser(us1);
                it->unfollow(us2);
            }
            else if (cmd == "like") {
                string us1;
                int id;
                ss >> us1 >> id;
                auto it = cont->getUser(us1);
                
                it->like(id);
            }
            else if (cmd == "add") {
                string user;
                ss >> user;
                cont->addUser(user);
            }
            else if (cmd == "rm") {
                string user;
                ss >> user;
                cont->rmUser(user);
            }
            else if (cmd == "rt") {
                string user, msg;
                int id;
                ss >> user >> id;
                msg = quebrar(ss.str(), user + " " + to_string(id) + " ");
                cont->sendRt(user, id, msg);
            }
            else   
                throw runtime_error("fail: comando invalido");
        } 
        catch (runtime_error e) {
            cout << e.what() << '\n';
        }
    }
    delete cont;
    return 0;
}