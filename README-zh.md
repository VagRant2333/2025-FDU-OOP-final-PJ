# DenPaKid GAME - 复旦大学2025OOP PJ

[切换到中文](README-zh.md) | [Switch to English](README.md)

## 游戏描述
1. **简介：**
<details>
<summary>点击展开</summary>
欢迎来到网格世界。这是一片破碎的赛博朋克荒原，空气中弥漫着致命的能量。摇摇欲坠的巨型建筑闪烁着灾难性过去的幽灵之光，交织着致命的电磁光束和毁灭性的磁暴。在这里，生存不取决于力量，而在于共振。

你是DenPaKid，一个在风暴中诞生的孤独跑者。利用你体内涌动的独特生物电荷，操控你的极性，驾驭磁流，并在绝望的冲刺中干扰致命的能量场。在这个物理规则本身既是最致命的敌人又是唯一盟友的环境中，这不仅仅是奔跑，更是与闪电共舞。

- **掌握电动力学：** 使用基于电荷的直观物理机制跳跃、滑行和反弹，将环境中的致命能量转化为武器。
- **穿越电磁死亡陷阱：** 在衰败的霓虹废墟中，超越电离激光，躲避磁涡旋，驾驭不稳定的电荷梯度。
- **揭开闪烁的回声：** 发现碎片化的数据卷轴，它们低语着关于网格崩溃和你自身电压赋予力量的被遗忘真相。
- **为生存而跑，为记忆而跑：** 你能将自己的极限推多远？你敢深入网格的电磁心脏多深？

电流在召唤。奔跑、适应、共振。你会成为传奇...还是只是静电中熄灭的另一簇火花？
</details>

2. **玩法说明：**


| 按键 | 动作描述 | 功能详情 |
|-----|--------------------|------------------|
| W   | 向北移动 | 四方向移动控制；每次按下触发一次位移。位移技能最多限制使用10次 |
| A   | 向西移动 | 四方向移动控制；每次按下触发一次位移。位移技能最多限制使用10次 |
| S   | 向南移动 | 四方向移动控制；每次按下触发一次位移。位移技能最多限制使用10次 |
| D   | 向东移动 | 四方向移动控制；每次按下触发一次位移。位移技能最多限制使用10次 |
| Q   | 增加电荷 | 增加电荷 |
| E   | 减少电荷 | 减少电荷 |
| 空格 | 切换电荷极性 | 在正电荷与负电荷之间切换（反之亦然） |


### 注意事项：
1. **位移技能：** 通过W/A/S/D移动会消耗位移电荷，在重新充电前最多使用10次。
2. **电荷管理：** Q/E键调整电荷强度，空格键翻转电荷极性（影响与电磁场的交互）。
3. **游戏机制：** 电荷水平直接影响玩家与环境场的交互和技能效果。


## 世界观设定
欢迎来到“网格”——一片荒凉、受辐射的巨型城市蔓延区，曾由革命性的共振能量技术（RET）提供动力。几十年前，RET核心发生灾难性过载（“闪烁”），导致社会崩溃，废墟中充满了致命的混沌电磁场。这些场表现为致命的光束（电离“激光”）、毁灭性的磁涡旋和不稳定的电荷区。只有像DenPaKid这样被“闪烁”变异的人，才能部分操控自身的生物电荷，在这个电磁死亡陷阱中穿行。你奔跑是为了生存，搜寻珍贵的数据卷轴，也许还能找到稳定网格的方法...或者逃离它。

## 项目概述
复旦大学2025年春季学期面向对象编程（OOP）课程期末项目

## 可修改的参数

<details>
<summary>点击展开</summary>
在`src/entities/Player.h`中：
这些常量控制玩家的核心能力。

- `static constexpr float MAX_CHARGE;` 玩家可拥有的最大绝对电荷。值越高，与场的交互越强。
- `static constexpr float MIN_CHARGE_MAGNITUDE;` 最小非零电荷强度。
- `static constexpr float CHARGE_STEP;` 每次按Q/E时增减的电荷量。增大可加快电荷管理速度。

在`src/physics/PhysicsEngine.cpp`中：
这些参数决定玩家移动的“手感”。

- `m_playerMass;`（在GameScene::setupInitialState()中设置）影响加速玩家所需的力。质量越高，玩家感觉越“重”，对力的反应越慢。
- `const float MAX_SPEED;` 玩家的最大可能速度。
- `float restitution;` 碰撞墙壁时的“弹性”因子（0到1）。1.0为完全弹性碰撞，0.0为完全无反弹。
- `const float DAMPING_FACTOR = 0.1f;` 简单的摩擦/空气阻力。值越高，无作用力时玩家减速越快，操控感更“紧凑”。

在`src/scene/GameScene.h`中：
这些控制游戏环境的行为。

- `static constexpr float B_FIELD_DENSITY_SCALE_FACTOR;` 按磁场强度缩放显示的'X'或'.'符号数量，纯粹是视觉参数。
- `static constexpr int MAX_B_FIELD_SYMBOLS_PER_ROW_COL = 15;` 限制磁场符号的网格大小，避免高场强时视觉混乱。

在`src/scene/GameScene.cpp`中：
这些控制事件频率和难度。

- `m_bgScrollSpeed = 150.f;`（在loadAssets()中）背景滚动速度，也影响m_distanceTraveled的增加速度。
- `m_timeBetweenLaserSpawns:` 在update()中随机在1.5秒到4.0秒之间。更改范围可调整激光生成频率。
- `m_timeBetweenScrollSpawns:` 在update()中随机在8.0秒到15.0秒之间。调整可改变卷轴出现频率。
- `laserSpeed:` 在spawnLaser()中随机在300到500之间（300 + m_rng() % 200）。扩大范围可增加激光难度。
- **场强设置**（在randomizeFields()中）：
  - `std::uniform_real_distribution<float> e_dist();` 电场强度范围。如(-100.f, 100.f)这样的宽范围会使游戏难度大幅增加。
  - `std::uniform_real_distribution<float> b_dist();` 磁场强度范围。

在`src/core/Game.h`中：
这些定义整体游戏目标。

- `const int m_totalScrolls = 5;` 需要收集的唯一卷轴总数。
- `const float m_requiredDistanceForWin;` 玩家必须超过的距离（除收集所有卷轴外）才能赢得游戏。
</details>

## 安装
```bash
git clone https://github.com/VagRant2333/2025-FDU-OOP-final-PJ.git
```

## 使用方法
```bash
# 请切换到游戏目录
cmake .
make

cd build/bin
./DenPaKid

# Windows系统：.\DenPaKid.exe

# 如果使用WSL：

# export DISPLAY=:0
# export ALSOFT_DRIVERS=pulse
# ./DenPaKid

# 如果使用远程SSH：暂不支持X11服务
```

## 鸣谢
本游戏使用了AI生成的资源和在线音乐
如有版权问题请联系，我将尽快删除。

## 联系方式
VagRant - email avialable on Github

项目链接：[https://github.com/VagRant2333/2025-FDU-OOP-final-PJ.git](https://github.com/VagRant2333/2025-FDU-OOP-final-PJ.git)