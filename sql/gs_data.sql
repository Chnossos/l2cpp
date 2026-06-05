-- Can't check for duplicates, so we reinsert all entries each time
DELETE FROM starting_items;
-- For test purposes only:
INSERT INTO starting_items (profession, item_template_id, quantity, equipped) VALUES
(NULL, 6408, NULL, TRUE),  -- Formal Wear
(NULL, 6621, NULL, FALSE), -- Infinity Spear
(NULL, 6611, NULL, TRUE),  -- Infinity Blade
(NULL, 6377, NULL, TRUE),  -- Imperial Crusader Shield
(NULL, 6618, NULL, FALSE), -- Infinity Fangs
(NULL, 6619, NULL, FALSE), -- Infinity Bow
(NULL, 1345, 100,  FALSE); -- Arrow (S-grade)

INSERT INTO starting_items (profession, item_template_id, quantity, equipped) VALUES
(NULL, 5588, NULL, NULL);  -- Tutorial Guide

INSERT OR IGNORE INTO characters (
    account_id
  , name
  , starting_profession
  , sex
  , hair_style
  , hair_color
  , face
  , title
  , pos_x
  , pos_y
  , pos_z
  , current_profession
)
VALUES
(1, 'Admin',  0,  0, 0, 0, 0, '{l2cpp}', -83968, 244634, -3500, 88), -- on subclass Duelist
(2, 'Admin2', 25, 1, 0, 0, 0, '{l2cpp}', -83968, 244634, -3500, 25); -- on main class Elven Mystic

INSERT OR IGNORE INTO character_professions VALUES
(1,  0), -- Human Fighter (main class)
(1, 88), -- Duelist       (1st subclass)
(2, 25); -- Elven Mystic  (main class)

INSERT OR IGNORE INTO character_statuses VALUES
(1,  0,          0, 0, 1, 1, 1), -- Human Figter lv.  1    0xp 0sp 1hp 1mp 1cp
(1, 88, 2099275833, 0, 1, 1, 1), -- Duelist      lv. 78 100%xp 0sp 1hp 1mp 1cp
(2, 25,          0, 0, 1, 1, 1); -- Elven Mystic lv.  1    0xp 0sp 1hp 1mp 1cp

INSERT INTO starting_locations (pos_x, pos_y, pos_z) VALUES (-83968, 244634, -3500); -- Talking Island GK
