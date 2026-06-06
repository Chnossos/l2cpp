# Changelog

All notable changes to this project will be documented in this file.  
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
In no particular order:
- Restore buffs on reconnection;
- Move static data from files to database;
- Skill usage prerequisites + effects conditions.

## [0.32.0] - 2026-06-06
### Added
- Default starting locations per race/profession, or globally overridden

### Changed
- `Position::orientation` type changed and initialization is possible in constructor
- Actors don't spawn at Talking Island GK by default anymore

### Fixed
- Parent profession was already filled, even for starting professions

## [0.31.1] - 2026-06-04
### Fixed
- Starting profession was ok while profession was not on creation

### Changed
- Some properties in table `professions` got moved to `character_templates`

## [0.31.0] - 2026-06-04
### Added
- Character templates (e.g. model collision data)
- Profession templates (e.g. stats)

### Fixed
- Stats are computed more correctly (especially buffs)

### Changed
- All `<class>Directory` classes moved to `game/directories`

## [0.30.0.1] - 2026-06-01
### Changed
- All Pimpl-related `struct <name>Impl` got renamed to `struct Impl`
- `MiniMapShow` renamed to `MiniMapToggle`
- Remains of `AbnormalEffect` got renamed to `Effect`
- `l2cpp::Exception` renamed to `Core::Exception`
- All sources moved to top-level folder `src`
- `login_server` renamed to `ls`
- `game_server` renamed to `gs`
- `common/l2cpp` renamed to `common`
- `#include` style has changed to be more readable (and sortable)

## [0.30.0] - 2026-06-01
### Added
- Character deletion (instantaneous)

### Fixed
- Starting items were duplicated at each start-up
- Inventory content was duplicated when entering the world more than once

## [0.29.0] - 2026-06-01
### Added
- Skill effects of the same skill can be applied to different targets from one another
- New effect target type: `AuraIncludingSelf` so that `Aura` doesn't always target self when it's not desirable
- New demonstration skills: `Aura Flare` (dmg + self buff), `Punch of Doom` (dmg + self stun (not implemented yet))
- System message when interrupting a skill cast

### Changed
- `SkillType` renamed to `SkillOperatingType`
- `SkillTargetType` renamed to `EffectTargetType`
- `EffectTargetType::AoE` renamed to `Multiple`
- `AbnormalEffect` renamed to `Effect`

### Fixed
- TargetNatures `Party`/`Clan`/`Alliance` now always include `Self` as a player is always its own party/clan/alliance
- `Mass Resurrection` doesn't target ennemies anymore

## [0.28.0] - 2026-05-26
### Added
- Starting items can be assigned globally, and/or per profession
- `Tutorial Guide` is given to every new character

### Changed
- Database foreign key checks enabled

### Fixed
- `character_statuses.sql` was missing during database initialization
- Items were cascade-removed at each server boot because item templates are reinserted

## [0.27.1] - 2026-05-26
### Fixed
- Packet OpCode was displayed in place of packet size in logs

## [0.27.0] - 2026-05-22
### Added
- Display project name and version on program startup

## [0.26.2] - 2026-05-22
### Fixed
- Broadcast max distance was way too low (client makes actors disappear from 6000 units onward)

## [0.26.1] - 2026-05-22
### Fixed
- Client can actually send 2-bytes OpCodes
- ManorList OpCode was wrong

## [0.26.0] - 2026-05-22
### Added
- Player can request and perform social actions

## [0.25.0] - 2026-05-21
### Added
- Skill usage message
- Skill effect on/off messages 
- Heal and damage amount messages
- Item (un)equiped message

## [0.24.4] - 2026-05-20
### Fixed
- It's no longer possible to execute a skill that was queued on a target that died from the main action when said skill is not meant to target corpses
- Mass Resurrection requires a valid target to be casted
- Free-target heal spells (e.g. _Greater Heal_) are allowed to target ennemies 

## [0.24.3] - 2026-05-20
### Fixed
- Level up message and animation were sent twice

## [0.24.2] - 2026-05-20
### Fixed
- Dual weapons attack animation was not playing

## [0.24.1] - 2026-05-18
### Fixed
- In-game time was not the same for everyone

## [0.24.0] - 2026-05-18
### Added
- ORM powered by a SQLite3 database to save/load the following:
  - accounts
  - characters (appearance, profession, preview data, shortcuts per profession, xp/sp/etc. per profession)
  - inventory
- Starting items on character creation (not race/profession dependent yet)
- Application will go into emergency shutdown when encountering an exception to save the world (pun intended)
- Admin can set self xp/sp

### Changed
- New logging pattern on both servers
- Character race is now deduced from profession
- Game and Login specific typedefs are segregated now

### Fixed
- RSA operations would fail repeatedly sometimes then go away
- Level up animation and related system message was not firing up in all scenarios
- Character is better cleaned up on disconnection
- Adding xp/sp was bugged toward the limits
- System messages correctly adapt to earning zero XP and/or SP
- MP and CP regen were incorrectly using HP regen value

## [0.23.1] - 2026-05-05
### Changed
- Use an internal event system to notify changes
- StatsUpdatePacket is now sent only once per world update
- Loot is distributed outside of Actor now

### Fixed
- AttackStance is disabled right upon death
- CharacterStatus (xp, sp…) was uninitialized
- Character instances moving in and out of preview were broken

## [0.23.0] - 2026-04-28
### Added
- Monters can have a Loot component with xp/sp reward on killing
- xp/sp rewards are notified in system chat
- An experience table is loaded, players can level up when earning enough xp (with level up animation broadcasted)

### Fixed
- Monster data wasn't populated correctly at creation (while regular npcs were ok)

## [0.22.0] - 2026-04-26
### Added
- HealEffect (instant + over time)
- ResurrectionEffect (single target + aura targetting)
- Resurrection confirmation modal for players

### Changed
- Some big numbers are locale-formatted for readability (e.g. during server initialization)

## [0.21.0] - 2026-04-23
### Added
- Load basic NPC table: `id`, `name`, `title`, default `titleColor`, `type` and `baseSpeed`
- Can spawn a NPC by its name (case-insensitive)

### Changed
- `NonPlayableActor` renamed to `Npc`
- `World` stores actors in a single container now instead of one container per type 

### Removed
- No more function name in exception formatting

## [0.20.0] - 2026-04-23

### Added
- **World:**
  - can look up any actor by GameObjectId
- **Skill:**
  - Load operating type (active, passive, etc.)
  - Load physical/magical status
  - Split target type (single target, AoE, aura…) from target nature (enemy, friendly, self…)
  - Demonstration skills: instant buff, toggle buff, single target damage, multiple targets damage, poison over time…
  - Casting speed is dependent on mAtkSpeed/pAtkSpeed depending on physical/magical status
- **Abnormal Effects:**
  - Buff and damage effects (instant + DoT) added
  - Can be reapplied or canceled
  - Toggle effect

### Improved
- Logs are better aligned
- Stats have been revamped (no more Stats+ComputedStats)

### Fixed
- Application loop was updating twice in a row every updates

### Removed
- HeaderOnlyPackets because they couldn't specify a name (would've needed  `std::basic_fixed_string`)

## [0.19.0] - 2026-04-14

### Added
- Check all RSA operations
- Show packet sending source location (login server)

### Changed
- Improved exception logs (login server)

## [0.18.1] - 2026-04-13

### Fixed
- Auto-regen wasn't triggering

## [0.18.0] - 2026-04-13

### Added
- source location is shown when sending a packet
- packet name is shown when sending

### Removed
- `Packet::finalize()` is not needed anymore, thus removing an additional state

### Fixed
- Program exit code indicated failure in case of success
- Packets are now correctly (shallow) copied

### Changed
- packet encrypt/decrypt (game server) were needlessly complicated
- common's CMakeLists' link targets are now used implicitely in dependant targets
- packet log format improved

## [0.17.0] - 2026-04-12

### Added
- HP/MP/CP regeneration tick system

## [0.16.0] - 2026-04-11

### Added
- Login server can handle multiple users concurrently
- Login server can be stopped gracefully with CTRL+C

## [0.15.0] - 2026-04-09

### Added
- SkillAction
- Actions can be canceled
- Gauge during skill animation
- Position can be deserialized
- Durations are serialized in a more uniform (and correct) way

### Changed
- `Actor::takeDamage()`
- Trying to learn an undefined skill no longer leads to an exception
- Action's performer is now stored for easy access
- Packet hexdumping is disabled by default now

### Fixed
- Skill animation duration was wrongly truncated during parsing

## [0.14.0] - 2026-04-05

### Added
- Actors can be scheduled for deletion
- Dead NPCs disappear after 5s

## [0.13.0] - 2026-04-05

### Added
- GameObject deletion packet (makes objects disappear on screen)

### Changed
- Ensure character removal from world in all scenarios (restart, exit, connection severed)

### Fixed
- Player is correctly unset from character in all scenarios

## [0.12.0] - 2026-04-05

### Added
- Utility to get distance between two positions/actors
- Surrounding actors are sent to player entering world
- Equality operator shortcut for Actors

### Changed
- `World::broadcastAround()` now checks distance around emitter

## [0.11.0] - 2026-04-03

### Added
- Damage can be inflicted and actor status is broadcasted to listeners
- The actor dies when HP reaches zero

### Fixed
- TargetClear was not working anymore

## [0.10.0] - 2026-04-03

### Added
- Monster targets display correct HP bar
- Groundwork for status broadcasting to listeners

### Changed
- Reworked HP/MP/CP/Weight from ActorStatus to Stats+ComputedStats

### Fixed
- Cannot attack oneself

## [0.9.0] - 2026-03-31

### Added
- Can now send system messages to player, with argument support
- Player is greeted upon entering the world

## [0.8.0] - 2026-03-31

### Added
- Characters created are restored as long as the server doesn't stop

### Changed
- Moved code related to character previews into World
- Some packets have been moved to their own implementation file

## [0.7.0] - 2026-03-28

### Added
- Dual weapons (swords, fists and bare fists) now inflict two hits as per their animation

### Changed
- AttackAction keeps track of the initial target so that the action completes even if player changes target
- AttackAction is more generic around the attacker type (Character vs. NPC/Monster)

### Removed
- NPC doesn't scream anymore when hit

## [0.6.0] - 2026-03-26

### Added
- Simple packet broadcasting to all characters
- Remove player and its characters from World on connection closed

### Changed
- Automatic test character now gets a unique name on creation
- Packet logs indicate to/from which session they are related to

## [0.5.0] - 2026-03-25

### Added
- First ECS systems implementation 
- Characters' attack stance toggles off after 5s

## [0.4.0] - 2026-03-24

### Added
- Action system is more fleshed out: current/next action algorithm, queueing actions, interruptible…

## [0.3.1] - 2026-03-24

### Fixed
- Game loop doesn't overuse the CPU when there's nothing much going on

## [0.3.0.1] - 2026-03-24

### Changed
- Replace `OptionalRef = std::option<std::reference_wrapper<T>>` with my own `OptRef` implementation (to avoid writing the ugly `opt->get()` all the damn time)

## [0.3.0] - 2026-03-23

### Added
- In-game time
- Can trigger sunset/sunrise events

## [0.2.0] - 2026-03-23

### Added
- NPCs can talk
- auto-attacking (attack stance, timed to atk speed)
- Arrows are automatically equipped with bow (PoC)
- Can look items up from ItemTemplate::id in ItemStorage

### Fixed
- Dual weapons display correctly (dual fists, dual swords)
