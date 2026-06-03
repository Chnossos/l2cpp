CREATE TABLE IF NOT EXISTS character_templates (
    starting_profession INTEGER NOT NULL CHECK (starting_profession >= 0)
  , sex                 INTEGER NOT NULL CHECK (sex IN (0, 1))
  , collision_height    DOUBLE  NOT NULL
  , collision_radius    DOUBLE  NOT NULL
  , PRIMARY KEY (starting_profession, sex)
);

INSERT OR IGNORE INTO character_templates VALUES
( 0,  0,  23.0,   9.0),
( 0,  1,  23.5,   8.0),
(10,  0,  22.8,   7.5),
(10,  1,  22.5,   6.5),
(18,  0,  24.0,   7.5),
(18,  1,  23.0,   7.5),
(25,  0,  24.0,   7.5),
(25,  1,  23.0,   7.5),
(31,  0,  24.0,   7.5),
(31,  1,  23.5,   7.0),
(38,  0,  24.0,   7.5),
(38,  1,  23.5,   7.0),
(44,  0,  28.0,  11.0),
(44,  1,  27.0,   7.0),
(49,  0,  27.5,   7.0),
(49,  1,  25.5,   8.0),
(53,  0,  18.0,   9.0),
(53,  1,  19.0,   5.0);
