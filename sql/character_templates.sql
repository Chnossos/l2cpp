CREATE TABLE IF NOT EXISTS character_templates (
    starting_profession     INTEGER PRIMARY KEY CHECK (starting_profession     >= 0)
  , male_collision_height   DOUBLE  NOT NULL    CHECK (male_collision_height   >= 0)
  , male_collision_radius   DOUBLE  NOT NULL    CHECK (male_collision_radius   >= 0)
  , female_collision_height DOUBLE  NOT NULL    CHECK (female_collision_height >= 0)
  , female_collision_radius DOUBLE  NOT NULL    CHECK (female_collision_radius >= 0)
  , str                     INTEGER NOT NULL    CHECK (str                     >= 0)
  , dex                     INTEGER NOT NULL    CHECK (dex                     >= 0)
  , con                     INTEGER NOT NULL    CHECK (con                     >= 0)
  , "int"                   INTEGER NOT NULL    CHECK ("int"                   >= 0)
  , wit                     INTEGER NOT NULL    CHECK (wit                     >= 0)
  , men                     INTEGER NOT NULL    CHECK (men                     >= 0)
  , patk                    INTEGER NOT NULL    CHECK (patk                    >= 0)
  , matk                    INTEGER NOT NULL    CHECK (matk                    >= 0)
  , pdef                    INTEGER NOT NULL    CHECK (pdef                    >= 0)
  , mdef                    INTEGER NOT NULL    CHECK (mdef                    >= 0)
  , patk_speed              INTEGER NOT NULL    CHECK (patk_speed              >= 0)
  , matk_speed              INTEGER NOT NULL    CHECK (matk_speed              >= 0)
  , run_speed               INTEGER NOT NULL    CHECK (run_speed               >= 0)
  , walk_speed              INTEGER NOT NULL    CHECK (walk_speed              >= 0)
  , FOREIGN KEY (starting_profession) REFERENCES professions (id) ON DELETE RESTRICT
);

INSERT OR IGNORE INTO character_templates VALUES
( 0,  23.0,   9.0,  23.5,  8.0,  40,  30,  43,  21,  11,  25,  4,  6,  80,  41,  300,  333,  115,  80),
(10,  22.8,   7.5,  22.5,  6.5,  22,  21,  27,  41,  20,  39,  3,  6,  54,  41,  300,  333,  120,  78),
(18,  24.0,   7.5,  23.0,  7.5,  36,  35,  36,  23,  14,  26,  4,  6,  80,  41,  300,  333,  125,  90),
(25,  24.0,   7.5,  23.0,  7.5,  21,  24,  25,  37,  23,  40,  3,  6,  54,  41,  300,  333,  122,  85),
(31,  24.0,   7.5,  23.5,  7.0,  41,  34,  32,  25,  12,  26,  4,  6,  80,  41,  300,  333,  122,  85),
(38,  24.0,   7.5,  23.5,  7.0,  23,  23,  24,  44,  19,  37,  3,  6,  54,  41,  300,  333,  122,  85),
(44,  28.0,  11.0,  27.0,  7.0,  40,  26,  47,  18,  12,  27,  4,  6,  80,  41,  300,  333,  117,  70),
(49,  27.5,   7.0,  25.5,  8.0,  27,  24,  31,  31,  15,  42,  3,  6,  54,  41,  300,  333,  121,  70),
(53,  18.0,   9.0,  19.0,  5.0,  39,  29,  45,  20,  10,  27,  4,  6,  80,  41,  300,  333,  115,  80);
