# l2cpp

[Lineage 2, Chronicle 4: Scions of Destiny][4] C++23 emulation servers.

This is my latest passion project. I've been wanting to do this since I was 15, and here I am 16 years later with enough programming skills to do this on my own.

Even though I never played C4 (began on Interlude), I'm targeting this one because the game is less big and the protocol is slightly easier. I'd love to support more protocols in the future.

## The goal

Go fast (but not too fast) and break things. YAGNI. Implement game design patterns I've studied years ago. Don't be too clever but have fun implementing and testing. DO **NOT** USE MULTIPLE THREADS. Leverage async code instead.

## Future releases

See the _Unrealeased_ section of [CHANGELOG.md](CHANGELOG.md)

## What's available?

If it's not in the list, it's probably not implemented, or very (very) partially implemented. Here's what's been done so far:

**Core**
- [x] Packet creation, reading and sending;
- [x] Auth & Game protocols encryption/decryption;
- [x] Exception stack handling.

**Login Server**
- [x] Protocol `c621` only;
- [x] Users can log in with any credentials, account is created if non existent;
- [x] Server list always displays 2 server (to test one up, one down), both lead to game server.

**Game Server**
<table>
  <tr>
    <th>Category</th>
    <th>Feature</th>
    <th>Status</th>
    <th>Details</th>
  </tr>
  <tr>
    <td align="center">Authentication</td>
    <td>Supported protocols</td>
    <td align="center"><code>656</code></td>
    <td></td>
  </tr>
  <tr>
    <td align="center" rowspan=3>Lobby</td>
    <td>Character creation</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td>Character deletion</td>
    <td align="center">🟢</td>
    <td>⚠️ Deletion is instantaneous!</td>
  </tr>
  <tr>
    <td>Character selection</td>
    <td align="center">🟢</td>
    <td>Every character spawns at Talking Island.</td>
  </tr>
  <tr>
    <td align="center" rowspan=4>Inventory</td>
    <td>List</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td>Equip gear</td>
    <td align="center">🟠</td>
    <td>No effects applied yet.</td>
  </tr>
  <tr>
    <td>Starting items</td>
    <td align="center">🟢</td>
    <td>Can be configured per starting profession and/or globally.<br>Gear items can be set to equipped.</td>
  </tr>
  <tr>
    <td>Item templates database</td>
    <td align="center">🟠</td>
    <td>Most templates are missing due to datapack conversion effort needed.</td>
  </tr>
  <tr>
    <td align="center" rowspan=2>Status Window</td>
    <td>Display values</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td>Refresh on update</td>
    <td align="center">🟢</td>
    <td>Optimization: max. once per World update.</td>
  </tr>
  <tr>
    <td align="center" rowspan=3>Mini-Map</td>
    <td>Open/Close</td>
    <td align="center">🟠</td>
    <td>World map only.</td>
  </tr>
  <tr>
    <td>Party members location</td>
    <td align="center">🔴</td>
    <td></td>
  </tr>
  <tr>
    <td>In-game time</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td align="center" rowspan=5>Social</td>
    <td>Send/Receive messages</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td>Send/Receive private messages</td>
    <td align="center">🟠</td>
    <td>Visually only.</td>
  </tr>
  <tr>
    <td>Chat restrictions</td>
    <td align="center">🔴</td>
    <td></td>
  </tr>
  <tr>
    <td>System messages</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td>Social actions</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td align="center" rowspan="2">Combat</td>
    <td>Auto-attacking</td>
    <td align="center">🟢</td>
    <td>Until target is dead.</td>
  </tr>
  <tr>
    <td>Corpse removal</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td align="center" rowspan=8>Skills</td>
    <td>List</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td>Learn</td>
    <td align="center">🟠</td>
    <td>Via admin command only.</td>
  </tr>
  <tr>
    <td>Use</td>
    <td align="center">🟢</td>
    <td>Single and multiple targets animation.</td>
  </tr>
  <tr>
    <td>Effects</td>
    <td align="center">🟠</td>
    <td>
      Available effects:
      <ul style="list-style: '– ';">
          <li>Instant buff (e.g. <code>Wind Walk</code>)</li>
          <li>Toggle buff (e.g. <code>Super Haste</code>)</li>
          <li>Instant damage (e.g. <code>Wind Strike</code>)</li>
          <li>Damage over time (e.g. <code>Poison</code>)</li>
          <li>Instant heal (e.g. <code>Battle Heal</code>)</li>
          <li>Heal over time (e.g. <code>Chant of Life</code>)</li>
          <li>Resurrection (e.g. <code>Mass Resurrection</code>)</li>
      </ul>
    </td>
  </tr>
  <tr>
    <td>Prerequisites</td>
    <td align="center">🟠</td>
    <td>No mana/health consumption yet.<br>Target matching combination is possible.</td>
  </tr>
  <tr>
    <td>Skill templates database</td>
    <td align="center">🟠</td>
    <td>All skills loaded with bare minimum info.</td>
  </tr>
  <tr>
  <tr>
    <td>Cast cancellation</td>
    <td align="center">🟢</td>
    <td></td>
  </tr>
  <tr>
    <td align="center">Shortcuts</td>
    <td>Add/Replace/Remove</td>
    <td align="center">🟠</td>
    <td>Limited to skills, items and actions.</td>
  </tr>
  <tr>
    <td align="center" rowspan="2">Movement</td>
    <td>Left-click</td>
    <td align="center">🟠</td>
    <td>No checks done whatsoever.</td>
  </tr>
  <tr>
    <td>Geodata</td>
    <td align="center">🔴</td>
    <td></td>
  </tr>
</table>

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

1. Download a game client supporting protocol 656
2. Build the project
3. Start a login server from the project root
4. Start a game server from the project root
5. Use the client to log in, and voilà!

## Credits

[l2auth][1], [L2jMobius][3] and [L2JLisvus][5].

[1]: https://github.com/Ruk33/l2auth
[2]: https://github.com/Ruk33
[3]: https://gitlab.com/MobiusDevelopment/L2J_Mobius/
[4]: https://legacy-lineage2.com/news/chronicle4_01.html
[5]: https://gitlab.com/TheDnR/l2j-lisvus
