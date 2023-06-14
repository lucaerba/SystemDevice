#include <iostream>
#include <iostream>
#include <set>
#include <string>
using namespace std;

struct Player {
    std::string name;
    int score;
};

bool operator<(const Player& p1, const Player& p2) {
    return p1.score > p2.score || (p1.score == p2.score && p1.name < p2.name);
}

class Leaderboard {
public:
    void addPlayer(const std::string& name, int score) {
        players_.insert({name, score});
    }

    void removePlayer(const std::string& name) {
        auto it = std::find_if(players_.begin(), players_.end(),
                               [&name](const auto& player) { return player.name == name; });
        if (it != players_.end()) {
            players_.erase(it);
        }
    }

    void updateScore(const std::string& name, int newScore) {
        removePlayer(name);
        addPlayer(name, newScore);
    }

    void printTopPlayers(int n) const {
        int i = 0;
        for (const auto& player : players_) {
            if (i >= n) break;
            std::cout << player.name << ": " << player.score << '\n';
            ++i;
        }
    }

private:
    std::set<Player> players_;
};

int main() {
    Leaderboard leaderboard;
    int v, n;
    int flag=1;
    Player p;
    while (cin>>v && flag){
        switch(v){
            case 1:
                cout<<"aggiungi nome giocatore"<<endl;
                cin>>p.name;
                cout<<"aggiungi score"<<endl;
                cin>>p.score;
                leaderboard.addPlayer(p.name, p.score);
                break;
            case 2:
                cout<<"nome del giocatore da eliminare"<<endl;
                cin>>p.name;
                leaderboard.removePlayer(p.name);
                break;
            case 3:
                cout<<"nome del giocatore da modificare"<<endl;
                cin>>p.name;
                cout<<"nuovo score"<<endl;
                cin>>p.score;
                leaderboard.updateScore(p.name, p.score);
                break;
            case 4:
                cout<<"quante posizioni vuoi vedere?"<<endl;
                cin>>n;
                leaderboard.printTopPlayers(n);
                break;
            default:
                flag=(!flag);
                break;
        }
    }


    return 0;
}