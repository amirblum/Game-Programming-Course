# Game Programming Course Exercise 1
In this project I implemented the skeleton of an OpenGL first-person game.

The exercise had two parts:

1) WASD control system
    * Doom-style WASD. W - forward, S - backward, A,D - turning.
    * I implemented the following bonus controls:
        a. Q/E to strafe side to side
        b. Z for a smooth crouch (tap to crouch, tap again to uncrouch)
        c. SPACE for parabolic jumping (with respect to the height of the
           terrain)
        d. SHIFT/CTRL/ALT (any one) while moving gives a boost (this was
           convenient to get around the large terrain)
            -> Note: due to the modifier implementation in glut, you need to
               press the modifier and then start moving. It doesn't work
               pressing it in the middle of movement. The boost works for
               movement, turning and strafing.
    * The speeds/jump heights/etc. are configurable through defines in
      Controller.cpp.

2) Procedurally generated terrain
    * We start with a flat grid of vertices. The size is configurable through
      GRID_WIDTH and GRID_LENGTH in main.cpp.
    * The project includes a non-functioning implementation of the Mid-Point
      Displacement algorithm. After a failed attempt I decided to go with
      Particle Deposition (it is the most interesting anyway), but I left the
      code there in case I want to expand it someday.
    * There is a full implementation of Particle Deposition. It works as
      follows:
        - A random spot in the world is chosen, and a semi-random number of
          iterations is chosen.
        - Every iteration, we move in a random direction and deposit a particle.
          This particle recursively rolls down the terrain until it reaches a
          vertex that is lower than all it's neighbors, then it raises that
          vertex up.
        - After the set amount of iterations, we choose a new random spot.
        - This creates interesting chunks all around the terrain.
        - Sometimes the deformer is in valley mode. In this mode it slightly
          lowers the vertices instead of raising them. This creates more
          interesting and dynamic terrain.
        - The terrain builds itself in real-time as the player walks around it.
          The time between deformations is configurable through the
          MILLIS_PER_DEFORMATION in main.cpp.
    * Therefore, the bonuses implemented are:
        a. Added randomness
        b. Inventive particle deposition.
        c. Gradal terrain generation

Have fun!