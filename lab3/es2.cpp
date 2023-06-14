#include <iostream>
#include <vector>
#include <set>

using namespace std;
struct Player{
    Player(string name, int score);
    string name;
    int score;
};

Player::Player(string name, int score) {
    Player::name = name;
    Player::score = score;
}

template <typename T>
bool cmp (const T& v1, const T& v2){
    if(v1.second<v2.second)
        return false;
    else
        return true;
}

class LeaderBord {
private:
    set<Player> board(cmp);
    public:
        void addPlayer(const std::string& name, int score){
            board.insert(new Player(name, score));
        };
        void removePlayer(const std::string& name){
            board.erase(name);
        };
        void updateScore(const std::string& name, int newScore){
            auto it=board.find(name);
            it->second=newScore;
        };
        void printTopPlayers(int n){
            int i=1;

            for (auto it=board.begin(); it!=board.end();it++){
                cout<<i<<": "<<it->first<<"->"<<it->second<<endl;
                i++;
            }
        };
};


int main() {
    LeaderBord leaderBord;
    int v, n;
    int flag=1;
    while (cin>>v && flag){
        string name;
        int score;
        switch(v){
            case 1:

                cout<<"aggiungi nome giocatore"<<endl;
                cin>>name;
                cout<<"aggiungi score"<<endl;
                cin>>score;
                leaderBord.addPlayer(new Player(name, score));
                break;
            case 2:
                cout<<"nome del giocatore da eliminare"<<endl;
                cin>>name;
                leaderBord.removePlayer(name);
                break;
            case 3:
                cout<<"nome del giocatore da modificare"<<endl;
                cin>>name;
                cout<<"nuovo score"<<endl;
                cin>>score;
                leaderBord.updateScore(name, score);
                break;
            case 4:
                cout<<"quante posizioni vuoi vedere?"<<endl;
                cin>>n;
                leaderBord.printTopPlayers(n);
                break;
            default:
                flag=!flag;
                break;
        }
    }


    return 0;
}
