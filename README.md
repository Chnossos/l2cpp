# l2cpp

[Lineage 2, Chronicle 4: Scions of Destiny][4] C++23 emulation servers.

This is my latest passion project. I've been wanting to do this since I was 15, and here I am 16 years later with enough programming skills to do this on my own.

Even though I never played C4 (began on Interlude), I'm targeting this one because the game is less big and the protocol is slightly easier. I'd love to support more protocols in the future.

## The goal

Go fast (but not too fast) and break things. YAGNI. Implement game design patterns I've studied years ago. Don't be too clever but have fun implementing and testing. DO **NOT** USE MULTIPLE THREADS. Leverage async code instead.

## What's available?

Unless explicitely specified, don't assume anything to be available. Here's what's been done so far:

**Core**
- [x] Packet creation, reading and sending;
- [x] Auth & Game protocols encryption/decryption;
- [x] Exception stack handling.

**Login Server**
- [x] Protocol `c621` only;
- [x] Users can log in with any credentials, account is created if non existent;
- [x] Server list always displays 2 server (to test one up, one down), both lead to game server.

**Game Server**
- [x] Protocol `656` only 
- [x] Character creation (with starting items)
- [x] World entering (every character spawns at Talking Island)
- [x] Moving around
- [x] Inventory list (gear items can be equipped and unequipped)
- [x] Mini-map
- [x] Character status
- [x] Target select and unselect
- [x] game loop to handle over-time actions and updates (regen, auto-attacks, DoTs…)
- [x] Skills list (admins can use `//learn <skill_id> <skill_level>` command)
- [x] Skill use, available effects:
  - Instant buff (e.g. `Wind Walk`)
  - Toggle buff (e.g. `Super Haste`)
  - Instant damage (e.g. `Wind Strike`)
  - Damage over time (e.g. `Poison`)
  - Instant heal (e.g. `Battle Heal`)
  - Heal over time (e.g. `Chant of Life`)
  - Resurrection (e.g. `Mass Resurrection`)
- [x] Skill target types: `self`, `single`, `aura`, `aoe`
- [x] Skill target nature (can be combined): `self`, `enemy`, `friendly`, `corpse`, `character`, `monster`, `npc`, etc. 
- [x] Skill casting cancellation with ESC key
- [x] Shortcuts: `skill`, `item`, `action`
- [x] Chat (no restrictions)
- [x] Auto-attacking (until target is dead)
- [x] Status (health, mana…) modifications broadcasting (once per world update)
- [x] In-game time
- [x] Corpse removal after a few seconds
- [x] System messages and confirmation modals (e.g. for resurrection)
- [x] Social actions

## What's coming next

See the _Unrealeased_ section of [CHANGELOG.md](CHANGELOG.md)

## How to build

I develop with the following environment:
 - Windows 11
 - Visual Studio Build Tools 2026
 - CMake 4.2 (required for VS 2026 generator)
 - Conan (latest as of 2026-04-12)
 - Ninja (latest as of 2026-04-12)

```shell
git clone https://github.com/Chnossos/l2cpp.git && cd l2cpp
conan install -b missing -c tools.cmake.cmaketoolchain:generator="Ninja Multi-Config"
cmake --preset conan-default
cmake --build build --preset conan-release # or conan-debug
```

## How to run

Build, then start a login server, then a game server. Now log in with a game client supporting protocol 656 and voilà!

## Credits

[l2auth][1], [L2jMobius][3] and [L2JLisvus][5].

[1]: https://github.com/Ruk33/l2auth
[2]: https://github.com/Ruk33
[3]: https://gitlab.com/MobiusDevelopment/L2J_Mobius/
[4]: https://legacy-lineage2.com/news/chronicle4_01.html
[5]: https://gitlab.com/TheDnR/l2j-lisvus
