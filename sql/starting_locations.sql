CREATE TABLE IF NOT EXISTS starting_locations (
    starting_profession INTEGER          CHECK (starting_profession >= 0) -- NULL override class-specific starting locations!
  , pos_x               INTEGER NOT NULL
  , pos_y               INTEGER NOT NULL
  , pos_z               INTEGER NOT NULL
  , orientation         INTEGER NOT NULL DEFAULT 0 CHECK (0 <= orientation AND orientation <= 65535)
  , PRIMARY KEY (starting_profession, pos_x, pos_y, pos_z)
  , FOREIGN KEY (starting_profession) REFERENCES professions (id) ON DELETE CASCADE
);
