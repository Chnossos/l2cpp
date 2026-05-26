CREATE TABLE IF NOT EXISTS starting_items (
    profession       INTEGER          -- NULL means item is given regardless of profession
  , item_template_id INTEGER NOT NULL
  , quantity         INTEGER          -- NULL means item is not stackable
  , equipped         BOOLEAN          -- NULL means item is not equippable
  , FOREIGN KEY (item_template_id) REFERENCES item_templates (id) ON DELETE CASCADE
);

CREATE TRIGGER IF NOT EXISTS starting_items_trigger
    AFTER
        INSERT ON characters
    BEGIN
        INSERT INTO
            items (template_id, owner_id, storage_id, quantity, equipped)
        SELECT
            item_template_id, NEW.id, 0, quantity, equipped
        FROM
            starting_items
        WHERE
            profession IS NULL OR profession = NEW.starting_profession;
    END;

INSERT OR IGNORE INTO starting_items (profession, item_template_id, quantity, equipped) VALUES
(NULL, 5588, NULL, NULL);  -- Tutorial Guide

-- For test purposes only:
INSERT OR IGNORE INTO starting_items (profession, item_template_id, quantity, equipped) VALUES
(NULL, 6408, NULL, TRUE),  -- Formal Wear
(NULL, 6621, NULL, FALSE), -- Infinity Spear
(NULL, 6611, NULL, TRUE),  -- Infinity Blade
(NULL, 6377, NULL, TRUE),  -- Imperial Crusader Shield
(NULL, 6618, NULL, FALSE), -- Infinity Fangs
(NULL, 6619, NULL, FALSE), -- Infinity Bow
(NULL, 1345, 100,  FALSE); -- Arrow (S-grade)
