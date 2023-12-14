#include "Scene.h"
#include "Utility.h"
#include "Monster.h"
#include "Move.h"

class Selection : public Scene {

public:
    // ————— CONSTRUCTOR ————— //
    ~Selection();

    // ————— METHODS ————— //
    // Inherited
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;

    // Not Inherited -- Own Functions
    Monster* monster_select();
};