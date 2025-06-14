# DenPaKid GAME - PJ for FDU OOP 25

[切换到中文](README-zh.md) | [Switch to English](README.md)

## game description
1. **intro:**
<details>
<summary>click to show</summary>
Welcome to The Grid. A shattered cyberpunk wasteland where the very air hums with lethal energy. Crumbling megastructures pulse with the ghost-light of a catastrophic past, crisscrossed by deadly EM beams and crushing magnetic storms. Survival here isn't about strength – it's about resonance.
You are DenPaKid, a lone runner born from the storm. Harness the unique bio-electric charge surging within you. Manipulate your polarity, ride magnetic currents, and disrupt deadly energy fields in a desperate dash through environments where physics itself is your deadliest foe and only ally.
This isn't just running. This is dancing with lightning.
Master Electrodynamics: Leap, slide, and repel using intuitive charge-based physics. Turn the environment's deadly energy against itself.
Navigate the Electro-Deathtrap: Outrace ionizing lasers, evade magnetic vortices, and surf unstable charge gradients in decaying, neon-drenched ruins.
Uncover the Flicker's Echo: Discover fragmented data-scrolls whispering forgotten truths about The Grid's collapse and your own voltage-born powers.
Run to Survive, Run to Remember: How far can you push your limits? How deep into The Grid's electric heart will you dare to go?
The current is calling. Run. Adapt. Resonate.
Will you become legend... or just another spark extinguished in the static?
</details>

2. **how to play:**


| Key | Action Description | Function Details |
|-----|--------------------|------------------|
| W   | Move North         | 4-directional movement control; each press triggers a displacement. Skills with displacement limited to 10 uses maximum. |
| A   | Move West          | 4-directional movement control; each press triggers a displacement. Skills with displacement limited to 10 uses maximum. |
| S   | Move South         | 4-directional movement control; each press triggers a displacement. Skills with displacement limited to 10 uses maximum. |
| D   | Move East          | 4-directional movement control; each press triggers a displacement. Skills with displacement limited to 10 uses maximum. |
| Q   | Increase Charge    | Increment charge magnitude |
| E   | Decrease Charge    | Decrement charge magnitude |
| Space | Toggle Charge Polarity | Switch charge from positive to negative (or vice versa). |


### Notes:
1. **Displacement Skills**: Movement via W/A/S/D consumes displacement charges, capped at 10 uses before recharge.
2. **Charge Management**: Q/E adjust charge magnitude, while Space flips charge polarity (affects interaction with electromagnetic fields).
3. **Game Mechanics**: Charge level directly influences player interaction with environmental fields and skill effectiveness.
  


## World Premise
Welcome to "The Grid" – a desolate, irradiated megacity sprawl once powered by revolutionary Resonance Energy Technology (RET). Decades ago, the RET core overloaded catastrophically ("The Flicker"), collapsing society and saturating the ruins with lethal, chaotic electromagnetic fields. These fields manifest as deadly beams (ionizing "Lasers"), crushing magnetic vortices, and unstable charge zones. Only those mutated by the Flicker, like DenPaKid, can partially manipulate their own bio-electric charge to navigate this electro-deathtrap. You run to survive, scavenge precious data-scrolls, and maybe find a way to stabilize the Grid... or escape it.

## Project Overview
This repository contains the final project for the Object-Oriented Programming (OOP) course in `FudanUniversity ShangHai China`, 2025 Spring. It demonstrates key concepts and practices in OOP through a practical implementation.

## arguments you can modify

<details>
<summary>click to show</summary>
In `src/entities/Player.h`
These constants control the player's core abilities.

static constexpr float MAX_CHARGE; The maximum absolute charge the player can have. Higher values mean stronger interactions with fields.
static constexpr float MIN_CHARGE_MAGNITUDE; The smallest non-zero charge magnitude.
static constexpr float CHARGE_STEP; How much charge is gained/lost with each press of Q/E. Increase for faster charge management.
In `src/physics/PhysicsEngine.cpp`
These parameters govern the "feel" of player movement.

m_playerMass; (Set in GameScene::setupInitialState()) Affects how much force is needed to accelerate the player. Higher mass means the player feels "heavier" and responds slower to forces.
const float MAX_SPEED; The player's maximum possible speed.
float restitution; The "bounciness" factor (0 to 1) when hitting a wall. 1.0 is a perfectly elastic bounce, 0.0 is no bounce at all.
const float DAMPING_FACTOR = 0.1f; A simple friction/air-resistance force. Higher values will cause the player to slow down faster when no forces are applied, making control feel "tighter".
In `src/scene/GameScene.h`
These control the game environment's behavior.

static constexpr float B_FIELD_DENSITY_SCALE_FACTOR; Scales how many 'X' or '.' symbols appear relative to B-field strength. A purely visual parameter.
static constexpr int MAX_B_FIELD_SYMBOLS_PER_ROW_COL = 15; Caps the grid size for B-field symbols to prevent visual clutter at high field strengths.
In `src/scene/GameScene.cpp`
These control event frequency and difficulty.

m_bgScrollSpeed = 150.f; (In loadAssets()) The speed at which the background scrolls, which also affects how quickly m_distanceTraveled increases.
m_timeBetweenLaserSpawns: This is randomized between 1.5s and 4.0s inside update(). You can change these bounds to make lasers more or less frequent.
m_timeBetweenScrollSpawns: Randomized between 8.0s and 15.0s inside update(). Adjust to change how often scrolls appear.
laserSpeed: Randomized between 300 and 500 (300 + m_rng() % 200) inside spawnLaser(). Increase this range for more challenging lasers.
Field Strengths (in randomizeFields()):
std::uniform_real_distribution<float> e_dist(); The range for electric field strength. A wider range like (-100.f, 100.f) will make the game much harder.
std::uniform_real_distribution<float> b_dist(); The range for magnetic field strength.
In `src/core/Game.h`
These define the overall game objectives.

const int m_totalScrolls = 5; The total number of unique scrolls to be collected.
const float m_requiredDistanceForWin; The distance the player must exceed (in addition to collecting all scrolls) to win the game.
</details>

## Installation
```bash
git clone https://github.com/VagRant2333/2025-FDU-OOP-final-PJ.git
```

## Usage
```bash
# Please change to the game's directory
cmake .
make

cd build/bin
./DenPaKid

# for windows: .\DenPaKid.exe

# if you use WSL:

# export DISPLAY=:0
# export ALSOFT_DRIVERS=pulse
# ./DenPaKid

# if you use remote SSH: X11 service is not supported yet
```

## Acknowledgments
this game uses AI generated assets and online music.
if copyright is violated, contact me and I will delete it ASAP.

## License
MIT License
### MIT License  

Copyright (c) [2025] [VagRant2333]  

Permission is hereby granted, free of charge, to any person obtaining a copy  
of this software and associated documentation files (the "Software"), to deal  
in the Software without restriction, including without limitation the rights  
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell  
copies of the Software, and to permit persons to whom the Software is  
furnished to do so, subject to the following conditions:  

The above copyright notice and this permission notice shall be included in all  
copies or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  
SOFTWARE.

## Contact
VagRant - email me or propose an issue!

Project Link: [https://github.com/VagRant2333/2025-FDU-OOP-final-PJ.git](https://github.com/VagRant2333/2025-FDU-OOP-final-PJ.git)