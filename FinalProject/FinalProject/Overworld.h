#include "Scene.h"
#include "Utility.h"
#include "Map.h"

class Overworld : public Scene {

public:
    // ————— CONSTRUCTOR ————— //
    ~Overworld();

    // ————— METHODS ————— //
    // Inherited
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};