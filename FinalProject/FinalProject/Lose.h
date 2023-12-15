#include "Scene.h"
#include "Utility.h"

class Lose : public Scene {

public:
    // ————— CONSTRUCTOR ————— //
    ~Lose();

    // ————— METHODS ————— //
    // Inherited
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};