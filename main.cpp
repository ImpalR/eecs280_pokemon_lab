#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>

#include "Pokemon.hpp"
#include "Trainer.hpp"

using namespace std;

class League {
private:
  Trainer* player;
  vector<Trainer*> gymLeaders;
  vector<string> results;
  int totalWins = 0;

public:
  League(istream &playerInput, istream &gymLeadersInput) {
    player = new Trainer(playerInput);

    int num_gym_leaders;
    string ignore;
    gymLeadersInput >> num_gym_leaders;
    gymLeadersInput >> ignore; 

    for(int i = 0; i < num_gym_leaders; ++i) {
      gymLeaders.push_back(new Trainer(gymLeadersInput));
    }
  }

  void simulateLeague() {
    for (Trainer* gymLeader : gymLeaders) {
      battle(gymLeader);
    }
  }

  void battle(Trainer* gymLeader) {
      int playerWins = 0;

      cout << "-----" << *player << " vs. " << *gymLeader << "-----" << endl;

    for (int i = 0; i < 5; ++i) {
      Pokemon leaderPokemon = gymLeader->choose_pokemon();
      cout << *gymLeader << " chooses " << leaderPokemon << endl;

      Pokemon playerPokemon = player->choose_pokemon(leaderPokemon.get_type());
      cout << *player << " chooses " << playerPokemon << endl;

      if (Pokemon_battle(playerPokemon, leaderPokemon)) {
        cout << playerPokemon << " defeats " << leaderPokemon << endl << endl;
        playerWins++;
      } 
      else {
        cout << leaderPokemon << " defeats " << playerPokemon << endl << endl;
      }
    }

    cout << "Result: " << *player << "=" << playerWins << ", "
         << *gymLeader << "=" << 5 - playerWins << endl;

    if (playerWins > 2) {
      results.push_back(gymLeader->get_name());
      totalWins++;
    } 

    if (playerWins == 0 || playerWins == 5) {
      cout << "It's a clean sweep!" << endl;
    }

    cout << endl;

    player->reset();
    gymLeader->reset();
  }

  Trainer* getPlayer() const {
    return player;
  }

  int getTotalWins() const {
    return totalWins;
  }

  const vector<string>& getResults() const {
    return results;
  }

  ~League() {
    delete player;
    for (Trainer* leader : gymLeaders) {
      delete leader;
    }
  }

  
};

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Usage: battle.exe PLAYER_FILE GYM_LEADERS_FILE" << endl;
    return 1;
  }

  ifstream player_in(argv[1]);
  if (!player_in.is_open()) {
    cout << "Unable to open " << argv[1] << endl;
    return 1;
  }

  ifstream gym_in(argv[2]);
  if (!gym_in.is_open()) {
    cout << "Unable to open " << argv[2] << endl;
    return 1;
  }

  League league(player_in, gym_in);
  league.simulateLeague();

  cout << *league.getPlayer() << " won " << league.getTotalWins() << " matches by defeating:" << endl;
  for (const string& result : league.getResults()) {
    cout << result << endl;
  }

    return 0;
}