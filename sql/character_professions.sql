CREATE TABLE IF NOT EXISTS character_professions (
    character_id INTEGER NOT NULL
  , profession   INTEGER NOT NULL
  , PRIMARY KEY (character_id, profession)
  , FOREIGN KEY (character_id) REFERENCES characters  (id) ON DELETE CASCADE
  , FOREIGN KEY (profession)   REFERENCES professions (id) ON DELETE CASCADE
);
