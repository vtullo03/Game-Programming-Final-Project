#include "Scene.h"

class Opening : public Scene {

public:
    // ————— CONSTRUCTOR ————— //
    ~Opening();

    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};