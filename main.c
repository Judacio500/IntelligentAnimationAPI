#include "ani.h"

int main(int argc, char** argv)
{
    // 1. Setup del Motor
    ANI *anim = initAnimation();
    SCENE *scene = initScene(60, 40); 
    
    // Lista Maestra de Objetos Vivos (Para animationSimple)
    LIST *liveObjects = NULL;

    // --- ESCENARIO ---

    // A) Suelo (Cafe)
    TRANSFORM *tFloor = initPhysics(NULL, initCoord(0, -6, 0), initCoord(40, 2, 1), initCoord(0,0,0));
    F *fFloor = generateFigure(RECTANGLE, initDesign(0.6, 0.4, 0.2, 1), 1, 1, 0,0,0, 0,0,0);
    
    OBJECT *floor = initObject("Suelo", "TERRAIN", tFloor, NULL);
    addColission(floor, fFloor);
    pushFigure(&floor->figures, fFloor);
    handleInsert(&liveObjects, floor, 0, SIMPLE);

    // B) Obstaculo (Gris)
    TRANSFORM *tWall = initPhysics(NULL, initCoord(5, -4, 0), initCoord(2, 2, 1), initCoord(0,0,0));
    F *fWall = generateFigure(RECTANGLE, initDesign(0.5, 0.5, 0.5, 1), 1, 1, 0,0,0, 0,0,0);
    
    OBJECT *wall = initObject("Muro", "TERRAIN", tWall, NULL);
    addColission(wall, fWall);
    pushFigure(&wall->figures, fWall);
    handleInsert(&liveObjects, wall, 0, SIMPLE);

    // --- JUGADOR (Verde) ---

    TRANSFORM *tHero = initPhysics(NULL, initCoord(-15, 0, 0), initCoord(2, 2, 1), initCoord(0,0,0));
    F *fHero = generateFigure(RECTANGLE, initDesign(0, 1, 0, 1), 1, 1, 0,0,0, 0,0,0);
    
    // Area de vision (Logica)
    F *fVision = generateColission(RECTANGLE, 6, 2); 
    
    OBJECT *hero = initObject("Heroe", "PLAYER", tHero, NULL);
    addColission(hero, fHero);
    addColission(hero, fVision);
    pushFigure(&hero->figures, fHero);

    // --- CEREBRO ---

    // 1. Parametros para SALTAR
    GP *pJump = (GP*)malloc(sizeof(GP));
    pJump->speedX = 0.3f;
    pJump->speedY = 0.9f; // Impulso inicial
    pJump->gravity = 0.04f;
    pJump->friction = 1.0f;
    pJump->stepCounter = 0;
    pJump->triggers = NULL;
    
    STATUS *stJump = generateStatus(Jump, NULL, pJump);

    // 2. Parametros para CAMINAR (Base)
    GP *pWalk = (GP*)malloc(sizeof(GP));
    pWalk->speedX = 0.3f;
    pWalk->speedY = 0;
    pWalk->gravity = 0.04f;
    pWalk->friction = 1.0f;
    pWalk->stepCounter = 0;
    pWalk->triggers = NULL;

    // Trigger: Si veo muro -> Cambiar a Salto
    TRIGGER *tVision = initTrigger((Check)checkVision, stJump);
    handleInsert(&pWalk->triggers, tVision, 0, SIMPLE);

    // Asignar estado inicial
    STATUS *stWalk = generateStatus(Walk, NULL, pWalk);
    hero->activeStatus = stWalk;
    handleInsert(&hero->statusStack, stWalk, 0, SIMPLE);

    handleInsert(&liveObjects, hero, 0, SIMPLE);

    // 3. GENERAR SIMULACION
    printf("Simulando 400 frames...\n");
    animationSimple(anim, scene, liveObjects, 400);

    printf("Paneles generados: %s\n", anim->panels->first ? "SI" : "NO");
    
    // 4. CORRER
    startGraphicsLoop(anim, argc, argv, "Demo Final - Salto Inteligente");

    printf("Por alguna razon finaliza?");
    return 0;
}