CREATE TABLE IF NOT EXISTS npc_spawn_swarms (
    name          TEXT    PRIMARY KEY
  , territory_tag TEXT    NOT NULL
  , max_npc_count INTEGER NOT NULL CHECK (max_npc_count >= 1)
  , FOREIGN KEY (territory_tag) REFERENCES npc_spawn_territories (tag) ON DELETE CASCADE
);

DELETE FROM npc_spawn_swarms;
INSERT INTO npc_spawn_swarms VALUES
('gludio32_qm1725_00_01',    'gludio32_qm1725_00',    10),
('gludio32_qm1725_01_01',    'gludio32_qm1725_01',    10),
('oren02_qm2119_00_01',      'oren02_qm2119_00',      15),
('oren23_qm2018_00_01',      'oren23_qm2018_00',      15),
('rhion07_qm1814_00_01',     'rhion07_qm1814_00',     15),
('schttgart07_qm2312_00_01', 'schttgart07_qm2312_00', 15);
