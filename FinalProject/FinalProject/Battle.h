#include "Scene.h"
#include "Monster.h"
#include "UI.h"
#include "Move.h"
#include "Utility.h"
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>

enum BattleState { PLAYERTURN, PLAYEREXECUTE, OPPTURN, END };

class Battle : public Scene {
private:
    BattleState battle_state;
    Monster* player_monster;
    Monster* opp_monster;
public:
    bool battle_ended = false;
    bool player_turn = false;
    bool player_inputted = false;

    // ————— CONSTRUCTOR ————— //
    ~Battle();

    // ————— METHODS ————— //
    // Inherited
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;

    // Not Inherited -- Own Functions
    void set_battle_monsters(Monster* player, Monster* opponent);
    void set_battle_UI();
    const BattleState const get_battle_state() { return battle_state; };
    void simulate_battle();
};