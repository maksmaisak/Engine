//
// Created by Maksym Maisak on 30/10/19.
//

#ifndef ENGINE_AICONTROLLERDIAGNOSTICSSYSTEM_H
#define ENGINE_AICONTROLLERDIAGNOSTICSSYSTEM_H

#include "System.h"
#include "Entity.h"

namespace en {
    class Actor;
}

/// Shows diagnostics on the currently selected AIController
class AIControllerDiagnosticsSystem : public en::System {

public:
    AIControllerDiagnosticsSystem();
    void draw() override;

private:
    void showAIController(const en::Actor& actor);

    en::Entity m_lastSelectedEntity;
};

#endif //ENGINE_AICONTROLLERDIAGNOSTICSSYSTEM_H
