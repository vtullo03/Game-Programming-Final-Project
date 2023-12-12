#include "Scene.h"

class Selection : public Scene {

public:
    // ————— CONSTRUCTOR ————— //
    ~Selection();

    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;

};