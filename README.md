<!-- Title -->
<p align="center">
  <img src="Readme_docs/Banner.gif" alt="Bloatware Banner" width="900"/>
</p>


<!-- Shields Icons -->
<p align="center">
  <a href="https://www.digipen.edu.sg/showcase/student-games/bloatware">
    <img src="https://img.shields.io/badge/PLAY_GAME-Bloatware-2f81f7?style=for-the-badge&logo=github"/>
  </a>

  <a href="https://www.youtube.com/watch?v=JPPYLP-RWhg">
    <img src="https://img.shields.io/badge/WATCH_TRAILER-YouTube-ff0000?style=for-the-badge&logo=youtube"/>
  </a>
</p>

<p align="center">
  📦 Installer available in <b><a href="./Bloatware_Installer/">Bloatware_Installer</a></b> or on the 
  <b><a href="https://www.digipen.edu.sg/showcase/student-games/bloatware">official DigiPen page</a></b>
</p>

<!-- Descriptions -->
<p align="center" style="max-width: 850px;">
  <b>
    Play as Byte, a hacker-virus blob trapped inside a corrupted AI system controlled by the rogue supercomputer AXIOM.
  </b>
</p>

<p align="center" style="max-width: 850px;">
  Master size-shifting mechanics to navigate physics-based environments, overcome adaptive AI systems, and infiltrate AXIOM’s core to deploy a destructive virus.
</p>

<!-- Trailer -->
<p align="center">
  <a href="https://www.youtube.com/watch?v=JPPYLP-RWhg">
    <img src="Readme_docs/Video_Thumbnail.png" width="900"/>
  </a>
</p>

## 🎮 Gameplay Showcase
<p align="center">
  <img src="Readme_docs/Levels_1.gif" width="49%" style="border: 2px solid #30363d; border-radius: 10px;"/>
  <img src="Readme_docs/Levels_2.gif" width="49%" style="border: 2px solid #30363d; border-radius: 10px;"/>
</p>

<p align="center">
  <img src="Readme_docs/Levels_3.gif" width="99%" style="border: 2px solid #30363d; border-radius: 10px;"/>
</p>

## ✨ Features
- 🔄 Size-shifting physics gameplay
- 🧠 Adaptive AI boss with Y-axis tracking attacks
- ⚡ Precision platforming with jump leniency system
- 🧩 Key & door progression puzzles
- 🌪 Wind tunnel mechanics (size-dependent interaction)
- 🎭 Fully scripted narrative cutscenes + voiceover system

## 🗂 Project Structure
BLOATWARE is organized into two main components: a **custom engine framework** and the **game implementation layer** built on top of it.

### ⚙️ Custom Game Engine (Core Framework)
The engine provides the foundational systems that power the entire game.

It handles:
- Core game loop and scene management  
- Input handling (keyboard & controller support)  
- Physics system (collision detection, movement, constraints)  
- Rendering pipeline and visual updates  
- Audio system and event-based playback  
- Core architecture for entity and gameplay systems  

> The engine is designed as a reusable framework for 2D gameplay systems.

### 🎮 Game (BLOATWARE Build Layer)
The game layer is built on top of the engine and defines the actual playable experience. 

It includes:
- Player mechanics (size-shifting system, movement behavior)  
- AXIOM AI logic and boss behavior systems  
- Cutscenes, dialogue, and narrative scripting  
- Level design, puzzles, and progression flow  
- Gameplay interactions (lasers, wind tunnels, keys & doors)  
- Game-specific UI, VFX, and audio content  

> This layer represents the final playable build distributed to players.


### 🔗 Relationship
The engine provides systems and infrastructure, while the game defines how those systems are used to create gameplay.

## 🎮 Controls
| Action | Keyboard | Controller |
|--------|----------|------------|
| Move | ← → | D-Pad |
| Jump | ↑ | X |
| Shrink | Space (hold) | R1 |
| Jump Boost | ↑ + Space | — |
| Pause | Esc | — |
| Restart | R | — |

**Cheats**
- Skip Cutscene: O / Enter / Triangle
- Skip Levels: 1 / L1
- FPS Toggle: G
- Fullscreen: Alt + Enter

## ⚠️ Technical Notes
- Best performance when plugged in
- Alt-tab may cause rendering/audio desync
- Pause menu does not pause audio (engine limitation)
- Boss fight uses adaptive laser cooldown system
- Some cutscene pauses are intentional design

## 🎬 Game Flow
<b>Main progression loop</b>
Menu → Tutorial → Main Levels → Mid Cutscene → Boss Mode (AXIOM) → Ending → Credits → Return

## 🔊 Audio
- Event-triggered voiceover system
- Persistent BGM/SFX layers
- Contextual audio tied to gameplay and death states

<!--- Members --->
## 👥 Team Members

<table>
  <tr>
    <!--- Mong Chuan --->
    <td align="center" style="padding:20px" width=180>
      <img src="https://github.com/mongchuan.png?size=200" width="100" height="100" style="border-radius:25%;" />
      <div><strong>Sim Mong Chuan</strong></div>
      <div style="font-size:12px; color:gray;">Project Manager <br/> Production Pipeline & Debugging </div>
      <div style="margin-top:6px;">
        <a href="https://www.linkedin.com/in/mong-chuan/" target="_blank">
        <img src="https://upload.wikimedia.org/wikipedia/commons/e/e9/Linkedin_icon.svg" 
                  alt="LinkedIn" width="18" height="18" style="vertical-align: middle;"/>
        Mong Chuan Sim
        </a>
        <br>
        <a href="https://github.com/mongchuan">
          <img src="https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" 
                    alt="GitHub" width="18" height="18" style="vertical-align: middle;" />
          @mongchuan
        </a>
      </div>
    </td>
    <!--- Wen Xi --->
    <td align="center" style="padding:20px" width=180>
      <img src="https://github.com/TeoWenXi.png?size=200" width="100" height="100" style="border-radius:25%;" />
      <div><strong>Teo Wen Xi</strong></div>
      <div style="font-size:12px; color:gray;">Technical Lead <br/> Engine Systems </div>
      <div style="margin-top:6px;">
        <a href="https://www.linkedin.com/in/teowenxi/" target="_blank">
        <img src="https://upload.wikimedia.org/wikipedia/commons/e/e9/Linkedin_icon.svg" 
                  alt="LinkedIn" width="18" height="18" style="vertical-align: middle;"/>
        Teo Wen Xi
        </a>
        <br>
        <a href="https://github.com/TeoWenXi">
          <img src="https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" 
                    alt="GitHub" width="18" height="18" style="vertical-align: middle;" />
          @TeoWenXi
        </a>
      </div>
    </td>
    <!--- Zephan --->
    <td align="center" style="padding:20px" width=180>
      <img src="https://github.com/zephan2000.png?size=200" width="100" height="100" style="border-radius:25%;" />
      <div><strong>Zephan Wong</strong></div>
      <div style="font-size:12px; color:gray;">Art & Design Lead <br/> Audio Design & UI/UX </div>
      <div style="margin-top:6px;">
        <a href="https://www.linkedin.com/in/zephan-wong/" target="_blank">
        <img src="https://upload.wikimedia.org/wikipedia/commons/e/e9/Linkedin_icon.svg" 
                  alt="LinkedIn" width="18" height="18" style="vertical-align: middle;"/>
        Zephan Wong
        </a>
        <br>
        <a href="https://github.com/zephan2000">
          <img src="https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" 
                    alt="GitHub" width="18" height="18" style="vertical-align: middle;" />
          @zephan2000
        </a>
      </div>
    </td>
    <!--- Jovan --->
    <td align="center" style="padding:20px" width=180>
      <img src="https://github.com/JovanLowZhuoWen.png?size=200" width="100" height="100" style="border-radius:25%;" />
      <div><strong>Jovan Low</strong></div>
      <div style="font-size:12px; color:gray;">Designer <br/> VFX & Technical Art </div>
      <div style="margin-top:6px;">
        <a href="https://www.linkedin.com/in/jovan-low-1b23542aa/" target="_blank">
        <img src="https://upload.wikimedia.org/wikipedia/commons/e/e9/Linkedin_icon.svg" 
                  alt="LinkedIn" width="18" height="18" style="vertical-align: middle;"/>
        Jovan Low
        </a>
        <br>
        <a href="https://github.com/JovanLowZhuoWen">
          <img src="https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" 
                    alt="GitHub" width="18" height="18" style="vertical-align: middle;" />
          @JovanLowZhuoWen
        </a>
      </div>
    </td>
  </tr>
  <tr>
    <!--- Li Heng --->
    <td align="center" style="padding:20px" width=180>
      <img src="https://github.com/SilentflameX.png?size=200" width="100" height="100" style="border-radius:25%;" />
      <div><strong>Hear Li Heng</strong></div>
      <div style="font-size:12px; color:gray;">Programmer <br/> Level Design Systems </div>
      <div style="margin-top:6px;">
        <a href="https://www.linkedin.com/in/hearliheng/" target="_blank">
        <img src="https://upload.wikimedia.org/wikipedia/commons/e/e9/Linkedin_icon.svg" 
                  alt="LinkedIn" width="18" height="18" style="vertical-align: middle;"/>
        Hear Li Heng
        </a>
        <br>
        <a href="https://github.com/SilentflameX">
          <img src="https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" 
                    alt="GitHub" width="18" height="18" style="vertical-align: middle;" />
          @SilentflameX
        </a>
      </div>
    </td>
    <!--- Xin Tian --->
    <td align="center" style="padding:20px" width=180>
      <img src="https://github.com/xintian-sia.png?size=200" width="100" height="100" style="border-radius:25%;" />
      <div><strong>Xin Tian Sia</strong></div>
      <div style="font-size:12px; color:gray;">Programmer <br/> Physics Systems </div>
      <div style="margin-top:6px;">
        <a href="https://www.linkedin.com/in/sia-xin-tian/" target="_blank">
        <img src="https://upload.wikimedia.org/wikipedia/commons/e/e9/Linkedin_icon.svg" 
                  alt="LinkedIn" width="18" height="18" style="vertical-align: middle;"/>
        Xin Tian Sia
        </a>
        <br>
        <a href="https://github.com/xintian-sia">
          <img src="https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" 
                    alt="GitHub" width="18" height="18" style="vertical-align: middle;" />
          @xintian-sia
        </a>
      </div>
    </td>
    <!--- Wen Jin --->
    <td align="center" style="padding:20px" width=180>
      <img src="https://github.com/ZsderDigipen.png?size=200" width="100" height="100" style="border-radius:25%;" />
      <div><strong>Wenjin Cai</strong></div>
      <div style="font-size:12px; color:gray;">Programmer <br/> Graphics Programming </div>
      <div style="margin-top:6px;">
        <a href="https://www.linkedin.com/in/wenjin-cai/" target="_blank">
        <img src="https://upload.wikimedia.org/wikipedia/commons/e/e9/Linkedin_icon.svg" 
                  alt="LinkedIn" width="18" height="18" style="vertical-align: middle;"/>
        Wenjin Cai
        </a>
        <br>
        <a href="https://github.com/ZsderDigipen">
          <img src="https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" 
                    alt="GitHub" width="18" height="18" style="vertical-align: middle;" />
          @ZsderDigipen
        </a>
      </div>
    </td>
    <!--- Kang Zheng --->
    <td align="center" style="padding:20px" width=180>
      <img src="https://github.com/TanKangZheng.png?size=200" width="100" height="100" style="border-radius:25%;" />
      <div><strong>Tan Kang Zheng</strong></div>
      <div style="font-size:12px; color:gray;">Programmer <br/> Gameplay Mechanics </div>
      <div style="margin-top:6px;">
        <a href="https://www.linkedin.com/in/tankangzhengsit/" target="_blank">
        <img src="https://upload.wikimedia.org/wikipedia/commons/e/e9/Linkedin_icon.svg" 
                  alt="LinkedIn" width="18" height="18" style="vertical-align: middle;"/>
        Tan Kang Zheng
        </a>
        <br>
        <a href="https://github.com/TanKangZheng">
          <img src="https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" 
                    alt="GitHub" width="18" height="18" style="vertical-align: middle;" />
          @TanKangZheng
        </a>
      </div>
    </td>
  </tr>
</table>

## 🏁 Credits
Developed by **Dead By Midnight** as part of DigiPen coursework.