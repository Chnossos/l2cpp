CREATE TABLE IF NOT EXISTS character_statuses (
    character_id INTEGER
  , profession   INTEGER
  , xp           INTEGER NOT NULL DEFAULT 0
  , sp           INTEGER NOT NULL DEFAULT 0
  , hp           INTEGER NOT NULL
  , mp           INTEGER NOT NULL
  , cp           INTEGER NOT NULL
  , PRIMARY KEY (character_id, profession)
  , FOREIGN KEY (character_id, profession) REFERENCES character_professions (character_id, profession) ON DELETE CASCADE
);
