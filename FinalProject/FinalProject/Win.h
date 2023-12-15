#include "Scene.h"
#include "Utility.h"

class Win : public Scene {

public:
    // ————— CONSTRUCTOR ————— //
    ~Win();

    // ————— METHODS ————— //
    // Inherited
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};