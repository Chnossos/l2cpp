CREATE TABLE IF NOT EXISTS item_templates (
    id        INTEGER PRIMARY KEY
  , name      TEXT    NOT NULL
  , category  INTEGER NOT NULL
  , grade     INTEGER NOT NULL
  , gear_slot INTEGER NOT NULL
);

INSERT OR IGNORE INTO item_templates VALUES
(1345, 'Shining Arrow',            5, 5, 256),
(5588, 'Tutorial Guide',           5, 0, 0),
(6377, 'Imperial Crusader Shield', 0, 5, 256),
(6408, 'Formal Wear',              1, 0, 131072),
(6611, 'Infinity Blade',           0, 5, 128),
(6618, 'Infinity Fang',            0, 5, 16384),
(6619, 'Infinity Bow',             0, 5, 16384),
(6621, 'Infinity Spear',           0, 5, 16384);
