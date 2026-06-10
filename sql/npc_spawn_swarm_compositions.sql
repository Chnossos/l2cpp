CREATE TABLE IF NOT EXISTS npc_spawn_swarm_compositions (
    swarm_name       TEXT    NOT NULL
  , npc_template_id  INTEGER NOT NULL
  , npc_count        INTEGER NOT NULL CHECK (npc_count        >= 0)
  , respawn_duration INTEGER NOT NULL CHECK (respawn_duration >= 0)
  , respawn_window   INTEGER NOT NULL CHECK (respawn_window   >= 0)
  , PRIMARY KEY (swarm_name, npc_template_id)
  , FOREIGN KEY (swarm_name) REFERENCES npc_spawn_swarms (name) ON DELETE CASCADE
);

DELETE FROM npc_spawn_swarm_compositions;
INSERT INTO npc_spawn_swarm_compositions VALUES
('gludio32_qm1725_00_01',     5198,  10,  15,  0),
('gludio32_qm1725_01_01',     5198,  10,  15,  0),
('oren02_qm2119_00_01',       5198,  15,  15,  0),
('oren23_qm2018_00_01',       5198,  15,  15,  0),
('rhion07_qm1814_00_01',      5198,  15,  15,  0),
('schttgart07_qm2312_00_01',  5198,  15,  15,  0);
